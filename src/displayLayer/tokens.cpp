//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "./tokens.h"

PXR_NAMESPACE_OPEN_SCOPE

DisplayLayerTokensType::DisplayLayerTokensType() :
    visible("visible", TfToken::Immortal),
    DisplayLayer("DisplayLayer", TfToken::Immortal),
    allTokens({
        visible,
        DisplayLayer
    })
{
}

TfStaticData<DisplayLayerTokensType> DisplayLayerTokens;

PXR_NAMESPACE_CLOSE_SCOPE
