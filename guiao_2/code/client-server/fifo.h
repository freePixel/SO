#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdint.h>

#include "process.h"

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

    };

    FIFO* create();
    void destroy(FIFO& _fifo);
    void in(FIFO& _fifo , int value);
    void out(FIFO& _fifo , int& value);
    
}