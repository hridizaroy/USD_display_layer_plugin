from pxr import Tf
from pxr.Usdviewq.plugin import PluginContainer
from pxr.Usdviewq.common import QtWidgets, QtCore

from pxr import DisplayLayer


class DisplayLayersPluginContainer(PluginContainer):
    def registerPlugins(self, plugRegistry, usdviewApi):
        displayLayersUI = self.deferredImport(".DisplayLayersUI")

        sendMail = self.deferredImport(".sendMail")

        self._openDisplayLayersUI = plugRegistry.registerCommandPlugin(
            "DisplayLayersPluginContainer.openDisplayLayersUI",
            "Open Display Layers UI",
            displayLayersUI.openDisplayLayersUI
        )

        self._sendMail = plugRegistry.registerCommandPlugin(
            "DisplayLayersPluginContainer.sendMail",
            "Send Mail",
            sendMail.SendMail
        )

    def configureView(self, plugRegistry, plugUIBuilder):
        menu = plugUIBuilder.findOrCreateMenu("Display Layers")
        menu.addItem(self._openDisplayLayersUI)
        menu.addItem(self._sendMail)      

Tf.Type.Define(DisplayLayersPluginContainer)
