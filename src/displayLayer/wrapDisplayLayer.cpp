//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "./displayLayer.h"
#include "pxr/usd/usd/schemaBase.h"

#include "pxr/usd/sdf/primSpec.h"

#include "pxr/usd/usd/pyConversions.h"
#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/pyResultConversions.h"
#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"

#include <boost/python.hpp>

#include <string>

using namespace boost::python;

PXR_NAMESPACE_USING_DIRECTIVE

namespace {

#define WRAP_CUSTOM                                                     \
    template <class Cls> static void _CustomWrapCode(Cls &_class)

// fwd decl.
WRAP_CUSTOM;

        
static UsdAttribute
_CreateVisibleAttr(DisplayLayerDisplayLayer &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateVisibleAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Bool), writeSparsely);
}

static std::string
_Repr(const DisplayLayerDisplayLayer &self)
{
    std::string primRepr = TfPyRepr(self.GetPrim());
    return TfStringPrintf(
        "DisplayLayer.DisplayLayer(%s)",
        primRepr.c_str());
}

} // anonymous namespace

void wrapDisplayLayerDisplayLayer()
{
    typedef DisplayLayerDisplayLayer This;

    class_<This, bases<UsdTyped> >
        cls("DisplayLayer");

    cls
        .def(init<UsdPrim>(arg("prim")))
        .def(init<UsdSchemaBase const&>(arg("schemaObj")))
        .def(TfTypePythonClass())

        .def("Get", &This::Get, (arg("stage"), arg("path")))
        .staticmethod("Get")

        .def("Define", &This::Define, (arg("stage"), arg("path")))
        .staticmethod("Define")

        .def("GetSchemaAttributeNames",
             &This::GetSchemaAttributeNames,
             arg("includeInherited")=true,
             return_value_policy<TfPySequenceToList>())
        .staticmethod("GetSchemaAttributeNames")

        .def("_GetStaticTfType", (TfType const &(*)()) TfType::Find<This>,
             return_value_policy<return_by_value>())
        .staticmethod("_GetStaticTfType")

        .def(!self)

        
        .def("GetVisibleAttr",
             &This::GetVisibleAttr)
        .def("CreateVisibleAttr",
             &_CreateVisibleAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))

        .def("__repr__", ::_Repr)
    ;

    _CustomWrapCode(cls);
}

// ===================================================================== //
// Feel free to add custom code below this line, it will be preserved by 
// the code generator.  The entry point for your custom code should look
// minimally like the following:
//
// WRAP_CUSTOM {
//     _class
//         .def("MyCustomMethod", ...)
//     ;
// }
//
// Of course any other ancillary or support code may be provided.
// 
// Just remember to wrap code in the appropriate delimiters:
// 'namespace {', '}'.
//
// ===================================================================== //
// --(BEGIN CUSTOM CODE)--

namespace {

WRAP_CUSTOM {
    typedef DisplayLayerDisplayLayer This;

    _class
        .def("initialize",
            static_cast<void (This::*)(const UsdStagePtr&, const VtDictionary&)>(&This::initialize),
            (arg("stage"), arg("data")))
        .def("initialize",
            static_cast<void (This::*)(const UsdStagePtr&)>(&This::initialize),
            (arg("stage")))

        .def("createNewLayer",
            &This::createNewLayer,
            (arg("layerName")))
        .def("removeLayer",
            &This::removeLayer,
            (arg("layerName")))

        .def("addItemToLayer",
            &This::addItemToLayer,
            (arg("layerName"), arg("path")))
        .def("removeItemFromLayer",
            &This::removeItemFromLayer,
            (arg("layerName"), arg("path")))
        .def("setLayerHighlight",
            &This::setLayerHighlight,
            (arg("layerName"), arg("isHighlighted")))

        .def("updateAllVisibilities",
            &This::updateAllVisibilities)
        .def("updateLayerVisibilities",
            &This::updateLayerVisibilities,
            (arg("layerName")))
        .def("setLayerVisibility",
            &This::setLayerVisibility,
            (arg("layerName"), arg("isVisible")))

        .def("updateMetadata",
            &This::updateMetadata)
    ;
}

}
