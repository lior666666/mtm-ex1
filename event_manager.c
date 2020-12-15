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

static PQElementPriority copyMemberEventsCounterGeneric(PQElementPriority generic_member)
{
    if(generic_member == NULL)
    {
        return NULL;
    }
    return memberCopy(generic_member);
}

static void freeMemberEventsCounter(PQElementPriority generic_member)
{
    memberDestroy(generic_member);
}

static int compareMembersEventsCounterGeneric(PQElementPriority first_generic_member, PQElementPriority second_generic_member)
{
    int compare_counter = compareMembersByEventsCounter(first_generic_member, second_generic_member);
    if(compare_counter != 0)
    {
        return compare_counter;
    }
    return memberGetId(second_generic_member) - memberGetId(first_generic_member);
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
    return dateCopy(generic_date);
}

static void freeDateGeneric(PQElementPriority date_generic)
{
    dateDestroy(date_generic);
}

static int compareDateGeneric(PQElementPriority first_date, PQElementPriority second_date)
{
    return dateCompare(second_date, first_date);
}
//*** <--

//find the event id in the queue, if exists, and copy this event (but with the new date!), or create new one by the arguments
static Event copyExistEventByIdWithNewDate(EventManager em, char* event_name, int event_id, Date new_date)
{
    Event event_pointer = (Event) pqGetFirst(em->events); // not sure if casting is required.
    char* current_name;
    int current_id;
    while(event_pointer != NULL)
    {
        current_name = eventGetName(event_pointer);
        current_id = eventGetId(event_pointer);
        if(current_id == event_id)
        {
            return eventCreate(current_name, event_id, new_date);
        }
        event_pointer = (Event) pqGetNext(em->events); // not sure if casting is required.
    }
    return eventCreate(event_name, event_id, new_date);
}

//check if there is an event with the same name on the same date
static bool isEqualEventsByNameAndDate(EventManager em, Event checking_event)
{
    Event event_pointer = (Event) pqGetFirst(em->events); // not sure if casting is required.
    while(event_pointer != NULL)
    {
        if(eventCompareName(event_pointer, checking_event) && eventCompareDate(event_pointer, checking_event) == 0)
        {
            return true;
        }
        event_pointer = (Event) pqGetNext(em->events); // not sure if casting is required.
    }
    return false;
}

//create tmp event by Id
static Event createTmpEvent(int event_id)
{
    Date tmp_date = dateCreate(1,1,1);
    if(tmp_date == NULL)
    {
        return NULL;
    }
    Event temp_event = eventCreate("tmp-event", event_id, tmp_date);
    dateDestroy(tmp_date);
    return temp_event;
}

//finds pointer to event with same ID, NULL if there is no such event.
static Event findEventById(EventManager em, Event tmp_event)
{
    Event event_pointer = (Event) pqGetFirst(em->events); // not sure if casting is required.
    while(event_pointer != NULL)
        {
            if(eventCompareId(event_pointer, tmp_event))
            {
                break;
            }
            event_pointer = (Event) pqGetNext(em->events); // not sure if casting is required.
        }
    return event_pointer;
}

//finds pointer to member with same ID, NULL if there is no such member.
static Member findMemberById(PriorityQueue members, Member tmp_member)
{
    Member member_pointer = (Member) pqGetFirst(members); // not sure if casting is required.
    while(member_pointer != NULL)
        {
            if(compareMembersById(member_pointer, tmp_member))
            {
                break;
            }
            member_pointer = (Member) pqGetNext(members); // not sure if casting is required.
        }
    return member_pointer;
}

//update the event counter of the member + update priority
static EventManagerResult updateMemberEventsCounterBy(EventManager em, Member tmp_member, int change_by)
{
    Member member_pointer = (Member) pqGetFirst(em->exist_members); // not sure if casting is required.
    while(member_pointer != NULL)
    {
        if(compareMembersById(member_pointer, tmp_member))
        {
            break;
        }
        member_pointer = (Member) pqGetNext(em->exist_members); // not sure if casting is required.
    }
    Member old_member = memberCopy(member_pointer);
    if(old_member == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    changeMemberEventsCounter(member_pointer, change_by);
    if(pqChangePriority(em->exist_members, member_pointer, old_member, member_pointer) == PQ_OUT_OF_MEMORY)
    {
        changeMemberEventsCounter(member_pointer, (change_by*-1)); // to make sure the priority changed back to what it was.
        return EM_OUT_OF_MEMORY;
    }
    return EM_SUCCESS;
}

//new internal function to save duplicate code of function 8 and 9
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

    Member tmp_member = memberCreate("tmp-member", member_id);
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
        free(em);
    }
}

