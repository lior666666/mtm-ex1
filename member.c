#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "member.h"

#define FIRST_MEMBER_EVENTS_CONTER_IS_GREATER 1
#define SECOND_MEMBER_EVENTS_CONTER_IS_GREATER -1

struct Member_t 
{
    int name;
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
    new_member->name = name;
    new_member->id = id;
    new_member->associated_events_counter = 0;
    return new_member;
}

//*** 2 ***
void memberDestroy(Member member)
{
    if (member != NULL)
    {
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
    Member copy_member = malloc(sizeof(*copy_member));
    if(copy_member == NULL)
    {
        return NULL;
    }
    copy_member->name = member->name;
    copy_member->id = member->id;
    copy_member->associated_events_counter = member->associated_events_counter;
    return copy_member;
}

//*** 4 ***
int eventsCounterCopy(int associated_events_counter)
{
    if (associated_events_counter == NULL || associated_events_counter < 0)
    {
        return NULL;
    }
    return associated_events_counter;
}

//*** 5 ***
bool compareMembersById(Member first_member, Member second_member)
{
    if (first_member == NULL || second_member == NULL)
    {
        return NULL;
    }
    return (first_member->id == second_member->id);
}

//*** 6 ***
int compareMembersByEventsCounter(Member first_member, Member second_member)
{
    if (first_member == NULL || second_member == NULL)
    {
        return NULL;
    }
    if (first_member->associated_events_counter > second_member->associated_events_counter)
    {
        return FIRST_MEMBER_EVENTS_CONTER_IS_GREATER;
    }
    else if (first_member->associated_events_counter < second_member->associated_events_counter)
    {
        return SECOND_MEMBER_EVENTS_CONTER_IS_GREATER;
    }
    return 0;
}

//*** 7 ***
char* getMemberName(Member member)
{
    if (member == NULL)
    {
        return NULL;
    }
    return member->name;
}

//*** 8 ***
int getMemberId(Member member)
{
    if (member == NULL)
    {
        return NULL;
    }
    return member->id;
}

//*** 9 ***
int getMemberEventsCounter(Member member)
{
    if (member == NULL)
    {
        return NULL;
    }
    return member->associated_events_counter;
}

//*** 10 ***
int increaseMemberEventsCounter(Member member)
{
    if (member == NULL)
    {
        return NULL;
    }
    member->associated_events_counter++;
    return member->associated_events_counter;
}

//*** 11 ***
int decreaseMemberEventsCounter(Member member)
{
    if (member == NULL || member->associated_events_counter == 0)
    {
        return NULL;
    }
    member->associated_events_counter--;
    return member->associated_events_counter;
}