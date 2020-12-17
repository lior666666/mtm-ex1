#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "event.h"
#include "member.h"
#include "priority_queue.h"
#include "event_manager.h"

#define ADD_MEMBER_TO_EVENT 1
#define REMOVE_MEMBER_FROM_EVENT -1
#define NO_EVENTS_PLANNED -1
#define TMP_EVENT_NAME "tmp-event"
#define CHANGE_BACK_EVENTS_COUNTER -1
#define TMP_MEMBER_NAME "tmp-member"

struct EventManager_t
{
    Date start_date;
    PriorityQueue exist_members;
    PriorityQueue events;
};

//************************* start generic member functions *************************
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
* memberCompareByIdAndEventsCounterGeneric:
*   Compares the two given members by events counter.
*   If equal counter, compare by id.
* @param first_generic_member - first generic member to cpmpare.
* @param second_generic_member - second generic member to compare.
* @return
* 	A negative integer if the first events counter is lower or if the counter
*       is equal and the id of the first member is bigger.
* 	0 if they're equal by events counter AND by id.
*	A positive integer if the first events counter is bigger or if the counter
*       is equal and the id of the first member is lower.
*/
static int memberCompareByIdAndEventsCounterGeneric(PQElementPriority first_generic_member,
                                                     PQElementPriority second_generic_member)
{
    int compare_counter = memberCompareByEventsCounter(first_generic_member, second_generic_member);
    if(compare_counter != 0)
    {
        return compare_counter;
    }
    return memberGetId(second_generic_member) - memberGetId(first_generic_member);
}
//************************* finish generic member functions *************************

//************************* start generic event functions *************************
/**
* eventCopyGeneric: Creates a copy of target generic event.
* @param generic_event - Target generic event.
* @return
*   NULL if a NULL was sent or a memory allocation failed.
*   An (generic) event containing the same elements as event otherwise.
*/
static PQElement eventCopyGeneric(PQElement generic_event)
{
    if(generic_event == NULL)
    {
        return NULL;
    }
    return eventCopy(generic_event);
}

/**
* eventDestroyGeneric: Deallocates an existing generic event.
* @param generic_event - target generic event.
*/
static void eventDestroyGeneric(PQElement generic_event)
{
    eventDestroy(generic_event);
}

/**
* eventCompareByIdGeneric: Compares the id between 2 generic events.
* @param first_generic_event - first generic event to cpmpare.
* @param second_generic_event - second generic event to compare.
* @return
* 	true - if the id's are equal.
*   false - if the id's are not the same or one of the generic events is NULL.
*/
static bool eventCompareByIdGeneric(PQElement first_generic_event, PQElement second_generic_event)
{
    return eventCompareId(first_generic_event, second_generic_event);
}

/**
* dateCopyGeneric: Creates a copy of target generic date.
* @param generic_date - Target generic date.
* @return
*   NULL if a NULL was sent or a memory allocation failed.
*   An (generic) date containing the same elements as date otherwise.
*/
static PQElementPriority dateCopyGeneric(PQElementPriority generic_date)
{
    if(generic_date == NULL)
    {
        return NULL;
    }
    return dateCopy(generic_date);
}

/**
* dateDestroyGeneric: Deallocates an existing generic date.
* @param date_generic - target generic date.
*/
static void dateDestroyGeneric(PQElementPriority date_generic)
{
    dateDestroy(date_generic);
}

/**
* dateCompareGeneric: Compares the id between 2 date.
* @param first_date - first date to cpmpare.
* @param second_date - second date to compare.
* @return
* 	A negative integer if first_date occurs first;
* 	0 if they're equal or one of the given dates is NULL;
*	A positive integer if first_date arrives after second_date.
*/
static int dateCompareGeneric(PQElementPriority first_date, PQElementPriority second_date)
{
    return dateCompare(second_date, first_date);
}
//************************* finish generic event functions *************************

