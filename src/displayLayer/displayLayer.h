//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef DISPLAYLAYER_GENERATED_DISPLAYLAYER_H
#define DISPLAYLAYER_GENERATED_DISPLAYLAYER_H

/// \file displayLayer/displayLayer.h

#include "pxr/pxr.h"
#include "./api.h"
#include "pxr/usd/usd/typed.h"
#include "pxr/usd/usd/prim.h"
#include "pxr/usd/usd/stage.h"

#include "pxr/base/vt/value.h"

#include "pxr/base/gf/vec3d.h"
#include "pxr/base/gf/vec3f.h"
#include "pxr/base/gf/matrix4d.h"

#include "pxr/base/tf/token.h"
#include "pxr/base/tf/type.h"

PXR_NAMESPACE_OPEN_SCOPE

class SdfAssetPath;

// -------------------------------------------------------------------------- //
// DISPLAYLAYER                                                               //
// -------------------------------------------------------------------------- //

/// \class DisplayLayerDisplayLayer
///
/// Represents display layers for organizing and controlling visibility of prims.
///
class DisplayLayerDisplayLayer : public UsdTyped
{
public:
    /// Compile time constant representing what kind of schema this class is.
    ///
    /// \sa UsdSchemaKind
    static const UsdSchemaKind schemaKind = UsdSchemaKind::ConcreteTyped;

    /// Construct a DisplayLayerDisplayLayer on UsdPrim \p prim .
    /// Equivalent to DisplayLayerDisplayLayer::Get(prim.GetStage(), prim.GetPath())
    /// for a \em valid \p prim, but will not immediately throw an error for
    /// an invalid \p prim
    explicit DisplayLayerDisplayLayer(const UsdPrim& prim=UsdPrim())
        : UsdTyped(prim)
    {
    }

    /// Construct a DisplayLayerDisplayLayer on the prim held by \p schemaObj .
    /// Should be preferred over DisplayLayerDisplayLayer(schemaObj.GetPrim()),
    /// as it preserves SchemaBase state.
    explicit DisplayLayerDisplayLayer(const UsdSchemaBase& schemaObj)
        : UsdTyped(schemaObj)
    {
    }

    /// Destructor.
    DISPLAYLAYER_API
    virtual ~DisplayLayerDisplayLayer();

    /// Return a vector of names of all pre-declared attributes for this schema
    /// class and all its ancestor classes.  Does not include attributes that
    /// may be authored by custom/extended methods of the schemas involved.
    DISPLAYLAYER_API
    static const TfTokenVector &
    GetSchemaAttributeNames(bool includeInherited=true);

    /// Return a DisplayLayerDisplayLayer holding the prim adhering to this
    /// schema at \p path on \p stage.  If no prim exists at \p path on
    /// \p stage, or if the prim at that path does not adhere to this schema,
    /// return an invalid schema object.  This is shorthand for the following:
    ///
    /// \code
    /// DisplayLayerDisplayLayer(stage->GetPrimAtPath(path));
    /// \endcode
    ///
    DISPLAYLAYER_API
    static DisplayLayerDisplayLayer
    Get(const UsdStagePtr &stage, const SdfPath &path);

    /// Attempt to ensure a \a UsdPrim adhering to this schema at \p path
    /// is defined (according to UsdPrim::IsDefined()) on this stage.
    ///
    /// If a prim adhering to this schema at \p path is already defined on this
    /// stage, return that prim.  Otherwise author an \a SdfPrimSpec with
    /// \a specifier == \a SdfSpecifierDef and this schema's prim type name for
    /// the prim at \p path at the current EditTarget.  Author \a SdfPrimSpec s
    /// with \p specifier == \a SdfSpecifierDef and empty typeName at the
    /// current EditTarget for any nonexistent, or existing but not \a Defined
    /// ancestors.
    ///
    /// The given \a path must be an absolute prim path that does not contain
    /// any variant selections.
    ///
    /// If it is impossible to author any of the necessary PrimSpecs, (for
    /// example, in case \a path cannot map to the current UsdEditTarget's
    /// namespace) issue an error and return an invalid \a UsdPrim.
    ///
    /// Note that this method may return a defined prim whose typeName does not
    /// specify this schema class, in case a stronger typeName opinion overrides
    /// the opinion at the current EditTarget.
    ///
    DISPLAYLAYER_API
    static DisplayLayerDisplayLayer
    Define(const UsdStagePtr &stage, const SdfPath &path);

protected:
    /// Returns the kind of schema this class belongs to.
    ///
    /// \sa UsdSchemaKind
    DISPLAYLAYER_API
    UsdSchemaKind _GetSchemaKind() const override;

private:
    // needs to invoke _GetStaticTfType.
    friend class UsdSchemaRegistry;
    DISPLAYLAYER_API
    static const TfType &_GetStaticTfType();

