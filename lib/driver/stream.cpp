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

#include <iostream>
#include <cassert>
#include <array>

#include "triton/driver/backend.h"
#include "triton/driver/stream.h"
#include "triton/driver/context.h"
#include "triton/driver/device.h"
#include "triton/driver/event.h"
#include "triton/driver/kernel.h"
#include "triton/driver/buffer.h"

namespace triton
{

namespace driver
{

inline CUcontext cucontext(){
  CUcontext result;
  dispatch::cuCtxGetCurrent(&result);
  return result;
}

stream::stream(CUstream stream, bool take_ownership): context_(cucontext(), take_ownership), cu_(stream, take_ownership)
{}

stream::stream(driver::context const & context): context_(context), cu_(CUstream(), true)
{
  ContextSwitcher ctx_switch(context_);
  dispatch::cuStreamCreate(&*cu_, 0);
}

void stream::synchronize()
{
  ContextSwitcher ctx_switch(context_);
  dispatch::cuStreamSynchronize(*cu_);
}

driver::context const & stream::context() const
{ return context_; }

void stream::enqueue(kernel const & kernel, std::array<size_t, 3> grid, std::array<size_t, 3> block, std::vector<Event> const *, Event* event){
  ContextSwitcher ctx_switch(context_);
  if(event)
    dispatch::cuEventRecord(((cu_event_t)*event).first, *cu_);
  dispatch::cuLaunchKernel(kernel, grid[0], grid[1], grid[2], block[0], block[1], block[2], 0, *cu_,(void**)kernel.cu_params(), NULL);
  if(event)
    dispatch::cuEventRecord(((cu_event_t)*event).second, *cu_);
}

void stream::write(buffer const & buffer, bool blocking, std::size_t offset, std::size_t size, void const* ptr){
  ContextSwitcher ctx_switch(context_);
  if(blocking)
    dispatch::cuMemcpyHtoD(buffer + offset, ptr, size);
  else
    dispatch::cuMemcpyHtoDAsync(buffer + offset, ptr, size, *cu_);
}

void stream::read(buffer const & buffer, bool blocking, std::size_t offset, std::size_t size, void* ptr){
  ContextSwitcher ctx_switch(context_);
  if(blocking)
    dispatch::cuMemcpyDtoH(ptr, buffer + offset, size);
  else
    dispatch::cuMemcpyDtoHAsync(ptr, buffer + offset, size, *cu_);
}

handle<CUstream> const & stream::cu() const
{ return cu_; }

}

}
