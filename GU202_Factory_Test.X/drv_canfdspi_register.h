/*******************************************************************************
  CAN FD SPI Driver: Register Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_canfdspi_register.h

  Summary:
    This header file contains SPI instruction defines, register address defines,
    register structures, and reset values of registers.

  Description:
    This file is used by the API.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2016-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _DRV_CANFDSPI_REGISTER_H
#define _DRV_CANFDSPI_REGISTER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files

#include "drv_canfdspi_defines.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
/* SPI Instruction Set */

#define cINSTRUCTION_RESET			0x00
#define cINSTRUCTION_READ			0x03
#define cINSTRUCTION_READ_CRC       0x0B
#define cINSTRUCTION_WRITE			0x02
#define cINSTRUCTION_WRITE_CRC      0x0A
#define cINSTRUCTION_WRITE_SAFE     0x0C

// *****************************************************************************
// *****************************************************************************
/* Register Addresses */

/* CAN FD Controller */
#define cREGADDR_CiCON  	0x000
#define cREGADDR_CiNBTCFG	0x004
#define cREGADDR_CiDBTCFG	0x008
#define cREGADDR_CiTDC  	0x00C

#define cREGADDR_CiTBC      0x010
#define cREGADDR_CiTSCON    0x014
#define cREGADDR_CiVEC      0x018
#define cREGADDR_CiINT      0x01C
#define cREGADDR_CiINTFLAG      cREGADDR_CiINT
#define cREGADDR_CiINTENABLE    (cREGADDR_CiINT+2)

#define cREGADDR_CiRXIF     0x020
#define cREGADDR_CiTXIF     0x024
#define cREGADDR_CiRXOVIF   0x028
#define cREGADDR_CiTXATIF   0x02C

#define cREGADDR_CiTXREQ    0x030
#define cREGADDR_CiTREC     0x034
#define cREGADDR_CiBDIAG0   0x038
#define cREGADDR_CiBDIAG1   0x03C

#define cREGADDR_CiTEFCON   0x040
#define cREGADDR_CiTEFSTA   0x044
#define cREGADDR_CiTEFUA    0x048
#define cREGADDR_CiFIFOBA   0x04C

#define cREGADDR_CiFIFOCON  0x050
#define cREGADDR_CiFIFOSTA  0x054
#define cREGADDR_CiFIFOUA   0x058
#define CiFIFO_OFFSET       (3*4)

#define cREGADDR_CiTXQCON  0x050
#define cREGADDR_CiTXQSTA  0x054
#define cREGADDR_CiTXQUA   0x058

// The filters start right after the FIFO control/status registers
#define cREGADDR_CiFLTCON   (cREGADDR_CiFIFOCON+(CiFIFO_OFFSET*CAN_FIFO_TOTAL_CHANNELS))
#define cREGADDR_CiFLTOBJ   (cREGADDR_CiFLTCON+CAN_FIFO_TOTAL_CHANNELS)
#define cREGADDR_CiMASK     (cREGADDR_CiFLTOBJ+4)

#define CiFILTER_OFFSET     (2*4)

/* MCP25xxFD Specific */
#define cREGADDR_OSC        0xE00
#define cREGADDR_IOCON      0xE04
#define cREGADDR_CRC    	0xE08
#define cREGADDR_ECCCON  	0xE0C
#define cREGADDR_ECCSTA  	0xE10
#ifndef MCP2517FD
#define cREGADDR_DEVID  	0xE14
#endif

/* RAM addresses */
#if defined(MCP2517FD) || defined(MCP2518FD)
#define cRAM_SIZE       2048
#endif

#define cRAMADDR_START  0x400
#define cRAMADDR_END    (cRAMADDR_START+cRAM_SIZE)

// *****************************************************************************
// *****************************************************************************
/* Register Structures */

// *****************************************************************************
//! General 32-bit Register

typedef union _REG_t {
    uint8_t byte[4];
    unsigned long word;
} REG_t;


// *****************************************************************************
// *****************************************************************************
/* CAN FD Controller */

// *****************************************************************************
//! CAN Control Register

