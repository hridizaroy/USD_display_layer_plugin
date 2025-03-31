//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef DISPLAYLAYER_TOKENS_H
#define DISPLAYLAYER_TOKENS_H

/// \file displayLayer/tokens.h

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// 
// This is an automatically generated file (by usdGenSchema.py).
// Do not hand-edit!
// 
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "pxr/pxr.h"
#include "./api.h"
#include "pxr/base/tf/staticData.h"
#include "pxr/base/tf/token.h"
#include <vector>

PXR_NAMESPACE_OPEN_SCOPE


/// \class DisplayLayerTokensType
///
/// \link DisplayLayerTokens \endlink provides static, efficient
/// \link TfToken TfTokens\endlink for use in all public USD API.
///
/// These tokens are auto-generated from the module's schema, representing
/// property names, for when you need to fetch an attribute or relationship
/// directly by name, e.g. UsdPrim::GetAttribute(), in the most efficient
/// manner, and allow the compiler to verify that you spelled the name
/// correctly.
///
/// DisplayLayerTokens also contains all of the \em allowedTokens values
/// declared for schema builtin attributes of 'token' scene description type.
/// Use DisplayLayerTokens like so:
///
/// \code
///     gprim.GetMyTokenValuedAttr().Set(DisplayLayerTokens->DisplayLayer);
/// \endcode
struct DisplayLayerTokensType {
    DISPLAYLAYER_API DisplayLayerTokensType();
    /// \brief "DisplayLayer"
    /// 
    /// Schema identifer and family for DisplayLayerDisplayLayer
    const TfToken DisplayLayer;
    /// A vector of all of the tokens listed above.
    const std::vector<TfToken> allTokens;
};

/// \var DisplayLayerTokens
///
/// A global variable with static, efficient \link TfToken TfTokens\endlink
/// for use in all public USD API.  \sa DisplayLayerTokensType
extern DISPLAYLAYER_API TfStaticData<DisplayLayerTokensType> DisplayLayerTokens;

PXR_NAMESPACE_CLOSE_SCOPE

#endif
