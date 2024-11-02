//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "./displayLayer.h"
#include "pxr/usd/usd/schemaRegistry.h"
#include "pxr/usd/usd/typed.h"

#include "pxr/usd/sdf/types.h"
#include "pxr/usd/sdf/assetPath.h"

PXR_NAMESPACE_OPEN_SCOPE

// Register the schema with the TfType system.
TF_REGISTRY_FUNCTION(TfType)
{
    TfType::Define<DisplayLayerDisplayLayer,
        TfType::Bases< UsdTyped > >();
    
    // Register the usd prim typename as an alias under UsdSchemaBase. This
    // enables one to call
    // TfType::Find<UsdSchemaBase>().FindDerivedByName("DisplayLayer")
    // to find TfType<DisplayLayerDisplayLayer>, which is how IsA queries are
    // answered.
    TfType::AddAlias<UsdSchemaBase, DisplayLayerDisplayLayer>("DisplayLayer");
}

/* virtual */
DisplayLayerDisplayLayer::~DisplayLayerDisplayLayer()
{
}

/* static */
DisplayLayerDisplayLayer
DisplayLayerDisplayLayer::Get(const UsdStagePtr &stage, const SdfPath &path)
{
    if (!stage) {
        TF_CODING_ERROR("Invalid stage");
        return DisplayLayerDisplayLayer();
    }
    return DisplayLayerDisplayLayer(stage->GetPrimAtPath(path));
}

/* static */
DisplayLayerDisplayLayer
DisplayLayerDisplayLayer::Define(
    const UsdStagePtr &stage, const SdfPath &path)
{
    static TfToken usdPrimTypeName("DisplayLayer");
    if (!stage) {
        TF_CODING_ERROR("Invalid stage");
        return DisplayLayerDisplayLayer();
    }
    return DisplayLayerDisplayLayer(
        stage->DefinePrim(path, usdPrimTypeName));
}

/* virtual */
UsdSchemaKind DisplayLayerDisplayLayer::_GetSchemaKind() const
{
    return DisplayLayerDisplayLayer::schemaKind;
}

/* static */
const TfType &
DisplayLayerDisplayLayer::_GetStaticTfType()
{
    static TfType tfType = TfType::Find<DisplayLayerDisplayLayer>();
    return tfType;
}

/* static */
bool 
DisplayLayerDisplayLayer::_IsTypedSchema()
{
    static bool isTyped = _GetStaticTfType().IsA<UsdTyped>();
    return isTyped;
}

/* virtual */
const TfType &
DisplayLayerDisplayLayer::_GetTfType() const
{
    return _GetStaticTfType();
}

UsdAttribute
DisplayLayerDisplayLayer::GetVisibleAttr() const
{
    return GetPrim().GetAttribute(DisplayLayerTokens->visible);
}

UsdAttribute
DisplayLayerDisplayLayer::CreateVisibleAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(DisplayLayerTokens->visible,
                       SdfValueTypeNames->Bool,
                       /* custom = */ false,
                       SdfVariabilityVarying,
                       defaultValue,
                       writeSparsely);
}

namespace {
static inline TfTokenVector
_ConcatenateAttributeNames(const TfTokenVector& left,const TfTokenVector& right)
{
    TfTokenVector result;
    result.reserve(left.size() + right.size());
    result.insert(result.end(), left.begin(), left.end());
    result.insert(result.end(), right.begin(), right.end());
    return result;
}
}

/*static*/
const TfTokenVector&
DisplayLayerDisplayLayer::GetSchemaAttributeNames(bool includeInherited)
{
    static TfTokenVector localNames = {
        DisplayLayerTokens->visible,
    };
    static TfTokenVector allNames =
        _ConcatenateAttributeNames(
            UsdTyped::GetSchemaAttributeNames(true),
            localNames);

    if (includeInherited)
        return allNames;
    else
        return localNames;
}

PXR_NAMESPACE_CLOSE_SCOPE

// ===================================================================== //
// Feel free to add custom code below this line. It will be preserved by
// the code generator.
//
// Just remember to wrap code in the appropriate delimiters:
// 'PXR_NAMESPACE_OPEN_SCOPE', 'PXR_NAMESPACE_CLOSE_SCOPE'.
// ===================================================================== //
// --(BEGIN CUSTOM CODE)--

#include <stdexcept>
#include <pxr/usd/usdGeom/tokens.h>
#include <pxr/usd/usdGeom/imageable.h>

PXR_NAMESPACE_OPEN_SCOPE

bool DisplayLayerDisplayLayer::layerExists(const std::string& layerName)
{
    return self.layers.count(layerName) > 0;
}

TfToken DisplayLayerDisplayLayer::getVisibilityToken(const bool isVisible)
{
    return UsdGeomTokens->inherited ? isVisible : UsdGeomTokens->invisible;
}

/**
* The function calling this is responsible for setting stage edit target
 */
