//
// Copyright 2017 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef DISPLAYLAYER_API_H
#define DISPLAYLAYER_API_H

#include "pxr/base/arch/export.h"

#if defined(PXR_STATIC)
#   define DISPLAYLAYER_API
#   define DISPLAYLAYER_API_TEMPLATE_CLASS(...)
#   define DISPLAYLAYER_API_TEMPLATE_STRUCT(...)
#   define DISPLAYLAYER_LOCAL
#else
#   if defined(DISPLAYLAYER_EXPORTS)
#       define DISPLAYLAYER_API ARCH_EXPORT
#       define DISPLAYLAYER_API_TEMPLATE_CLASS(...) ARCH_EXPORT_TEMPLATE(class, __VA_ARGS__)
#       define DISPLAYLAYER_API_TEMPLATE_STRUCT(...) ARCH_EXPORT_TEMPLATE(struct, __VA_ARGS__)
#   else
#       define DISPLAYLAYER_API ARCH_IMPORT
#       define DISPLAYLAYER_API_TEMPLATE_CLASS(...) ARCH_IMPORT_TEMPLATE(class, __VA_ARGS__)
#       define DISPLAYLAYER_API_TEMPLATE_STRUCT(...) ARCH_IMPORT_TEMPLATE(struct, __VA_ARGS__)
#   endif
#   define DISPLAYLAYER_LOCAL ARCH_HIDDEN
#endif

#endif
