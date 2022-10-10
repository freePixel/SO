#include "fifo.h"
#include <stdexcept>
namespace Fifo
{
    #define N 10

    #define NACCESS 0
    #define NVALUES 1
    #define NSLOTS 2

    static void down(int semid, unsigned short index)
    {
        struct sembuf op = {index, -1, 0};
        psemop(semid, &op, 1);
    }

    /* ************************************************* */

    static void up(int semid, unsigned short index)
    {
        struct sembuf op = {index, 1, 0};
        psemop(semid, &op, 1);
    }


    

    FIFO* create()
    {
        int fifoId = pshmget(IPC_PRIVATE , sizeof(FIFO) , 0600 | IPC_CREAT | IPC_EXCL);
        FIFO* fifo = (FIFO*)pshmat(fifoId , NULL , 0);
        fifo->fifoId = fifoId;
        for(uint32_t i = 0; i < 10 ; i++)
            fifo->slot[i] = -1;

        fifo->ii = 0;
        fifo->ri = 0;
        fifo->cnt = 0;
        
        fifo->semid = psemget(IPC_PRIVATE, 3 , 0600 | IPC_CREAT | IPC_EXCL);

        for(int i=0;i<10;i++)
            up(fifo->semid , NSLOTS);

        up(fifo->semid , NACCESS);

        if(fifo == NULL) throw std::runtime_error("Error creating buffer");
        return fifo;
    }

    void destroy(FIFO& _fifo)
    {
        pshmctl(_fifo.fifoId , IPC_RMID , NULL);
        pshmdt(&_fifo);
    }

    void in(FIFO& _fifo , int value)
    {
        down(_fifo.semid , NSLOTS);
        down(_fifo.semid , NACCESS);

        _fifo.slot[_fifo.ii] = value;
        _fifo.ii = (_fifo.ii + 1) % N;
        _fifo.cnt++;

        up(_fifo.semid , NVALUES);
        up(_fifo.semid , NACCESS);
    }

    void out(FIFO& _fifo , int& value)
    {
        down(_fifo.semid , NVALUES);
        down(_fifo.semid , NACCESS);

        value = _fifo.slot[_fifo.ri];
        _fifo.ri = (_fifo.ri + 1) % N;
        _fifo.cnt--;

        up(_fifo.semid , NACCESS);
        up(_fifo.semid , NSLOTS);
    }

    
}