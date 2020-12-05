#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "date.h"
struct Date_t 
{
    int day; 
    int month; 
    int year; 
};
// Is there an option of illigal year? 
Date dateCreate(int day, int month, int year)
{
    if(day<1&&day>30)
    {
        return NULL;
    }
    if(month<1&&month>12)
    {
        return NULL;
    }
    Date new_date = malloc(sizeof(*new_date));
    if(new_date ==NULL)
    {
        return NULL;
    }
    new_date->day = day;
    new_date->month = month; 
    new_date->year = year; 
    return new_date;
}

Date dateCopy(Date date)
{
    if (date ==NULL)
    {
        return NULL;
    }
    Date copy_date = malloc(sizeof(*copy_date));
    if(copy_date == NULL)
    {
        return NULL;
    }
    copy_date->day = date->day;
    copy_date->month = date->month;
    copy_date->year = date->year;
    return copy_date;
}

int dateCompare(Date date1, Date date2)
{
    if(date1 == NULL || date2 == NULL)
    {
        return 0;
    }
    if(date1->year==date2->year)
    {
        if(date1->month==date2->month)
        {
            if (date1->day == date2->day)
            {
               return 0; 
            }
            return date1->day-date2->day;  
        }
        return date1->month-date2->month; 
    }
    return date1->year-date2->year;
}


