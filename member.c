#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "member.h"

#define SUCCESS 0
#define NULL_ARGUMENT -1
#define NEGATIVE_COUNTER -1
struct Member_t 
{
    char* name;
    int id;
    int associated_events_counter;
};

//*** 1 ***
Member memberCreate(char* name, int id)
{
    if(name == NULL || id == NULL)
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

//*** 2 ***
void memberDestroy(Member member)
{
    if (member != NULL)
    {
        free(member->name);
        free(member);
    }
}

//*** 3 ***
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

//*** 4 ***
bool compareMembersById(Member first_member, Member second_member)
{
    if (first_member == NULL || second_member == NULL)
    {
        return false;
    }
    return first_member->id == second_member->id;
}

//*** 5 ***
int compareMembersByEventsCounter(Member first_member, Member second_member)
{
    if (first_member == NULL || second_member == NULL)
    {
        return 0;
    }
    return first_member->associated_events_counter - second_member->associated_events_counter;
}

//*** 6 ***
void memberGet(Member member, char* name, int* id, int* associated_events_counter)
{
    if(member != NULL)
    {
        if(name != NULL)
        {
            *name = member->name;
        }
        if(id != NULL)
        {
            *id = member->id; 
        }
        if(associated_events_counter != NULL)
        {
            *associated_events_counter = member->associated_events_counter; 
        }
    }
}

//*** 7 ***
int changeMemberEventsCounter(Member member, int change_by)
{
    if (member == NULL || member->associated_events_counter == NULL)
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