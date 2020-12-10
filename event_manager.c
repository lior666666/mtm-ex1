#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "date.h"
#include "member.h"
#include "priority_queue.h"
#include "event_manager.h"
//remember to remove "priority_queue.h"

struct EventManager_t 
{
    Date start_date;
    PriorityQueue exist_members;
    PriorityQueue events;
};

//*** generic member functions -->
static PQElement memberCopyGeneric(PQElement generic_member)
{
    if(generic_member == NULL)
    {
        return NULL;
    }
    return memberCopy(generic_member);
}

static void memberDestroyGeneric(PQElement generic_member)
{
    memberDestroy(generic_member);
}

static bool equalGenericMember(PQElement first_generic_member, PQElement second_generic_member)
{
    return compareMembersById(first_generic_member, second_generic_member);
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
//*** <--

//*** generic event functions -->
static PQElement eventCopyGeneric(PQElement generic_event)
{
    if(generic_event == NULL)
    {
        return NULL;
    }
    return eventCopy(generic_event);
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
    return memberCopy(generic_date);
}

static void freeDateGeneric(PQElementPriority date_generic)
{
    dateDestroy(date_generic);
}

static int compareDateGeneric(PQElementPriority first_date, PQElementPriority second_date)
{
    return dateCompare(first_date, second_date);
}
//*** <--


static Event createFakeEvent(int event_id)
{
    Date fake_date = dateCreate(1,1,1);
    if(fake_date == NULL)
    {
        return NULL;
    }
    Event temp_event = eventCreate("fake event", event_id, fake_date);
    dateDestroy(fake_date);
    if(temp_event == NULL)
    {
        return NULL;
    }
    return temp_event; 
}
static Member createFakeMember(int member_id)
{
    Member temp_member = memberCreate("fake member", member_id);
    if(temp_member == NULL)
    {
        return NULL;
    }
    return temp_member; 
}

static EventManagerResult updateMemberEventsCounterBy(EventManager em, Member fake_member, int change_by)
{
    Member member_element_pointer = (Member)pqGetFirst(em->exist_members); // not sure if casting is required.
    while(member_element_pointer != NULL)
    {
        if(compareMembersById(member_element_pointer, fake_member))
        {
            break;
        }
        member_element_pointer = (Member) pqGetNext(em->exist_members); // not sure if casting is required. 
    }   
    changeMemberEventsCounter(member_element_pointer, change_by);
    int *new_member_associated_events_counter;
    memberGet(member_element_pointer, NULL, NULL, *new_member_associated_events_counter);
    int old_member_associated_events_counter = *new_member_associated_events_counter - 1; 
    if(pqChangePriority(em->exist_members, member_element_pointer, &old_member_associated_events_counter, new_member_associated_events_counter) == PQ_OUT_OF_MEMORY)
    {
        return EM_OUT_OF_MEMORY;  
    }
    return EM_SUCCESS; 
}

//*** 1 ***
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
        return NULL;
    }
    event_manager->exist_members = pqCreate(memberCopyGeneric, memberDestroyGeneric, equalGenericMember, copyMemberEventsCounterGeneric, freeMemberEventsCounter, compareMembersEventsCounterGeneric);
    if(event_manager->exist_members == NULL)
    {
        dateDestroy(event_manager->start_date);
        pqDestroy(event_manager->events);
        free(event_manager);
        return NULL;
    }
    return event_manager;
}

//*** 2 ***
void destroyEventManager(EventManager em)
{
    if (em != NULL)
    {
        dateDestroy(em->start_date);
        pqDestroy(em->exist_members);
        pqDestroy(em->events);
    }
}

