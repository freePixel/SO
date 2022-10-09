#include "service.h"
#include <string.h>
#include "process.h"
#include <stdlib.h>


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
}

void consume()
{
    Service::processService();
}

int main(int argc , char* argv[])
{
    unsigned int producers  = 4;
    unsigned int consumers  = 4;

    pid_t cpid[consumers];

    for(unsigned int i = 0 ; i < consumers; i++)
    {
        if((cpid[i] = pfork()) == 0)
        {
            consume();
            exit(0);
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
    }

    for(unsigned int i=0;i<consumers;i++)
    {
        pwaitpid(cpid[i] , NULL , 0);
    }

    for(unsigned int i=0;i<producers;i++)
    {
        pwaitpid(ppid[i] , NULL , 0);
    }

    return EXIT_SUCCESS;

}