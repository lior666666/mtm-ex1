#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "event.h"
#include <string.h>
#include "date.h"
#include "member.h"
#include "priority_queue.h"
//remember to remove "priority_queue.h"

struct Event_t 
{
    int event_id; 
    char* event_name;
    Date event_date; 
    PriorityQueue event_members; 
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

static PQElementPriority copyIdMemberGeneric(PQElementPriority generic_id)
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

static void freeIdGeneric(PQElementPriority generic_id)
{
    return;
}

static int compareMembersIdGeneric(PQElementPriority first_member_id, PQElementPriority second_member_id)
{
    return *(int *) first_member_id - *(int *) second_member_id;
}

Event eventCreate(char* event_name, int event_id, Date event_date)
{
    if(event_name == NULL || event_id == NULL || event_date == NULL)
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
    if(new_event->event_name = NULL)
    {
        dateDestroy(new_event->event_date);
        free(new_event); 
        return NULL;
    }
    strcpy(new_event->event_name, event_name);
    new_event->event_id = event_id;
    new_event->event_members = pqCreate(memberCopyGeneric, memberDestroyGeneric, equalGenericMember, copyIdMemberGeneric, freeIdGeneric, compareMembersIdGeneric);
    if(new_event->event_members == NULL)
    {
        dateDestroy(new_event->event_date);
        free(new_event->event_name);
        free(new_event); 
        return NULL;
    }
    return new_event; 
}

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

Event eventCopy(Event event)
{
    if (event == NULL)
    {
        return NULL;
    }
    Event event_copy = eventCreate(event->event_name, event->event_id, event->event_date);
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

void eventGet(Event event, char* event_name, int* event_id, Date event_date)
{
    if(event != NULL)
    {
        if(event_name != NULL)
        {
            *event_name = event->event_name;
        }
        if(event_id!= NULL)
        {
            *event_id = event->event_id; 
        }
        if(event_date != NULL)
        {
            event_date = event->event_date; 
        }
    }
}

PriorityQueue eventGetMembers(Event event)
{
    if(event == NULL)
    {
        return NULL; 
    }
    return event->event_members; 
}

bool eventCompareId(Event first_event, Event second_event)
{
    if(first_event == NULL || second_event == NULL)
    {
        return false; 
    }
    return first_event->event_id == second_event->event_id;
}

bool eventCompareName(Event first_event, Event second_event)
{
    if(first_event == NULL || second_event == NULL)
    {
        return false; 
    }
    return strcmp(first_event->event_name, second_event->event_name) == 0;
}

int eventCompareDate(Event first_event, Event second_event)
{
    if(first_event == NULL || second_event == NULL)
    {
        return 0; 
    }
    return dateCompare(first_event->event_date, second_event->event_date);
}