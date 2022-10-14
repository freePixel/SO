#pragma once

#include "fifo.h"
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>

#include "thread.h"
#include "dbc.h"

namespace Fifo
{
    struct FIFO
    {

        int semid;
        uint32_t ii;
        uint32_t ri;
        uint32_t cnt;
        int slot[10];
        int fifoId;

        pthread_mutex_t accessCR = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t fifoNotFull = PTHREAD_COND_INITIALIZER;
        pthread_cond_t fifoNotEmpty = PTHREAD_COND_INITIALIZER;

    };

    FIFO* create();
    void destroy(FIFO& _fifo);
    void in(FIFO& _fifo , int value);
    void out(FIFO& _fifo , int& value);
    
}