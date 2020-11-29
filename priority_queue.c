#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "priority_queue.h"

/**we can add here more things if needed! */
struct PriorityQueue_t
{
    PQElement pqe_element; 
    PQElementPriority pq_element_priority;
    PriorityQueue next_in_line;
    PriorityQueue iterator;

    CopyPQElement copyElement;
    FreePQElement freePqElement;
    EqualPQElements equalPqElement;
    CopyPQElementPriority copyPqElementPriority;
    FreePQElementPriority freePqElementPriority; 
    ComparePQElementPriorities comparePqElementPriority
};

PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element, EqualPQElements equal_elements,
                    CopyPQElementPriority copy_priority,FreePQElementPriority free_priority, ComparePQElementPriorities compare_priorities)
{
    if(copy_element == NULL || free_element ==NULL || equal_elements == NULL || copy_priority == NULL || free_priority==NULL||compare_priorities==NULL)
    {
        return NULL;
    }
    PriorityQueue priority_queue = malloc(sizeof(PriorityQueue));
    if(priority_queue == NULL)
    {
        return NULL; 
    }
    priority_queue-> copyElement = copy_element;
    priority_queue-> freePqElement = free_element;
    priority_queue-> equalPqElement = equal_elements;
    priority_queue-> copyPqElementPriority = copy_priority;
    priority_queue-> freePqElementPriority = free_priority;
    priority_queue-> comparePqElementPriority = compare_priorities;
    priority_queue->pq_element_priority = NULL;
    priority_queue->pqe_element = NULL;
    priority_queue-> next_in_line = NULL;
    priority_queue->iterator = NULL;
    return priority_queue;
 }
 /** Internal function, to create next element in our queue*/ 
static PriorityQueue CreateNextInLine(PriorityQueue our_queue, PQElement element, PQElementPriority element_priority)
{
    PriorityQueue priority_queue = malloc(sizeof(PriorityQueue));
    if(priority_queue==NULL)
    {
     return NULL;
    }
    priority_queue-> copyElement = NULL;
    priority_queue-> freePqElement = NULL;
    priority_queue-> equalPqElement = NULL;
    priority_queue-> copyPqElementPriority = NULL;
    priority_queue-> freePqElementPriority = NULL;
    priority_queue-> comparePqElementPriority = NULL;
    priority_queue->pqe_element = our_queue->copyElement(element);
    if(priority_queue->pqe_element ==NULL)
    {
        free(priority_queue);
        return NULL;
    }
    priority_queue->pq_element_priority = our_queue->copyPqElementPriority(element_priority);
     if(priority_queue->pq_element_priority ==NULL)
    {
        free(priority_queue);
        return NULL;
    }
    priority_queue-> next_in_line = NULL;
    priority_queue->iterator = NULL;
    return priority_queue; 
}
 PriorityQueue pqCopy(PriorityQueue queue)
 {
    if(queue == NULL)
    {
        return NULL;
    }
     /** Allocating the new queue for the first time.*/ 
    PriorityQueue copied_priority_queue  = pqCreate(queue->copyElement, queue->freePqElement, queue->equalPqElement, queue->copyPqElementPriority, 
    queue->freePqElementPriority, queue->comparePqElementPriority);
    if(copied_priority_queue==NULL)
    {
        return NULL;
    }
     /** making temp pointers to go through the queue.*/ 
    PriorityQueue temp_queue_pointer = queue->next_in_line;
    PriorityQueue temp_copied_priority_queue_pointer = copied_priority_queue;

    /**Maybe there is a way to make the code shotrer and not duplicate*/
    
    while(temp_queue_pointer!=NULL) 
    {
        /**copying the next one in the queue*/
        temp_copied_priority_queue_pointer->next_in_line = CreateNextInLine(copied_priority_queue, temp_queue_pointer->pqe_element, temp_queue_pointer->pq_element_priority);
        if(temp_copied_priority_queue_pointer->next_in_line==NULL)
        {
            pqDestroy(copied_priority_queue);
            return NULL;
        }
        temp_queue_pointer = temp_queue_pointer->next_in_line;
        temp_copied_priority_queue_pointer = temp_copied_priority_queue_pointer->next_in_line;
    }
    return copied_priority_queue;
 }

 bool pqContains(PriorityQueue queue, PQElement element)
 {
     if(element == NULL || queue == NULL)
     {
         return false; 
     }
     PriorityQueue temp_queue_pointer = queue;
     while(temp_queue_pointer != NULL)
     {
         if(queue->equalPqElement(temp_queue_pointer->pqe_element, element))
         {
            return true;
         }
        temp_queue_pointer = temp_queue_pointer->next_in_line; 
     }
     return false; 
 }


/** THIS FUNCTION NOT FINISHED YET!!!*/
PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element, PQElementPriority old_priority, PQElementPriority new_priority)
{
    if(queue == NULL || element == NULL || old_priority == NULL || new_priority == NULL)
    {
        return PQ_NULL_ARGUMENT; 
    }
    PriorityQueue temp_queue_pointer = queue->next_in_line;
    while (temp_queue_pointer!=NULL)
    {
        if(queue->equalPqElement(temp_queue_pointer->pqe_element, element) == true && queue->comparePqElementPriority(temp_queue_pointer->pq_element_priority,old_priority) == 0)
        {
            /** continue from here!*/ 
        }
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}


