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

Service::ServiceRequest get_random_request()
{
    Service::ServiceRequest request;
    request.op = (Service::operation)(rand() % 2);
    uint random_string_idx = rand() % (sizeof(random_strings) / sizeof(const char*));
    uint str_size = sizeof(random_strings[random_string_idx]) / sizeof(const char);
    request.data = (char*)malloc(sizeof(char) * str_size);
    request.size = str_size;
    return request;
}

void produce_and_get_response()
{
    Service::ServiceRequest sreq = get_random_request();
    Service::ServiceResponse response;
    Service::callService(sreq , response);
    printf("Server awnser: %d\n" , response.data[0]);

}

void consume()
{
    Service::processService();
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
        printf("Consumer %d terminated\n" , cpid[i]);
    }

    for(unsigned int i=0;i<producers;i++)
    {
        pwaitpid(ppid[i] , NULL , 0);
        printf("Producer %d terminated\n" , ppid[i]);
    }

    Service::destroy();

    return EXIT_SUCCESS;

}