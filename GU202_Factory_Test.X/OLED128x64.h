/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
//#ifndef          XC_HEADER_TEMPLATE_H
//#define	XC_HEADER_TEMPLATE_H
 
// #include <stdlib.h>
// #include <string.h>

// typedef unsigned char uint8_t ; 
// typedef unsigned int    uint16_t ;

#define OLED_ADDRESS    0x3c          //3C -->  I2C address for OLED  0111100 
#define X_WIDTH         128+1                   // +1 to eliminate white dot of most right edge of screen
#define Y_WIDTH         64
#define OLED_Command_Mode         0x80  // SSD1360
#define OLED_Data_Mode		    0x40  // SSD1306
// #define _BV(x) (1 << x)    
#define BLACK   0
#define WHITE   1
#define swap(a, b) { uint8_t t = a; a = b; b = t; }

// I2C2_MESSAGE_STATUS I2C_status;
// i2c2_status_t I2C_status;

// OLED
void  OLEDWrCmd(uint8_t command);
void  OLEDWrDat(uint8_t data);
void  OLED_Init();
void displayOn(void);
void displayOff(void);
void drawPixel(uint8_t x, uint8_t y, uint8_t color);
void OLED_CLS(void);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Put6x8Str(uint8_t x, uint8_t y, const uint8_t ch[]);
// void OLED_Put8x16Str(uint8_t x, uint8_t y, const uint8_t ch[]);
void OLED_Put8x16Str(uint8_t x, uint8_t y, const uint8_t ch[]);
void OLED_Put8x16ASCII(uint8_t x, uint8_t y, uint8_t no, uint8_t data[]);
void OLED_Put16x16Ch(uint8_t x, uint8_t y, uint8_t index);
void Draw_BMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);

// ELOAD
//void CurrentSet(void);
//void Read_IVT(void);



//#endif	/* XC_HEADER_TEMPLATE_H */

