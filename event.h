#ifndef EVENT_H_
#define EVENT_H_

#include "priority_queue.h"
#include "date.h"
#include <stdbool.h>

/** Type for defining the event */
typedef struct Event_t *Event;

/**
* eventCreate: Allocates a new event.
*
* @param event_name - the day of the event.
* @param event_id - the id of the event.
* @param event_date - the date of the event.
* @return
* 	NULL - if allocation failed of one of args equales NULL. 
* 	A new Event in case of success.
*/
Event eventCreate(char* event_name, int event_id, Date event_date);

/**
* eventDestroy: Deallocates an existing Event.
*
* @param event - Target event to be deallocated. 
*/
void eventDestroy(Event event);

/**
* eventCopy: Creates a copy of target Event.
*
* @param event - Target Event.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	An Event containing the same elements as event otherwise.
*/
Event eventCopy(Event event);

/**
* eventGet: Returns the name and the id number of the event. 
*
* @param event - Target Event.
* @param event_name - the pointer to assign to name of the Event.
* @param event_id - the pointer to assign to id of the Event.
* @param event_date - the pointer to assign to date of the Event.
*
* @return
*  the event neme and event id and event date is assigned to the pointers. if the pointer is NULL, do nothing with this pointer. 
*/
void eventGet(Event event, char* event_name, int* event_id, Date event_date);

/**
* eventGetMembers: Returns the members list of the event. 
*
* @param event - Target Event.
*
* @return
* 	returns NULL in case the event is NULL.
* 	Otherwise returns the queue of the members.
*/
PriorityQueue eventGetMembers(Event event);

/**
* eventCompareId: compares the id between 2 events. 
* @param first_event - first Event to cpmpare.
* @param second_event - second Event to compare with the first one.
* @return
* 		true - if the id's are equal. 
*       false - if the id's are not the same or one of the events is NULL. 
*/
bool eventCompareId(Event first_event, Event second_event);

/**
* eventCompareName: compares the name between 2 events. 
* @param first_event - first Event to cpmpare.
* @param second_event - second Event to compare with the first one.
* @return
* 		true - if the name's are equal. 
*       false - if the name's are not the same. 
*/
bool eventCompareName(Event first_event, Event second_event);

#endif //EVENT_H_