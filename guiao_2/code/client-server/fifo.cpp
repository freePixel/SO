#include "fifo.h"

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


    

    void create(FIFO& _fifo)
    {
        
    }

    void destroy(FIFO& _fifo)
    {
        
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