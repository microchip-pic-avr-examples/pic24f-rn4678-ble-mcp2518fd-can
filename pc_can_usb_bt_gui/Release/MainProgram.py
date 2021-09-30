from PyQt5.QtWidgets import QApplication, QDialog
import sys
import DemoGUI
from SerThread import SerialThreadClass


class MainClass(QDialog, DemoGUI.Ui_Dialog):                    #Pull in the Gui that was created
    def __init__(self):                                         #Start the instance of the Gui
        super().__init__()                                      #Pull in the Gui applicaiton proxy to be used in this script
        self.setupUi(self)                                      #Run the Gui setup code to define the display
        self.pushButton.clicked.connect(self.btnClickedEvent)

        self.SerialLink=SerialThreadClass()                     #Start and instance of the serail tread in the main applicaiton
        self.SerialLink.NanoMsg.connect(self.textEdit.append)   #Connect the textedit box to the serial pipe message incoming

        self.SerialLink.start()                                 #Run the serial tread program

    def btnClickedEvent(self):
        self.SerialLink.sendSerial()                            #Tell the serial tread to start the sendSerial Command
        print("Pressing Button")                                #this is a print to the console for testing


    


SerialTreadApp = QApplication(sys.argv)                         #This is the name of the application being executed
window=MainClass()                                              #This is the widget created for the appliction overlay
window.show()                                                   #Show the dialog QWidget
SerialTreadApp.exec_()                                          #Exectute the application that runs the setCentralWidget
