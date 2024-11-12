from pxr.Usdviewq.qt import QtWidgets, QtCore
import displaylayerplugin.DisplayLayer as DL

class DisplayLayersUI:
    __slots__ = ["__displayLayer", "__path", "__layerNameInput",
                "__table", "usdviewApi", "__layerNameCol"]
                
    def __init__(self, usdviewApi):
        self.__path = "/DisplayLayerContainer"

        stage = usdviewApi.dataModel.stage

        # Check if a Display Layer Prim already exists at the given path
        prim = stage.GetPrimAtPath(self.__path)
        if not prim:
            # Add layer prim to stage
            self.__displayLayer = DL.DisplayLayer.Define(stage, self.__path)
            self.__displayLayer.initialize(stage)
        else:
            self.__displayLayer = DL.DisplayLayer(prim)
            layers = prim.GetCustomDataByKey(self.__displayLayer.getLayersKey())
            self.__displayLayer.initialize(stage, layers)

        self.usdviewApi = usdviewApi

        # Layer Table init
        table_header = ["Layer name", "Visible", "Highlight", "Add selected", \
                        "Remove selected", "Delete"]
        self.__layerNameCol = 0
        self.__table = QtWidgets.QTableWidget()
        self.__table.setColumnCount(len(table_header))

        self.__table.setHorizontalHeaderLabels(table_header)


    """
    Opens the Display Layers UI
    """
    def open_display_layers_UI(self):
        window = self.usdviewApi.qMainWindow
        dialog = QtWidgets.QDialog(window)
        dialog.setWindowTitle("Display Layers")

        dialog.setMinimumWidth((window.size().width()/1.5))

        # add layout to dialog and launch
        layout = self.generate_main_UI_layout(dialog)
        dialog.setLayout(layout)

        dialog.show()

    """
    Generates the layout for the main Display Layers UI
    """
    def generate_main_UI_layout(self, dialog):
        # Add table and bottom bar buttons
        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.__table)
        
        buttons = self.generate_bottom_bar_buttons(dialog)
        layout.addLayout(buttons)

        return layout


    """
    Generates the layout for the buttons in the bar below the table
    """
    def generate_bottom_bar_buttons(self, dialog):
        # Buttons for the bar below the table
        closeButton = QtWidgets.QPushButton("Close")
        createNewLayerButton = QtWidgets.QPushButton("Create new layer")

        buttonLayout = QtWidgets.QHBoxLayout()
        buttonLayout.addStretch(0.5)

        buttonLayout.addWidget(closeButton)
        buttonLayout.addWidget(createNewLayerButton)

        closeButton.clicked.connect(lambda : dialog.close())
        createNewLayerButton.clicked.connect(lambda: self.open_create_new_layer_dialog())
        
        return buttonLayout


    """
    Opens the "Create New Layer" dialog
    """
    def open_create_new_layer_dialog(self):
        window = self.usdviewApi.qMainWindow
        dialog = QtWidgets.QDialog(window)
        dialog.setWindowTitle("Create new layer")

        dialog.setMinimumWidth((window.size().width()/2))

        # add layout to dialog and launch
        layout = self.generate_create_new_layer_layout(dialog)
        dialog.setLayout(layout)

        # Call exec_ because we want the rest of the UI to be inactive until
        # this dialog is closed
        # Because we don't want multiple "Create new layer" dialogs being open
        # at the same time
        dialog.exec_()

    """
    Layout for the "Create New Layer" dialog
    """
    def generate_create_new_layer_layout(self, dialog):
        layerNamelabel = QtWidgets.QLabel("Layer name:")

        # Store layer name input as property because we will need its value
        # when using the DisplayLayer API
        self.__layerNameInput = QtWidgets.QLineEdit()

        buttons = self.generate_create_new_layer_buttons(dialog)

        layout = QtWidgets.QVBoxLayout()

        layout.addWidget(layerNamelabel)
        layout.addWidget(self.__layerNameInput)
        layout.addLayout(buttons)

        return layout

    """
    Buttons for the "Create New Layer" dialog
    """
    def generate_create_new_layer_buttons(self, dialog):
        closeButton = QtWidgets.QPushButton("Close")
        createButton = QtWidgets.QPushButton("Create")

        buttonLayout = QtWidgets.QHBoxLayout()
        buttonLayout.addStretch(0.5)

        buttonLayout.addWidget(closeButton)
        buttonLayout.addWidget(createButton)

        closeButton.clicked.connect(lambda: dialog.close())

        createButton.clicked.connect(lambda: ( \
                self.__displayLayer.createNewLayer(\
                self.__layerNameInput.text()), \
                self.new_layer_added(self.__layerNameInput.text()), \
                dialog.close()
            ))
        
        return buttonLayout

    """
    Updates the table UI with the newly added layer
    """
    def new_layer_added(self, layer_name):
        # Add new row to table
        rowPos = self.__table.rowCount()
        self.__table.insertRow(rowPos)

        # Populate new row
        colPos = 0
        QT_CHECKED_STATE = 2

        # Layer name
        self.__table.setItem(rowPos, colPos, QtWidgets.QTableWidgetItem(layer_name))
        colPos += 1

        # Visibility
        visibilityCheckbox = QtWidgets.QCheckBox()
        visibilityCheckbox.setChecked(True)
        visibilityCheckbox.stateChanged.connect(lambda state, layer = layer_name: \
                        self.__displayLayer.setLayerVisibility( \
                        layer, state == QT_CHECKED_STATE))
        self.__table.setCellWidget(rowPos, colPos, visibilityCheckbox)
        colPos += 1

        # Highlight
        highlightCheckbox = QtWidgets.QCheckBox()
        highlightCheckbox.setChecked(False)
        highlightCheckbox.stateChanged.connect(lambda state, layer = layer_name: \
                        self.__displayLayer.setLayerHighlight( \
                        layer, state == QT_CHECKED_STATE))
        self.__table.setCellWidget(rowPos, colPos, highlightCheckbox)
        colPos += 1

        # Add selected
        addSelectedButton = QtWidgets.QPushButton("Add")
        addSelectedButton.clicked.connect(lambda: \
            self.add_selected_to_layer(layer_name))
        self.__table.setCellWidget(rowPos, colPos, addSelectedButton)
        colPos += 1

        # Remove selected
        removeSelected = QtWidgets.QPushButton("Remove")
        removeSelected.clicked.connect(lambda: \
            self.remove_selected_from_layer(layer_name))
        self.__table.setCellWidget(rowPos, colPos, removeSelected)
        colPos += 1

        # Delete
        deleteButton = QtWidgets.QPushButton("X")
        deleteButton.clicked.connect(lambda: (\
            self.__displayLayer.removeLayer(layer_name), \
            self.layer_removed(layer_name)
        ))
        self.__table.setCellWidget(rowPos, colPos, deleteButton)

    """
    Removes the layer row from the table UI
    """
    def layer_removed(self, layer_name):
        # loop through table until you find the layer
        numRows = self.__table.rowCount()

        for row in range(numRows):
            layer = self.__table.item(row, self.__layerNameCol)

            if layer and layer.text() == layer_name:
                self.__table.removeRow(row) 

    
    """
    Handler for "Add selected to layer" buttons
    """
    def add_selected_to_layer(self, layer_name):
        prims = self.usdviewApi.dataModel.selection.getPrims()

        for prim in prims:
            path = prim.GetPath().pathString
            self.__displayLayer.addItemToLayer(layer_name, path)

    """
    Handler for "Remove selected from layer" buttons
    """
    def remove_selected_from_layer(self, layer_name):
        prims = self.usdviewApi.dataModel.selection.getPrims()

        for prim in prims:
            path = prim.GetPath().pathString
            self.__displayLayer.removeItemFromLayer(layer_name, path)