/**
* copyExistEventByIdWithNewDate:
*   Looks for event in event manager by id.
*   If exists, return a copy of it with new date.
*   Otherwise, return new event with given name, id, new date.
* @param em - Target event manager.
* @param event_name - Target event name.
* @param event_id - Target event id.
* @param new_date - Target new date.
* @return
* 	NULL if allocation failed.
* 	A copy of the exist event with the new date, if it exists.
*	A new event with the given name, id, new date otherwise.
*/
static Event copyExistEventByIdWithNewDate(EventManager em, char* event_name, int event_id, Date new_date)
{
    Event event_pointer = (Event) pqGetFirst(em->events);
    char* current_name;
    int current_id;
    while(event_pointer != NULL)
    {
        current_name = eventGetName(event_pointer);
        current_id = eventGetId(event_pointer);
        if(current_id == event_id)
        {
            return eventCreate(current_name, event_id, new_date, true);
        }
        event_pointer = (Event) pqGetNext(em->events);
    }
    return eventCreate(event_name, event_id, new_date, true);
}

/**
* isEqualEventsByNameAndDate: Check if there is an event with the same name on the same date
* @param em - Target event manager.
* @param checking_event - Target event with the name and date to compare.
* @return
* 	true - if there is event with the same name and date as checking_event.
*   false - otherwise.
*/
static bool isEqualEventsByNameAndDate(EventManager em, Event checking_event)
{
    Event event_pointer = (Event) pqGetFirst(em->events);
    while(event_pointer != NULL)
    {
        if(eventCompareName(event_pointer, checking_event) && eventCompareDate(event_pointer, checking_event) == 0)
        {
            return true;
        }
        event_pointer = (Event) pqGetNext(em->events);
    }
    return false;
}

/**
* createTmpEvent: Create temporary event with given id.
* @param event_id - Target event id.
* @return
* 	NULL if allocation failed.
*   The temporary event otherwise.
*/
static Event createTmpEvent(int event_id)
{
    Date tmp_date = dateCreate(1,1,1);
    if(tmp_date == NULL)
    {
        return NULL;
    }
    Event tmp_event = eventCreate(TMP_EVENT_NAME, event_id, tmp_date, true);
    dateDestroy(tmp_date);
    return tmp_event;
}

/**
* findEventById: Finds event with same ID in the event manager.
* @param em - Target event manager.
* @param event - Target event with id to look for.
* @return
* 	NULL if there is no such event.
*   The event with the same id as the given event.
*/
static Event findEventById(EventManager em, Event event)
{
    Event event_pointer = (Event) pqGetFirst(em->events);
    while(event_pointer != NULL)
        {
            if(eventCompareId(event_pointer, event))
            {
                break;
            }
            event_pointer = (Event) pqGetNext(em->events);
        }
    return event_pointer;
}

/**
* findMemberById: Finds member with same ID in the members queue.
* @param members - Target members priority queue.
* @param member - Target member with id to look for.
* @return
* 	NULL if there is no such member.
*   The member with the same id as the given member.
*/
static Member findMemberById(PriorityQueue members, Member member)
{
    Member member_pointer = (Member) pqGetFirst(members);
    while(member_pointer != NULL)
        {
            if(memberCompareById(member_pointer, member))
            {
                break;
            }
            member_pointer = (Member) pqGetNext(members);
        }
    return member_pointer;
}

