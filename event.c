#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "member.h"
#include "event.h"

#define INVALID_ID -1

struct Event_t
{
    int event_id;
    char* event_name;
    Date event_date;
    PriorityQueue event_members;
};

/**
* memberCopyGeneric: Creates a copy of target generic member.
* @param generic_member - Target generic member.
* @return
*   NULL if a NULL was sent or a memory allocation failed.
*   An (generic) member containing the same elements as member otherwise.
*/
static PQElement memberCopyGeneric(PQElement generic_member)
{
    if(generic_member == NULL)
    {
        return NULL;
    }
    return memberCopy(generic_member);
}

/**
* memberDestroyGeneric: Deallocates an existing generic member.
* @param generic_member - target generic member.
*/
static void memberDestroyGeneric(PQElement generic_member)
{
    memberDestroy(generic_member);
}

/**
* memberCompareByIdGeneric: Compares the id between 2 generic members.
* @param first_generic_member - first generic member to cpmpare.
* @param second_generic_member - second generic member to compare.
* @return
* 	true - if the id's are equal.
*   false - if the id's are not the same or one of the generic members is NULL.
*/
static bool memberCompareByIdGeneric(PQElement first_generic_member, PQElement second_generic_member)
{
    return memberCompareById(first_generic_member, second_generic_member);
}

/**
* memberCompareByIdGeneric: Copy the id of a member.
* @param generic_id - generic id of a member.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
*   A copy of the id of the member otherwise.
*/
static PQElementPriority memberCopyIdGeneric(PQElementPriority generic_id)
{
    if(generic_id == NULL)
    {
        return NULL;
    }
    int *copy_id = malloc(sizeof(*copy_id));
    if(copy_id == NULL)
    {
        return NULL;
    }
    *copy_id = *(int *) generic_id;
    return copy_id;
}

/**
* memberDestroyIdGeneric: Deallocates an existing generic id of a member.
* @param generic_id - target generic id.
*/
static void memberDestroyIdGeneric(PQElementPriority generic_id)
{
    free(generic_id);
}

/**
* membersCompareIdGeneric: Compares the id between 2 members.
* @param first_member_id - first id to cpmpare.
* @param second_member_id - second id to compare.
* @return
* 	A negative integer if the first id is bigger.
* 	0 if they're equal.
*	A positive integer if the second id is bigger.
*/
static int membersCompareIdGeneric(PQElementPriority first_member_id, PQElementPriority second_member_id)
{
    return *(int *) second_member_id - *(int *) first_member_id;
}

/**
* eventCreate: Allocates a new event.
* @param event_name - the day of the event.
* @param event_id - the id of the event.
* @param event_date - the date of the event.
* @param add_members_or_not - a flag that decides if to allocate the members priority queue or not.
* @return
* 	NULL - if allocation failed of one of args equales NULL.
* 	A new Event in case of success.
*/
Event eventCreate(char* event_name, int event_id, Date event_date, bool add_members_or_not)
{
    if(event_name == NULL || event_date == NULL)
    {
        return NULL;
    }
    Event new_event = malloc(sizeof(*new_event));
    if(new_event == NULL)
    {
        return NULL;
    }
    new_event->event_date = dateCopy(event_date);
    if(new_event->event_date == NULL)
    {
        free(new_event);
        return NULL;
    }
    new_event->event_name = malloc(strlen(event_name)+1);
    if(new_event->event_name == NULL)
    {
        dateDestroy(new_event->event_date);
        free(new_event);
        return NULL;
    }
    strcpy(new_event->event_name, event_name);
    new_event->event_id = event_id;
    if(add_members_or_not)
    {
        new_event->event_members = pqCreate(memberCopyGeneric, memberDestroyGeneric, memberCompareByIdGeneric,
                                        memberCopyIdGeneric, memberDestroyIdGeneric, membersCompareIdGeneric);
        if(new_event->event_members == NULL)
        {
            dateDestroy(new_event->event_date);
            free(new_event->event_name);
            free(new_event);
            return NULL;
        }
    }
    return new_event;
}

