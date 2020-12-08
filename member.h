#ifndef MEMBER_H_
#define MEMBER_H_

#include <stdbool.h>

/** Type for defining the member */
typedef struct Member_t *Member;

/**
* memberCreate: Allocates a new member with zero associated events.
*
* @param name - the name of the member.
* @param id - the id of the member.
* @return
* 	NULL - if allocation failed or if a NULL was sent as one of the parameters.
* 	A new Member in case of success.
*/
Member memberCreate(char* name, int id);

/**
* memberDestroy: Deallocates an existing Member.
*
* @param member - Target Member to be deallocated. If member is NULL nothing will be done
*/
void memberDestroy(Member member);

/**
* memberCopy: Creates a copy of target Member.
*
* @param member - Target Member.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Member containing the same elements as member otherwise.
*/
Member memberCopy(Member member);

/**
* compareMembersById: identify equal members by comparing the target member's ID.
*
* @param first_member - First target Member.
* @param second_member - Second target Member.
* @return
* 	false if a NULL was sent.
* 	true if they're equal, false otherwise.
*/
bool compareMembersById(Member first_member, Member second_member);

/**
* compareMembersByEventsCounter: identify which one of the target members associating with more events.
*
* @param first_member - First target Member.
* @param second_member - Second target Member.
* @return
* 	0 if a NULL was sent (user must check that NULL does not send).
* 	A positive integer if the first member associating with more events.
* 	0 if they're associating with the same amount of events;
*	A negative integer if the second member associating with more events.
*/
int compareMembersByEventsCounter(Member first_member, Member second_member);

/**
* memberGet: Returns the name, id number and the associated events counter of the member. 
*
* @param member - Target Member.
* @param name - the pointer to assign to name of the Member.
* @param id - the pointer to assign to id of the Member.
* @param associated_events_counter - the pointer to assign to associated events counter of the Member.
*
* @return
*  the member name, id and associated events counter are assigned to the pointers. if the pointer is NULL, do nothing with this pointer. 
*/
void memberGet(Member member, char* name, int* id, int* associated_events_counter);

/**
* changeMemberEventsCounter: increases or decrease the associated events counter of the target Member by change_by, and returns it.
*
* @param member - Target Member.
* @param change_by - the change for the counter.
* @return
* 	-1 if a NULL was sent or there is negative counter after the operation.
* 	the update associated events counter of the member otherwise.
*/
int changeMemberEventsCounter(Member member, int change_by)

#endif //MEMBER_H_
