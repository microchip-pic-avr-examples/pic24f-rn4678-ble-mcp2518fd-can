
#include "drv_canfdspi_api.h"
#include "drv_canfdspi_register.h"
#include "User_2518FD_App.h"

#define DRV_CANFDSPI_INDEX_0  0   

unsigned int    ramInitialized = 0 ;

CAN_CONFIG config;
CAN_OPERATION_MODE opMode;

CAN_TX_FIFO_CONFIG txConfig;
CAN_TX_FIFO_EVENT txFlags;
CAN_TX_MSGOBJ txObj;
uint8_t txd[MAX_DATA_BYTES];

CAN_RX_FIFO_CONFIG rxConfig;
REG_CiFLTOBJ fObj;
REG_CiMASK mObj;
CAN_RX_FIFO_EVENT rxFlags;
CAN_RX_MSGOBJ rxObj;
uint8_t rxd[MAX_DATA_BYTES];                                                    // The default MAX_DATA_BYTES is 64 - Calvin 

// CAN_BITTIME_SETUP selectedBitTime = CAN_500K_2M;
CAN_BITTIME_SETUP selectedBitTime = CAN_125K_500K ;     // Changed by Calvin


uint8_t tec;
uint8_t rec;
CAN_ERROR_STATE errorFlags;


extern  struct  
{
    unsigned char       Data_Received ;
    unsigned int          SID ;
    unsigned char       EID_FLAG ;
    unsigned long       SID_EID ;
    unsigned char       DLC ;
    unsigned char       data[8] ;  
} CAN_Receive_Queue_StdCAN ;

void    APP_CAN_Transmit_1(void)
{
                txObj.bF.id.SIDL = 0x200 % 256 ;
                txObj.bF.id.SIDH = 0x200/256 ; 

                txObj.bF.ctrl.DLC = CAN_DLC_4;
                txObj.bF.ctrl.IDE = 0;
                txObj.bF.ctrl.BRS = 0;
                txObj.bF.ctrl.FDF = 0;

                txd[0] = 0x55;

                // Clear other data bytes
                txd[1] = 0xaa;
                txd[2] = 0x88;
                txd[3] = 0x99;

                Nop();
                Nop();
                APP_TransmitMessageQueue();
}

void APP_CANFDSPI_Init(void)
{
    // Reset device
    DRV_CANFDSPI_Reset(DRV_CANFDSPI_INDEX_0);

    // Enable ECC and initialize RAM
    // DRV_CANFDSPI_EccEnable(DRV_CANFDSPI_INDEX_0);

//    if (!ramInitialized) {
        DRV_CANFDSPI_RamInit(DRV_CANFDSPI_INDEX_0, 0xff);
//        ramInitialized = true;
//    }

    // Configure device
    DRV_CANFDSPI_ConfigureObjectReset(&config);
    config.IsoCrcEnable = 1;
    config.StoreInTEF = 0;

    DRV_CANFDSPI_Configure(DRV_CANFDSPI_INDEX_0, &config);

    // Setup TX FIFO
    DRV_CANFDSPI_TransmitChannelConfigureObjectReset(&txConfig);
    txConfig.FifoSize = 7;
    txConfig.PayLoadSize = CAN_PLSIZE_64;
    txConfig.TxPriority = 1;

    DRV_CANFDSPI_TransmitChannelConfigure(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, &txConfig);

    // Setup RX FIFO
    DRV_CANFDSPI_ReceiveChannelConfigureObjectReset(&rxConfig);
    rxConfig.FifoSize = 15;
    rxConfig.PayLoadSize = CAN_PLSIZE_64;

    DRV_CANFDSPI_ReceiveChannelConfigure(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, &rxConfig);

    // Setup RX Filter
    fObj.word = 0;
    fObj.bF.SIDL = 0xda;
    fObj.bF.SIDH = 0x00;
    fObj.bF.EXIDE = 0;
    fObj.bF.EIDL = 0x00;
    fObj.bF.EIDH = 0x00;
    fObj.bF.EIDU = 0x00;

    DRV_CANFDSPI_FilterObjectConfigure(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, &fObj.bF);

    // Setup RX Mask
    mObj.word = 0;
    mObj.bF.MSIDL = 0x0;
    mObj.bF.MSIDH = 0x0;
    mObj.bF.MIDE = 0; // Only allow standard IDs
    mObj.bF.MEIDL = 0x0;
    mObj.bF.MEIDH = 0x0;
    mObj.bF.MEIDU = 0x0; 
    DRV_CANFDSPI_FilterMaskConfigure(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, &mObj.bF);

    // Link FIFO and Filter
    DRV_CANFDSPI_FilterToFifoLink(DRV_CANFDSPI_INDEX_0, CAN_FILTER0, APP_RX_FIFO, true);

    // Setup Bit Time
    DRV_CANFDSPI_BitTimeConfigure(DRV_CANFDSPI_INDEX_0, selectedBitTime, CAN_SSP_MODE_AUTO, CAN_SYSCLK_20M);

    // Setup Transmit and Receive Interrupts
//    DRV_CANFDSPI_GpioModeConfigure(DRV_CANFDSPI_INDEX_0, GPIO_MODE_INT, GPIO_MODE_INT);
//    DRV_CANFDSPI_TransmitChannelEventEnable(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, CAN_TX_FIFO_NOT_FULL_EVENT);
//    DRV_CANFDSPI_ReceiveChannelEventEnable(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, CAN_RX_FIFO_NOT_EMPTY_EVENT);
//    DRV_CANFDSPI_ModuleEventEnable(DRV_CANFDSPI_INDEX_0, CAN_TX_EVENT | CAN_RX_EVENT);

    // Select Normal Mode
    DRV_CANFDSPI_OperationModeSelect(DRV_CANFDSPI_INDEX_0, CAN_CLASSIC_MODE);
}


