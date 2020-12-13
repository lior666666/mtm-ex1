#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "date.h"

#define DAYS_IN_MONTH 30
#define MONTHS_IN_YEAR 12

struct Date_t 
{
    int day; 
    int month; 
    int year; 
};

//*** 1 ***
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

//*** 2 ***
void dateDestroy(Date date)
{
    if (date != NULL)
    {
        free(date);
    }
}

//*** 3 ***
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

//*** 4 ***
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

//*** 5 ***
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

//*** 6 ***
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