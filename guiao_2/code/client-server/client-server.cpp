#include "service.h"
#include <string.h>
#include "process.h"
#include <stdlib.h>
#include <stdio.h>

const char* random_strings[] = {
    "Lorem",
    "Ipsum",
    "is",
    "simply",
    "dummy",
    "of",
    "the",
    "printing",
    "and",
    "typesetting",
    "industry",
    "1500",
    "unchanged",
    "desktop",
    "publishing"};

void produce_and_get_response()
{
    Service::ServiceRequest sreq;
    char* name = new char[9];
    strcpy(name , "Jonh Doe");
    sreq.data = name;
    Service::operation op = Service::operation::letters;
    sreq.op = op;
    sreq.size = 9;

    Service::ServiceResponse response;
    Service::callService(sreq , response);
    printf("Producer terminated\n");
}

void consume()
{
    Service::processService();
    printf("Consumer terminated\n");
}

int main(int argc , char* argv[])
{
    unsigned int producers  = 4;
    unsigned int consumers  = 4;

    Service::create();

    pid_t cpid[consumers];

    for(unsigned int i = 0 ; i < consumers; i++)
    {
        if((cpid[i] = pfork()) == 0)
        {
            consume();
            exit(0);
        }
        else{
            printf("Consumer %d launched\n" , cpid[i]);
        }
    }

    pid_t ppid[producers];

    for(unsigned int i=0;i< producers;i++)
    {
        if((ppid[i] = pfork()) == 0)
        {
            produce_and_get_response();
            exit(0);
        }
        else{
            printf("Producer %d launched\n" , ppid[i]);
        }
    }

    for(unsigned int i=0;i<consumers;i++)
    {
        pwaitpid(cpid[i] , NULL , 0);
    }

    for(unsigned int i=0;i<producers;i++)
    {
        pwaitpid(ppid[i] , NULL , 0);
    }

    Service::destroy();

    return EXIT_SUCCESS;

}