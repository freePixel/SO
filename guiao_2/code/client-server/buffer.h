#pragma once

#define BSIZE 128

namespace Buffer
{
    struct BUFFER
    {
        int length;
        char data[BSIZE];
    };
    void clear(BUFFER& _buffer);
    void write(BUFFER& _buffer , char data[] , int length);
    void read(BUFFER& _buffer , char* dest);
}