bool DisplayLayerDisplayLayer::updateMemberVisibility(const SdfPath& path,
                                                        bool isVisible)
{
    auto prim = this.stage->GetPrimAtPath(path);

    if (!prim || !UsdGeomImageable(prim))
    {
        return false;
    }

    auto visibilityToken = getVisibilityToken(isVisible);

    UsdGeomImageable imageablePrim(prim);
    imageablePrim.GetVisibilityAttr().Set(visibilityToken);

    return true;
}

void DisplayLayerDisplayLayer::updateMetadata()
{
    GetPrim().SetCustomDataByKey(layersKey, layers);
}


void DisplayLayerDisplayLayer::initialize(
            const UsdStagePtr &stage, VtDictionary& data = VtDictionary())
{
    this.stage = stage;
    this.layers = data;

    overrideLayer = UsdLayer::CreateNew("displayLayerOverrides.usda");

    if (data.empty())
    {
        updateMetadata();
    }
    else
    {
        updateAllVisibilities();
    }
}


void DisplayLayerDisplayLayer::createNewLayer(const std::string& layerName)
{
    if (layerExists(layerName))
    {
        std::runtime_error(layerName + " already exists.");
    }

    layers[layerName] = VtDictionary();
    layers[layerName][membersKey] = VtDictionary();
    layers[layerName][visibilityKey] = true;

    updateMetadata();
}


bool DisplayLayerDisplayLayer::removeLayer(const std::string& layerName)
{
    if (!layerExists())
    {
        return false;
    }

    setLayerVisibility(layerName, true);

    layers.erase(layerName);
    updateMetadata();

    return true;
}


void DisplayLayerDisplayLayer::addItemToLayer(const std::string& layerName, 
                                                const SdfPath& path)
{
    if (!layerExists(layerName))
    {
        std::runtime_error("Layer \"" + layerName + "\" doesn't exist.");
    }

    // Check if prim is already in another layer
    for (const auto& layer : layers)
    {
        if (layer[membersKey].count(path) > 0)
        {
            // already added to this layer
            if (layer == layerName)
            {
                return true;
            }
            
            // added to another layer
            std::runtime_error(std::string(path) + " is already in another layer");
        }
    }

    auto prim = this.stage->GetPrimAtPath(path);

    if (!prim || !UsdGeomImageable(prim))
    {
        throw std::runtime_error(std::string(path) + " doesn't exist or is not"
                                    + " UsdGeomImageable.");
    }

    layers[layerName][membersKey][path] = true;

    updateMetadata();

    bool isVisible = layers[layerName][visibilityKey];
    updateMemberVisibility(path, isVisible);
}


bool DisplayLayerDisplayLayer::removeItemFromLayer(const std::string& layerName,
                                                    const SdfPath& path)
{
    if (!layerExists(layerName) || layers[layerName][membersKey].count(path) == 0)
    {
        return false;
    }

    layers[layerName][membersKey].erase(path);
    updateMetadata();

    UsdLayerRefPtr originalLayer = stage->GetEditTarget();

    if (originalLayer != overrideLayer)
    {
        stage->setEditTarget(overrideLayer);
    }

    // Revert visibility of item
    updateMemberVisibility(path, true);

    if (originalLayer != overrideLayer)
    {
        stage->setEditTarget(originalLayer);
    }

    return true;
}


void DisplayLayerDisplayLayer::updateAllVisibilities()
{
    UsdLayerRefPtr originalLayer = stage->GetEditTarget();
    
    if (originalLayer != overrideLayer)
    {
        stage->setEditTarget(overrideLayer);
    }

    for (const auto& layerName : layers)
    {
        updateLayerVisibilities(layerName);
    }

    if (originalLayer != overrideLayer)
    {
        stage->setEditTarget(originalLayer);
    }
}


/**
* The function calling this is responsible for setting the stage edit target
 */
void DisplayLayerDisplayLayer::updateLayerVisibilities(const std::string& layerName)
{
    if (!layerExists(layerName))
    {
        return;
    }

    bool isVisible = layers[layerName][visibilityKey];

    for (const auto& path : layers[layerName][membersKey])
    {
        bool success = updateMemberVisibility(path, isVisible);

        if (!success)
        {
            // Failure => prim path either doesn't exist or is not UsdGeomImageable
            // Delete this prim from the layer
            layers[layerName][membersKey].erase(path);
        }
    }
}


void DisplayLayerDisplayLayer::setLayerVisibility(const std::string& layerName,
                                                    bool isVisible)
{
    if (!layerExists(layerName))
    {
        return;
    }

    // If current visibility is the same as the new one, return
    if (layers[layerName][visibilityKey] == isVisible)
    {
        return;
    }

    layers[layerName][visibilityKey] = isVisible;

    updateMetadata();

    UsdLayerRefPtr originalLayer = stage->GetEditTarget();
    
    if (originalLayer != overrideLayer)
    {
        stage->setEditTarget(overrideLayer);
    }

    updateLayerVisibilities(layerName);

    if (originalLayer != overrideLayer)
    {
        stage->setEditTarget(originalLayer);
    }
}

void DisplayLayerDisplayLayer::saveLayer()
{
    overrideLayer->save();
}


PXR_NAMESPACE_CLOSE_SCOPE