/**
* eventDestroy: Deallocates an existing Event.
* @param event - Target event to be deallocated.
*/
void eventDestroy(Event event)
{
    if(event != NULL)
    {
        free(event->event_name);
        dateDestroy(event->event_date);
        pqDestroy(event->event_members);
        free(event);
    }
}

/**
* eventCopy: Creates a copy of target Event.
* @param event - Target Event.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	An Event containing the same elements as event otherwise.
*/
Event eventCopy(Event event)
{
    if (event == NULL)
    {
        return NULL;
    }
    Event event_copy = eventCreate(event->event_name, event->event_id, event->event_date, false);
    if(event_copy == NULL)
    {
        return NULL;
    }
    event_copy->event_members = pqCopy(event->event_members);
    if(event_copy->event_members == NULL)
    {
        eventDestroy(event_copy);
        return NULL;
    }
    return event_copy;
}

/**
* eventGetDate: Returns the event date.
* @param event - Target Event.
* @return
*   if the event is NULL, return NULL.
*   Otherwise returns event date.
*/
Date eventGetDate(Event event)
{
    if(event == NULL)
    {
        return NULL;
    }
    return event->event_date;
}

/**
* eventGetName: Returns the event name.
* @param event - Target Event.
* @return
*   if the event is NULL, returns NULL.
*   Otherwise returns event name.
*/
char* eventGetName(Event event)
{
    if(event == NULL)
    {
        return NULL;
    }
    return event->event_name;
}

/**
* eventGetId: Returns the event id.
* @param event - Target Event.
* @return
*   if the event is NULL, returns -1.
*   Otherwise returns event id.
*/
int eventGetId(Event event)
{
    if(event == NULL)
    {
        return INVALID_ID;
    }
    return event->event_id;
}

/**
* eventGetMembers: Returns the members list of the event.
* @param event - Target Event.
* @return
* 	returns NULL in case the event is NULL.
* 	Otherwise return the queue of the members.
*/
PriorityQueue eventGetMembers(Event event)
{
    if(event == NULL)
    {
        return NULL;
    }
    return event->event_members;
}

/**
* eventCompareId: compares the id between 2 events.
* @param first_event - first Event to cpmpare.
* @param second_event - second Event to compare with the first one.
* @return
* 	true - if the id's are equal.
*   false - if the id's are not the same or one of the events is NULL.
*/
bool eventCompareId(Event first_event, Event second_event)
{
    if(first_event == NULL || second_event == NULL)
    {
        return false;
    }
    return first_event->event_id == second_event->event_id;
}

/**
* eventCompareName: compares the name between 2 events.
* @param first_event - first Event to cpmpare.
* @param second_event - second Event to compare with the first one.
* @return
* 	true - if the name's are equal.
*   false - if the name's are not the same.
*/
bool eventCompareName(Event first_event, Event second_event)
{
    if(first_event == NULL || second_event == NULL)
    {
        return false;
    }
    return strcmp(first_event->event_name, second_event->event_name) == 0;
}

/**
* eventCompareDate: compares the date between 2 events.
* @param first_event - first Event to cpmpare.
* @param second_event - second Event to compare with the first one.
* @return
* 	A negative integer if the date of the firstevent occurs first.
* 	0 if they're equal or one of the dates is NULL or one of the events is NULL.
*	A positive integer if the date of the firstevent arrives after the date of the second event.
*/
int eventCompareDate(Event first_event, Event second_event)
{
    if(first_event == NULL || second_event == NULL)
    {
        return 0;
    }
    return dateCompare(first_event->event_date, second_event->event_date);
}

/**
* eventUpdateDate: change event date to new_date.
* @param event - event to change.
* @param new_date - event new date.
*/
void eventUpdateDate(Event event, Date new_date)
{
    if (event != NULL && new_date != NULL)
    {
        Date date_to_remove = event->event_date;
        event->event_date = dateCopy(new_date);
        dateDestroy(date_to_remove);
    }
}