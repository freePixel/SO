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
        int bufferId = pshmget(IPC_PRIVATE , sizeof(BUFFER) , 0600 | IPC_CREAT | IPC_EXCL);

        BUFFER* buffer = (BUFFER*)pshmat(bufferId , NULL , 0);
        buffer->bufferId = bufferId;
        buffer->semid = psemget(IPC_PRIVATE , 1 , 0600 | IPC_CREAT | IPC_EXCL);

        set_solved(*buffer);

        return buffer;
    }

    void wait_until_solved(BUFFER& _buffer)
    {
        struct sembuf op = {0 , -1 , 0};
        psemop(_buffer.semid , &op , 1); //put semaphore at 0

        psemop(_buffer.semid , &op , 1); //wait until server increments semaphore
    }

    void set_solved(BUFFER& _buffer)
    {
        struct sembuf op = {0 , 1 , 0};
        psemop(_buffer.semid , &op , 1);
    }

    void destroy(Buffer::BUFFER& _buffer)
    {
        int buffid = _buffer.bufferId;
        pshmdt(&_buffer);
        pshmctl(buffid , IPC_RMID , NULL);
    }
}