/**
* updateMemberEventsCounterBy:
*   Find the member with the same id as the given member.
*   Update it's events counter by given change_by.
*   Change it's priority due to the change in events counter.
* @param em - Target events manager.
* @param member - Target member with id to look for.
* @param change_by - Target arguement that decide how to change the events counter.
* @return
* 	EM_OUT_OF_MEMORY if allocation failed (if change priority failed, change back the events counter).
*   EM_SUCCESS otherwise (complete the changes).
*/
static EventManagerResult updateMemberEventsCounterBy(EventManager em, Member member, int change_by)
{
    Member member_pointer = (Member) pqGetFirst(em->exist_members);
    while(member_pointer != NULL)
    {
        if(memberCompareById(member_pointer, member))
        {
            break;
        }
        member_pointer = (Member) pqGetNext(em->exist_members);
    }
    Member old_member = memberCopy(member_pointer);
    if(old_member == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    memberChangeEventsCounter(member_pointer, change_by);
    if(pqChangePriority(em->exist_members, member_pointer, old_member, member_pointer) == PQ_OUT_OF_MEMORY)
    {
        memberChangeEventsCounter(member_pointer, (change_by*CHANGE_BACK_EVENTS_COUNTER));
        memberDestroy(old_member);
        return EM_OUT_OF_MEMORY;
    }
    memberDestroy(old_member);
    return EM_SUCCESS;
}

/**
* emAddOrRemoveMemberToEvent: internal function to prevent duplicate code.
*   Please see the comments in functions emAddMemberToEvent and emRemoveMemberFromEvent.
*/
static EventManagerResult emAddOrRemoveMemberToEvent(EventManager em, int member_id, int event_id, int flag)
{
    if(em == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    if(member_id < 0)
    {
        return EM_INVALID_MEMBER_ID;
    }
    Event tmp_event = createTmpEvent(event_id);
    if(tmp_event == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    Event event_element_pointer = findEventById(em, tmp_event);
    if(event_element_pointer == NULL)
    {
        eventDestroy(tmp_event);
        return EM_EVENT_ID_NOT_EXISTS;
    }
    eventDestroy(tmp_event);

    Member tmp_member = memberCreate(TMP_MEMBER_NAME, member_id);
    if(tmp_member == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    if(!pqContains(em->exist_members, tmp_member))
    {
        memberDestroy(tmp_member);
        return EM_MEMBER_ID_NOT_EXISTS;
    }
    Member current_member = findMemberById(em->exist_members, tmp_member);
    memberDestroy(tmp_member);
    if(current_member == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    if(flag == ADD_MEMBER_TO_EVENT)
    {
        if(pqContains(eventGetMembers(event_element_pointer), current_member))
        {
            return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
        }
        if(updateMemberEventsCounterBy(em, current_member, flag) == EM_OUT_OF_MEMORY)
        {
            return EM_OUT_OF_MEMORY;
        }
        if(pqInsert(eventGetMembers(event_element_pointer), current_member, &member_id) == PQ_OUT_OF_MEMORY)
        {
            return EM_OUT_OF_MEMORY;
        }
        return EM_SUCCESS;
    }
    else
    {
        if(!pqContains(eventGetMembers(event_element_pointer), current_member))
        {
            return EM_EVENT_AND_MEMBER_NOT_LINKED;
        }
        if(updateMemberEventsCounterBy(em, current_member, flag) == EM_OUT_OF_MEMORY)
        {
            return EM_OUT_OF_MEMORY;
        }
        pqRemoveElement(eventGetMembers(event_element_pointer), current_member);
        return EM_SUCCESS;
    }
}

/**
* createEventManager: Allocates a new event manager with the given start date.
* @param date - The start date of the new event manager.
* @return
* 	NULL - if the parameter is NULL or allocations failed.
* 	A new event manager in case of success.
*/
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
    event_manager->events = pqCreate(eventCopyGeneric, eventDestroyGeneric, eventCompareByIdGeneric,
                                    dateCopyGeneric, dateDestroyGeneric, dateCompareGeneric);
    if(event_manager->events == NULL)
    {
        dateDestroy(event_manager->start_date);
        free(event_manager);
        return NULL;
    }
    event_manager->exist_members = pqCreate(memberCopyGeneric, memberDestroyGeneric, memberCompareByIdGeneric,
                                memberCopyGeneric, memberDestroyGeneric, memberCompareByIdAndEventsCounterGeneric);
    if(event_manager->exist_members == NULL)
    {
        dateDestroy(event_manager->start_date);
        pqDestroy(event_manager->events);
        free(event_manager);
        return NULL;
    }
    return event_manager;
}

/**
* destroyEventManager: Deallocates an existing event manager. Clears all elements by using the free functions.
* @param queue - Target priority queue to be deallocated. If priority queue is NULL nothing will be done.
*/
void destroyEventManager(EventManager em)
{
    if (em != NULL)
    {
        dateDestroy(em->start_date);
        pqDestroy(em->exist_members);
        pqDestroy(em->events);
        free(em);
    }
}

/**
* emAddEventByDate: Add a new event to the event manager in the given date.
* @param em - The event manager that will contain the new event.
* @param event_name - The name of the new event.
* @param date - The date of the new event.
* @param event_id - The id of the new event.
* @return
* 	EM_NULL_ARGUMENT if a NULL was sent as one of the parameters
*   EM_INVALID_DATE if the given date is before the start date of the event manager.
*   EM_INVALID_EVENT_ID if the given event id is negative.
*   EM_EVENT_ALREADY_EXISTS if an event with the same given name is already exists on the given date.
*   EM_EVENT_ID_ALREADY_EXISTS if the given event id is already exists as id of one of the exist events.
*   EM_OUT_OF_MEMORY if an allocation failed.
* 	EM_SUCCESS if the new event added to the event manager successfully.
*/
EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
    if(em == NULL || event_name == NULL || date == NULL)
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
    Event new_event_element = eventCreate(event_name, event_id, date, true);
    if(new_event_element == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    if(isEqualEventsByNameAndDate(em, new_event_element))
    {
        eventDestroy(new_event_element);
        return EM_EVENT_ALREADY_EXISTS;
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

/**
* emAddEventByDiff: Add a new event to the event manager on the given days after the start date of the event manager.
* @param em - The event manager that will contain the new event.
* @param event_name - The name of the new event.
* @param days - The days that the event should be added after the start date.
* @param event_id - The id of the new event.
* @return
* 	EM_NULL_ARGUMENT if a NULL was sent as one of the parameters
*   EM_INVALID_DATE if the given date is before the start date of the event manager (days in negative).
*   EM_INVALID_EVENT_ID if the given event id is negative.
*   EM_EVENT_ALREADY_EXISTS if an event with the same given name is already exists on the given date.
*   EM_EVENT_ID_ALREADY_EXISTS if the given event id is already exists as id of one of the exist events.
*   EM_OUT_OF_MEMORY if an allocation failed.
* 	EM_SUCCESS if the new event added to the event manager successfully.
*/
EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
    if (em == NULL || event_name == NULL)
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

/**
* emRemoveEvent: Remove the event with the given id from the event manager.
* @param em - The event manager that the event will be remove from.
* @param event_id - The id of the event.
* @return
* 	EM_NULL_ARGUMENT if a NULL was sent as one of the parameters
*   EM_INVALID_EVENT_ID if the given event id is negative.
*   EM_EVENT_NOT_EXISTS if there is no such event with the given event id in the event manager.
*   EM_OUT_OF_MEMORY if an allocation failed.
* 	EM_SUCCESS if the event removed from the event manager successfully.
*/
EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
    if(em == NULL || em->events == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event temp_event = createTmpEvent(event_id);
    if(temp_event == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }

    Event event_pointer = findEventById(em, temp_event);
    if (event_pointer == NULL)
    {
        eventDestroy(temp_event);
        return EM_EVENT_NOT_EXISTS;
    }
    PriorityQueue members_to_remove = eventGetMembers(event_pointer);
    Member member_pointer_copy;
    Member member_pointer = (Member) pqGetFirst(members_to_remove);
    while(member_pointer != NULL)
    {
        member_pointer_copy = member_pointer;
        if(emRemoveMemberFromEvent(em, memberGetId(member_pointer_copy), event_id) == EM_OUT_OF_MEMORY)
        {
            eventDestroy(temp_event);
            return EM_OUT_OF_MEMORY;
        }
        member_pointer = (Member) pqGetFirst(members_to_remove);
    }
    pqRemoveElement(em->events, temp_event);
    eventDestroy(temp_event);
    return EM_SUCCESS;
}

/**
* emChangeEventDate: Change the date of the event with the given event id, to the given event date.
* @param em - The event manager that the event will be change from.
* @param event_id - The id of the event.
* @param new_date - The new date of the event.
* @return
* 	EM_NULL_ARGUMENT if a NULL was sent as one of the parameters
*   EM_INVALID_DATE if the given date is before the start date of the event manager.
*   EM_INVALID_EVENT_ID if the given event id is negative.
*   EM_EVENT_ID_NOT_EXISTS if there is no such event with the given event id in the event manager.
*   EM_EVENT_ALREADY_EXISTS if an event with the same name is exists on the given new date.
*   EM_OUT_OF_MEMORY if an allocation failed.
* 	EM_SUCCESS if the event date changed successfully.
*/
EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date)
{
    if (em == NULL || new_date == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(dateCompare(new_date, em->start_date) < 0)
    {
        return EM_INVALID_DATE;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event temp_event = copyExistEventByIdWithNewDate(em, TMP_EVENT_NAME, event_id, new_date);
    if(temp_event == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    if(!pqContains(em->events, temp_event))
    {
        eventDestroy(temp_event);
        return EM_EVENT_ID_NOT_EXISTS;
    }
    if(isEqualEventsByNameAndDate(em, temp_event))
    {
        eventDestroy(temp_event);
        return EM_EVENT_ALREADY_EXISTS;
    }
    Event event_to_change = findEventById(em, temp_event);
    Date current_date = eventGetDate(event_to_change);
    if(pqChangePriority(em->events, event_to_change, current_date, new_date) == PQ_OUT_OF_MEMORY)
    {
        eventDestroy(temp_event);
        return EM_OUT_OF_MEMORY;
    }
    eventUpdateDate(event_to_change, new_date);
    eventDestroy(temp_event);
    return EM_SUCCESS;
}

/**
* emAddMember: Add a new member to the event manager.
* @param em - The event manager that will contain the new member.
* @param member_name - The name of the new member.
* @param member_id - The id of the new member.
* @return
* 	EM_NULL_ARGUMENT if a NULL was sent as one of the parameters
*   EM_INVALID_MEMBER_ID if the given member id is negative.
*   EM_MEMBER_ID_ALREADY_EXISTS if the member id is already exists on the event manager.
*   EM_OUT_OF_MEMORY if an allocation failed.
* 	EM_SUCCESS if the new member added to the event manager successfully.
*/
EventManagerResult emAddMember(EventManager em, char* member_name, int member_id)
{
    if(em == NULL || member_name == NULL)
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
        memberDestroy(new_member);
        return EM_MEMBER_ID_ALREADY_EXISTS;
    }
    if(pqInsert(em->exist_members, new_member, new_member) == PQ_OUT_OF_MEMORY)
    {
        memberDestroy(new_member);
        return EM_OUT_OF_MEMORY;
    }
    memberDestroy(new_member);
    return EM_SUCCESS;
}

/**
* emAddMemberToEvent: Add a member with the given member id to the group of members
    that managing the event with the given event id.
* @param em - The event manager that will contain the member.
* @param member_id - The id of the member.
* @param event_id - The id of the event.
* @return
* 	EM_NULL_ARGUMENT if a NULL was sent as one of the parameters
*   EM_INVALID_EVENT_ID if the given event id is negative.
*   EM_INVALID_MEMBER_ID if the given member id is negative.
*   EM_EVENT_ID_NOT_EXISTS if there is no such event with the given event id in the event manager.
*   EM_MEMBER_ID_NOT_EXISTS if there is no such member with the given member id in the event manager.
*   EM_EVENT_AND_MEMBER_ALREADY_LINKED if the event with the event id is already managed by
        the member with the member id.
*   EM_OUT_OF_MEMORY if an allocation failed.
* 	EM_SUCCESS if linked the member to the event successfully.
*/
EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{
    return emAddOrRemoveMemberToEvent(em, member_id, event_id, ADD_MEMBER_TO_EVENT);
}

/**
* emRemoveMemberFromEvent: Remove a member with the given member id from the group of members
    that managing the event with the given event id.
* @param em - The event manager that the member will be removed from.
* @param member_id - The id of the member.
* @param event_id - The id of the event.
* @return
* 	EM_NULL_ARGUMENT if a NULL was sent as one of the parameters
*   EM_INVALID_EVENT_ID if the given event id is negative.
*   EM_INVALID_MEMBER_ID if the given member id is negative.
*   EM_EVENT_ID_NOT_EXISTS if there is no such event with the given event id in the event manager.
*   EM_MEMBER_ID_NOT_EXISTS if there is no such member with the given member id in the event manager.
*   EM_EVENT_AND_MEMBER_NOT_LINKED if the event with the event id is not managed by the member with the member id.
*   EM_OUT_OF_MEMORY if an allocation failed.
* 	EM_SUCCESS if unlinked the member from the event successfully.
*/
EventManagerResult emRemoveMemberFromEvent (EventManager em, int member_id, int event_id)
{
    return emAddOrRemoveMemberToEvent(em, member_id, event_id, REMOVE_MEMBER_FROM_EVENT);
}

/**
* emTick: Promote the time on the event manager by the given days and remove the events that their time has passed.
* @param em - The event manager that sould be promoted.
* @param days - The days that the event manager should be promoted by.
* @return
* 	EM_NULL_ARGUMENT if a NULL was sent as one of the parameters
*   EM_INVALID_DATE if the given days is negative or zero.
*   EM_OUT_OF_MEMORY if an allocation failed.
* 	EM_SUCCESS if promote time and remove events from the event manager successfully.
*/
EventManagerResult emTick(EventManager em, int days)
{
    if(em == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(days <= 0)
    {
        return EM_INVALID_DATE;
    }
    while(days > 0)
    {
        dateTick(em->start_date);
        days--;
    }
    Event event_current_pointer = (Event) pqGetFirst(em->events);
    Event event_remove_pointer;
    Date current_event_date;
    while(event_current_pointer != NULL)
    {
        current_event_date = eventGetDate(event_current_pointer);
        if(dateCompare(current_event_date, em->start_date) < 0)
        {
            event_remove_pointer = event_current_pointer;
            emRemoveEvent(em, eventGetId(event_remove_pointer));
            event_current_pointer = (Event) pqGetFirst(em->events);
        }
        else
        {
            event_current_pointer = (Event) pqGetNext(em->events);
        }
    }
    return EM_SUCCESS;
}

/**
* emGetEventsAmount: Returns the amount of the planned events in the event manager.
* @param em - The event manager that contains the events.
* @return
* 	-1 if a NULL was sent as the parameter.
*   The amount of the planned events in the event manager otherwise.
*/
int emGetEventsAmount(EventManager em)
{
    if(em == NULL)
    {
        return NO_EVENTS_PLANNED;
    }
    return pqGetSize(em->events);
}

/**
* emGetNextEvent: Returns the next planned event. If their more then one in this day, returns the first that added.
* @param em - The event manager that contains the events.
* @return
* 	NULL if a NULL was sent as the parameter.
*   A pointer to the next planned event in the event manager otherwise.
*/
char* emGetNextEvent(EventManager em)
{
    if(em == NULL)
    {
        return NULL;
    }
    Event next_event = (Event) pqGetFirst(em->events);
    if(next_event == NULL)
    {
        return NULL;
    }
    return eventGetName(next_event);
}

/**
* emPrintAllEvents:
*   Print the events in the event manager with their date and managing members.
*   The events will be printed by their days.
*   If there are more then one on the same day, print first the event that has added first.
*   If there are more then one members that managed the same event, print the member with the lower id first.
* @param em - The event manager that contains the events.
* @param file_name - The file name that the events should be printed to.
*/
void emPrintAllEvents(EventManager em, const char* file_name)
{
    if(em != NULL && file_name != NULL)
    {
        FILE* stream = fopen(file_name, "a");
        if(stream != NULL)
        {
            Event current_event_pointer = (Event) pqGetFirst(em->events);
            char* current_event_name;
            Date current_event_date;
            int day, month, year;
            Member current_member_pointer;
            char* current_member_name;
            while(current_event_pointer != NULL)
            {
                current_event_name = eventGetName(current_event_pointer);
                current_event_date = eventGetDate(current_event_pointer);
                dateGet(current_event_date, &day, &month, &year);
                fprintf(stream, "%s,%d.%d.%d", current_event_name, day, month, year);
                current_member_pointer = (Member) pqGetFirst(eventGetMembers(current_event_pointer));
                while(current_member_pointer != NULL)
                {
                    current_member_name = memberGetName(current_member_pointer);
                    fprintf(stream, ",%s", current_member_name);
                    current_member_pointer = (Member) pqGetNext(eventGetMembers(current_event_pointer));
                }
                fprintf(stream, "\n");
                current_event_pointer = (Event) pqGetNext(em->events);
            }
            fclose(stream);
        }
    }
}

/**
* emPrintAllResponsibleMembers:
*   Print the exist members in the event manager with the events they are managed.
*   The members will be printed by their amount of events that they managed.
*   If there are more then one on the same amount, print the member with the lower id first.
*   Don't print members that don't managed events.
* @param em - The event manager that contains the members.
* @param file_name - The file name that the members should be printed to.
*/
void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{
    if(em != NULL && file_name != NULL)
    {
        FILE* stream = fopen(file_name, "a");
        if(stream != NULL)
        {
            Member current_member_pointer = (Member) pqGetFirst(em->exist_members);
            char* current_member_name;
            int current_member_associated_events_counter;
            while(current_member_pointer != NULL)
            {
                current_member_name = memberGetName(current_member_pointer);
                current_member_associated_events_counter = memberGetEventsCounter(current_member_pointer);
                if(current_member_associated_events_counter > 0)
                {
                    fprintf(stream, "%s,%d\n", current_member_name, current_member_associated_events_counter);
                }
                current_member_pointer = (Member) pqGetNext(em->exist_members);
            }
            fclose(stream);
        }
    }
}