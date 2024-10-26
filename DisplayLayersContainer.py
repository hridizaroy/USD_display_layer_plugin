from pxr import DisplayLayer, Sdf, Vt

# TODO: How to handle case where a layer's member is deleted
# The API might help in that case?

# TODO: Maybe create members with a max_size (fixed size array)
    # Maybe double size if full

# TODO: Maybe create another class in the schema tht stores a list of display layers

class DisplayLayersContainer:
    __slots__ = ["__layers", "__numLayers", "__stage", "__basePath"]

    def __init__(self, stage):
        self.__layers = list()
        self.__numLayers = 0
        self.__stage = stage
        self.__basePath = "/DisplayLayers"


    def create_new_layer(self):
        layer_path = Sdf.Path(self.__basePath + "/Layer_" + str(self.__numLayers))
        layer = DisplayLayer.Layer.Define(self.__stage, layer_path)
        
        self.__layers.append(layer)
        self.__numLayers += 1

        self.update_visibilities(self.__numLayers - 1)

    def remove_layer(self, index):
        # TODO: How to update visibility?
        removeLayer = self.__stage.RemovePrimAtPath(self.__layers[index].GetPath())
        self.__layers.pop(index)

        if removeLayer:
            print("Layer removed successfully")

    def add_item_to_layer(self, index, path):
        # TODO: Check if this item is already attached to a layer
        # Maybe use API/rel for this

        if (index < self.__numLayers):
            layer = self.__layers[index]
            members = layer.GetMembersAttr().Get()

            layer.GetMembersAttr().Set(members + Vt.StringArray([path]))

            self.update_visibilities(index)

    def remove_item_from_layer(self, index, path):
        layer = self.__layers[index]
        members = layer.GetMembersAttr().Get()
        
        # Find index of item to remove
        idx_to_remove = members.index(path)

        # If valid index found, remove item
        if idx_to_remove != -1:
            members.pop(idx_to_remove)
            layer.GetMembersAttr().Set(members)

        self.update_visibilities(index)

    def update_visibilities(self, index):
        pass

    def toggle_layer_visibility(self, index):
        pass