//*** 3 ***
EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
    if(em == NULL || event_name == NULL || date == NULL || event_id == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(dateCompare(date, em->start_date) < 0)
    {
        return EM_INVALID_DATE;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event event_element_pointer = (Event) pqGetFirst(em->events); // not sure if casting is required.
    Event new_event_element = eventCreate(event_name, event_id, date);
    if(new_event_element == NULL)
    {
        return EM_OUT_OF_MEMORY; 
    }
    while(event_element_pointer != NULL)
    {
        if(eventCompareName(event_element_pointer, new_event_element) && eventCompareDate(event_element_pointer, new_event_element) == 0)
        {
            eventDestroy(new_event_element);
            return EM_EVENT_ALREADY_EXISTS; 
        }
        event_element_pointer = (Event) pqGetNext(em->events); // not sure if casting is required. 
    }
    if(pqContains(em->events, new_event_element))
    {
        eventDestroy(new_event_element);
        return EM_EVENT_ID_ALREADY_EXISTS;
    }
    if(pqInsert(em->events, new_event_element, date) == PQ_OUT_OF_MEMORY)
    {
       eventDestroy(new_event_element);
       return EM_OUT_OF_MEMORY; 
    }
    eventDestroy(new_event_element);
    return EM_SUCCESS; 
}

//*** 4 ***
EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
    if (em == NULL || em->start_date == NULL || event_name == NULL || days == NULL || event_id == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(days < 0)
    {
        return EM_INVALID_DATE;
    }
    Date date = dateCopy(em->start_date);
    if (date == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    while(days > 0)
    {
        dateTick(date);
        days--;
    }
    EventManagerResult result = emAddEventByDate(em, event_name, date, event_id);
    dateDestroy(date);
    return result;
}

//*** 5 ***
EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
    if(em == NULL || em->events == NULL || event_id == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event temp_event = createFakeEvent(event_id);
    if(temp_event == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    if(pqRemoveElement(em->events, temp_event) == PQ_ELEMENT_DOES_NOT_EXISTS)
    {
        eventDestroy(temp_event);
        return EM_EVENT_NOT_EXISTS;
    }
    eventDestroy(temp_event);
    return EM_SUCCESS; 
}

EventManagerResult emAddMember(EventManager em, char* member_name, int member_id)
{
    if(em == NULL || member_name == NULL || member_id == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(member_id < 0)
    {
        return EM_INVALID_MEMBER_ID;
    }
    Member new_member = memberCreate(member_name, member_id);
    if(new_member == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    if(pqContains(em->exist_members, new_member))
    {
        return EM_MEMBER_ID_ALREADY_EXISTS;
    }
    int associated_events_counter= 0;
    if(pqInsert(em->exist_members, new_member, &associated_events_counter) == PQ_OUT_OF_MEMORY)
    {
      memberDestroy(new_member);
      return EM_OUT_OF_MEMORY; 
    }
    memberDestroy(new_member);
    return EM_SUCCESS; 
}

EventManagerResult emRemoveMemberFromEvent (EventManager em, int member_id, int event_id)
{
    if(em ==NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(member_id<0)
    {
        return EM_INVALID_MEMBER_ID;
    }
    Event fake_event = createFakeEvent(event_id);
    if(fake_event == NULL)
    {
        return EM_OUT_OF_MEMORY; 
    }
    if(!pqContains(em->events, fake_event))
    {
        destroyEvent(fake_event);
        return EM_EVENT_ID_NOT_EXISTS;
    }
    Event event_element_pointer = (Event) pqGetFirst(em->events); // not sure if casting is required.
    while(event_element_pointer != NULL)
    {
        if(eventCompareId(event_element_pointer, fake_event))
        {
            break;
        }
        event_element_pointer = (Event) pqGetNext(em->events); // not sure if casting is required. 
    }
    destroyEvent(fake_event);
    Member fake_member = createFakeMember(member_id);
    if(fake_member == NULL)
    {
        return EM_OUT_OF_MEMORY; 
    }
    if(!pqContains(eventGetMembers(event_element_pointer), fake_member))
    {
        memberDestroy(fake_member);
        return EM_MEMBER_ID_NOT_EXISTS;
    }
    if(updateMemberEventsCounterBy(em, fake_member, -1) == EM_OUT_OF_MEMORY)
    {
        memberDestroy(fake_member);
        return EM_OUT_OF_MEMORY;
    }  
    pqRemoveElement(eventGetMembers(event_element_pointer), fake_member);
    memberDestroy(fake_member);
    return EM_SUCCESS; 
}



