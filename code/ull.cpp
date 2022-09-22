/*
 *
 * \author (2016) Artur Pereira <artur at ua.pt>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "ull.h"

namespace ull
{
    /* ************************************************* */

    /* The information support data structure  */
    struct Register
    {
        uint32_t nmec;       //!< student number
        const char *name;    //!< student name
    };

    /* The linked-list support data structure */
    struct Node 
    {
        Register reg;
        struct Node *next;
    };

    static Node *head = NULL;

    /* ************************************************* */

    void reset()
    {
        Node* ptr = head;
        while(ptr != NULL)
        {
            Node* temp = ptr->next;
            free((void*)ptr->reg.name);
            free(ptr);
            ptr = temp;
        }

        head = NULL;
    }

    /* ************************************************* */

    void load(const char *fname)
    {
        FILE* ptr = fopen(fname , "r");
        if(ptr == NULL)
        {
            fprintf(stderr , "File can't be opened!");
        }
        else{
            ssize_t read;
            int lc = 1;
            size_t len = 0;
            char* line = NULL;
            while((read = getline(&line,&len,ptr) != -1))
            {
                char* name = new char[50];
                int32_t nmec;
                
                sscanf(line , "%[^;]; %d\n" , name , &nmec);
                //fprintf(stderr , "{%s,%d}\n" , name , nmec);
                if(name != NULL)
                {
                    insert(nmec , name);
                    
                }
                else{
                    fprintf(stderr , "Error reading file, line = %d\n" , lc);
                }

                delete name;
            
                lc++;
            }
            delete line;
        }
        fclose(ptr);

    }

    /* ************************************************* */

    void print()
    {
        Node* ptr = head;
        while(ptr != NULL)
        {
            if(ptr->next != NULL) printf("{%d , %s} -> " , ptr->reg.nmec , ptr->reg.name);
            else{
                printf("{%d , %s}" , ptr->reg.nmec , ptr->reg.name);
            }
            ptr = ptr->next;
        }
    }

    /* ************************************************* */

    void insert(uint32_t nmec, const char *name)
    {
        if(head == NULL)
        {
            head = new Node();
            head->reg.nmec = nmec;
            head->reg.name = strdup(name);
            return;
        }
        Node* ptr = head;
        while(ptr != NULL)
        {
            //insert into end
            if(ptr->next == NULL)
            {
                ptr->next = new Node();
                ptr->next->reg.name = strdup(name);
                ptr->next->reg.nmec = nmec;
                return;
            }
            
            //insert into middle
            if(ptr->next->reg.nmec > nmec)
            {
                Node* temp = ptr->next;
                ptr->next = new Node();
                ptr->next->reg.name = strdup(name);
                ptr->next->reg.nmec = nmec;
                ptr->next->next = temp;
                return;
            }
            ptr = ptr->next;
        }
    }

    /* ************************************************* */

    const char *query(uint32_t nmec)
    {
        Node* ptr = head;
        while(ptr != NULL)
        {
            if(ptr->reg.nmec == nmec)
            {
                return strdup(ptr->reg.name); //return copy in order to avoid outside changes
            }
            ptr = ptr->next;
        }
        return NULL;
    }

    /* ************************************************* */

    void remove(uint32_t nmec)
    {
        //base case
        if(head->reg.nmec == nmec)
        {
            Node* temp = head->next;
            free((void*)head->reg.name);
            free(head);
            head = temp;
            return;
        }

        //middle case
        Node* ptr = head;
        while(ptr->next != NULL)
        {
            if(ptr->next->reg.nmec == nmec)
            {
                if(ptr->next->next != NULL)
                {
                    Node* temp = ptr->next->next;
                    free((void*)ptr->next->reg.name);
                    free(ptr->next);
                    ptr->next = temp;
                    return;
                }
                else{
                    free((void*)ptr->next->reg.name);
                    free(ptr->next);
                    ptr->next = NULL;
                    return;
                }
            }

            ptr = ptr->next;
        }
    }

    /* ************************************************* */

    void saveIntoFile(const char* output_file)
    {
        Node* ptr = head;
        FILE* file = fopen(output_file , "w");

        if(file == NULL)
        {
            fprintf(stderr , "Error creating file, name= %s\n" , output_file);
        }

        while(ptr != NULL)
        {
            int nmec = ptr->reg.nmec;
            const char* name = ptr->reg.name;
            uint32_t size = floor(log10(abs(nmec))) + strlen(name) + 4;
            char* output_str = new char[size];
            sprintf(output_str , "%s;%d\n" , name , nmec);
            fputs(output_str , file);
            delete output_str;
            ptr = ptr->next;
        }

        fclose(file);
    }





    /* ************************************************* */

}