void APP_TransmitMessageQueue()
{

    uint8_t attempts = MAX_TXQUEUE_ATTEMPTS;

    // Check if FIFO is not full
    do {
#ifdef APP_USE_TX_INT
        Delay_us(50);
#else
        DRV_CANFDSPI_TransmitChannelEventGet(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, &txFlags);
#endif
        if (attempts == 0) {
            Nop();
            Nop();
            DRV_CANFDSPI_ErrorCountStateGet(DRV_CANFDSPI_INDEX_0, &tec, &rec, &errorFlags);
            return;
        }
        attempts--;
    }
#ifdef APP_USE_TX_INT
    while (!APP_TX_INT());
#else
    while (!(txFlags & CAN_TX_FIFO_NOT_FULL_EVENT));
#endif

    // Load message and transmit
    uint8_t n = DRV_CANFDSPI_DlcToDataBytes(txObj.bF.ctrl.DLC);

    DRV_CANFDSPI_TransmitChannelLoad(DRV_CANFDSPI_INDEX_0, APP_TX_FIFO, &txObj, txd, n, true);

}


// ------------------------------------------------------------------------------------------------------------------------------------
// -----------      Only Receive Standard CAN Message since there is no CAN FD node for evaluation
//                      Calvin - 2021.05.20 
// -------------------------------------------------------------------------------------------------------------------------------------
APP_STATES APP_ReceiveMessage_Tasks()
{
    APP_STATES nextState;
    
    //    uint8_t i;
    // Normally we got to APP_STATE_PAYLOAD
    nextState = APP_STATE_PAYLOAD;
    // 
    // Check if FIFO is not empty
    // 
    DRV_CANFDSPI_ReceiveChannelEventGet(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, &rxFlags);
    if (rxFlags & CAN_RX_FIFO_NOT_EMPTY_EVENT) {

        // Get message
        DRV_CANFDSPI_ReceiveMessageGet(DRV_CANFDSPI_INDEX_0, APP_RX_FIFO, &rxObj, rxd, MAX_DATA_BYTES);
        
        CAN_Receive_Queue_StdCAN.DLC = rxObj.bF.ctrl.DLC ;
        CAN_Receive_Queue_StdCAN.SID =  (rxObj.bF.id.SIDH * 256) + rxObj.bF.id.SIDL ;
        CAN_Receive_Queue_StdCAN.data[0] = rxd[0];
        CAN_Receive_Queue_StdCAN.data[1] = rxd[1];
        CAN_Receive_Queue_StdCAN.data[2] = rxd[2];
        CAN_Receive_Queue_StdCAN.data[3] = rxd[3];
        CAN_Receive_Queue_StdCAN.data[4] = rxd[4];
        CAN_Receive_Queue_StdCAN.data[5] = rxd[5];
        CAN_Receive_Queue_StdCAN.data[6] = rxd[6];
        CAN_Receive_Queue_StdCAN.data[7] = rxd[7];
        CAN_Receive_Queue_StdCAN.Data_Received = 1 ;
//       switch (rxObj.bF.id.SID) {
//            case TX_REQUEST_ID:
//
//                // Check for TX request command
//                Nop();
//                Nop();
//                txObj.bF.id.SID = TX_RESPONSE_ID;
//
//                txObj.bF.ctrl.DLC = rxObj.bF.ctrl.DLC;
//                txObj.bF.ctrl.IDE = rxObj.bF.ctrl.IDE;
//                txObj.bF.ctrl.BRS = rxObj.bF.ctrl.BRS;
//                txObj.bF.ctrl.FDF = rxObj.bF.ctrl.FDF;
//
//                for (i = 0; i < MAX_DATA_BYTES; i++) txd[i] = rxd[i];
//
//                APP_TransmitMessageQueue();
//                break;
//
//            case LED_STATUS_ID:
//                // Check for LED command
//                // APP_LED_Write(rxd[0]);
//                break;
//
//            case BITTIME_SET_ID:
//                // Check for Bit Time Setup command
////                Nop();
////                Nop();
////                selectedBitTime = rxd[0];
////                nextState = APP_STATE_REQUEST_CONFIG;
//                break;
//
//            case BUTTON_STATUS_ID:
//                // Check for Button Status command
//                // This can be used to test two EVBs without a CAN tool
//                // S1 turns on D1&D2
//                if (rxd[0] & 0x01) {
//                    APP_LED_Set(APP_LED_D1);
//                    APP_LED_Set(APP_LED_D2);
//                } else {
//                    APP_LED_Clear(APP_LED_D1);
//                    APP_LED_Clear(APP_LED_D2);
//                }
//
//                // S2 turns on D3&D4
//                if (rxd[0] & 0x02) {
//                    APP_LED_Set(APP_LED_D3);
//                    APP_LED_Set(APP_LED_D4);
//                } else {
//                    APP_LED_Clear(APP_LED_D3);
//                    APP_LED_Clear(APP_LED_D4);
//                }
//
//                // S3 turns on D5&D6
//                if (rxd[0] & 0x04) {
//                    APP_LED_Set(APP_LED_D5);
//                    APP_LED_Set(APP_LED_D6);
//                } else {
//                    APP_LED_Clear(APP_LED_D5);
//                    APP_LED_Clear(APP_LED_D6);
//                }
//
//                // S1 turns on D7&D8
//                if (rxd[0] & 0x08) {
//                    APP_LED_Set(APP_LED_D7);
//                    APP_LED_Set(APP_LED_D8);
//                } else {
//                    APP_LED_Clear(APP_LED_D7);
//                    APP_LED_Clear(APP_LED_D8);
//                }
//                break;
//
//            case PAYLOAD_ID:
//                // Check for Payload command
//                Nop();
//                Nop();
//                payload.On = rxd[0];
//                payload.Dlc = rxd[1];
//                if (rxd[2] == 0) payload.Mode = true;
//                else payload.Mode = false;
//                payload.Counter = 0;
//                payload.Delay = rxd[3];
//                payload.BRS = rxd[4];

//                break;
//
//            case BITTIME_CFG_GET_ID:
//                APP_TransmitBitTimeConfig(BITTIME_CFG_125K_ID);
//                APP_TransmitBitTimeConfig(BITTIME_CFG_250K_ID);
//                APP_TransmitBitTimeConfig(BITTIME_CFG_500K_ID);
//                APP_TransmitBitTimeConfig(BITTIME_CFG_1M_ID);
//                break;
//        }
    }

    //    APP_LED_Clear(APP_RX_LED);

    return nextState;
}

