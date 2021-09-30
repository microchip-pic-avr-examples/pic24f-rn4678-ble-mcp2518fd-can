from PyQt5.QtCore import QUrl
from PyQt5.QtWidgets import *
from PyQt5.QtWebEngineWidgets import QWebEngineSettings, QWebEngineView, QWebEnginePage
from PyQt5.QtGui import QIcon
import PyQt5
import sys

class MyBrowser(QWebEnginePage):

    def userAgentForUrl(self, url):
        return "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36"

class Web(QWebEngineView):

    def load(self, url):
        self.setUrl(QUrl(url))

    def adjustTitle(self):
        self.setWindowTitle(self.title())

    def disableJS(self):
        settings = QWebEngineSettings.globalSettings()
        settings.setAttribute(QWebEngineSettings.JavascriptEnabled, False)

class Main(QWidget):

    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Name')
        self.setWindowIcon(QIcon('icon.png'))

        web = Web()

        web.load("https://www.microchip.com/treelinktool/")

        self.btn = QPushButton('Button', self)
        self.btn.resize(self.btn.sizeHint())
        lay = QVBoxLayout(self)
        lay.addWidget(self.btn)
        lay.addWidget(web)

app = QApplication(sys.argv)
main = Main()
main.show()
app.exec_()