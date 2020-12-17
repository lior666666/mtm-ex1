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

/**
* pqCreateNextInLine: Creates the next element in the priority queue
* @param queue_head - Target queue head.
* @param element - Target element to create.
* @param element_priority - Target priority to connect with the element.
* @return
*   NULL if the priority queue that sent is NULL or a memory allocation failed.
*   The new element to connect to the priority queue otherwise.
*/
static PriorityQueue pqCreateNextInLine(PriorityQueue queue_head, PQElement element,
                                        PQElementPriority element_priority)
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

/**
* pqInsertElement:
    checking where to insert the given element to the priority queue by it's priority.
    if the priority queue is empty --> insert as the first element in the priority queue.
    otherwise, find the location by compare priorities and insert.
* @param queue_head - Target queue head.
* @param new_element_in_queue - Target new element that should be insert to the priority queue.
*/
static void pqInsertElement(PriorityQueue queue_head, PriorityQueue new_element_in_queue) 
{
    PriorityQueue previous_element = queue_head;
    PriorityQueue current_element = queue_head->next_in_line;
    while(current_element != NULL)
    {
        if(queue_head->comparePqElementPriority(current_element->pq_element_priority,
                                                new_element_in_queue->pq_element_priority) < 0)
        {
            previous_element->next_in_line = new_element_in_queue;
            new_element_in_queue->next_in_line = current_element;
            return;
        }
        previous_element = current_element;
        current_element = current_element->next_in_line;
    }
    previous_element->next_in_line = new_element_in_queue;
    new_element_in_queue->next_in_line = NULL;
}

/**
* pqDestroyElements: Deallocates an existing priority queue from the given element.
* @param queue_head - Target queue head.
* @param delete_from_element - Target element in priority queue which should destroy from it to the last one.
*/
static void pqDestroyElements(PriorityQueue queue_head, PriorityQueue delete_from_element)
{
    while(delete_from_element != NULL)
    {
        PriorityQueue delete_head = delete_from_element;
        delete_from_element = delete_from_element->next_in_line;
        queue_head->freePqElement(delete_head->pqe_element);
        queue_head->freePqElementPriority(delete_head->pq_element_priority);
        free(delete_head);
    }
}

/**
* pqCreate: Allocates a new empty priority queue.
* @param copy_element - Function pointer to be used for copying data elements into
*  	the priority queue or when copying the priority queue.
* @param free_element - Function pointer to be used for removing data elements from
* 	the priority queue
* @param compare_element - Function pointer to be used for comparing elements
* 	inside the priority queue. Used to check if new elements already exist in the priority queue.
* @param copy_priority - Function pointer to be used for copying priority into
*  	the priority queue or when copying the priority queue.
* @param free_priority - Function pointer to be used for removing priority from
* 	the priority queue
* @param compare_priority - Function pointer to be used for comparing elements
* 	inside the priority queue. Used to check if new elements already exist in the priority queue.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new priority queue in case of success.
*/
PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element,
                        EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
                        FreePQElementPriority free_priority, ComparePQElementPriorities compare_priorities)
{
    if(copy_element == NULL || free_element ==NULL || equal_elements == NULL ||
        copy_priority == NULL || free_priority == NULL|| compare_priorities == NULL)
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

/**
* pqDestroy: Deallocates an existing priority queue. Clears all elements by using the free functions.
* @param queue - Target priority queue to be deallocated. If priority queue is NULL nothing will be done.
*/
void pqDestroy(PriorityQueue queue)
{
    pqDestroyElements(queue, queue->next_in_line);
    free(queue);
}

/**
* pqCopy: Creates a copy of target priority queue.
*   Iterator values for both priority queues are undefined after this operation.
* @param queue - Target priority queue.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Priority Queue containing the same elements as queue otherwise.
*/
PriorityQueue pqCopy(PriorityQueue queue)
{
    PriorityQueue queue_head = queue;
    if(queue_head == NULL)
    {
        return NULL;
    }
    queue_head->iterator = NULL;
    PriorityQueue copied_priority_queue  = pqCreate(queue_head->copyElement, queue_head->freePqElement,
                                        queue_head->equalPqElement, queue_head->copyPqElementPriority, 
                                        queue_head->freePqElementPriority, queue_head->comparePqElementPriority);
    if(copied_priority_queue == NULL)
    {
        return NULL;
    }
    PriorityQueue temp_queue_pointer = queue_head->next_in_line;
    PriorityQueue temp_copied_priority_queue_pointer = copied_priority_queue;
    while(temp_queue_pointer!=NULL) 
    {
        temp_copied_priority_queue_pointer->next_in_line = pqCreateNextInLine(copied_priority_queue,
                                        temp_queue_pointer->pqe_element, temp_queue_pointer->pq_element_priority);
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

/**
* pqGetSize: Returns the number of elements in a priority queue
* @param queue - The priority queue which size is requested
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the number of elements in the priority queue.
*/
int pqGetSize(PriorityQueue queue)
{
    int counter = 0;
    while(queue != NULL)
    {
        counter++;
        queue = queue->next_in_line;
    }
    return counter - 1;
}

/**
* pqContains: Checks if an element exists in the priority queue. The element will be
*   considered in the priority queue if one of the elements in the priority queue it determined equal
*   using the comparison function used to initialize the priority queue.
* @param queue - The priority queue to search in
* @param element - The element to look for. Will be compared using the comparison function.
* @return
* 	false - if one or more of the inputs is null, or if the key element was not found.
* 	true - if the key element was found in the priority queue.
*/
bool pqContains(PriorityQueue queue, PQElement element)
{
    PriorityQueue queue_head = queue;
    if(element == NULL || queue_head == NULL)
    {
        return false; 
    }
    PriorityQueue temp_queue_pointer = queue_head->next_in_line;
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

/**
* pqInsert: add a specified element with a specific priority.
*   Iterator's value is undefined after this operation.
* @param queue - The priority queue for which to add the data element
* @param element - The element which need to be added.
* @param priority - The new priority to associate with the given element.
*   A copy of the element will be inserted as supplied by the copying function
*   which is given at initialization.
* @return
* 	PQ_NULL_ARGUMENT if a NULL was sent as one of the parameters.
* 	PQ_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying an element failed).
* 	PQ_SUCCESS the paired elements had been inserted successfully.
*/
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
    PriorityQueue new_element_in_queue = pqCreateNextInLine(queue_head, element, priority);
    if(new_element_in_queue == NULL)
    {
        return PQ_OUT_OF_MEMORY;
    }
    pqInsertElement(queue_head, new_element_in_queue);
    return PQ_SUCCESS;
}

/**
* pqChangePriority: Changes a priority of specific element with a specific priority in the priority queue.
*   If there are multiple same elements with same priority, only the first element's priority needs to be changed.
*   Element that its value has changed is considered as reinserted element.
*	Iterator's value is undefined after this operation
* @param queue - The priority queue for which the element from.
* @param element - The element which need to be found and whos priority we want to change.
* @param old_priority - The old priority of the element which need to be changed.
* @param new_priority - The new priority of the element.
* @return
* 	PQ_NULL_ARGUMENT if a NULL was sent as one of the parameters.
* 	PQ_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying an element failed).
* 	PQ_ELEMENT_DOES_NOT_EXISTS if element with old_priority does not exists in the queue.
* 	PQ_SUCCESS the paired elements had been inserted successfully.
*/
PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                    PQElementPriority old_priority, PQElementPriority new_priority)
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
        if(queue_head->equalPqElement(current_queue_pointer->pqe_element, element) == true && 
            queue_head->comparePqElementPriority(current_queue_pointer->pq_element_priority,old_priority) == 0)
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
            pqInsertElement(queue, current_queue_pointer);
            return PQ_SUCCESS;
        }
        current_queue_pointer = current_queue_pointer->next_in_line;
        temp_queue_previous = temp_queue_previous->next_in_line;
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}

