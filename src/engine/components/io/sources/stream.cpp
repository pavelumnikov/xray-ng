/*

Copyright (c) 2015, Pavel Umnikov 
All rights reserved. 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met: 

 * Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY 
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
DAMAGE. 

 */

#include "io/stream.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, io)

bool stream::can_read() const
{
    return false;
}

bool stream::can_write() const
{
    return false;
}

bool stream::can_seek() const
{
    return false;
}

size_t stream::get_size() const
{
    return 0;
}

size_t stream::get_position() const
{
    return 0;
}

void stream::close()
{
}

bool stream::is_open() const
{
    return true;
}

bool stream::write(memory::buffer_ref source, size_t bytes_to_write)
{
    XR_DEBUG_ASSERTION(this->is_open());
    XR_UNREFERENCED_PARAMETER(source, bytes_to_write);

    return false;
}

bool stream::read(memory::buffer_ref destination, size_t bytes_to_read)
{
    XR_DEBUG_ASSERTION(this->is_open());
    XR_UNREFERENCED_PARAMETER(destination, bytes_to_read);
    return false;
}

void stream::seek(ptrdiff_t offset, seek_origin origin)
{
    XR_DEBUG_ASSERTION(this->is_open());
    XR_UNREFERENCED_PARAMETER(offset, origin);
}

void stream::flush()
{
    XR_DEBUG_ASSERTION(this->is_open());
}

bool stream::eof() const
{
    XR_DEBUG_ASSERTION(this->is_open());
    return true;
}

XR_NAMESPACE_END(xr, io)
//-----------------------------------------------------------------------------------------------------------