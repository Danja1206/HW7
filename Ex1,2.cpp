#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

class Date
{
public:
    Date(int _day, int _month, int _year) : day(_day), month(_month), year(_year) {}
    int getDay() { return day; }
    int getMonth() { return month; }
    int getYear() { return year; }
    friend ostream& operator<<(ostream& str, const Date& date)
    {
        str << date.day << "." << date.month << "." << date.year;
        return str;
    }
private:
    int day, month, year;
};

bool isNull(unique_ptr<Date>& date) { return date.get() == nullptr; }
unique_ptr<Date>& maxDate(unique_ptr<Date>& date1, unique_ptr<Date>& date2)
{
    if (date1.get()->getYear() > date2.get()->getYear())
        return date1;
    else if (date1.get()->getYear() < date2.get()->getYear())
        return date2;
    else
    {
        if (date1.get()->getMonth() > date2.get()->getMonth())
            return date1;
        else if (date1.get()->getMonth() < date2.get()->getMonth())
            return date2;
        else
        {
            if (date1.get()->getDay() > date2.get()->getDay())
                return date1;
            else
                return date2;
        }
    }
}
void swapDates(unique_ptr<Date>& date1, unique_ptr<Date>& date2)
{
    unique_ptr<Date> temp(move(date1));
    date1 = move(date2);
    date2 = move(temp);
}

int main()
{
    unique_ptr<Date> today(new Date(22, 05, 2022));
    cout << today->getDay() << endl;
    cout << today->getMonth() << endl;
    cout << today->getYear() << endl;
    cout << *today << endl;
    unique_ptr<Date> date;

    cout << "today is " << (isNull(today) ? "null" : "not null") << endl;
    cout << "date is " << (isNull(date) ? "null" : "not null") << endl;

    date = move(today);

    cout << "today is " << (isNull(today) ? "null" : "not null") << endl;
    cout << "date is " << (isNull(date) ? "null" : "not null") << endl;

    cout << *date << endl;
    unique_ptr<Date> date1(new Date(9, 07, 2020));
    unique_ptr<Date> date2(new Date(10, 07, 2020));
    unique_ptr<Date> date3(new Date(11, 07, 2020));
    cout << *maxDate(date1, date2) << endl;
    swapDates(date2, date3);
    cout << *maxDate(date1, date2) << endl;
}