/**
* pqRemove: Removes the highest priority element from the priority queue.
*   If there are multiple elements with the same highest priority, the first inserted element
*   should be removed first.
*   The elements are removed and deallocated using the free functions supplied at initialization.
*   Iterator's value is undefined after this operation.
* @param queue - The priority queue to remove the element from.
* @return
* 	PQ_NULL_ARGUMENT if a NULL was sent to the function.
* 	PQ_SUCCESS the most prioritized element had been removed successfully.
*/
PriorityQueueResult pqRemove(PriorityQueue queue)
{
    PriorityQueue queue_head = queue;
    if(queue_head == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    queue_head->iterator = NULL;
    PriorityQueue element_to_remove = queue_head->next_in_line;
    if(element_to_remove != NULL)
    {
        queue_head->next_in_line = element_to_remove->next_in_line;
        element_to_remove->next_in_line = NULL;
        pqDestroyElements(queue_head, element_to_remove);
    }
    return PQ_SUCCESS;
}

/**
* pqRemoveElement: 
*   Removes the highest priority element from the priority queue which have its value equal to element.
*   If there are multiple elements with the same highest priority, the first inserted element
*   should be removed first.
*   The elements are removed and deallocated using the free functions supplied at initialization.
*   Iterator's value is undefined after this operation.
* @param queue - The priority queue to remove the elements from.
* @param element
* 	The element to find and remove from the priority queue. The element will be freed using the
* 	free function given at initialization. The priority associated with this element
*   will also be freed using the free function given at initialization.
* @return
* 	PQ_NULL_ARGUMENT if a NULL was sent to the function.
* 	PQ_ELEMENT_DOES_NOT_EXISTS if given element does not exists.
* 	PQ_SUCCESS the most prioritized element had been removed successfully.
*/
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
            pqDestroyElements(queue_head, current_queue_pointer);
            return PQ_SUCCESS;
        }
        current_queue_pointer = current_queue_pointer->next_in_line;
        previous_queue = previous_queue->next_in_line; 
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}

/**
* pqClear: Removes all elements and priorities from target priority queue.
* The elements are deallocated using the stored free functions.
* @param queue
* 	Target priority queue to remove all element from.
* @return
* 	PQ_NULL_ARGUMENT - if a NULL pointer was sent.
* 	PQ_SUCCESS - Otherwise.
*/
PriorityQueueResult pqClear(PriorityQueue queue)
{
    PriorityQueue queue_head = queue;
    if(queue_head == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    queue_head->iterator = NULL;
    while(queue_head->next_in_line != NULL)
    {
        pqRemove(queue_head);
    }
    return PQ_SUCCESS;
}

/**
* pqGetFirst: Sets the internal iterator (also called current element) to
*	the first element in the priority queue. The internal order derived from the priorities,
*   and the tie-breaker between two equal priorities is the insertion order.
*	Use this to start iterating over the priority queue.
*	To continue iteration use pqGetNext
* @param queue - The priority queue for which to set the iterator and return the first element.
* @return
* 	NULL if a NULL pointer was sent or the priority queue is empty.
* 	The first key element of the priority queue otherwise.
*/
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

/**
* pqGetNext: Advances the priority queue iterator to the next element and returns it.
* @param queue - The priority queue for which to advance the iterator
* @return
* 	NULL if reached the end of the priority queue,
*   or the iterator is at an invalid state or a NULL sent as argument.
* 	The next element on the priority queue in case of success.
*/
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