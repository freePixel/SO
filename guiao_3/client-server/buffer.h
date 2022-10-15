#pragma once

#define BSIZE 128
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdint.h>

#include "process.h"
#include "thread.h"
#include "dbc.h"

namespace Buffer
{
    struct BUFFER
    {
        int length;
        char data[BSIZE];
        pthread_cond_t solved;
        pthread_mutex_t accessCR;
    };
    BUFFER* create();
    void destroy(BUFFER& _buffer);
    void clear(BUFFER& _buffer);
    void write(BUFFER& _buffer , char data[] , int length);
    void read(BUFFER& _buffer , char* dest);

    void wait_until_solved(BUFFER& _buffer);
    void set_solved(BUFFER& _buffer);
}