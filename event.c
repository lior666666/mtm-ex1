#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "event.h"
#include <string.h>
#include "date.h"
#include "member.h"
#include "priority_queue.h"

struct Event_t 
{
    int event_id; 
    char* event_name;
    Date event_date; 
    PriorityQueue event_members; 
};

static PQElement copyMemberGeneric(PQElement generic_memmber)
{
    if(generic_memmber == NULL)
    {
        return NULL;
    }
    Member copy_member = memberCopy(generic_memmber);
    if(copy_member ==NULL)
    {
        return NULL;
    }
    return copy_member; 
}

static void freeGenericMember(PQElement gemeric_memeber)
{
    memberDestroy(gemeric_memeber);
}

static bool equalGenericMember(PQElement generic_member1, PQElement generic_member2)
{
    if(compareMembersById(generic_member1, generic_member2)==0)
        return true;
    return false; 
}

static PQElementPriority copyIdMemberGeneric(PQElementPriority generic_id)
{
   if(generic_id == NULL)
    {
        return NULL;
    }
    int *copy_id = malloc(sizeof(*copy_id));
    if(copy_id ==NULL)
    {
        return NULL;
    }
    *copy_id = *(int *) generic_id;
    return copy_id; 
}

static void freeGenericId(PQElementPriority gemeric_id)
{
    return;
}

static int compareGenericMemberPriorityId(PQElementPriority member_id_1, PQElementPriority member_id_2)
{
    return *(int *) member_id_1 - *(int *) member_id_2;
}
Event eventCreate(char* event_name, int event_id, Date event_date)
{
    if(event_name ==NULL || event_date == NULL)
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
    new_event->event_name = malloc(strlen(event_name));
    if(new_event->event_name = NULL)
    {
        free(new_event); 
        return NULL;
    }
    strcpy(new_event->event_name, event_name);
    new_event->event_id = event_id;
    new_event->event_members = pqCreate(copyMemberGeneric, freeGenericMember, equalGenericMember, copyIdMemberGeneric, freeGenericId, compareGenericMemberPriorityId);
    if(new_event->event_members == NULL)
    {
        free(new_event); 
        return NULL;
    }
    return new_event; 
}
void eventDestroy(Event event)
{
    if(event != NULL)
    {
        if(event->event_name!=NULL)
        {
            free(event->event_name);
        }
        pqDestroy(event->event_members);
        free(event);
    }
}

Event eventCopy(Event event)
{
    Event copy_event =  eventCreate(event->event_name, event->event_id,event->event_date);
    if(copy_event==NULL)
    {
        return NULL;
    }
    copy_event->event_members = pqCopy(event->event_members);
    if(copy_event->event_members==NULL)
    {
        eventDestroy(copy_event);
        return NULL;  
    }
    return copy_event; 
}

void eventGet(Event event, char** event_name, int* event_id, Date event_date)
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


bool eventCompareId(Event event1, Event event2)
{
    if(event1 == NULL || event2 == NULL)
    {
        return false; 
    }
    if(event1->event_id == event2->event_id)
    {
        return true; 
    }
    return false; 
}


bool eventCompareName(Event event1, Event event2)
{
    if(event1 == NULL || event2 == NULL)
    {
        return false; 
    }
     if(strcmp(event1->event_name, event2->event_name)==0)
    {
        return true; 
    }
    return false;
}



