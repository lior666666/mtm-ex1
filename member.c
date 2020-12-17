#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "member.h"

#define NEGATIVE_COUNTER -1
#define NULL_ARGUMENT -1
#define SUCCESS 0
#define INVALID_ID -1
#define INVALID_COUNTER -1

struct Member_t 
{
    char* name;
    int id;
    int associated_events_counter;
};

/**
* memberCreate: Allocates a new member with zero associated events.
* @param name - the name of the member.
* @param id - the id of the member.
* @return
* 	NULL - if allocation failed or if a NULL was sent as one of the parameters.
* 	A new Member in case of success.
*/
Member memberCreate(char* name, int id)
{
    if(name == NULL)
    {
        return NULL;
    }
    Member new_member = malloc(sizeof(*new_member));
    if(new_member == NULL)
    {
        return NULL;
    }
    new_member->name = malloc(strlen(name)+1);
    if(new_member->name == NULL)
    {
        free(new_member);
        return NULL;
    }
    strcpy(new_member->name, name);
    new_member->id = id;
    new_member->associated_events_counter = 0;
    return new_member;
}

/**
* memberDestroy: Deallocates an existing Member.
* @param member - Target Member to be deallocated. If member is NULL nothing will be done
*/
void memberDestroy(Member member)
{
    if (member != NULL)
    {
        free(member->name);
        free(member);
    }
}

/**
* memberCopy: Creates a copy of target Member.
* @param member - Target Member.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Member containing the same elements as member otherwise.
*/
Member memberCopy(Member member)
{
    if (member == NULL)
    {
        return NULL;
    }
    Member copy_member = memberCreate(member->name, member->id);
    if(copy_member == NULL)
    {
        return NULL;
    }
    copy_member->associated_events_counter = member->associated_events_counter;
    return copy_member;
}

/**
* memberCompareById: identify equal members by comparing the target member's ID.
* @param first_member - First target Member.
* @param second_member - Second target Member.
* @return
* 	false if a NULL was sent.
* 	true if they're equal, false otherwise.
*/
bool memberCompareById(Member first_member, Member second_member)
{
    if (first_member == NULL || second_member == NULL)
    {
        return false;
    }
    return first_member->id == second_member->id;
}

/**
* memberCompareByEventsCounter: identify which one of the target members associating with more events.
* @param first_member - First target Member.
* @param second_member - Second target Member.
* @return
* 	0 if a NULL was sent (user must check that NULL does not send).
* 	A positive integer if the first member associating with more events.
* 	0 if they're associating with the same amount of events;
*	A negative integer if the second member associating with more events.
*/
int memberCompareByEventsCounter(Member first_member, Member second_member)
{
    if (first_member == NULL || second_member == NULL)
    {
        return 0;
    }
    return first_member->associated_events_counter - second_member->associated_events_counter;
}

/**
* memberGetName: Returns the member name.
* @param member - Target Member.
* @return
*   returns NULL in case the member is NULL.
* 	Otherwise returns member name.
*/
char* memberGetName(Member member)
{
    if(member == NULL)
    {
        return NULL; 
    }
    return member->name;
}

/**
* memberGetId: Returns the member id.
* @param member - Target Member.
* @return
*   returns -1 in case the member is NULL.
* 	Otherwise returns member id.
*/
int memberGetId(Member member)
{
     if(member == NULL)
    {
        return INVALID_ID; 
    }
    return member->id;
}

/**
* memberGetEventsCounter: Returns the member associated events counter.
* @param member - Target Member.
* @return
*   returns -1 in case the member is NULL.
* 	Otherwise returns member associated events counter.
*/
int memberGetEventsCounter(Member member)
{
     if(member == NULL)
    {
        return INVALID_COUNTER; 
    }
    return member->associated_events_counter;
}

/**
* memberChangeEventsCounter: increases or decrease the associated events counter of the target Member by change_by.
* @param member - Target Member.
* @param change_by - the change for the counter.
* @return
* 	-1 if a NULL was sent or there is negative counter after the operation.
* 	zero otherwise.
*/
int memberChangeEventsCounter(Member member, int change_by)
{
    if (member == NULL)
    {
        return NULL_ARGUMENT;
    }
    member->associated_events_counter += change_by;
    if (member->associated_events_counter < 0)
    {
        member->associated_events_counter = 0; 
        return NEGATIVE_COUNTER;
    }
    return SUCCESS;
}