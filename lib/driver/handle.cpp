/* Copyright 2015-2017 Philippe Tillet
* 
* Permission is hereby granted, free of charge, to any person obtaining 
* a copy of this software and associated documentation files 
* (the "Software"), to deal in the Software without restriction, 
* including without limitation the rights to use, copy, modify, merge, 
* publish, distribute, sublicense, and/or sell copies of the Software, 
* and to permit persons to whom the Software is furnished to do so, 
* subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be 
* included in all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <cassert>
#include <memory>
#include "triton/driver/handle.h"

namespace triton
{

namespace driver
{

//CUDA
inline void _delete(CUcontext x) { dispatch::cuCtxDestroy(x); }
inline void _delete(CUdeviceptr x) { dispatch::cuMemFree(x); }
inline void _delete(CUstream x) { dispatch::cuStreamDestroy(x); }
inline void _delete(CUdevice) { }
inline void _delete(CUevent x) { dispatch::cuEventDestroy(x); }
inline void _delete(CUfunction) { }
inline void _delete(CUmodule x) { dispatch::cuModuleUnload(x); }
inline void _delete(cu_event_t x) { _delete(x.first); _delete(x.second); }
inline void _delete(cu_platform){}

//Constructor
template<class CUType>
handle<CUType>::handle(CUType cu, bool take_ownership): h_(new CUType(cu)), has_ownership_(take_ownership)
{ }


template<class CUType>
handle<CUType>::~handle(){
  if(has_ownership_ && h_ && h_.unique() && *h_)
    _delete(*h_);
}

template class handle<CUdeviceptr>;
template class handle<CUstream>;
template class handle<CUcontext>;
template class handle<CUdevice>;
template class handle<cu_event_t>;
template class handle<CUfunction>;
template class handle<CUmodule>;
template class handle<cu_platform>;

}
}
