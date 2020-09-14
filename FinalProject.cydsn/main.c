/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

uint8 i;

uint8 ByteCount = 0;

uint8 firstbyte = 0;
uint8 CommandReady = 0;
uint8 Count = 0;
uint8 Status;

uint8 ReceivedBuffer[70]; 
uint8 TransmitBuffer[70]; 


uint16 PingOutput[32];
uint16 PongOutput[32];

uint8 ping = 1;
uint8 datacount = 0;
uint8 dataready = 0;

struct command_protocol
{
    uint8 packet_size;
    uint8 command;
    uint8 buffer[64]; 
} Command_Packet;

struct command_protocol Transmit_Packet;

CY_ISR(ByteReceived)
{ 
    LEDDrive_Write(1);
    ReceivedBuffer[ByteCount++] = (uint8) (LabVIEW_UART_GetByte()&0x00ff);
    if(firstbyte == 0)
    {
        ByteCounter_WriteCompare(ReceivedBuffer[0]);
        firstbyte = 1;
   
    }
    Count = ByteCounter_ReadCounter();
    Status = ByteCounter_ReadStatusRegister();
    LEDDrive_Write(0);
}

CY_ISR(CommandReceived)
{
    uint8 i;
    
    if(CommandReady == 0)
    {
        Command_Packet.packet_size = ReceivedBuffer[0];
        Command_Packet.command = ReceivedBuffer[1];
    
        for(i=0;i<(Command_Packet.packet_size - 2);i++)
        {
            Command_Packet.buffer[i] =  ReceivedBuffer[i+2];
        }
    
        firstbyte = 0;
        CommandReady = 1;
        ByteCount = 0;
    }
    ByteCounter_ReadStatusRegister();
    LEDDrive_Write(0);
}



CY_ISR(TimerInterrupt)
{
    ADC_DelSig_IsEndConversion(ADC_DelSig_WAIT_FOR_RESULT);
    if(ping)
    {
        PingOutput[datacount] = ADC_DelSig_GetResult16();
    }
    else
    {
        PongOutput[datacount] = ADC_DelSig_GetResult16();
    }
    datacount = (datacount+1) & 0x01F;
    if(datacount == 0)
    {
        ping ^= 1; //switches ping to pong, vice versa
        dataready = 1;
    }
    Timer_1_ReadStatusRegister();
}
    

int main(void)
{
    
    UARTReset_Write(0);
    ByteCountReset_Write(0);
    
    LabVIEW_UART_Start();
    
    
    WaveGenerator_Start();
    ADC_DelSig_Start();
    
    TimerReset_Write(0);
    Timer_1_Start();
    TimerReset_Write(1);
    
    TimerInterrupt_Start();
    TimerInterrupt_StartEx(TimerInterrupt);
    
    Timer_1_Stop();
    
    ADC_DelSig_IRQ_Enable();
    
    ByteCounter_Start();
    ByteCounter_Enable();
    ByteCounter_SetInterruptMode(ByteCounter_STATUS_CMP_INT_EN_MASK);
    
    ByteReceived_Start();
    ByteReceived_StartEx(ByteReceived);
    ByteReceived_Enable();
    
    CommandReceived_Start();
    CommandReceived_StartEx(CommandReceived);
    CommandReceived_Enable();
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ADC_DelSig_StartConvert();
    
    for(;;)
    {
    if(CommandReady)
        {
            Transmit_Packet.command = Command_Packet.command;
            TransmitBuffer[1] = Transmit_Packet.command;
            Transmit_Packet.packet_size = Command_Packet.packet_size;
            TransmitBuffer[0] = Transmit_Packet.packet_size;
            
            switch(Command_Packet.command)
            {
                case 1: // Start Timer
                    for(i=0;i<Transmit_Packet.packet_size;i++)
                    {
                            Transmit_Packet.buffer[i+2] = Command_Packet.buffer[i];
                            TransmitBuffer[i+2] = Transmit_Packet.buffer[i+2];
                    }
                    CommandReady = 0;
                    
                    TimerReset_Write(0);
                    Timer_1_Start(); 
                    
                break;
                    
                case 2: // Start Acquisition
                    if (ping == 1 && dataready == 1)
                    {
                        for(i=0;i<32;i++)
                        {
                                TransmitBuffer[2*i+6] = (PongOutput[i])& 0xFF;
                                TransmitBuffer[2*i+7] = (PongOutput[i])>>8;
                        }
                        
                        TransmitBuffer[2] = (Timer_1_ReadPeriod())& 0xFF; // Timer Period
                        TransmitBuffer[3] = (Timer_1_ReadPeriod())>>8;
                        TransmitBuffer[4] = (Timer_1_ReadPeriod())>>16;
                        TransmitBuffer[5] = (Timer_1_ReadPeriod())>>24;
                        dataready = 0;
                        TransmitBuffer[0] = 70;
                    }    
                    else if(ping == 0 && dataready == 1)
                    {
                        for(i=0;i<32;i++)
                        {
                                TransmitBuffer[2*i+6] = (PingOutput[i])& 0xFF;
                                TransmitBuffer[2*i+7] = (PingOutput[i])>>8;
                        }
                        TransmitBuffer[2] = (Timer_1_ReadPeriod())& 0xFF;
                        TransmitBuffer[3] = (Timer_1_ReadPeriod())>>8;
                        TransmitBuffer[4] = (Timer_1_ReadPeriod())>>16;
                        TransmitBuffer[5] = (Timer_1_ReadPeriod())>>24;
                        dataready = 0;
                        TransmitBuffer[0] = 70;
                    }                   
                    CommandReady = 0;                    
                
                break;         
                    
                case 3: // Stop Timer
                    for(i=0;i<Transmit_Packet.packet_size;i++)
                    {
                            Transmit_Packet.buffer[i+2] = Command_Packet.buffer[i];
                            TransmitBuffer[i+2] = Transmit_Packet.buffer[i+2];
                    }
                    CommandReady = 0;
                    
                    Timer_1_Stop(); 
                    TimerReset_Write(0);
                    
                break;
                       
                default:
                break;
            }
            LabVIEW_UART_PutArray(TransmitBuffer,TransmitBuffer[0]);
        }
      
    }
}

/* [] END OF FILE */
