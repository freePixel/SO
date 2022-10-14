#include "fifo.h"

namespace Fifo
{
    #define N 10

    FIFO* create()
    {
        FIFO* fifo = (FIFO*)malloc(sizeof(fifo));
        
        if(fifo == NULL)
        {
            perror("Fail creating fifo");
            exit(EXIT_FAILURE);
        }

        for(uint32_t i=0;i<N;i++)
        {
            fifo->slot[i] = -1;
        }
        fifo->ii = fifo->ri = 0;
        fifo->cnt = 0;

        return fifo;
    }

    void destroy(FIFO& _fifo)
    {
        require(&_fifo != NULL , "fifo must exist");
        if(&_fifo != NULL)
        {
            free(&_fifo);
        }
    }

    static bool isFull(FIFO& _fifo)
    {
        return _fifo.cnt == N;
    }

    static bool isEmpty(FIFO& _fifo)
    {
        return _fifo.cnt == 0;
    }

    void in(FIFO& _fifo , int value)
    {
        mutex_lock(&_fifo.accessCR);

        while(isFull(_fifo))
        {
            cond_wait(&_fifo.fifoNotFull , &_fifo.accessCR);
        }

        _fifo.slot[_fifo.ii] = value;
        _fifo.ii = (_fifo.ii + 1) % N;
        _fifo.cnt++;

        cond_broadcast(&_fifo.fifoNotEmpty);
        mutex_unlock(&_fifo.accessCR);        
    }

    void out(FIFO& _fifo , int& value)
    {
        mutex_lock(&_fifo.accessCR);

        while(isEmpty(_fifo))
        {
            cond_wait(&_fifo.fifoNotEmpty , &_fifo.accessCR);
        }
        value = _fifo.slot[_fifo.ri];
        _fifo.ri = (_fifo.ri + 1) % N;
        _fifo.cnt--;

        cond_broadcast(&_fifo.fifoNotFull);
        mutex_unlock(&_fifo.accessCR);
        
    }

    
}