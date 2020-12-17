#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "date.h"

#define DAYS_IN_MONTH 30
#define MONTHS_IN_YEAR 12

struct Date_t
{
    int day;
    int month;
    int year;
};

/**
* dateCreate: Allocates a new date.
* @param day - the day of the date.
* @param month - the month of the date.
* @param year - the year of the date.
* @return
* 	NULL - if allocation failed or date is illegal.
* 	A new Date in case of success.
*/
Date dateCreate(int day, int month, int year)
{
    if(day < 1 || day > DAYS_IN_MONTH)
    {
        return NULL;
    }
    if(month < 1 || month > MONTHS_IN_YEAR)
    {
        return NULL;
    }
    Date new_date = malloc(sizeof(*new_date));
    if(new_date == NULL)
    {
        return NULL;
    }
    new_date->day = day;
    new_date->month = month;
    new_date->year = year;
    return new_date;
}

/**
* dateDestroy: Deallocates an existing Date.
* @param date - Target date to be deallocated. If priority queue is NULL nothing will be done
*/
void dateDestroy(Date date)
{
    if (date != NULL)
    {
        free(date);
    }
}

/**
* dateCopy: Creates a copy of target Date.
* @param date - Target Date.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Date containing the same elements as date otherwise.
*/
Date dateCopy(Date date)
{
    if (date == NULL)
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

/**
* dateGet: Returns the day, month and year of a date
* @param date - Target Date
* @param day - the pointer to assign to day of the date into.
* @param month - the pointer to assign to month of the date into.
* @param year - the pointer to assign to year of the date into.
*
* @return
* 	false if one of pointers is NULL.
* 	Otherwise true and the date is assigned to the pointers.
*/
bool dateGet(Date date, int* day, int* month, int* year)
{
    if (date == NULL || day == NULL || month == NULL || year == NULL)
    {
        return false;
    }
    *day = date->day;
    *month = date->month;
    *year = date->year;
    return true;
}

/**
* dateCompare: compares to dates and return which comes first
* @return
* 		A negative integer if date1 occurs first;
* 		0 if they're equal or one of the given dates is NULL;
*		A positive integer if date1 arrives after date2.
*/
int dateCompare(Date date1, Date date2)
{
    if(date1 == NULL || date2 == NULL)
    {
        return 0;
    }
    if(date1->year == date2->year)
    {
        if(date1->month == date2->month)
        {
            if (date1->day == date2->day)
            {
               return 0;
            }
            return (date1->day)-(date2->day);
        }
        return (date1->month)-(date2->month);
    }
    return (date1->year)-(date2->year);
}

/**
* dateTick: increases the date by one day, if date is NULL should do nothing.
* @param date - Target Date
*/
void dateTick(Date date)
{
    if (date != NULL)
    {
        if (date->day != DAYS_IN_MONTH)
        {
            date->day++;
        }
        else
        {
            date->day = 1;
            if (date->month == MONTHS_IN_YEAR)
            {
                date->year++;
                date->month = 1;
            }
            else
            {
                date->month++;
            }
        }
    }
}