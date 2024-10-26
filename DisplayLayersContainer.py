from pxr import DisplayLayer, Sdf, Vt, UsdGeom

# TODO: How to handle case where a layer's member is deleted
# The API might help in that case?

# TODO: Maybe create members with a max_size (fixed size array)
    # Maybe double size if full

# TODO: Maybe create another class in the schema tht stores a list of display layers

# TODO: How to get pre-existing display layers

# TODO: Is saving layers attribute worth it? I'm doing it for efficiency purposes
# To avoid the conversion overhead of GetCustomDataByKey

# TODO: is the displaylayer custom schema justified? It basically has nothing
# Is there some alternative?

# TODO: Is prim.GetVisibilityAttr().Set(UsdGeom.Tokens.inherited) a good way to revert visibility?

# TODO: Issues when renaming paths - would it have been solved if I stored rels


class DisplayLayersContainer:
    __slots__ = ["__stage", "__path", "__prim", "__layers", "__layersKey", \
                    "__membersKey", "__visibilityKey"]

    def __init__(self, stage):
        self.__stage = stage
        self.__path = "/DisplayLayers"
        self.__layersKey = "layers"
        self.__membersKey = "members"
        self.__visibilityKey = "isVisible"

        # Check if a display layer prim already exists
        if not stage.GetPrimAtPath(self.__path):
            # Add layer prim to stage
            DisplayLayer.DisplayLayer.Define(stage, self.__path)

        # Convert to UsdPrim to be able to use customData
        self.__prim = stage.GetPrimAtPath(self.__path)

        # Check if layers dictionary already exists
        self.__layers = self.__prim.GetCustomDataByKey(self.__layersKey)

        # Create layers dictionary if it doesn't exist
        if self.__layers is None:
            self.__layers = dict()
            self.update_custom_data()
        else:
            # Update visibilities if layers dictionary already exists
            self.update_all_visibilities()


    def create_new_layer(self, layer_name):
        # Check if layer with same name already exists
        if layer_name in self.__layers:
            pass # TODO: Throw error
            return

        self.__layers[layer_name] = { \
            self.__membersKey: dict(), \
            self.__visibilityKey: True \
        }

        self.update_custom_data()


    def remove_layer(self, layer_name):
        # TODO: How to update visibility?

        if layer_name not in self.__layers:
            return # TODO: Throw error

        self.revert_visibility_of_layer(layer_name)
        
        del self.__layers[layer_name]
        self.update_custom_data()


    def add_item_to_layer(self, layer_name, path):
        # Maybe use API/rel for this

        # Check if layer exists
        if layer_name not in self.__layers:
            pass # TODO: Throw error
            return

        # Check if prim is already in a display layer
        already_added = False
        for layer_dict in self.__layers.values():
            if path in layer_dict[self.__membersKey]:
                already_added = True
                break

        if already_added:
            # TODO: Throw error, mention which layer it is added to
            return

        prim = self.__stage.GetPrimAtPath(path)
        if not prim:
            # TODO: Throw error
            return

        # Make sure only UsdGeom prims can be added
        if not UsdGeom.Imageable(prim):
            # TODO: Throw error
            return
        
        self.__layers[layer_name][self.__membersKey][path] = True
        self.update_custom_data()
        # TODO: Show success message

        self.update_visibility_of_member(layer_name, path)


    def remove_item_from_layer(self, layer_name, path):
        # Check if layer exists
        if layer_name not in self.__layers:
            pass # TODO: Throw error
            return

        if path not in self.__layers[layer_name][self.__membersKey]:
            # TODO: Throw error
            return

        del self.__layers[layer_name][self.__membersKey][path]
        self.update_custom_data()
        # TODO: Show success

        self.revert_visibility_of_member(path)

    
    def update_all_visibilities(self):
        for layer_name in self.__layers.keys():
            self.update_visibilities(layer_name)


    def update_visibilities(self, layer_name):
        # Check if layer exists
        if layer_name not in self.__layers:
            pass # TODO: Throw error
            return

        visibilityToken = self.get_visibility_token(
            self.__layers[layer_name][self.__visibilityKey])

        self.set_visibilities(layer_name, visibilityToken)


    def get_visibility_token(self, isVisible):
        if isVisible:
            return UsdGeom.Tokens.visible
        else:
            return UsdGeom.Tokens.invisible


    def update_visibility_of_member(self, layer_name, path):
        # Check if layer exists
        if layer_name not in self.__layers:
            pass # TODO: Throw error
            return

        if path not in self.__layers[layer_name][self.__membersKey]:
            # TODO: Throw error
            return

        prim = self.__stage.GetPrimAtPath(path)

        if not prim or not UsdGeom.Imageable(prim):
            # TODO: Throw error
            return

        visibilityToken = self.get_visibility_token(
            self.__layers[layer_name][self.__visibilityKey])
        
        prim = UsdGeom.Imageable(prim)
        prim.GetVisibilityAttr().Set(visibilityToken)


    def toggle_layer_visibility(self, layer_name):
        # Check if layer exists
        if layer_name not in self.__layers:
            pass # TODO: Throw error
            return

        current_visibility = self.__layers[layer_name][self.__visibilityKey]

        self.set_layer_visibility(layer_name, not current_visibility)


    def set_layer_visibility(self, layer_name, isVisible):
        # Check if layer exists
        if layer_name not in self.__layers:
            pass # TODO: Throw error
            return

        self.__layers[layer_name][self.__visibilityKey] = isVisible

        self.update_custom_data()

        self.update_visibilities(layer_name)


    def revert_visibility_of_layer(self, layer_name):
        isVisible = UsdGeom.Tokens.inherited
        self.set_visibilities(layer_name, isVisible)


    def set_visibilities(self, layer_name, visibilityToken):
        # Check if layer exists
        if layer_name not in self.__layers:
            pass # TODO: Throw error
            return
        
        for member in self.__layers[layer_name][self.__membersKey].keys():
            prim = self.__stage.GetPrimAtPath(member)

            if not prim or not UsdGeom.Imageable(prim):
                # TODO: Either throw error or delete this member
                del self.__layers[layer_name][self.__membersKey][member]
                continue
            
            prim = UsdGeom.Imageable(prim)
            prim.GetVisibilityAttr().Set(visibilityToken)


    def revert_visibility_of_member(self, path):
        prim = self.__stage.GetPrimAtPath(path)

        if not prim or not UsdGeom.Imageable(prim):
            # TODO: Throw error
            return
        
        prim = UsdGeom.Imageable(prim)
        prim.GetVisibilityAttr().Set(UsdGeom.Tokens.inherited)


    def update_custom_data(self):
        self.__prim.SetCustomDataByKey(self.__layersKey, self.__layers)
