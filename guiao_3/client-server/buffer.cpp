#include "buffer.h"
#include <stdexcept>
#include <string.h>

#define SOLVED 0

namespace Buffer
{
    void clear(BUFFER& _buffer)
    {
        _buffer.length = 0;
    }

    void write(BUFFER& _buffer , char data[] , int length)
    {
        if(_buffer.length + length >= BSIZE)
        {
            throw std::runtime_error("Buffer length exceeded");
        }
        else{
            memcpy(&_buffer.data[_buffer.length] , data , length);  
            _buffer.length += length;
        }
    }

    void read(BUFFER& _buffer , char* dest)
    {
        memcpy(dest , _buffer.data , _buffer.length);
        clear(_buffer);
    }

    Buffer::BUFFER* create()
    {
        return new BUFFER;
    }

    void wait_until_solved(BUFFER& _buffer)
    {

    }

    void set_solved(BUFFER& _buffer)
    {

    }

    void destroy(Buffer::BUFFER& _buffer)
    {
        delete &_buffer;
    }
}