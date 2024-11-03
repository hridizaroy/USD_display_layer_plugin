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
#include "./tokens.h"

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
    // --------------------------------------------------------------------- //
    // VISIBLE 
    // --------------------------------------------------------------------- //
    /// Controls the visibility of all prims in this layer.
    ///
    /// | ||
    /// | -- | -- |
    /// | Declaration | `bool visible = 1` |
    /// | C++ Type | bool |
    /// | \ref Usd_Datatypes "Usd Type" | SdfValueTypeNames->Bool |
    DISPLAYLAYER_API
    UsdAttribute GetVisibleAttr() const;

    /// See GetVisibleAttr(), and also 
    /// \ref Usd_Create_Or_Get_Property for when to use Get vs Create.
    /// If specified, author \p defaultValue as the attribute's default,
    /// sparsely (when it makes sense to do so) if \p writeSparsely is \c true -
    /// the default for \p writeSparsely is \c false.
    DISPLAYLAYER_API
    UsdAttribute CreateVisibleAttr(VtValue const &defaultValue = VtValue(), bool writeSparsely=false) const;

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
#include <unordered_map>
#include <unordered_set>

private:
    struct Layer
    {
        // Using string instead of SdfPath because of hashing issues
        std::unordered_set<std::string> members;
        bool isVisible;

        bool operator==(const Layer& other) const
        {
            return isVisible == other.isVisible && members == other.members;
        }
    };

    std::unordered_map<std::string, Layer> layers;
    UsdStagePtr stage;
    SdfLayerRefPtr overrideLayer;

    // Keys
    const TfToken LAYERS_KEY = TfToken("layers");
    const TfToken MEMBERS_KEY = TfToken("members");
    const TfToken VISIBILITY_KEY = TfToken("isVisible");

    const std::string OVERRIDE_LAYER_NAME = "displayLayerOverrides.usda";

    bool layerExists(const std::string& layerName) const;

    TfToken getVisibilityToken(const bool isVisible) const;

    bool updateMemberVisibility(const SdfPath& path, bool isVisible);

    /**
    * Converts the layers map to a VtDictionary and adds it to the prim's metadata
    */
    void updateMetadata() const;

public:
    DISPLAYLAYER_API
    void initialize(const UsdStagePtr &stage);

    DISPLAYLAYER_API
    void initialize(const UsdStagePtr &stage,
                    const VtDictionary& data = VtDictionary());

    DISPLAYLAYER_API
    void createNewLayer(const std::string& layerName);

    DISPLAYLAYER_API
    bool removeLayer(const std::string& layerName);

    DISPLAYLAYER_API
    void addItemToLayer(const std::string& layerName, const SdfPath& path);

    DISPLAYLAYER_API
    bool removeItemFromLayer(const std::string& layerName, const SdfPath& path);

    DISPLAYLAYER_API
    void updateAllVisibilities();

    DISPLAYLAYER_API
    void updateLayerVisibilities(const std::string& layerName);

    DISPLAYLAYER_API
    void setLayerVisibility(const std::string& layerName, bool isVisible);

    DISPLAYLAYER_API
    void saveLayer();

};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
