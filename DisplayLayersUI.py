import DisplayLayersContainer

from pxr.Usdviewq.qt import QtWidgets, QtCore

class DisplayLayersUI:
    __slots__ = ["__displayLayersContainer", "__layerNameInput", "__table", \
                "usdviewApi", "__layerNameCol"]
    def __init__(self, usdviewApi):
        self.__displayLayersContainer = \
            DisplayLayersContainer.DisplayLayersContainer(usdviewApi.dataModel.stage)
        self.__displayLayersContainer.register_observer(self)

        self.usdviewApi = usdviewApi

        table_header = ["Layer name", "Layer color", "Visible", "Add selected", \
                        "Remove selected", "Highlight", "Delete"]
        self.__layerNameCol = 0
        self.__table = QtWidgets.QTableWidget()
        self.__table.setColumnCount(len(table_header))

        self.__table.setHorizontalHeaderLabels(table_header)


    def open_display_layers_UI(self):
        window = self.usdviewApi.qMainWindow
        dialog = QtWidgets.QDialog(window)
        dialog.setWindowTitle("Display Layers")

        dialog.setMinimumWidth((window.size().width()/1.5))

        # add layout to dialog and launch
        layout = self.generate_main_UI_layout(dialog)
        dialog.setLayout(layout)

        dialog.exec_()

    def generate_main_UI_layout(self, dialog):
        # VBox for overall
        # Table Widget, HBox containing buttons
        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.__table)
        
        buttons = self.generate_bottom_bar_buttons(dialog)
        layout.addLayout(buttons)

        return layout


    def generate_bottom_bar_buttons(self, dialog):
        # create buttons
        closeButton = QtWidgets.QPushButton("Close")
        createNewLayerButton = QtWidgets.QPushButton("Create new layer")

        buttonLayout = QtWidgets.QHBoxLayout()
        buttonLayout.addStretch(0.5)

        buttonLayout.addWidget(closeButton)
        buttonLayout.addWidget(createNewLayerButton)

        closeButton.clicked.connect(lambda : dialog.close())
        createNewLayerButton.clicked.connect(lambda: self.open_create_new_layer_dialog())
        
        return buttonLayout


    def open_create_new_layer_dialog(self):
        window = self.usdviewApi.qMainWindow
        dialog = QtWidgets.QDialog(window)
        dialog.setWindowTitle("Create new layer")

        dialog.setMinimumWidth((window.size().width()/2))

        # add layout to dialog and launch
        layout = self.generate_create_new_layer_layout(dialog)
        dialog.setLayout(layout)

        dialog.exec_()

    def generate_create_new_layer_layout(self, dialog):
        layerNamelabel = QtWidgets.QLabel("Layer name:")
        self.__layerNameInput = QtWidgets.QLineEdit()

        buttons = self.generate_create_new_layer_buttons(dialog)

        layout = QtWidgets.QVBoxLayout()

        layout.addWidget(layerNamelabel)
        layout.addWidget(self.__layerNameInput)
        layout.addLayout(buttons)

        return layout

    def generate_create_new_layer_buttons(self, dialog):
        closeButton = QtWidgets.QPushButton("Close")
        createButton = QtWidgets.QPushButton("Create")

        buttonLayout = QtWidgets.QHBoxLayout()
        buttonLayout.addStretch(0.5)

        buttonLayout.addWidget(closeButton)
        buttonLayout.addWidget(createButton)

        closeButton.clicked.connect(lambda: dialog.close())
        createButton.clicked.connect(lambda: ( \
                self.__displayLayersContainer.create_new_layer(\
                self.__layerNameInput.text()), \
                dialog.close()
            ))
        
        return buttonLayout

    def new_layer_added(self, layer_name):
        # Add new row to table
        rowPos = self.__table.rowCount()
        self.__table.insertRow(rowPos)

        # Populate new row
        colPos = 0

        # Layer name
        self.__table.setItem(rowPos, colPos, QtWidgets.QTableWidgetItem(layer_name))
        colPos += 1

        # Color
        self.__table.setItem(rowPos, colPos, QtWidgets.QTableWidgetItem("stub"))
        colPos += 1

        # Visibility
        visibilityCheckbox = QtWidgets.QTableWidgetItem()
        visibilityCheckbox.setFlags(visibilityCheckbox.flags() | QtCore.Qt.ItemIsUserCheckable)
        visibilityCheckbox.setCheckState(QtCore.Qt.Checked)
        self.__table.setItem(rowPos, colPos, visibilityCheckbox)
        colPos += 1

        # Add selected
        self.__table.setItem(rowPos, colPos, QtWidgets.QTableWidgetItem("stub"))
        colPos += 1

        # Remove selected
        self.__table.setItem(rowPos, colPos, QtWidgets.QTableWidgetItem("stub"))
        colPos += 1

        # Highlight
        highlightCheckbox = QtWidgets.QTableWidgetItem()
        highlightCheckbox.setFlags(highlightCheckbox.flags() | QtCore.Qt.ItemIsUserCheckable)
        highlightCheckbox.setCheckState(QtCore.Qt.Unchecked)
        self.__table.setItem(rowPos, colPos, highlightCheckbox)
        colPos += 1

        # Delete
        deleteButton = QtWidgets.QPushButton("X")
        deleteButton.clicked.connect(lambda: \
            self.__displayLayersContainer.remove_layer(layer_name))
        self.__table.setCellWidget(rowPos, colPos, deleteButton)

    def layer_removed(self, layer_name):
        # loop through table until you find the layer
        numRows = self.__table.rowCount()

        for row in range(numRows):
            layer = self.__table.item(row, self.__layerNameCol)

            if layer and layer.text() == layer_name:
                self.__table.removeRow(row) 