typedef union _REG_CiCON {

    struct {
        unsigned DNetFilterCount : 5;
        unsigned IsoCrcEnable : 1;
        unsigned ProtocolExceptionEventDisable : 1;
        unsigned unimplemented1 : 1;
        unsigned WakeUpFilterEnable : 1;
        unsigned WakeUpFilterTime : 2;
        unsigned unimplemented2 : 1;
        unsigned BitRateSwitchDisable : 1;
        unsigned unimplemented3 : 3;
        unsigned RestrictReTxAttempts : 1;
        unsigned EsiInGatewayMode : 1;
        unsigned SystemErrorToListenOnly : 1;
        unsigned StoreInTEF : 1;
        unsigned TXQEnable : 1;
        unsigned OpMode : 3;
        unsigned RequestOpMode : 3;
        unsigned AbortAllTx : 1;
        unsigned TxBandWidthSharing : 4;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiCON;

// *****************************************************************************
//! Nominal Bit Time Configuration Register

typedef union _REG_CiNBTCFG {

    struct {
        unsigned SJW : 7;
        unsigned unimplemented1 : 1;
        unsigned TSEG2 : 7;
        unsigned unimplemented2 : 1;
        unsigned TSEG1 : 8;
        unsigned BRP : 8;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiNBTCFG;

// *****************************************************************************
//! Data Bit Time Configuration Register

typedef union _REG_CiDBTCFG {

    struct {
        unsigned SJW : 4;
        unsigned unimplemented1 : 4;
        unsigned TSEG2 : 4;
        unsigned unimplemented2 : 4;
        unsigned TSEG1 : 5;
        unsigned unimplemented3 : 3;
        unsigned BRP : 8;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiDBTCFG;

// *****************************************************************************
//! Transmitter Delay Compensation Register

typedef union _REG_CiTDC {

    struct {
        unsigned TDCValue : 6;
        unsigned unimplemented1 : 2;
        unsigned TDCOffset : 7;
        unsigned unimplemented2 : 1;
        unsigned TDCMode : 2;
        unsigned unimplemented3 : 6;
        unsigned SID11Enable : 1;
        unsigned EdgeFilterEnable : 1;
        unsigned unimplemented4 : 6;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiTDC;

// *****************************************************************************
//! Time Stamp Configuration Register

typedef union _REG_CiTSCON {

    struct {
        unsigned TBCPrescalerL : 8;
        unsigned TBCPrescalerH : 2;
        unsigned unimplemented1 : 6;
        unsigned TBCEnable : 1;
        unsigned TimeStampEOF : 1;
        unsigned unimplemented2 : 8;
        unsigned unimplemented3 : 6;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiTSCON;

// *****************************************************************************
//! Interrupt Vector Register

typedef union _REG_CiVEC {

    struct {
        unsigned ICODE : 7;
        unsigned unimplemented1 : 1;
        unsigned FilterHit : 5;
        unsigned unimplemented2 : 3;
        unsigned TXCODE : 7;
        unsigned unimplemented3 : 1;
        unsigned RXCODE : 7;
        unsigned unimplemented4 : 1;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiVEC;

// *****************************************************************************
//! Interrupt Flags

typedef struct _CAN_INT_FLAGS {
    unsigned TXIF : 1;
    unsigned RXIF : 1;
    unsigned TBCIF : 1;
    unsigned MODIF : 1;
    unsigned TEFIF : 1;
    unsigned unimplemented1 : 3;

    unsigned ECCIF : 1;
    unsigned SPICRCIF : 1;
    unsigned TXATIF : 1;
    unsigned RXOVIF : 1;
    unsigned SERRIF : 1;
    unsigned CERRIF : 1;
    unsigned WAKIF : 1;
    unsigned IVMIF : 1;
} CAN_INT_FLAGS;

// *****************************************************************************
//! Interrupt Enables

typedef struct _CAN_INT_ENABLES {
    unsigned TXIE : 1;
    unsigned RXIE : 1;
    unsigned TBCIE : 1;
    unsigned MODIE : 1;
    unsigned TEFIE : 1;
    unsigned unimplemented2 : 3;

    unsigned ECCIE : 1;
    unsigned SPICRCIE : 1;
    unsigned TXATIE : 1;
    unsigned RXOVIE : 1;
    unsigned SERRIE : 1;
    unsigned CERRIE : 1;
    unsigned WAKIE : 1;
    unsigned IVMIE : 1;
} CAN_INT_ENABLES;

// *****************************************************************************
//! Interrupt Register

typedef union _REG_CiINT {

    struct {
        CAN_INT_FLAGS IF;
        CAN_INT_ENABLES IE;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiINT;

// *****************************************************************************
//! Interrupt Flag Register

typedef union _REG_CiINTFLAG {
    CAN_INT_FLAGS IF;
    uint16_t word;
    uint8_t byte[2];
} REG_CiINTFLAG;

// *****************************************************************************
//! Interrupt Enable Register

typedef union _REG_CiINTENABLE {
    CAN_INT_ENABLES IE;
    uint16_t word;
    uint8_t byte[2];
} REG_CiINTENABLE;

// *****************************************************************************
//! Transmit/Receive Error Count Register

typedef union _REG_CiTREC {

    struct {
        unsigned RxErrorCount : 8;
        unsigned TxErrorCount : 8;
        unsigned ErrorStateWarning : 1;
        unsigned RxErrorStateWarning : 1;
        unsigned TxErrorStateWarning : 1;
        unsigned RxErrorStatePassive : 1;
        unsigned TxErrorStatePassive : 1;
        unsigned TxErrorStateBusOff : 1;
        unsigned unimplemented1 : 8;
        unsigned unimplemented2 : 2;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiTREC;

// *****************************************************************************
//! Diagnostic Register 0

typedef union _REG_CiBDIAG0 {

    struct {
        unsigned NRxErrorCount : 8;
        unsigned NTxErrorCount : 8;
        unsigned DRxErrorCount : 8;
        unsigned DTxErrorCount : 8;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiBDIAG0;

// *****************************************************************************
//! Diagnostic Register 1

typedef union _REG_CiBDIAG1 {

    struct {
        unsigned ErrorFreeMsgCountL : 8;
        unsigned ErrorFreeMsgCountH : 8;
        unsigned NBit0Error : 1;
        unsigned NBit1Error : 1;
        unsigned NAckError : 1;
        unsigned NFormError : 1;
        unsigned NStuffError : 1;
        unsigned NCRCError : 1;
        unsigned unimplemented1 : 1;
        unsigned TXBOError : 1;
        unsigned DBit0Error : 1;
        unsigned DBit1Error : 1;
        unsigned DAckError : 1;
        unsigned DFormError : 1;
        unsigned DStuffError : 1;
        unsigned DCRCError : 1;
        unsigned ESI : 1;
        unsigned unimplemented2 : 1;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiBDIAG1;

// *****************************************************************************
//! Transmit Event FIFO Control Register

typedef union _REG_CiTEFCON {

    struct {
        unsigned TEFNEIE : 1;
        unsigned TEFHFIE : 1;
        unsigned TEFFULIE : 1;
        unsigned TEFOVIE : 1;
        unsigned unimplemented1 : 1;
        unsigned TimeStampEnable : 1;
        unsigned unimplemented2 : 2;
        unsigned UINC : 1;
        unsigned unimplemented3 : 1;
        unsigned FRESET : 1;
        unsigned unimplemented4 : 8;
        unsigned unimplemented5 : 5;
        unsigned FifoSize : 5;
        unsigned unimplemented6 : 3;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiTEFCON;

// *****************************************************************************
//! Transmit Event FIFO Status Register

typedef union _REG_CiTEFSTA {

    struct {
        unsigned TEFNotEmptyIF : 1;
        unsigned TEFHalfFullIF : 1;
        unsigned TEFFullIF : 1;
        unsigned TEFOVIF : 1;
        unsigned unimplemented1 : 8;
        unsigned unimplemented2 : 8;
        unsigned unimplemented3 : 8;
        unsigned unimplemented4 : 4;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiTEFSTA;

// *****************************************************************************
//! Transmit Queue Control Register

typedef union _REG_CiTXQCON {

    struct {
        unsigned TxNotFullIE : 1;
        unsigned unimplemented1 : 1;
        unsigned TxEmptyIE : 1;
        unsigned unimplemented2 : 1;
        unsigned TxAttemptIE : 1;
        unsigned unimplemented3 : 2;
        unsigned TxEnable : 1;
        unsigned UINC : 1;
        unsigned TxRequest : 1;
        unsigned FRESET : 1;
        unsigned unimplemented4 : 5;
        unsigned TxPriority : 5;
        unsigned TxAttempts : 2;
        unsigned unimplemented5 : 1;
        unsigned FifoSize : 5;
        unsigned PayLoadSize : 3;
    } txBF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiTXQCON;

// *****************************************************************************
//! Transmit Queue Status Register

typedef union _REG_CiTXQSTA {

    struct {
        unsigned TxNotFullIF : 1;
        unsigned unimplemented1 : 1;
        unsigned TxEmptyIF : 1;
        unsigned unimplemented2 : 1;
        unsigned TxAttemptIF : 1;
        unsigned TxError : 1;
        unsigned TxLostArbitration : 1;
        unsigned TxAborted : 1;
        unsigned FifoIndex : 5;
        unsigned unimplemented3 : 8;
        unsigned unimplemented4 : 8;
        unsigned unimplemented5 : 3;
    } txBF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiTXQSTA;

// *****************************************************************************
//! FIFO Control Register

typedef union _REG_CiFIFOCON {
    // Receive FIFO

    struct {
        unsigned RxNotEmptyIE : 1;
        unsigned RxHalfFullIE : 1;
        unsigned RxFullIE : 1;
        unsigned RxOverFlowIE : 1;
        unsigned unimplemented1 : 1;
        unsigned RxTimeStampEnable : 1;
        unsigned unimplemented2 : 1;
        unsigned TxEnable : 1;
        unsigned UINC : 1;
        unsigned unimplemented3 : 1;
        unsigned FRESET : 1;
        unsigned unimplemented4 : 8;
        unsigned unimplemented5 : 5;
        unsigned FifoSize : 5;
        unsigned PayLoadSize : 3;
    } rxBF;

    // Transmit FIFO

    struct {
        unsigned TxNotFullIE : 1;
        unsigned TxHalfFullIE : 1;
        unsigned TxEmptyIE : 1;
        unsigned unimplemented1 : 1;
        unsigned TxAttemptIE : 1;
        unsigned unimplemented2 : 1;
        unsigned RTREnable : 1;
        unsigned TxEnable : 1;
        unsigned UINC : 1;
        unsigned TxRequest : 1;
        unsigned FRESET : 1;
        unsigned unimplemented3 : 5;
        unsigned TxPriority : 5;
        unsigned TxAttempts : 2;
        unsigned unimplemented4 : 1;
        unsigned FifoSize : 5;
        unsigned PayLoadSize : 3;
    } txBF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiFIFOCON;

// *****************************************************************************
//! FIFO Status Register

typedef union _REG_CiFIFOSTA {
    // Receive FIFO

    struct {
        unsigned RxNotEmptyIF : 1;
        unsigned RxHalfFullIF : 1;
        unsigned RxFullIF : 1;
        unsigned RxOverFlowIF : 1;
        unsigned unimplemented1 : 4;
        unsigned FifoIndex : 5;
        unsigned unimplemented2 : 8;
        unsigned unimplemented3 : 8;
        unsigned unimplemented4 : 3;
    } rxBF;

    // Transmit FIFO

    struct {
        unsigned TxNotFullIF : 1;
        unsigned TxHalfFullIF : 1;
        unsigned TxEmptyIF : 1;
        unsigned unimplemented1 : 1;
        unsigned TxAttemptIF : 1;
        unsigned TxError : 1;
        unsigned TxLostArbitration : 1;
        unsigned TxAborted : 1;
        unsigned FifoIndex : 5;
        unsigned unimplemented2 : 8;
        unsigned unimplemented3 : 8;
        unsigned unimplemented4 : 3;
    } txBF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiFIFOSTA;

// *****************************************************************************
//! FIFO User Address Register

typedef union _REG_CiFIFOUA {

    struct {
        unsigned UserAddressL : 8;
        unsigned UserAddressH : 4;
        unsigned unimplemented1 : 4;
        unsigned unimplemented2 : 8;
        unsigned unimplemented3 : 8;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiFIFOUA;

// *****************************************************************************
//! Filter Control Register

typedef union _REG_CiFLTCON_BYTE {

    struct {
        unsigned BufferPointer : 5;
        unsigned unimplemented1 : 2;
        unsigned Enable : 1;
    } bF;
    uint8_t byte;
} REG_CiFLTCON_BYTE;

// *****************************************************************************
//! Filter Object Register

typedef union _REG_CiFLTOBJ {
    CAN_FILTEROBJ_ID bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiFLTOBJ;

// *****************************************************************************
//! Mask Object Register

typedef union _REG_CiMASK {
    CAN_MASKOBJ_ID bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CiMASK;


// *****************************************************************************
// *****************************************************************************
/* MCP25xxFD Specific */

// *****************************************************************************
//! Oscillator Control Register

typedef union _REG_OSC {

    struct {
        unsigned PllEnable : 1;
        unsigned unimplemented1 : 1;
        unsigned OscDisable : 1;
#ifdef MCP2517FD
        unsigned unimplemented2 : 1;
#else
        unsigned LowPowerModeEnable : 1;
#endif
        unsigned SCLKDIV : 1;
        unsigned CLKODIV : 2;
        unsigned unimplemented3 : 1;
        unsigned PllReady : 1;
        unsigned unimplemented4 : 1;
        unsigned OscReady : 1;
        unsigned unimplemented5 : 1;
        unsigned SclkReady : 1;
        unsigned unimplemented6 : 3;
        unsigned unimplemented7 : 8;
        unsigned unimplemented8 : 8;        
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_OSC;

// *****************************************************************************
//! I/O Control Register

typedef union _REG_IOCON {

    struct {
        unsigned TRIS0 : 1;
        unsigned TRIS1 : 1;
        unsigned unimplemented1 : 2;
        unsigned ClearAutoSleepOnMatch : 1;
        unsigned AutoSleepEnable : 1;
        unsigned XcrSTBYEnable : 1;
        unsigned unimplemented2 : 1;
        unsigned LAT0 : 1;
        unsigned LAT1 : 1;
        unsigned unimplemented3 : 5;
        unsigned HVDETSEL : 1;
        unsigned GPIO0 : 1;
        unsigned GPIO1 : 1;
        unsigned unimplemented4 : 6;
        unsigned PinMode0 : 1;
        unsigned PinMode1 : 1;
        unsigned unimplemented5 : 2;
        unsigned TXCANOpenDrain : 1;
        unsigned SOFOutputEnable : 1;
        unsigned INTPinOpenDrain : 1;
        unsigned unimplemented6 : 1;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_IOCON;

// *****************************************************************************
//! CRC Regsiter

typedef union _REG_CRC {

    struct {
        unsigned int CRCL : 8;
        unsigned int CRCH : 8;
        unsigned CRCERRIF : 1;
        unsigned FERRIF : 1;
        unsigned unimplemented1 : 6;
        unsigned CRCERRIE : 1;
        unsigned FERRIE : 1;
        unsigned unimplemented2 : 6;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_CRC;

// *****************************************************************************
//! ECC Control Register

typedef union _REG_ECCCON {

    struct {
        unsigned EccEn : 1;
        unsigned SECIE : 1;
        unsigned DEDIE : 1;
        unsigned unimplemented1 : 5;
        unsigned Parity : 7;
        unsigned unimplemented2 : 1;
        unsigned unimplemented3 : 8;
        unsigned unimplemented4 : 8;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_ECCCON;

// *****************************************************************************
//! ECC Status Register

typedef union _REG_ECCSTA {

    struct {
        unsigned unimplemented1 : 1;
        unsigned SECIF : 1;
        unsigned DEDIF : 1;
        unsigned unimplemented2 : 5;
        unsigned unimplemented2a : 8;
        unsigned ErrorAddressL : 8;
        unsigned ErrorAddressH : 4;
        unsigned unimplemented3 : 4;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_ECCSTA;

// *****************************************************************************
//! DEVID Register

#ifndef MCP2517FD
typedef union _REG_DEVID {

    struct {
        unsigned REV : 4;
        unsigned DEV : 4;
        unsigned unimplemented1 : 8;
        unsigned unimplemented2 : 8;
        unsigned unimplemented3 : 8;
    } bF;
    unsigned long word;
    uint8_t byte[4];
} REG_DEVID;
#endif


// *****************************************************************************
// *****************************************************************************
/* Register Reset Values */

// *****************************************************************************
/* CAN FD Controller */

// Control Register Reset Values up to FIFOs
const static unsigned long canControlResetValues[] = {
    /* Address 0x000 to 0x00C */
    0x04980760, 0x003E0F0F, 0x000E0303, 0x00021000,
    /* Address 0x010 to 0x01C */
    0x00000000, 0x00000000, 0x40400040, 0x00000000,
    /* Address 0x020 to 0x02C */
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    /* Address 0x030 to 0x03C */
    0x00000000, 0x00200000, 0x00000000, 0x00000000,
    /* Address 0x040 to 0x04C */
    0x00000400, 0x00000000, 0x00000000, 0x00000000
};

// FIFO Register Reset Values
const static unsigned long canFifoResetValues[] = {
    0x00600400, 0x00000000, 0x00000000
};

// Filter Control Register Reset Values
const static unsigned long canFilterControlResetValue = 0x00000000;

// Filter and Mask Object Reset Values
const static unsigned long canFilterObjectResetValues[] = {
    0x00000000, 0x00000000
};

// *****************************************************************************
/* MCP25xxFD */

#if defined(MCP2517FD) || defined(MCP2518FD)
const static unsigned long mcp25xxfdControlResetValues[] = {
    0x00000460, 0x00000003, 0x00000000, 0x00000000, 0x00000000
};
#endif


#endif // _DRV_CANFDSPI_REGISTER_H
