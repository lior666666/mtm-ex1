#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "event.h"
#include <string.h>
#include "priority_queue.h"

struct Event_t 
{
    int event_id; 
    char* event_name;
    PriorityQueue event_members; 
};



Event eventCreate(char* event_name, int event_id)
{
    if(event_name ==NULL)
    {
        return NULL;
    }
    Event new_event = malloc(sizeof(*new_event)); 
    if(new_event == NULL)
    {
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
    new_event->event_members = NULL; // NEEDS TO BE CHANGED TO CREATE AN EMPTY QUEUE.  
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
    Event copy_event =  eventCreate(event->event_name, event->event_id);
    if(copy_event==NULL)
    {
        return NULL;
    }
    copy_event->event_members = pqCopy(event->event_members);
    if(copy_event->event_members==NULL)
    {
        eventDestroy(copy_event);
    }
    return copy_event; 
}

void eventGet(Event event, char** event_name, int* event_id)
{
    if(event_name != NULL)
    {
        *event_name = event->event_name;
    }
    if(event_id!= NULL)
    {
        *event_id = event->event_id; 
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



