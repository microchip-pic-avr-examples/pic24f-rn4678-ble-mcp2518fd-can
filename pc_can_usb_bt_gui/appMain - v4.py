# embedded setting : id(0x00da) / baudrate(125kbps)
import serial, sys

from PyQt5.QtWidgets import  QApplication, QDialog, QMessageBox
from PyQt5.QtCore import pyqtSignal, QThread

from ui_dialog import Ui_Dialog

## define
COMMAND_RX_RTC = 0x01
COMMAND_RX_CAN = 0x04
COMMAND_TX_CAN = 0x05
LEN_COMMAND_RX_RTC = 7
LEN_COMMAND_RX_CAN = 15
LEN_COMMAND_TX_CAN = 15


ur_tx_trigger = False
buf_ur_tx = bytearray(50)

class special_block():
    global buf_ur_rx
    buf_ur_rx = bytearray(50)
    global len_buf_temp


## class - serial
class SerialThreadClass(QThread):                           #Create a QTread of the serial call we will use for uart
    ur_rx_frame_signal=pyqtSignal()                                 #signal to send data from this thread
    
    def __init__(self,parent=None):
        super(SerialThreadClass,self).__init__(parent)
        self.seriport = serial.Serial('COM14', baudrate=921600, timeout=0.01)
        self.ur_rx_frame_signal.connect(do_urrx)

    def run(self):
        global ur_tx_trigger
        while True:
           if(self.seriport.inWaiting() > 0):
               special_block.buf_ur_rx = self.seriport.read(30)
               special_block.len_buf_temp = len(special_block.buf_ur_rx)
               # print(special_block.buf_ur_rx.hex())
               self.ur_rx_frame_signal.emit()
               
           elif(ur_tx_trigger==True):
               ur_tx_trigger=False
               print("!!!got it!!!")
               self.seriport.write(buf_ur_tx)


## class - ui
class QmyDialog(QDialog): 
   def __init__(self, parent=None):
      super().__init__(parent)   #呼叫父類別建構函數，建立窗體
      self.ui=Ui_Dialog()        #建立UI物件
      self.ui.setupUi(self)      #建構UI界面 
      self.ui.lcdNumber_sec.setStyleSheet('background:red')
      # self.ui.button_clr_rx.clicked.connect(self.do_clear_can_list)
      self.ui.button_tx0.clicked.connect(do_urtx)
     
      #Start and instance of the serail tread in the main applicaiton
      self.SerialLink=SerialThreadClass()         
      self.SerialLink.start()   

   # def do_clear_can_list(self):
   #     self.ui.listWidget.clear()


## emit function - got the data from uart
global do_urrx
def do_urrx():
    # print(special_block.len_buf_temp)
    # if the received buffer is not just 1 packet
    num_bufs = 1
    if(special_block.len_buf_temp > (special_block.buf_ur_rx[2]+3)):
       num_bufs = 2
    offset = 0
    for i in range(num_bufs):
       # print(special_block.buf_ur_rx[offset+2])
       if(special_block.buf_ur_rx[offset+3] == COMMAND_RX_RTC):
           ascii_buf = str(special_block.buf_ur_rx[offset+4])+":"+str(
               special_block.buf_ur_rx[offset+5])+":"+str(special_block.buf_ur_rx[offset+6])
           mainform.ui.lcdNumber_sec.display(ascii_buf)
  
       elif(special_block.buf_ur_rx[offset+3] == COMMAND_RX_CAN):
           CAN_ID=special_block.buf_ur_rx[offset+4]+special_block.buf_ur_rx[offset+5]*256
           CAN_LEN=special_block.buf_ur_rx[offset+6]
           ascii_buf="ID:"+hex(CAN_ID)+" len:"+hex(CAN_LEN)
           for j in range(CAN_LEN):
               ascii_buf += " "+hex(special_block.buf_ur_rx[offset+7+j])
           mainform.ui.listWidget.addItem(ascii_buf)
       offset += special_block.buf_ur_rx[offset+2]+3


## emit function - got the trigger to send the uart
global do_urtx
def do_urtx():
    global ur_tx_trigger
    
    # print(type(mainform.ui.table_tx.item(0,0)))
    # print(type(mainform.ui.table_tx.item(0,1).text()))
    tmp_str=mainform.ui.table_tx.item(0,0).text()
    tx_can_id=int(tmp_str, base=16)
    if(tx_can_id > 0x7ff):
        # print("Error ID")
        msgBox.setText("Error ID") # 加入文字
        msgBox.show() # 顯示 msgBox
        return
    print(tx_can_id)
    
    tmp_str=mainform.ui.table_tx.item(0,1).text()
    tx_can_dlc=int(tmp_str, base=16)
    if(tx_can_dlc>8) or (tx_can_dlc==0):
        # print("Error DLC")
        msgBox.setText("Error DLC") # 加入文字
        msgBox.show() # 顯示 msgBox
        return
    print(tx_can_dlc)

    for i in range(tx_can_dlc):
        tmp_str=mainform.ui.table_tx.item(0,2+i).text()
        tx_can_int=int(tmp_str, base=16)
        tx_can_byte=tx_can_int.to_bytes(1, byteorder="little")
        print("tx_can_int:{} ; tx_can_byte:{}".format(tx_can_int, tx_can_byte))
        print("tx_can_byte:{} ; buf_ur_tx:{}".format(type(tx_can_byte), type(buf_ur_tx)))
        print("i:{}".format(i))
        buf_ur_tx[7+i]=tx_can_int
        # buf_ur_tx[7+i]=tx_can_byte[0]

        
    buf_ur_tx[0]=0x55
    buf_ur_tx[1]=0xaa
    buf_ur_tx[2]=12    # len field : 55 aa len + cmd sidL sidH dlc 8bytes
    buf_ur_tx[3]=0x05  # cmd
    buf_ur_tx[4]=tx_can_id % 256
    buf_ur_tx[5]=(tx_can_id >>8)
    buf_ur_tx[6]=tx_can_dlc
    del(buf_ur_tx[15:])
    # print("len of buf_ur_tx:{}".format(len(buf_ur_tx)))
    # print(buf_ur_tx.hex())
    ur_tx_trigger=True
   

## init & go
app = QApplication(sys.argv) 
msgBox = QMessageBox()  # 建立一個 message Box 對話框
msgBox.setIcon(QMessageBox.Information) # msgBox 加入一個 "information" icon

mainform=QmyDialog()       
mainform.show()               
sys.exit(app.exec_()) 
