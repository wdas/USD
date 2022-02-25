//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#include "pxr/pxr.h"
#include "pxr/usd/usd/transaction.h"
#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usd/common.h"

#include <boost/python.hpp>

using namespace boost::python;

PXR_NAMESPACE_USING_DIRECTIVE


// Expose C++ RAII class as python context manager.
struct PythonUsdTransaction
{
    PythonUsdTransaction(const UsdStageWeakPtr& stage)
        : _makeContext([&]() { return new UsdTransaction(stage); })
    {}

    // Instantiate the C++ class object and hold it by shared_ptr.
    void __enter__() { _context.reset(_makeContext()); }

    // Drop the shared_ptr.
    void __exit__(object, object, object) { _context.reset(); }

private:
    std::shared_ptr<UsdTransaction> _context;
    std::function<UsdTransaction *()> _makeContext;
};

void
wrapUsdTransaction()
{    
    class_<PythonUsdTransaction>("Transaction", no_init)
        .def(init<const UsdStageWeakPtr&>(arg("stage")))
        .def("__enter__", &PythonUsdTransaction::__enter__)
        .def("__exit__", &PythonUsdTransaction::__exit__)
        ;
}
