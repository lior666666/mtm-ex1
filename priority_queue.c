#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "priority_queue.h"

#define ELEMENTS_IN_EMPTY_QUEUE -1

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
    ComparePQElementPriorities comparePqElementPriority;
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
    priority_queue->copyElement = copy_element;
    priority_queue->freePqElement = free_element;
    priority_queue->equalPqElement = equal_elements;
    priority_queue->copyPqElementPriority = copy_priority;
    priority_queue->freePqElementPriority = free_priority;
    priority_queue->comparePqElementPriority = compare_priorities;
    priority_queue->pq_element_priority = NULL;
    priority_queue->pqe_element = NULL;
    priority_queue-> next_in_line = NULL;
    priority_queue->iterator = NULL;
    return priority_queue;
 }

 void pqDestroy(PriorityQueue queue)
 {
     while(queue != NULL)
     {
        queue->freePqElement(queue->pqe_element);
        queue->freePqElementPriority(queue->pq_element_priority);
        pqDestroy(queue->next_in_line); //destroy all the list with recursion
        pqDestroy(queue->iterator); //destroy the iterator (in all other elements with iterator==NULL, the recursion will stop immediately)
        free(queue);
     }
 }

 PriorityQueue pqCopy(PriorityQueue queue)
 {
    if(queue == NULL)
    {
        return NULL;
    }
    PriorityQueue copied_priority_queue  = pqCreate(queue->copyElement, queue->freePqElement, queue->equalPqElement, queue->copyPqElementPriority, 
    queue->freePqElementPriority, queue->comparePqElementPriority);
    if(copied_priority_queue==NULL)
    {
        return NULL;
    }
    copied_priority_queue->pqe_element = queue->copyElement(queue->pqe_element);
    copied_priority_queue->pq_element_priority = queue->copyPqElementPriority(queue->pq_element_priority);
    copied_priority_queue->iterator = copied_priority_queue->next_in_line;
    queue->iterator = queue->next_in_line;
    /**Maybe there is a way to make the code shotrer and not duplicate*/
    /**Also, maybe we don't want to pass the fileds of the pointers to the functions to every new element in the queue.*/
    /**Also, I am not sure if we are supposed to use the iterator inside the function or not .*/
    
    while(queue->iterator!=NULL)
    {
        copied_priority_queue->iterator = pqCreate(queue->iterator->copyElement, queue->iterator->freePqElement, queue->iterator->equalPqElement, queue->iterator->copyPqElementPriority, 
        queue->iterator->freePqElementPriority, queue->iterator->comparePqElementPriority);
        if(copied_priority_queue->iterator==NULL)
        {
            pqDestroy(copied_priority_queue);
            return NULL;
        }
        copied_priority_queue->iterator->pqe_element = queue->iterator->copyElement(queue->iterator->pqe_element);
        copied_priority_queue->iterator->pq_element_priority = queue->iterator->copyPqElementPriority(queue->iterator->pq_element_priority);
        copied_priority_queue->iterator = copied_priority_queue->iterator->next_in_line;
        queue->iterator = queue->iterator->next_in_line;
    }
    return copied_priority_queue;
 }

 int pqGetSize(PriorityQueue queue)
 {
     int counter = 0;
     while(queue != NULL)
     {
         counter++;
         queue = queue->next_in_line;
     }
     return counter-ELEMENTS_IN_EMPTY_QUEUE; //return the length of the queue BUT without the head (that does not represent an element in the queue)
 }

/** I am not sure if we are supposed to use the iterator inside the function or not .*/
 bool pqContains(PriorityQueue queue, PQElement element)
 {
     if(element == NULL || queue == NULL)
     {
         return false; 
     }
     queue->iterator = queue; 
     while(queue->iterator != NULL)
     {
         if(queue->equalPqElement(queue->pqe_element, element)==0)
         {
            return true;
         }
        queue->iterator = queue->iterator->next_in_line; 
     }
     return false; 
 }

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if (queue == NULL || element == NULL || priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    PriorityQueue new_element_in_queue = malloc(sizeof(PriorityQueue));
    PQElement new_element = queue->copyElement(element);
    PQElementPriority new_priority = queue->copyPqElementPriority(priority);
    if(new_element_in_queue == NULL || new_element == NULL || new_priority == NULL) //if copy failed, Do the value is NULL? need to be checked
    {
        return PQ_OUT_OF_MEMORY;
    }
    new_element_in_queue->copyElement = NULL;
    new_element_in_queue->freePqElement = NULL;
    new_element_in_queue->equalPqElement = NULL;
    new_element_in_queue->copyPqElementPriority = NULL;
    new_element_in_queue->freePqElementPriority = NULL;
    new_element_in_queue->comparePqElementPriority = NULL;
    new_element_in_queue->pq_element_priority = new_priority;
    new_element_in_queue->pqe_element = new_element;
    new_element_in_queue->iterator = NULL; //Iterator's value is undefined after this operation

    PriorityQueue previous_element_in_queue = queue;
    PriorityQueue current_element = queue->next_in_line;
    while(current_element != NULL) //enter only if there is at least one real element in the queue
    {
        if(queue->comparePqElementPriority(current_element->pq_element_priority, new_priority) < 0)
        {
            previous_element_in_queue->next_in_line = new_element_in_queue;
            new_element_in_queue->next_in_line = current_element;
            return PQ_SUCCESS;
        }
        previous_element_in_queue = current_element;
        current_element = current_element->next_in_line;
    }
    //if there is no real elements in the queue OR new_priority is the lowest priority
    current_element->next_in_line = new_element_in_queue;
    new_element_in_queue->next_in_line = NULL;
    return PQ_SUCCESS;
}
