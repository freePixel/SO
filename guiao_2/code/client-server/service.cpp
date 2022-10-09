#include "service.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdint.h>
#include <string.h>

#include "buffer.h"
#include "fifo.h"
#include "process.h"


namespace Service
{
    #define N 10

  
    static Buffer::BUFFER pool[N];
    static Fifo::FIFO* freeBuffers; //pointer avoids c++ default struct constructor call
    static Fifo::FIFO* pendingRequests;

    void create()
    {
        freeBuffers = (Fifo::FIFO*)malloc(sizeof(Fifo::FIFO));
        pendingRequests = (Fifo::FIFO*)malloc(sizeof(Fifo::FIFO));

        Fifo::create(*freeBuffers);
        Fifo::create(*pendingRequests);
        for(int i=0;i<N;i++)
            Fifo::in(*freeBuffers , i);
    }

    void destroy()
    {
        Fifo::destroy(*freeBuffers);
        Fifo::destroy(*pendingRequests);
    }

    /*          FIFO Functions          */

    /*-------------------------------------------*/

    void callService(ServiceRequest& req , ServiceResponse& res)
    {
        int id;
        Fifo::out(*freeBuffers , id); //get id
        Buffer::write(pool[id] , (char*)req.op , sizeof(req.op));
        Buffer::write(pool[id] , res.data , res.size);
        Fifo::in(*pendingRequests , id);
        //wait until server writes
        res.data = (char*)malloc(sizeof(char) * pool[id].length);
        memcpy(res.data , pool[id].data , pool[id].length);
        res.size = pool[id].length;
    }

    void processService()
    {
        int id;
        Fifo::out(*pendingRequests , id);
        
    }
}