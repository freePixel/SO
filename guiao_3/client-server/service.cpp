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

    static Buffer::BUFFER *pool[N];
    static Fifo::FIFO *freeBuffers;
    static Fifo::FIFO *pendingRequests;

    void create()
    {
        freeBuffers = Fifo::create();
        pendingRequests = Fifo::create();

        for (int i = 0; i < N; i++)
            Fifo::in(*freeBuffers, i);

        for (int i = 0; i < N; i++)
            pool[i] = Buffer::create();
    }

    void destroy()
    {
        Fifo::destroy(*freeBuffers);
        Fifo::destroy(*pendingRequests);
        for (int i = 0; i < 10; i++)
            Buffer::destroy(*pool[i]);
    }

    /*          FIFO Functions          */

    /*-------------------------------------------*/

    static void produceResponse(ServiceRequest &req, ServiceResponse &res)
    {
        if (req.op == operation::letters)
        {
            uint letters = 0;
            for (int i = 0; i < req.size; i++)
            {
                if ((req.data[i] >= 'a' && req.data[i] <= 'z') || (req.data[i] >= 'A' && req.data[i] <= 'Z'))
                    letters++;
            }
            char *response = (char *)malloc(2 * sizeof(char));
            response[0] = (char)letters;
            response[1] = '\0';
            res.data = response;
            res.size = 2 * sizeof(char);
        }

        else if (req.op == operation::digits)
        {
            uint digits = 0;
            for (int i = 0; i < req.size; i++)
            {
                if (req.data[i] >= '0' && req.data[i] <= '9')
                    digits++;
            }
            char *response = (char *)malloc(2 * sizeof(char));
            response[0] = (char)digits;
            response[1] = '\0';
            res.data = response;
            res.size = 2 * sizeof(char);
        }
    }

    void callService(ServiceRequest &req, ServiceResponse &res)
    {
        int id;
        Fifo::out(*freeBuffers, id); // take a buffer out of fifo of free buffers
        char op = (char)req.op;
        Buffer::write(*pool[id], &op, sizeof(char)); // put request data on buffer
        Buffer::write(*pool[id], req.data, req.size);
        Fifo::in(*pendingRequests, id);       // add buffer to fifo of pending requests
        Buffer::wait_until_solved(*pool[id]); // wait (blocked) until a response is available */
        res.data = (char *)malloc(sizeof(char) * pool[id]->length);
        res.size = pool[id]->length;
        Buffer::read(*pool[id], res.data);
    }

    void processService()
    {
        int id;
        Fifo::out(*pendingRequests, id); // get id

        ServiceRequest req;
        req.data = (char *)malloc((pool[id]->length) * sizeof(char));
        req.size = (pool[id]->length) * sizeof(char);
        Buffer::read(*pool[id], req.data);

        operation op = (operation)(int)(req.data[0]);
        req.op = op;
        ServiceResponse res;
        produceResponse(req, res); // produce response
        free(req.data);
        Buffer::clear(*pool[id]);
        Buffer::write(*pool[id], res.data, res.size);
        free(res.data);
        Buffer::set_solved(*pool[id]);
    }
}