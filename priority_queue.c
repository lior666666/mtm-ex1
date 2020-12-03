#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "priority_queue.h"

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

 /** Internal function, to create next element in our queue
  * recieve the head of the queue, element and priority
  * return new element for the queue*/ 
static PriorityQueue createNextInLine(PriorityQueue queue_head, PQElement element, PQElementPriority element_priority)
{
    PriorityQueue priority_queue = malloc(sizeof(*priority_queue));
    if(priority_queue == NULL)
    {
        return NULL;
    }
    priority_queue->copyElement = NULL;
    priority_queue->freePqElement = NULL;
    priority_queue->equalPqElement = NULL;
    priority_queue->copyPqElementPriority = NULL;
    priority_queue->freePqElementPriority = NULL;
    priority_queue->comparePqElementPriority = NULL;
    priority_queue->pqe_element = queue_head->copyElement(element);
    if(priority_queue->pqe_element == NULL)
    {
        free(priority_queue);
        return NULL;
    }
    priority_queue->pq_element_priority = queue_head->copyPqElementPriority(element_priority);
     if(priority_queue->pq_element_priority == NULL)
    {
        free(priority_queue);
        return NULL;
    }
    priority_queue->next_in_line = NULL;
    priority_queue->iterator = NULL;
    return priority_queue; 
}

 /** Internal function, to insert specified element to the queue
  * recieve the head of queue, and element to insert
  * insert the element based on it's priority*/ 
static void insertElementPointer(PriorityQueue queue_head, PriorityQueue new_element_in_queue) 
{
    PriorityQueue previous_element_in_queue = queue_head;
    PriorityQueue current_element = queue_head->next_in_line;
    while(current_element != NULL) //enter only if there is at least one real element in the queue
    {
        if(queue_head->comparePqElementPriority(current_element->pq_element_priority, new_element_in_queue->pq_element_priority) < 0)
        {
            previous_element_in_queue->next_in_line = new_element_in_queue;
            new_element_in_queue->next_in_line = current_element;
            return;
        }
        previous_element_in_queue = current_element;
        current_element = current_element->next_in_line;
    }
    //if there is no real elements in the queue OR new_priority is the lowest priority
    queue_head->next_in_line = new_element_in_queue;
    new_element_in_queue->next_in_line = NULL;
}

 /** Internal function, to destroy all elements in the queue
  * recieve the head of queue, the free functions*/ 
static void pqDestroyElements(PriorityQueue queue, FreePQElement free_pq_element, FreePQElementPriority free_pq_element_priority)
{
    while(queue != NULL)
    {
        free_pq_element(queue->pqe_element);
        free_pq_element_priority(queue->pq_element_priority);
        pqDestroyElements(queue->next_in_line, free_pq_element, free_pq_element_priority); //destroy all the list with recursion
        free(queue);
        break;
    }
}

