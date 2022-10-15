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
        mutex_lock(&_buffer.accessCR);

        if(_buffer.length + length >= BSIZE)
        {
            throw std::runtime_error("Buffer length exceeded");
        }
        else{
            memcpy(&_buffer.data[_buffer.length] , data , length);  
            _buffer.length += length;
        }

        mutex_unlock(&_buffer.accessCR);
    }

    void read(BUFFER& _buffer , char* dest)
    {
        mutex_lock(&_buffer.accessCR);
        memcpy(dest , _buffer.data , _buffer.length);
        clear(_buffer);
        mutex_unlock(&_buffer.accessCR);
    }

    Buffer::BUFFER* create()
    {
        BUFFER* buffer = new BUFFER;
        buffer->accessCR = PTHREAD_MUTEX_INITIALIZER;
        buffer->solved = PTHREAD_COND_INITIALIZER;
        return buffer;
    }

    void wait_until_solved(BUFFER& _buffer)
    {
        cond_wait(&_buffer.solved , &_buffer.accessCR);
    }

    void set_solved(BUFFER& _buffer)
    {
        cond_broadcast(&_buffer.solved);
    }

    void destroy(Buffer::BUFFER& _buffer)
    {
        delete &_buffer;
    }
}