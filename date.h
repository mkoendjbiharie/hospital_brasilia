/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   date.h
 * Author: marlon
 *
 * Created on 18 de Setembro de 2016, 09:38
 */

#include <stdio.h>
#include <string>
#include <iostream>
//#include <string.h>
using namespace std;

class date
{
private:
    unsigned short int day;
    unsigned short int month;
    unsigned short int year;
    //unsigned short int day_of_week;
    int month_limit();
public:
    date();
    date(string dia);
    date(int in_day, int in_month, int in_year);//, int in_day_of_week);
    bool operator==(date dia);
    date& operator=(date rhs);
    date& operator++();    //pre-increment (++date)
    date& operator--();    //pre-increment (++date)
    date operator++(int);  //post-increment (date++)
    date operator--(int);  //post-increment (date++)

    //these are "special" they don't work exactly as you might think and are slightly broken right now,
    //but I'm putting them in regardless
    //they also don't work on DOW right now.
    date& operator+=(const date& rhs);               
    date& operator-=(const date& rhs);
    //inline date operator+(date lhs, const date& rhs);
    //inline date operator-(date lhs, const date& rhs);
    bool is_leap_year();
    void println();
};


bool date::operator==(date dia){
    return (this->day==dia.day&&this->month==dia.month&&this->year==dia.year);
}

date::date(string dia){
    this->day=atoi(dia.substr(0,2).c_str());
    this->month=atoi(dia.substr(3,2).c_str());
    this->year=atoi(dia.substr(6,4).c_str());
}

void date::println(){
    cout<<this->day<<"/"<<this->month<<"/"<<this->year<<endl;
}
 
date::date(){
    this->day = 1;
    this->month = 1;
    this->year = 1900;
//    this->day_of_week = in_day_of_week;
}

date::date(int in_day, int in_month, int in_year)//, int in_day_of_week)
{
    this->day = in_day;
    this->month = in_month;
    this->year = in_year;
//    this->day_of_week = in_day_of_week;
}

int date::month_limit()
{
    switch (this->month)
    {
    case 4:
    case 6:
    case 9:
    case 11:
    {
        return 30;
        break;
    }
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 12:
    {
        return 31;
        break;
    }
    case 2:
        if (this->is_leap_year())
        {
            return 29;
            break;
        }
        else
        {
            return 28;
            break;
        }

    }
}

bool date::is_leap_year()
{
    if (this->year % 4 != 0) return false;
    else
        if (this->year % 100 != 0) return true;
        else
            if (this->year % 400 != 0) return false;
            else
                return true;
}

/*date& date::operator=(date rhs)
{
  date(rhs);
  return *this;
}*/

date& date::operator++()
{
    //this->day_of_week = (this->day_of_week + 1) % 7; //increase weekday
    date *dia=this;
    dia->day++;
    if ((dia->day) > month_limit())           //increase day, if larger than month size...
        {
            dia->day = 1;                           //reset day to 1
            if (++this->month > 12)                  //increase month, if larger than year size
            {
              dia->month = 1;                       //reset month to 1
              dia->year++;                          //increase year
            }
        }
    return *dia;
}

date& date::operator--()
{
    date *dia=this;
    dia->day--;
    if ((dia->day)<1){
        if(dia->month==1){
            dia->month=12;
            dia->year--;
        }
        else
            dia->month--;
        dia->day=dia->month_limit();
    }
    return *dia;
}

date date::operator++(int)
{
    date tmp(*this);
    operator++();
    return tmp;
}

date date::operator--(int)
{
    date tmp(*this);
    operator--();
    return tmp;
}

//adds years on, then months, then days
date& date::operator+=(const date& rhs)
{
    this->year  += rhs.year;
    this->month += rhs.month;
    this->day   += rhs.day;
    if (this->month > 12)  //get to the right month
    {
        this->year  = this->month / 12;
        this->month = this->month % 12;
    }
    if (this->day > month_limit())
    {
        this->month = this->day / month_limit();
        this->day   = this->day % month_limit();
        if (this->month > 12)  //recalculate  **yes, I know this is currently wrong if more than one month is added on in days**
        {
            this->year  = this->month / 12;
            this->month = this->month % 12;
        }
    }
    return *this;
}

/*inline date::date operator+(date lhs, const date& rhs)
{
  lhs += rhs;
  return lhs;
}*/

//subtracts years, then months, then days
date& date::operator-=(const date& rhs)
{
    if ((rhs.year < this->year) || ((rhs.year == this->year) && (rhs.month < this->month))
       || ((rhs.year == this->year) && (rhs.month == this->month)) && (rhs.day < this->day))
    
        //date(rhs);
    
    this->year -= rhs.year;
    this->month -= rhs.month;
    this->day -= rhs.day;

    return *this;
}