//*** 1 ***
PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element, EqualPQElements equal_elements,
                    CopyPQElementPriority copy_priority,FreePQElementPriority free_priority, ComparePQElementPriorities compare_priorities)
{
    if(copy_element == NULL || free_element ==NULL || equal_elements == NULL || copy_priority == NULL || free_priority==NULL||compare_priorities==NULL)
    {
        return NULL;
    }
    PriorityQueue priority_queue = malloc(sizeof(*priority_queue));
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

//*** 2 ***
void pqDestroy(PriorityQueue queue)
{
    pqDestroyElements(queue, queue->freePqElement, queue->freePqElementPriority);
}

//*** 3 ***
PriorityQueue pqCopy(PriorityQueue queue)
{
    PriorityQueue queue_head = queue;
    if(queue_head == NULL)
    {
        return NULL;
    }
    queue_head->iterator = NULL;
    /** Allocating the new queue for the first time.*/ 
    PriorityQueue copied_priority_queue  = pqCreate(queue_head->copyElement, queue_head->freePqElement, queue_head->equalPqElement, queue_head->copyPqElementPriority, 
    queue_head->freePqElementPriority, queue_head->comparePqElementPriority);
    if(copied_priority_queue == NULL)
    {
        return NULL;
    }

    /** making temp pointers to go through the queue.*/ 
    PriorityQueue temp_queue_pointer = queue_head->next_in_line;
    PriorityQueue temp_copied_priority_queue_pointer = copied_priority_queue;

    while(temp_queue_pointer!=NULL) 
    {
        /**copying the next one in the queue*/
        temp_copied_priority_queue_pointer->next_in_line = createNextInLine(copied_priority_queue, temp_queue_pointer->pqe_element, temp_queue_pointer->pq_element_priority);
        if(temp_copied_priority_queue_pointer->next_in_line == NULL)
        {
            pqDestroy(copied_priority_queue);
            return NULL;
        }
        temp_queue_pointer = temp_queue_pointer->next_in_line;
        temp_copied_priority_queue_pointer = temp_copied_priority_queue_pointer->next_in_line;
    }
    return copied_priority_queue;
 }

//*** 4 ***
int pqGetSize(PriorityQueue queue)
{
    int counter = 0;
    while(queue != NULL)
    {
        counter++;
        queue = queue->next_in_line;
    }
    return counter-1; //return the length of the queue BUT without the head (that does not represent an element in the queue)
}

//*** 5 ***
bool pqContains(PriorityQueue queue, PQElement element)
{
    PriorityQueue queue_head = queue;
    if(element == NULL || queue_head == NULL)
    {
        return false; 
    }
    PriorityQueue temp_queue_pointer = queue_head;
    while(temp_queue_pointer != NULL)
    {
        if(queue_head->equalPqElement(temp_queue_pointer->pqe_element, element))
        {
            return true;
        }
        temp_queue_pointer = temp_queue_pointer->next_in_line; 
    }
    return false; 
}

//*** 6 ***
PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    PriorityQueue queue_head = queue;
    if(queue_head != NULL)
    {
        queue_head->iterator = NULL;
    }
    if (queue_head == NULL || element == NULL || priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    PriorityQueue new_element_in_queue = createNextInLine(queue_head, element, priority);
    if(new_element_in_queue == NULL)
    {
        return PQ_OUT_OF_MEMORY;
    }
    insertElementPointer(queue_head, new_element_in_queue);
    return PQ_SUCCESS;
}

//*** 7 ***
PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element, PQElementPriority old_priority, PQElementPriority new_priority)
{
    PriorityQueue queue_head = queue;
    if(queue_head != NULL)
    {
        queue_head->iterator = NULL;
    }
    if(queue_head == NULL || element == NULL || old_priority == NULL || new_priority == NULL)
    {
        return PQ_NULL_ARGUMENT; 
    }
    PriorityQueue current_queue_pointer = queue_head->next_in_line;
    PriorityQueue temp_queue_previous = queue_head;
    while(current_queue_pointer != NULL)  
    {
        if(queue_head->equalPqElement(current_queue_pointer->pqe_element, element) == true && queue_head->comparePqElementPriority(current_queue_pointer->pq_element_priority,old_priority) == 0) 
        {
            PriorityQueue temp_priority_element = queue_head->copyPqElementPriority(new_priority);
            if(temp_priority_element == NULL) 
            {
                return PQ_OUT_OF_MEMORY;
            }
            temp_queue_previous->next_in_line = current_queue_pointer->next_in_line;
            current_queue_pointer->next_in_line = NULL;
            queue_head->freePqElementPriority(current_queue_pointer->pq_element_priority);
            current_queue_pointer->pq_element_priority = temp_priority_element;
            insertElementPointer(queue, current_queue_pointer);
            return PQ_SUCCESS;
        }
        current_queue_pointer = current_queue_pointer->next_in_line;
        temp_queue_previous = temp_queue_previous->next_in_line;
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}

//*** 8 ***
PriorityQueueResult pqRemove(PriorityQueue queue)
{
    PriorityQueue queue_head = queue;
    if(queue_head == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    queue_head->iterator = NULL;
    PriorityQueue element_to_remove = queue_head->next_in_line;
    if(element_to_remove != NULL) //if there is REAL elements in the queue
    {
        queue_head->next_in_line = element_to_remove->next_in_line;
        element_to_remove->next_in_line = NULL;
        pqDestroyElements(element_to_remove, queue_head->freePqElement, queue_head->freePqElementPriority);
    }
    return PQ_SUCCESS;
}

//*** 9 ***
PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
    PriorityQueue queue_head = queue;
    if(queue_head != NULL)
    {
        queue_head->iterator = NULL;
    }
    if(queue_head == NULL || element == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    PriorityQueue current_queue_pointer = queue_head->next_in_line;
    PriorityQueue previous_queue = queue_head;
    while(current_queue_pointer != NULL)
    {
        if(queue_head->equalPqElement(current_queue_pointer->pqe_element, element))
        {
            previous_queue->next_in_line = current_queue_pointer->next_in_line;
            current_queue_pointer->next_in_line = NULL;
            pqDestroyElements(current_queue_pointer, queue_head->freePqElement, queue_head->freePqElementPriority);
            return PQ_SUCCESS;
        }
        current_queue_pointer = current_queue_pointer->next_in_line;
        previous_queue = previous_queue->next_in_line; 
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}

//*** 10 ***
PriorityQueueResult pqClear(PriorityQueue queue)
{
    PriorityQueue queue_head = queue;
    if(queue_head == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    queue_head->iterator = NULL;
    pqDestroyElements(queue_head->next_in_line, queue_head->freePqElement, queue_head->freePqElementPriority);
    return PQ_SUCCESS;
}

//*** 11 ***
PQElement pqGetFirst(PriorityQueue queue)
{
    PriorityQueue queue_head = queue;
    if(queue_head == NULL || queue_head->next_in_line == NULL)
    {
        return NULL;
    }
    queue_head->iterator = queue_head->next_in_line; 
    return queue_head->iterator->pqe_element;
}

//*** 12 ***
PQElement pqGetNext(PriorityQueue queue)
{
    PriorityQueue queue_head = queue;
    if(queue_head == NULL || queue_head->iterator == NULL || queue_head->iterator->next_in_line == NULL)
    {
        return NULL;
    }
    queue_head->iterator = queue_head->iterator->next_in_line;
    return queue_head->iterator->pqe_element;
}