import serial
from PyQt5.QtCore import pyqtSignal, QThread
import sys

class SerialThreadClass(QThread):                           #Create a QTread of the serial call we will use for uart
    NanoMsg=pyqtSignal(str)                                 #signal to send data from this thread

    def __init__(self,parent=None):
        super(SerialThreadClass,self).__init__(parent)
        self.seriport = serial.Serial('COM3', timeout=1)

    def run(self):
        while True:
            InternalTempData =self.seriport.readline()      #create a string to bring in the serial line
            self.NanoMsg.emit(str(InternalTempData))        #Emit the message received from the serial port to the thread signal
            print (InternalTempData)                        #print the serial data received to the console for debuggin

    def sendSerial(self):                                   #create a method for sending codes from the Serial Port
        self.seriport.write(b'A')                           #write an 'A' to the serial port for LED control
