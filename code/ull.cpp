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

}
