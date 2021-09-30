#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <xc.h>

#include "OLED128x64.h"            
#define ReverseScreen

//extern void ReadEncoder(void);
uint8_t     OLED_Data[4] ;
#include "oled_fonts.c"
void OLED_Init(void)
{
//    __delay_us(50);
            OLEDWrCmd(0xAE);   //display off
            OLEDWrCmd(0x20);	//Set Memory Addressing Mode	
            OLEDWrCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
            OLEDWrCmd(0xB0);	//Set Page Start Address for Page Addressing Mode,0-7
            OLEDWrCmd(0xC8);	//Set COM Output Scan Direction
            OLEDWrCmd(0x00);    //---set low column address
            OLEDWrCmd(0x10);    //---set high column address
            OLEDWrCmd(0x40);    //--set start line address
            OLEDWrCmd(0x81);    //--set contrast control register
            OLEDWrCmd(0x7F);
            OLEDWrCmd(0xA1);    //--set segment re-map 0 to 127
            OLEDWrCmd(0xA6);    //--set normal display
            OLEDWrCmd(0xA8);    //--set multiplex ratio(1 to 64)
            OLEDWrCmd(0x3F);    //
            OLEDWrCmd(0xA4);    //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
            OLEDWrCmd(0xD3);    //-set display offset
    
	OLEDWrCmd(0x00);    //-not offset
	OLEDWrCmd(0xD5);    //--set display clock divide ratio/oscillator frequency
	OLEDWrCmd(0xF0);    //--set divide ratio
	OLEDWrCmd(0xD9);    //--set pre-charge period
	OLEDWrCmd(0x22);    //
	OLEDWrCmd(0xDA);    //--set com pins hardware configuration
	OLEDWrCmd(0x12);
	OLEDWrCmd(0xDB);    //--set vcomh
	OLEDWrCmd(0x20);    //0x20,0.77xVcc
	OLEDWrCmd(0x8D);    //--set DC-DC enable
	OLEDWrCmd(0x14);    //
	OLEDWrCmd(0xAF);    //--turn on oled panel    
}

/********** copy from LQ12864 **********/

/******** display 16*16 matrix, coordinate(x,y) y range 0~7 *********/
void OLED_Put16x16Ch(uint8_t x, uint8_t y, uint8_t index)
{
	unsigned char     i, j;
    
    i= index;                       // point to character which will be put to LCD
	OLED_Set_Pos(x, y);
	for(j=0; j<16; j++)              // upper part of Chinese character
	{
		OLEDWrDat(Font16x16[i][j]);
	}
	OLED_Set_Pos(x, (unsigned char)(y+1) );
	for(j=16; j<32; j++)            // lower part of Chinese character
	{
		OLEDWrDat(Font16x16[i][j]);
	} 	  	
}

/*********************set OLED display location************************************/
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
	OLEDWrCmd((uint8_t) (0xb0+y) );
	OLEDWrCmd((uint8_t)(((x&0xf0)>>4)|0x10));
	OLEDWrCmd((uint8_t)((x&0x0f)|0x01));
}

/********** display 6*8 ASCII character at coordinate(x,y), y range 0~7 ***********/
void OLED_Put6x8Str(uint8_t x, uint8_t y, const uint8_t ch[])
{
	uint8_t c, i, j;
    
    j = 0;
	while (ch[j]!='\0')
	{
		c = (uint8_t) (ch[j] - 0x20);
		if(x>126) {
            x=0;
            y++;
        }
		OLED_Set_Pos(x,y);
		for(i=0; i<6; i++)
            OLEDWrDat(Font6x8[c][i]);
		x += 6;
		j++;
	}
}

/******* display 8*16 ASCII character at coordinate(x,y), y range 0~7 *******/
void OLED_Put8x16Str(uint8_t x, uint8_t y, const uint8_t ch[])
{
	uint8_t c, i, j;
    j = 0;
	while (ch[j]!='\0')
	{
		c = (uint8_t) (ch[j] - 0x20);    // ASCII not start from 0, it start from 32 (20H) to 126 (7EH)
		if(x>120) {
            x=0;
            y++;
        }
		OLED_Set_Pos(x, y);            
		for(i=0; i<8; i++) { OLEDWrDat(Font8x16[c][i]); }       // upper part of the ASCII character
		OLED_Set_Pos(x, (uint8_t) (y+1));
		for(i=8; i<16; i++) { OLEDWrDat(Font8x16[c][i]); }      // lower part of the ASCII character
		x += 8; // 8x16 font so x move 8 for next character
		j++;    // next character
	}
}

/******* display 8*16 ASCII character at coordinate(x,y), y range 0~7 *******/
void OLED_Put8x16ASCII(uint8_t x, uint8_t y, uint8_t no, uint8_t data[])
{
	uint8_t cIndex, i, j;
    
    j = 0;
    cIndex = 0 ;
	while (j < no)
	{
		cIndex = (uint8_t) (data[j] - 0x20);    // ASCII not start from 0, it start from 32 (20H) to 126 (7EH)
		if(x>120) {
            x=0;
            y++;
        }
		OLED_Set_Pos(x, y);             // upper part of the ASCII character
		for(i=0; i<8; i++) {
            OLEDWrDat((uint8_t)Font8x16[cIndex][i]);
            //OLEDWrDat(0xc3) ;
//            ReadEncoder();
        }
		OLED_Set_Pos(x, (uint8_t) (y+1));
		for(i=8; i<16; i++) {            // lower part of the ASCII character
            OLEDWrDat((uint8_t)Font8x16[cIndex][i]);
            // OLEDWrDat(0x3c) ;
//            ReadEncoder();
        }
		x += 8; // 8x16 font so x move 8 for next character
		j++;    // next data byte
	}
}


void OLED_CLS(void)
{
	uint8_t     x, y;    
	for(y=0; y<8; y++)
	{
		OLEDWrCmd((uint8_t) (0xb0+y));
		OLEDWrCmd(0x00);
		OLEDWrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)      OLEDWrDat(0);       
	}
}

void displayOn(void)             // Turns on Display 
{
    OLEDWrCmd(0xAF);        //display on
}

void displayOff(void)           // Turns off Display 
{
    OLEDWrCmd(0xAE);		//display off
}


/***** display BMP picture, initial coordinate(x,y), x range 0~127, y range 0~7 *****/
void Draw_BMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
	uint16_t j;
	uint8_t x, y;

//    if(y1%8 == 0)   y = y1 >> 3;    //y1/8;      
//    else            y= (y1 >> 3) + 1; //y1/8+1;
    j = 0;
	for(y=y0; y<y1; y++)
	{
		OLED_Set_Pos(x0,y);
        for(x=x0; x<x1; x++) {      
	    	OLEDWrDat(BMP[j++]);
	    }
	}
}

/********************* End of LQ12864 ******************/

void  OLEDWrCmd(uint8_t command)
{            
        i2c_write1ByteRegister(OLED_ADDRESS, OLED_Command_Mode, command);
}

void  OLEDWrDat(uint8_t data)
{   
           
        i2c_write1ByteRegister(OLED_ADDRESS, OLED_Data_Mode, data)  ;      
}


// the most basic function, set a single pixel
//void drawPixel(uint8_t x, uint8_t y, uint8_t color) 
//{
//    if ((x < 0) || (x >= 128) || (y < 0) || (y >= 64))
//        return;
//
//    // x is which column
//    if (color == WHITE) 
//        BMP_buffer[x+ (y/8)*128] |= _BV((y%8));  
//    else
//        BMP_buffer[x+ (y/8)*128] &= ~_BV((y%8)); 
//}
