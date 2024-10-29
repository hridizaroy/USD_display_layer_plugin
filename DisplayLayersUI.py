import DisplayLayersContainer

from pxr.Usdviewq.qt import QtWidgets, QtCore

# TODO: Consider making DisplayLayersUI a child of QMainWindow?

class ButtonThread(QtCore.QThread):
    finished = QtCore.Signal()

    def __init__(self, function, args):
        super().__init__()
        self.function = function
        self.args = args

    def run(self):
        self.function(*self.args)
        self.finished.emit()

class DisplayLayersUI:
    __slots__ = ["__displayLayersContainer", "__layerNameInput", "__table", \
                "usdviewApi", "__layerNameCol", "__activeThreads"]
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
        self.__activeThreads = []


    def open_display_layers_UI(self):
        window = self.usdviewApi.qMainWindow
        dialog = QtWidgets.QDialog(window)
        dialog.setWindowTitle("Display Layers")

        dialog.setMinimumWidth((window.size().width()/1.5))

        # add layout to dialog and launch
        layout = self.generate_main_UI_layout(dialog)
        dialog.setLayout(layout)

        dialog.show()

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

        dialog.show()

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
        visibilityCheckbox = QtWidgets.QCheckBox()
        visibilityCheckbox.setChecked(True)
        visibilityCheckbox.stateChanged.connect(lambda state, layer = layer_name: \
                                self.visibility_checkbox_clicked(state, layer))
        self.__table.setCellWidget(rowPos, colPos, visibilityCheckbox)
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

        # Highlight
        highlightCheckbox = QtWidgets.QCheckBox()
        highlightCheckbox.setChecked(False)
        self.__table.setCellWidget(rowPos, colPos, highlightCheckbox)
        colPos += 1

        # Delete
        deleteButton = QtWidgets.QPushButton("X")
        deleteButton.clicked.connect(lambda: \
            self.__displayLayersContainer.remove_layer(layer_name))
        self.__table.setCellWidget(rowPos, colPos, deleteButton)

    def visibility_checkbox_clicked(self, state, layer_name):
        QT_CHECKED = 2
        thread = ButtonThread(self.__displayLayersContainer.set_layer_visibility, \
                    (layer_name, state == 2))

        thread.finished.connect(lambda: self.thread_finished(thread))

        self.__activeThreads.append(thread)
        thread.start()

    def thread_finished(self, thread):
        if thread in self.__activeThreads:
            self.__activeThreads.remove(thread)
        
        thread.deleteLater()

    def layer_removed(self, layer_name):
        # loop through table until you find the layer
        numRows = self.__table.rowCount()

        for row in range(numRows):
            layer = self.__table.item(row, self.__layerNameCol)

            if layer and layer.text() == layer_name:
                self.__table.removeRow(row) 

    
    def add_selected_to_layer(self, layer_name):
        prims = self.usdviewApi.dataModel.selection.getPrims()

        for prim in prims:
            path = prim.GetPath().pathString
            self.__displayLayersContainer.add_item_to_layer(layer_name, path)

    def remove_selected_from_layer(self, layer_name):
        prims = self.usdviewApi.dataModel.selection.getPrims()

        for prim in prims:
            path = prim.GetPath().pathString
            self.__displayLayersContainer.remove_item_from_layer(layer_name, path)
