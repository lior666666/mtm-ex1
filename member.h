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
* eventsCounterCopy: Creates a copy of the associated events counter of the target Member.
*
* @param associated_events_counter - Target associated events counter.
* @return
* 	NULL if a NULL was sent or a negative integer was sent.
* 	A copy of the associated events counter that sent otherwise.
*/
int eventsCounterCopy(int associated_events_counter);

/**
* compareMembersById: identify equal members by comparing the target member's ID.
*
* @param first_member - First target Member.
* @param second_member - Second target Member.
* @return
* 	NULL if a NULL was sent.
* 	true if they're equal, false otherwise.
*/
bool compareMembersById(Member first_member, Member second_member);

/**
* compareMembersByEventsCounter: identify which one of the target members associating with more events.
*
* @param first_member - First target Member.
* @param second_member - Second target Member.
* @return
* 	NULL if a NULL was sent.
* 	A positive integer if the first member associating with more events.
* 	0 if they're associating with the same amount of events;
*	A negative integer if the second member associating with more events.
*/
int compareMembersByEventsCounter(Member first_member, Member second_member);

/**
* getMemberName: returns the name of the target member.
*
* @param member - Target Member.
* @return
* 	NULL if a NULL was sent.
* 	the name of the member otherwise.
*/
char* getMemberName(Member member);

/**
* getMemberId: returns the id of the target member.
*
* @param member - Target Member.
* @return
* 	NULL if a NULL was sent.
* 	the id of the member otherwise.
*/
int getMemberId(Member member);

/**
* getMemberEventsCounter: returns the associated events counter of the target Member.
*
* @param member - Target Member.
* @return
* 	NULL if a NULL was sent.
* 	the associated events counter of the member otherwise.
*/
int getMemberEventsCounter(Member member);

/**
* increaseMemberEventsCounter: increases the associated events counter of the target Member by 1, and returns it.
*
* @param member - Target Member.
* @return
* 	NULL if a NULL was sent.
* 	the update associated events counter of the member otherwise.
*/
int increaseMemberEventsCounter(Member member);

/**
* decreaseMemberEventsCounter: decreases the associated events counter of the target Member by 1, and returns it.
*
* @param member - Target Member.
* @return
* 	NULL if a NULL was sent or the current associated events counter is zero.
* 	the update associated events counter of the member otherwise.
*/
int decreaseMemberEventsCounter(Member member);

#endif //MEMBER_H_
