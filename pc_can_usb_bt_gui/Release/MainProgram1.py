from PyQt5.QtWidgets import QApplication, QDialog
import sys
import DemoGUI

class MainClass(QDialog, DemoGUI.Ui_Dialog):    #Pull in the Gui that was created
    def __init__(self):                         #Start the instance of the Gui
        super().__init__()                      #Pull in the Gui applicaiton proxy to be used in this script
        self.setupUi(self)                      #Run the Gui setup code to define the display
        self.pushButton.clicked.connect(self.btnClickedEvent)

    def btnClickedEvent(self):
        print("Pressing Button")                #this is a print to the console

if __name__=="__main__":
    app = QApplication(sys.argv)

    window = MainClass()
    window.show() # IMPORTANT!!!!! Windows are hidden by default.

    # Start the event loop.
    app.exec_()                            #Exectute the application that runs the setCentralWidget