    static bool _IsTypedSchema();

    // override SchemaBase virtuals.
    DISPLAYLAYER_API
    const TfType &_GetTfType() const override;

public:
    // ===================================================================== //
    // Feel free to add custom code below this line, it will be preserved by 
    // the code generator. 
    //
    // Just remember to: 
    //  - Close the class declaration with }; 
    //  - Close the namespace with PXR_NAMESPACE_CLOSE_SCOPE
    //  - Close the include guard with #endif
    // ===================================================================== //
    // --(BEGIN CUSTOM CODE)--

#include <pxr/usd/sdf/layer.h>
#include <pxr/base/gf/vec3f.h>
#include <unordered_map>
#include <unordered_set>

private:
    /// Describes a single layer that holds a group of prims
    struct Layer
    {
        // Using string instead of SdfPath because of hashing issues
        std::unordered_set<std::string> members;

        bool isVisible;
        bool isHighlighted;

        SdfLayerRefPtr highlightLayer;

        // Needed for using this as a value in sets
        bool operator==(const Layer& other) const
        {
            return members == other.members;
        }
    };

    std::unordered_map<std::string, Layer> layers;
    UsdStagePtr stage;
    SdfLayerRefPtr overrideLayer;

    // Keys for metadata
    const TfToken LAYERS_KEY = TfToken("layers");
    const TfToken MEMBERS_KEY = TfToken("members");
    const TfToken VISIBILITY_KEY = TfToken("isVisible");
    const TfToken HIGHLIGHT_KEY = TfToken("isHighlighted");

    const GfVec3f HIGHLIGHT_COLOR{1.0f, 0.0f, 0.0f};

    /// Check if a layer with the given name exists in the layers map
    bool layerExists(const std::string& layerName) const;

    /// Returns UsdGeom Visibility Token
    TfToken getVisibilityToken(const bool isVisible) const;

    /// Updates visibility of the prim at the given path
    bool updateMemberVisibility(const SdfPath& path, const bool isVisible) const;

    /// Updates display color of the prim at the given path
    bool updateMemberHighlight(const SdfPath& path,
                        const std::string& layerName, bool isHighlighted);

    /// Updates display color of the members of the given layer based on 
    /// if the layer is highlighted
    void updateLayerHighlight(const std::string& layerName);

public:
    /// Initializes the stage and override layer
    /// Needs to be called before performing any other operations on this prim
    DISPLAYLAYER_API
    void initialize(const UsdStagePtr& stage);

    /// Populates the layers map with the given data in addition to initializing
    /// Used when we are starting from a pre-existing Display Layer prim
    DISPLAYLAYER_API
    void initialize(const UsdStagePtr& stage,
                    const VtDictionary& data);

    /// Add a new layer to the layers map
    DISPLAYLAYER_API
    void createNewLayer(const std::string& layerName);

    /// Remove a layer from the layers map and update visibility and display
    /// color of the layer's members
    /// Returns false if the layer could not be removed (e.g., if it doesn't exist)
    DISPLAYLAYER_API
    bool removeLayer(const std::string& layerName);

    /// Add a UsdGeomImageable prim to the given layer
    /// Updates visibility and highlight color of prim
    DISPLAYLAYER_API
    void addItemToLayer(const std::string& layerName, const SdfPath& path);

    /// Removes the given prim from the layer
    /// Returns false if the prim could not be removed
    /// (e.g., if the layer doesn't contain it)
    DISPLAYLAYER_API
    bool removeItemFromLayer(const std::string& layerName, const SdfPath& path);

    /// Sets highlight status of the given layer and updates the display color
    /// of its members
    DISPLAYLAYER_API
    void setLayerHighlight(const std::string& layerName, bool isHighlighted);

    /// Sets visibility status of the given layer and updates the visibility
    /// of its members
    DISPLAYLAYER_API
    void setLayerVisibility(const std::string& layerName, const bool isVisible);

    /// Update visibility and display color of all members of all layers
    DISPLAYLAYER_API
    void updateAllVisibilities();

    /// Update visibility of members of the given layer
    DISPLAYLAYER_API
    void updateLayerVisibilities(const std::string& layerName);

    /// Converts the layers map to a VtDictionary and adds it to the prim's metadata
    DISPLAYLAYER_API
    void updateMetadata() const;

    /// Returns the metadata key for storing the display layer's data
    DISPLAYLAYER_API
    TfToken getLayersKey() const;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
