#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "priority_queue.h"

#define ELEMENTS_IN_EMPTY_QUEUE -1

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
    PriorityQueue priority_queue = malloc(sizeof(PriorityQueue));
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
static void insertElementPointer(PriorityQueue our_queue, PriorityQueue new_element_in_queue) // This internal function inserts a spesific fully allocated new element to our queue. 
{
    PriorityQueue previous_element_in_queue = our_queue;
    PriorityQueue current_element = our_queue->next_in_line;
    while(current_element != NULL) //enter only if there is at least one real element in the queue
    {
        if(our_queue->comparePqElementPriority(current_element->pq_element_priority, new_element_in_queue->pq_element_priority) < 0)
        {
            previous_element_in_queue->next_in_line = new_element_in_queue;
            new_element_in_queue->next_in_line = current_element;
            return;
        }
        previous_element_in_queue = current_element;
        current_element = current_element->next_in_line;
    }
    //if there is no real elements in the queue OR new_priority is the lowest priority
    current_element->next_in_line = new_element_in_queue;
    new_element_in_queue->next_in_line = NULL;
}

//*** 1 ***
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

//*** 2 ***
void pqDestroy(PriorityQueue queue)
{
    while(queue != NULL)
    {
        queue->freePqElement(queue->pqe_element);
        queue->freePqElementPriority(queue->pq_element_priority);
        pqDestroy(queue->next_in_line); //destroy all the list with recursion
        free(queue);
    }
}

//*** 3 ***
PriorityQueue pqCopy(PriorityQueue queue_head)
{
    if(queue_head == NULL)
    {
        return NULL;
    }
    /** Allocating the new queue for the first time.*/ 
    PriorityQueue copied_priority_queue  = pqCreate(queue_head->copyElement, queue_head->freePqElement, queue_head->equalPqElement, queue_head->copyPqElementPriority, 
    queue_head->freePqElementPriority, queue_head->comparePqElementPriority);
    if(copied_priority_queue == NULL)
    {
        return NULL;
    }
    queue_head->iterator = NULL; // Should we make this null here? What happen if the malloc failed?

    /** making temp pointers to go through the queue.*/ 
    PriorityQueue temp_queue_pointer = queue_head->next_in_line;
    PriorityQueue temp_copied_priority_queue_pointer = copied_priority_queue;

    while(temp_queue_pointer!=NULL) 
    {
        /**copying the next one in the queue*/
        temp_copied_priority_queue_pointer->next_in_line = CreateNextInLine(copied_priority_queue, temp_queue_pointer->pqe_element, temp_queue_pointer->pq_element_priority);
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
    return counter-ELEMENTS_IN_EMPTY_QUEUE; //return the length of the queue BUT without the head (that does not represent an element in the queue)
}

//*** 5 ***
bool pqContains(PriorityQueue queue_head, PQElement element)
{
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
PriorityQueueResult pqInsert(PriorityQueue queue_head, PQElement element, PQElementPriority priority)
{
    if (queue_head == NULL || element == NULL || priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    PriorityQueue new_element_in_queue = createNextInLine(queue_head, element, priority);
    if(new_element_in_queue == NULL) //if copy failed, Do the value is NULL? need to be checked
    {
        return PQ_OUT_OF_MEMORY;
    }
    insertElementPointer(queue_head, new_element_in_queue);
    return PQ_SUCCESS;
}