//*** 3 ***
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
    Event new_event_element = eventCreate(event_name, event_id, date);
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

//*** 4 ***
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

//*** 5 ***
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
    Member member_pointer = (Member) pqGetFirst(members_to_remove); // not sure if casting is needed.
    while(member_pointer != NULL)
    {
        member_pointer_copy = member_pointer;
        member_pointer = (Member) pqGetNext(members_to_remove); // not sure if casting is needed.
        if(emRemoveMemberFromEvent(em, memberGetId(member_pointer_copy), event_id) == EM_OUT_OF_MEMORY)
        {
            eventDestroy(temp_event);
            return EM_OUT_OF_MEMORY;
        }
    }
    pqRemoveElement(em->events, temp_event);
    eventDestroy(temp_event);
    return EM_SUCCESS;
}

//*** 6 ***
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
    Event temp_event = copyExistEventByIdWithNewDate(em, "tmp-event", event_id, new_date);
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
    updateEventDate(event_to_change, new_date);
    eventDestroy(temp_event);
    return EM_SUCCESS;
}

//*** 7 ***
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

//*** 8 ***
EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{
    return emAddOrRemoveMemberToEvent(em, member_id, event_id, ADD_MEMBER_TO_EVENT);
}

//*** 9 ***
EventManagerResult emRemoveMemberFromEvent (EventManager em, int member_id, int event_id)
{
    return emAddOrRemoveMemberToEvent(em, member_id, event_id, REMOVE_MEMBER_FROM_EVENT);
}

//*** 10 ***
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
    Event event_current_pointer = (Event) pqGetFirst(em->events); // not sure if casting is required.
    Event event_next_pointer;
    Date current_event_date;
    while(event_current_pointer != NULL)
    {
        current_event_date = eventGetDate(event_current_pointer);
        //datePrint(current_event_date);
        if(dateCompare(current_event_date, em->start_date) < 0)
        {
            event_next_pointer = (Event) pqGetNext(em->events); // not sure if casting is required.
            emRemoveEvent(em, eventGetId(event_current_pointer));
            event_current_pointer = event_next_pointer;
        }
        else
        {
            event_current_pointer = (Event) pqGetNext(em->events); // not sure if casting is required.
        }
    }
    return EM_SUCCESS;
}

//*** 11 ***
int emGetEventsAmount(EventManager em)
{
    if(em == NULL)
    {
        return NO_EVENTS_PLANNED;
    }
    return pqGetSize(em->events);
}

//*** 12 ***
char* emGetNextEvent(EventManager em)
{
    if(em == NULL)
    {
        return NULL;
    }
    Event next_event = (Event) pqGetFirst(em->events); // not sure if casting is required.
    if(next_event == NULL)
    {
        return NULL;
    }
    return eventGetName(next_event);
}

//*** 13 ***
void emPrintAllEvents(EventManager em, const char* file_name)
{
    if(em != NULL && file_name != NULL)
    {
        FILE* stream = fopen(file_name, "a");
        if(stream != NULL)
        {
            Event current_event_pointer = (Event) pqGetFirst(em->events); // not sure if casting is required.
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
                current_member_pointer = (Member) pqGetFirst(eventGetMembers(current_event_pointer)); // not sure if casting is required.
                while(current_member_pointer != NULL)
                {
                    current_member_name = memberGetName(current_member_pointer);
                    fprintf(stream, ",%s", current_member_name);
                    current_member_pointer = (Member) pqGetNext(eventGetMembers(current_event_pointer)); // not sure if casting is required.
                }
                fprintf(stream, "\n");
                current_event_pointer = (Event) pqGetNext(em->events);
            }
            fclose(stream);
        }
    }
}

//*** 14 ***
void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{
    if(em != NULL && file_name != NULL)
    {
        FILE* stream = fopen(file_name, "a");
        if(stream != NULL)
        {
            Member current_member_pointer = (Member) pqGetFirst(em->exist_members); // not sure if casting is required.
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
                current_member_pointer = (Member) pqGetNext(em->exist_members); // not sure if casting is required.
            }
            fclose(stream);
        }
    }
}
