from pxr import Tf
from pxr.Usdviewq.plugin import PluginContainer
from pxr.Usdviewq.common import QtWidgets, QtCore

class DisplayLayersPluginContainer(PluginContainer):
    def registerPlugins(self, plugRegistry, usdviewApi):
        self.displayLayersUIModule = self.deferredImport(".DisplayLayersUI")

        # Store it as property so that we can close and re-open display layers
        # UI with saved state
        self._displayLayersUI = None

        self._openDisplayLayersUI = plugRegistry.registerCommandPlugin(
            "DisplayLayersPluginContainer.openDisplayLayersUI",
            "Open Display Layers UI",
            self.openDisplayLayersUI
        )

    def openDisplayLayersUI(self, usdviewApi):
        # Create new UI object if it doesn't already exists
        if not self._displayLayersUI:
            self._displayLayersUI = self.displayLayersUIModule.DisplayLayersUI(usdviewApi)

        self._displayLayersUI.open_display_layers_UI()
        

    def configureView(self, plugRegistry, plugUIBuilder):
        menu = plugUIBuilder.findOrCreateMenu("Display Layer")
        menu.addItem(self._openDisplayLayersUI)

Tf.Type.Define(DisplayLayersPluginContainer)
