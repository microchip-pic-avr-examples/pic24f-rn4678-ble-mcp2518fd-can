/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ64GU202
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "OLED128x64.h"
#include "mcc_generated_files/usb/usb.h"

#include "drv_canfdspi_defines.h"
#include "drv_canfdspi_api.h"
#include "User_2518FD_App.h"

#define MCP9800_ADDRESS     0x4d 

#define LUKE_MODIFY 1
/* ##uart format
 * 0x55 / 0xaa / length / command / data.....  , suggest to append "length"/"checksum"/"tail", but no time
 * #command <Just for CAN 2.0
 * 0x01 : RTC value from evm => 0x01 / hour+min+sec(3 bytes) => len=4
 * 0x04 : CAN BUS received   => 0x04 / id(2 bytes) / CAN BUS data(1 byte DLC  + 8 bytes data) => len=12
 * 0x05 : CAN BUS transmited => 0x05 / id(2 bytes) / CAN BUS data(1 byte DLC  + 8 bytes data) => len=12
 * 
 */

#if LUKE_MODIFY == 1

#define UR_CMD_RTC_RX  0x01
#define UR_CMD_CAN_RX  0x04
#define UR_CMD_CAN_TX  0x05

unsigned char uart_buf_rx[60];
#endif


/*
                         Main application
 */

unsigned char   T1_OverFlow = 0 ;
unsigned char   MCP2518FD_GPIO_Action = 0 ;
unsigned int    MCP9800_ReadValue = 0 ;

unsigned char   MCP9800_ReadBuf[8] ;
unsigned char   DispBuf[16] ;

uint8_t         TimeSec, TimeMin, TimeHr ;
uint8_t         ASCII_Buf[64] ;
const uint8_t   PIC24_STRING[] = "PIC24FJ64GU202  " ;
const uint8_t   CLOCK_STRING[] = "TIME:0  :0  :   " ;
const uint8_t   MCP9800_STRING[] =  "MCP9800 :       " ;

struct  
{
    unsigned char       Data_Received ;
    unsigned int        SID ;
    unsigned char       EID_FLAG ;
    unsigned long       SID_EID ;
    unsigned char       DLC ;
    unsigned char       data[8] ;  
} CAN_Receive_Queue_StdCAN ;

