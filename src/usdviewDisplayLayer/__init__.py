from pxr import Tf
from pxr.Usdviewq.plugin import PluginContainer
from pxr.Usdviewq.common import QtWidgets, QtCore

from pxr import DisplayLayer


class DisplayLayersPluginContainer(PluginContainer):
    def registerPlugins(self, plugRegistry, usdviewApi):
        self.displayLayersUIModule = self.deferredImport(".DisplayLayersUI")

        sendMail = self.deferredImport(".sendMail")

        self._displayLayersUI = None

        self._openDisplayLayersUI = plugRegistry.registerCommandPlugin(
            "DisplayLayersPluginContainer.openDisplayLayersUI",
            "Open Display Layers UI",
            self.openDisplayLayersUI
        )

        self._sendMail = plugRegistry.registerCommandPlugin(
            "DisplayLayersPluginContainer.sendMail",
            "Send Mail",
            sendMail.SendMail
        )

    def openDisplayLayersUI(self, usdviewApi):
        if not self._displayLayersUI:
            self._displayLayersUI = self.displayLayersUIModule.DisplayLayersUI(usdviewApi)

        self._displayLayersUI.open_display_layers_UI()
        

    def configureView(self, plugRegistry, plugUIBuilder):
        menu = plugUIBuilder.findOrCreateMenu("Display Layers")
        menu.addItem(self._openDisplayLayersUI)
        menu.addItem(self._sendMail)      

Tf.Type.Define(DisplayLayersPluginContainer)
