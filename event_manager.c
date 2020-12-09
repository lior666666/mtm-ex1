#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "date.h"
#include "member.h"
#include "event_manager.h"
#include "priority_queue.h"
// remember to remove the line above

struct EventManager_t 
{
    Date start_date;
    PriorityQueue exist_members;
    PriorityQueue events;
};


static PQElement memberCopyGeneric(PQElement generic_member)
{
    if(generic_member == NULL)
    {
        return NULL;
    }
    Member member_copy = memberCopy(generic_member);
    if(member_copy == NULL)
    {
        return NULL;
    }
    return member_copy; 
}

static void memberDestroyGeneric(PQElement generic_member)
{
    memberDestroy(generic_member);
}

static bool equalGenericMember(PQElement first_generic_member, PQElement second_generic_member)
{
    return compareMembersById(first_generic_member, second_generic_member) == 0;
}

static PQElementPriority copyMemberEventsCounterGeneric(PQElementPriority generic_events_counter)
{
    if(generic_events_counter == NULL)
    {
        return NULL;
    }
    int *copy_generic_events_counter = malloc(sizeof(*copy_generic_events_counter));
    if(copy_generic_events_counter == NULL)
    {
        return NULL;
    }
    *copy_generic_events_counter = *(int *) generic_events_counter;
    return copy_generic_events_counter; 
}

static void freeMemberEventsCounter(PQElementPriority copy_generic_events_counter)
{
    return;
}

static int compareMembersEventsCounterGeneric(PQElementPriority first_events_counter, PQElementPriority second_events_counter)
{
    return *(int *) first_events_counter - *(int *) second_events_counter;
}



static PQElement eventCopyGeneric(PQElement generic_event)
{
    if(generic_event == NULL)
    {
        return NULL;
    }
    Event event_copy = eventCopy(generic_event);
    if(event_copy == NULL)
    {
        return NULL;
    }
    return event_copy; 
}

static void eventDestroyGeneric(PQElement generic_event)
{
    eventDestroy(generic_event);
}

static bool equalGenericEvent(PQElement first_generic_event, PQElement second_generic_event)
{
    return eventCompareId(first_generic_event, second_generic_event);
}

static PQElementPriority copyDateGeneric(PQElementPriority generic_date)
{
     if(generic_date == NULL)
    {
        return NULL;
    }
    Date copy_date = memberCopy(generic_date);
    if(copy_date == NULL)
    {
        return NULL;
    }
    return copy_date; 
}

static void freeDateGeneric(PQElementPriority date_generic)
{
    dateDestroy(date_generic);
}

static int compareDateGeneric(PQElementPriority first_date, PQElementPriority second_date)
{
    return dateCompare(first_date, second_date);
}

EventManager createEventManager(Date date)
{
    if(date == NULL)
    {
        return NULL;
    }
    EventManager event_manager = malloc(sizeof(*event_manager)); 
    if(event_manager == NULL)
    {
        return NULL;
    }
    event_manager->start_date = dateCopy(date); 
    if(event_manager->start_date == NULL)
    {
        free(event_manager);
        return NULL;
    }
    event_manager->events = pqCreate(eventCopyGeneric, eventDestroyGeneric, equalGenericEvent, copyDateGeneric, freeDateGeneric, compareDateGeneric);
    if(event_manager->events == NULL)
    {
        dateDestroy(event_manager->start_date);
        free(event_manager);
    }
    event_manager->exist_members = pqCreate(memberCopyGeneric, memberDestroyGeneric, equalGenericMember, copyMemberEventsCounterGeneric, freeMemberEventsCounter, compareMembersEventsCounterGeneric);
    if(event_manager->exist_members == NULL)
    {
        dateDestroy(event_manager->start_date);
        pqDestroy(event_manager->events);
        free(event_manager);
    }
    return event_manager;
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
    if(em == NULL || event_name == NULL || date == NULL || event_id == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(dateCompare(date, em->start_date)<0)
    {
        return EM_INVALID_DATE;
    }
    if(event_id<0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event event_element = (Event) pqGetFirst(em->events); // not sure if casting is required. 
    char* event_element_name;  // not sure if we allowed to it. 
    Date element_date;  // not sure if we allowed to it. 
    while(event_element != NULL)
    {
        eventGet(event_element, event_element_name, NULL, element_date);
        if(eventCompareName(event_element, event_element_name, event_name) && dateCompare(element_date, date) == 0)
        {
            return EM_EVENT_ALREADY_EXISTS; 
        }
        event_element = (Event) pqGetNext(em->events); // not sure if casting is required. 
    }
    event_element = eventCreate(event_name, event_id, date);
    if(event_element == NULL)
    {
        return EM_OUT_OF_MEMORY; 
    }
    if(pqContains(em->events, event_element))
    {
        return EM_EVENT_ID_ALREADY_EXISTS; 
    }
    if(pqInsert(em->events, event_element, date) == PQ_OUT_OF_MEMORY)
    {
       eventDestroy(event_element)
       return EM_OUT_OF_MEMORY; 
    }
    return EM_SUCCESS; 
}

EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
    if(em == NULL || event_id == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id<0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Date fake_date = dateCreate(1,1,1);
    if(fake_date == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    Event temp_event = eventCreate("fake event", event_id, fake_date);
    if(temp_event == NULL)
    {
        dateDestroy(fake_date);
        return EM_OUT_OF_MEMORY;
    }
    if(pqRemoveElement(em->events, temp_event) == PQ_ELEMENT_DOES_NOT_EXISTS)
    {
        dateDestroy(fake_date);
        eventDestroy(temp_event)
        return EM_ELEMENT_NOT_EXISTS
    }
    dateDestroy(fake_date);
    eventDestroy(temp_event)
    return EM_SUCCESS; 
}








