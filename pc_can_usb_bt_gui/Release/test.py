import sys
from PyQt5.QtWidgets import QApplication, QMainWindow


app = QApplication(sys.argv)

window = QMainWindow()
window.show() # IMPORTANT!!!!! Windows are hidden by default.

# Start the event loop.
app.exec_()