/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

//! Use RX and TX Interrupt pins to check FIFO status
#define APP_USE_RX_INT
//#define APP_USE_TX_INT
//#define APP_USE_RX_TX_INT


//! Blink LEDs after changing bit time
//#define APP_BLINK_LED_AFTER_SETBITTIME

#define MAX_TXQUEUE_ATTEMPTS 50

// Switches
#define APP_DEBOUNCE_TIME  100
#define APP_SWITCH_PRESSED  false
#define APP_SWITCH_RELEASED true

// Special LEDs
#define APP_INIT_LED    APP_LED_D8
#define APP_TX_LED      APP_LED_D2
#define APP_RX_LED      APP_LED_D3
#define APP_TX_JPG_LED  APP_LED_D4
#define APP_RX_JPG_LED  APP_LED_D5

#define APP_LED_TIME    50000

// Message IDs
#define FILE_START_ID 0xd0
#define FILE_STOP_ID  0xdf
#define FILE_DATA_ID  0xda

#define TX_REQUEST_ID               0x300
#define TX_RESPONSE_ID              0x301
#define BUTTON_STATUS_ID        0x20
#define LED_STATUS_ID               0x200
#define BITTIME_SET_ID              0x100
#define PAYLOAD_ID                  0x101
#define BITTIME_CFG_GET_ID  0x600
#define BITTIME_CFG_125K_ID 0x601
#define BITTIME_CFG_250K_ID 0x602
#define BITTIME_CFG_500K_ID 0x603
#define BITTIME_CFG_1M_ID   0x604

// Transmit Channels
#define APP_TX_FIFO CAN_FIFO_CH2

// Receive Channels
#define APP_RX_FIFO CAN_FIFO_CH1

// Switch states

typedef struct {
    bool S1;
    bool S2;
    bool S3;
    bool S4;
} APP_SwitchState;

// Payload

typedef struct {
    bool On;
    uint8_t Dlc;
    bool Mode;
    uint8_t Counter;
    uint8_t Delay;
    bool BRS;
} APP_Payload;


// *****************************************************************************

/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
 */

typedef enum {
    // Initialization
    APP_STATE_INIT = 0,
    APP_STATE_REQUEST_CONFIG,
    APP_STATE_WAIT_FOR_CONFIG,
    APP_STATE_INIT_TXOBJ,

    // POR signaling
    APP_STATE_FLASH_LEDS,

    // Transmit and Receive
    APP_STATE_TRANSMIT,
    APP_STATE_RECEIVE,
    APP_STATE_PAYLOAD,

    // Switch monitoring
    APP_STATE_SWITCH_CHANGED
} APP_STATES;


// *****************************************************************************

/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct {
    /* The application's current state */
    APP_STATES state;

    /* TODO: Define any additional data used by the application. */


} APP_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
 */


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

//! Application Initialization

void APP_Initialize(void);

//! Application Tasks
// This routine must be called from SYS_Tasks() routine.

void APP_Tasks(void);

//! Switch Debounce

void APP_DebounceSwitches(bool* changed);

//! Write LEDs based on input byte

void APP_LED_Write(uint8_t led);

//! Clear One LED

inline void APP_LED_Clear(uint8_t led);

//! Set One LED

inline void APP_LED_Set(uint8_t led);

//! Initialize CANFDSPI
void APP_CANFDSPI_Init();

//! Add message to transmit FIFO
void APP_TransmitMessageQueue();

//! Decode received messages
APP_STATES APP_ReceiveMessage_Tasks();

void APP_PayLoad_Tasks();

void APP_TransmitBitTimeConfig(uint32_t cfg);

#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