void    User_T1_ISR(void)
{
    T1_OverFlow = 1 ;
}
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TimeSec = 0 ;
    TimeMin = 0 ;
    TimeHr = 0 ;
    
    spi1_open(SPI1_DEFAULT);
    APP_CANFDSPI_Init() ;
    
    TMR1_SetInterruptHandler(User_T1_ISR) ;
    OLED_Init();
    OLED_CLS();

    //OLED_Put8x16Str( 0,0, (const uint8_t) " PIC24FJ64GU202") ;
    OLED_Put8x16Str(0,0,PIC24_STRING) ;
    OLED_Put8x16Str(0,2,CLOCK_STRING) ;
    OLED_Put8x16Str(0,4,MCP9800_STRING) ;
    
    DRV_CANFDSPI_GpioDirectionConfigure(0, GPIO_OUTPUT, GPIO_OUTPUT) ;
    while (1)
    {
        CAN_Receive_Queue_StdCAN.Data_Received =0 ;
        APP_ReceiveMessage_Tasks() ;
        if (CAN_Receive_Queue_StdCAN.Data_Received )
        {
                     if( USBUSARTIsTxTrfReady() == true)
            {
#if LUKE_MODIFY == 0
                sprintf(ASCII_Buf, "I got a CAN Message ID=%d , Data = %d, %d, %d, %d \n\r", CAN_Receive_Queue_StdCAN.SID, CAN_Receive_Queue_StdCAN.data[0],  \
                                     CAN_Receive_Queue_StdCAN.data[1], CAN_Receive_Queue_StdCAN.data[2], CAN_Receive_Queue_StdCAN.data[3]);
                putsUSBUSART(ASCII_Buf);
#else
                // b0(header:0x55) b1(command:0x0a<Received CAN> b2(payload length)
                //memcpy(ASCII_Buf, )
                uint8_t tmp1;
                
                uart_buf_rx[0] = 0x55;
                uart_buf_rx[1] = 0xaa;
                uart_buf_rx[2] = 12;
                uart_buf_rx[3] = UR_CMD_CAN_RX;
                uart_buf_rx[4] = CAN_Receive_Queue_StdCAN.SID;
                uart_buf_rx[5] = (CAN_Receive_Queue_StdCAN.SID)>>8;
                uart_buf_rx[6] = CAN_Receive_Queue_StdCAN.DLC;
                for(tmp1=0;tmp1<CAN_Receive_Queue_StdCAN.DLC; tmp1++)
                    uart_buf_rx[7+tmp1] = CAN_Receive_Queue_StdCAN.data[tmp1];
                putUSBUSART(uart_buf_rx, uart_buf_rx[2]+3);
#endif
            }        
                     
                       printf ("Got a Message ID=%d , Data = %d, %d, %d, %d \n\r",CAN_Receive_Queue_StdCAN.SID, CAN_Receive_Queue_StdCAN.data[0],  \
                                  CAN_Receive_Queue_StdCAN.data[1] ,CAN_Receive_Queue_StdCAN.data[2] , CAN_Receive_Queue_StdCAN.data[3] ) ;
        }
        
        
#if LUKE_MODIFY ==1
        uint8_t buf_command_rx[30],
                len_buf_command;
        

        //## USB CDC Received task
        len_buf_command=getsUSBUSART(buf_command_rx, 30);
        if (len_buf_command ==15)
        {
            LED1_RA2_Toggle();
            APP_CAN_Transmit_2(&(buf_command_rx[7]), buf_command_rx[6]);
        }
#endif
        // Add your application code
        if (T1_OverFlow)
        {          
//            // MCP9800_ReadValue = i2c_read2ByteRegister(MCP9800_ADDRESS , 0x00 ) ;
            i2c_readDataBlock(MCP9800_ADDRESS, 0x00 , MCP9800_ReadBuf, 2) ;
                    sprintf (DispBuf,"%2d C",MCP9800_ReadBuf[0]);
                    OLED_Put8x16ASCII(80 , 4, 4, DispBuf) ;     
                    
            if (MCP2518FD_GPIO_Action)
            {
                DRV_CANFDSPI_GpioPinSet(0,GPIO_PIN_1,GPIO_HIGH) ;
                MCP2518FD_GPIO_Action = 0 ;
            }
            else
                {
                    DRV_CANFDSPI_GpioPinSet(0,GPIO_PIN_1,GPIO_LOW) ;
                    MCP2518FD_GPIO_Action = 1 ;
                }
                     
            // printf("I am RN4678 \n\r") ;
            
            //LED1_RA2_Toggle();
            //LED2_RA4_Toggle();
            TimeSec ++ ;

            if (TimeSec >= 120 )
            {
                TimeSec = 0 ;
                TimeMin ++ ;
                sprintf (ASCII_Buf, "0  \0",TimeSec/2) ;
                OLED_Put8x16Str(104,2,ASCII_Buf) ;
                sprintf (ASCII_Buf, "%d\0",TimeMin) ;
                OLED_Put8x16Str(72,2,ASCII_Buf) ;
                    sprintf (ASCII_Buf, "%d\0",TimeHr) ;
                    OLED_Put8x16Str(40,2,ASCII_Buf) ;
                if (TimeMin >= 60)
                {
                    TimeMin = 0 ;
                    TimeHr ++ ;
                    if (TimeHr >= 12)
                    {
                        TimeHr = 0 ;
                    }
                }
            }
            
            if (!(TimeSec % 2))
                {
                    sprintf (ASCII_Buf, "%d\0",TimeSec/2) ;
                    OLED_Put8x16Str(104,2,ASCII_Buf) ;
#if LUKE_MODIFY == 1
                uart_buf_rx[0] = 0x55;
                uart_buf_rx[1] = 0xaa;
                uart_buf_rx[2] = 4;
                uart_buf_rx[3] = UR_CMD_RTC_RX; // command
                uart_buf_rx[4] = TimeHr;
                uart_buf_rx[5] = TimeMin;
                uart_buf_rx[6] = TimeSec / 2;
                putUSBUSART(uart_buf_rx, 7);
                APP_CAN_Transmit_1() ;
#endif
                }

            //APP_CAN_Transmit_1() ;
            T1_OverFlow = 0 ;
        }
       CDCTxService() ;
    }

    return 1;
}
/**
 End of File
*/

