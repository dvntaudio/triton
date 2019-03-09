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

#ifndef TDL_INCLUDE_DRIVER_KERNEL_H
#define TDL_INCLUDE_DRIVER_KERNEL_H

#include "triton/driver/module.h"
#include "triton/driver/handle.h"

#include <memory>

namespace triton
{

namespace driver
{

class buffer;

// Kernel
class kernel: public handle_interface<kernel, CUfunction>
{
public:
  //Constructors
  kernel(driver::module const & program, const char * name);
  //Accessors
  handle<CUfunction> const & cu() const;
  driver::module const & module() const;
  //Arguments setters
  void setArg(unsigned int index, std::size_t size, void* ptr);
  void setArg(unsigned int index, buffer const &);
  template<class T> void setArg(unsigned int index, T value) { setArg(index, sizeof(T), (void*)&value); }
  //Arguments getters
  void* const* cu_params() const;

private:
  handle<CUfunction> cu_;
  driver::module program_;
  unsigned int address_bits_;
  std::vector<std::shared_ptr<void> >  cu_params_store_;
  std::vector<void*>  cu_params_;
};

}

}

#endif

