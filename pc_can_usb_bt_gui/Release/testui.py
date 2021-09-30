import sys
from PyQt5.QtWidgets import QApplication, QDialog, QMainWindow
import DemoGUI

class MainClass (QDialog, DemoGUI.Ui_Dialog):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

if __name__=="__main__":
    app = QApplication(sys.argv)

    window = MainClass()
    window.show() # IMPORTANT!!!!! Windows are hidden by default.

    # Start the event loop.
    app.exec_()