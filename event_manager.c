#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "date.h"
#include "member.h"
#include "priority_queue.h" //remember to remove

struct EventManager_t 
{
    Date start_date;
    PriorityQueue exist_members;
    PriorityQueue events;
};