#include "buffer.h"
#include <stdexcept>
#include <string.h>



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
            for(int i=0;i<length;i++)
            {
                _buffer.data[i+_buffer.length] = data[i];
            }
            _buffer.length += length;            
        }
    }

    void read(BUFFER& _buffer , char* dest)
    {
        memcpy(dest , _buffer.data , _buffer.length);
        clear(_buffer);
    }
}