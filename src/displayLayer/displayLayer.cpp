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

/*static*/
const TfTokenVector&
DisplayLayerDisplayLayer::GetSchemaAttributeNames(bool includeInherited)
{
    static TfTokenVector localNames;
    static TfTokenVector allNames =
        UsdTyped::GetSchemaAttributeNames(true);

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
#include <pxr/usd/usdGeom/gprim.h>
#include <pxr/base/vt/array.h>
#include <pxr/base/vt/dictionary.h>
#include <pxr/usd/usd/editContext.h>

PXR_NAMESPACE_OPEN_SCOPE


bool DisplayLayerDisplayLayer::layerExists(const std::string& layerName) const
{
    return layers.find(layerName) != layers.end();
}

TfToken DisplayLayerDisplayLayer::getVisibilityToken(const bool isVisible) const
{
    return isVisible ? UsdGeomTokens->inherited : UsdGeomTokens->invisible;
}

bool DisplayLayerDisplayLayer::updateMemberVisibility(const SdfPath& path,
                                                        bool isVisible) const
{
    auto prim = stage->GetPrimAtPath(path);

    if (!prim || !prim.IsActive() || !UsdGeomImageable(prim))
    {
        return false;
    }

    auto visibilityToken = getVisibilityToken(isVisible);

    // Make sure we're making the edits in our override layer
    UsdEditContext editContext(stage, UsdEditTarget(overrideLayer));

    UsdGeomImageable imageablePrim(prim);
    imageablePrim.GetVisibilityAttr().Set(visibilityToken);

    return true;
}

void DisplayLayerDisplayLayer::updateLayerHighlight(const std::string& layerName)
{
    // Make sure we're making the edits in our highlight layer
    UsdEditContext editContext(stage, UsdEditTarget(layers[layerName].highlightLayer));
    
    for (const auto& member: layers[layerName].members)
    {
        SdfPath path = SdfPath(member);
        UsdPrim prim = stage->GetPrimAtPath(path);

        if (prim.IsValid() && prim.IsActive())
        {
            UsdGeomGprim gprim(prim);
            if (gprim)
            {
                if (layers[layerName].isHighlighted)
                {
                    gprim.GetDisplayColorAttr().Set(VtArray<GfVec3f>{HIGHLIGHT_COLOR});
                }
                else
                {
                    gprim.GetDisplayColorAttr().ClearAtTime(UsdTimeCode::Default());
                }
            }
        }
    }

    if (!layers[layerName].isHighlighted)
    {
        layers[layerName].highlightLayer->Clear();
    }
}


void DisplayLayerDisplayLayer::initialize(const UsdStagePtr& stage)
{
    this->stage = stage;
    overrideLayer = SdfLayer::CreateAnonymous();
    stage->GetRootLayer()->InsertSubLayerPath(overrideLayer->GetIdentifier());
}


void DisplayLayerDisplayLayer::initialize(const UsdStagePtr& stage,
                                            const VtDictionary& data)
{
    initialize(stage);

    for (auto& item : data)
    {
        const std::string& layerName = item.first;
        VtDictionary value = item.second.Get<VtDictionary>();

        layers[layerName].isVisible = value[VISIBILITY_KEY].Get<bool>();
        layers[layerName].isHighlighted = value[HIGHLIGHT_KEY].Get<bool>();

        layers[layerName].highlightLayer = SdfLayer::CreateAnonymous();
        overrideLayer->InsertSubLayerPath(layers[layerName].highlightLayer->GetIdentifier());

        const VtArray<SdfPath> members = value[MEMBERS_KEY].Get<VtArray<SdfPath>>();

        for (const auto& member : members)
        {
            layers[layerName].members.insert(member.GetString());
        }
    }

    if (!data.empty())
    {
        updateAllVisibilities();
    }
}


void DisplayLayerDisplayLayer::createNewLayer(const std::string& layerName)
{
    if (!stage)
    {
        throw std::runtime_error("Display Layer has not been initialized.");
    }

    if (layerExists(layerName))
    {
        std::runtime_error(layerName + " already exists.");
    }

    layers[layerName].isVisible = true;

    layers[layerName].isHighlighted = false;
    layers[layerName].highlightLayer = SdfLayer::CreateAnonymous();
    overrideLayer->InsertSubLayerPath(layers[layerName].highlightLayer->GetIdentifier());
}


bool DisplayLayerDisplayLayer::removeLayer(const std::string& layerName)
{
    if (!layerExists(layerName))
    {
        return false;
    }

    setLayerVisibility(layerName, true);
    setLayerHighlight(layerName, false);

    layers.erase(layerName);

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
    for (const auto& layerData : layers)
    {

        if (layerData.second.members.count(path.GetString()) > 0)
        {
            // already added to this layer
            if (layerData.first == layerName)
            {
                return;
            }
            
            // added to another layer
            std::runtime_error(path.GetString() + " is already in another layer");
        }
    }

    auto prim = stage->GetPrimAtPath(path);

    if (!prim || !UsdGeomImageable(prim))
    {
        throw std::runtime_error(path.GetString() + " doesn't exist or is not"
                                    + " UsdGeomImageable.");
    }

    layers[layerName].members.insert(path.GetString());

    updateMemberVisibility(path, layers[layerName].isVisible);
}


bool DisplayLayerDisplayLayer::removeItemFromLayer(const std::string& layerName,
                                                    const SdfPath& path)
{
    if (!layerExists(layerName) || layers[layerName].members.count(path.GetString()) == 0)
    {
        return false;
    }

    layers[layerName].members.erase(path.GetString());

    // Revert visibility of item
    updateMemberVisibility(path, true);

    return true;
}

void DisplayLayerDisplayLayer::setLayerHighlight(const std::string& layerName,
                                                const bool isHighlighted)
{
    if (!layerExists(layerName))
    {
        std::runtime_error("Layer " + layerName + " does not exist.");
    }

    layers[layerName].isHighlighted = isHighlighted;
    updateLayerHighlight(layerName);
}


void DisplayLayerDisplayLayer::updateAllVisibilities()
{
    for (const auto& layerData : layers)
    {
        std::string layerName = layerData.first;
        updateLayerHighlight(layerName);
        updateLayerVisibilities(layerName);
    }
}


void DisplayLayerDisplayLayer::updateLayerVisibilities(const std::string& layerName)
{
    if (!layerExists(layerName))
    {
        return;
    }

    bool isVisible = layers[layerName].isVisible;

    for (const auto& path : layers[layerName].members)
    {
        bool success = updateMemberVisibility(SdfPath(path), isVisible);

        if (!success)
        {
            // Failure => prim path either doesn't exist or is not UsdGeomImageable
            // Delete this prim from the layer
            layers[layerName].members.erase(path);
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
    if (layers[layerName].isVisible == isVisible)
    {
        return;
    }

    layers[layerName].isVisible = isVisible;
    updateLayerVisibilities(layerName);
}

void DisplayLayerDisplayLayer::updateMetadata() const
{
    VtDictionary data;

    for (const auto& pair : layers)
    {
        const std::string& layerName = pair.first;
        const Layer& layer = pair.second;

        VtDictionary layerData;
        VtArray<SdfPath> members;

        for (const auto& member : layer.members)
        {
            members.push_back(SdfPath(member));
        }

        layerData[VISIBILITY_KEY] = layer.isVisible;
        layerData[MEMBERS_KEY] = members;
        layerData[HIGHLIGHT_KEY] = layer.isHighlighted;

        data[layerName] = layerData;
    }

    GetPrim().SetCustomDataByKey(LAYERS_KEY, VtValue(layers));
}

TfToken DisplayLayerDisplayLayer::getLayersKey() const
{
    return LAYERS_KEY;
}

PXR_NAMESPACE_CLOSE_SCOPE
