#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "priority_queue.h"

/**we can add here more things if needed! */
struct PriorityQueue_t{
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

/*DELETE AFTER YOU READ - remember that we must add {} in condition statment even if there is just one row inside*/
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

/*DELETE AFTER YOU READ - I changed variable name from "priority_queue" to "copied_priority_queue" - more informative*/
 PriorityQueue pqCopy(PriorityQueue queue){
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

/** I am not sure if we are supposed to use the iterator inside the function or not .*/
 pqContains(PriorityQueue queue, PQElement element){
     if(element == NULL || queue == NULL)
         return false; 
     queue->iterator = queue; 
     while(queue->iterator != NULL)
     {
         if(queue->equalPqElement(queue->pqe_element, element)==0)
            return true;
        queue->iterator = queue->iterator->next_in_line; 
     }
     return false; 
 }


 





 











                        

        
