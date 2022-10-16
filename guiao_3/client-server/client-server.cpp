#include "service.h"
#include <string.h>
#include "process.h"
#include <stdlib.h>
#include <stdio.h>
#include<ctime>
#include  "thread.h"

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
    uint random_string_idx = rand() % (sizeof(random_strings) / sizeof(random_strings[0]));
    uint str_size = strlen(random_strings[random_string_idx]);
    request.data = (char*)malloc(sizeof(char) * str_size);
    memcpy(request.data , random_strings[random_string_idx] , str_size);
    request.size = str_size;
    return request;
}

void* produce_and_get_response(void* parg)
{
    srand(gettid());
    Service::ServiceRequest sreq = get_random_request();
    if(sreq.op == Service::operation::letters)
        printf("Client query (thread id %d): operation = letters , text = %s\n" , gettid() ,sreq.data);
    else if(Service::operation::digits){
        printf("Client query (thread id %d): operation = digits  , text = %s\n" , gettid() ,sreq.data);
    }
    Service::ServiceResponse response;
    Service::callService(sreq , response);
    printf("Server awnser (thread id %d): %d\n" , gettid() ,response.data[0]);
    free(sreq.data);
    free(response.data);
    return NULL;
    
}

void* consume(void *argp)
{
    printf("Consuming (thread id %d)\n" , gettid());
    Service::processService();
    printf("thread id %d successfully consumed\n" , gettid());
    return NULL;
}

struct ARGV
{
    uint id;
    uint niter;
};

int main(int argc , char* argv[])
{
    uint niter = 1;
    uint nthr = 4;
    
    Service::create();
    

    pthread_t cthr[nthr];
    ARGV carg[nthr];
    printf("Launching %d consumer threads, each performing %d iterations\n" , nthr , niter);
    for(uint i=0;i<nthr;i++)
    {
        carg[i].id = i;
        carg[i].niter = niter;
        thread_create(&cthr[i] , NULL , consume , &carg[i]);
    }

    pthread_t pthr[nthr];
    ARGV parg[nthr];
    printf("Launchind %d producer threads, each performing %d iterations\n" , nthr , niter);
    for(uint i=0;i<nthr;i++)
    {
        parg[i].id = i;
        parg[i].niter = niter;
        thread_create(&pthr[i] , NULL , produce_and_get_response , &parg[i]);
    }

    for(uint i=0;i<nthr;i++)
    {
        thread_join(pthr[i] , NULL);
        printf("Producer thread %d has terminated\n" , i);
    }

    for(uint i=0;i<nthr;i++)
    {
        thread_join(cthr[i] , NULL);
        printf("Consumer thread %d has terminated\n" , i);
    }

    Service::destroy();
    
    return EXIT_SUCCESS;

}