//
// Created by alonm on 15/02/2021.
//

#include "date.h"

void date::set(int d, int m, int y) noexcept(false) {
    setDay(d);
    setMonth(m);
    setYear(y);
}

void date::set(string &date) noexcept(false) {
    int d, m, y;
    replace_if(date.begin(), date.end(), [](char c) -> bool { return !isdigit(c); }, ' ');
    if (all_of(date.begin(), date.end(), isspace))
        throw input_invalid("invalid date input received");
    stringstream(date) >> d >> m >> y;
    set(d,m,y);
}

void date::setDay(int val) noexcept(false) {
    if (val < 1 || val > 31)
        throw input_invalid("day must be between 1 and 31");
    day_ = val;
}

void date::setMonth(int val) noexcept(false) {
    if (val < 1 || val > 12)
        throw input_invalid("month must be between 1 and 12");
    month_ = val;
}

void date::setYear(int val) noexcept(false) {
    if (year_ < 0)
        throw input_invalid("year must be a positive number");
    year_ = val;
}

ostream &operator<<(ostream &os, const date &date) {
    os << date.getDay() << "/" << date.getMonth() << "/" << date.getYear() << endl;
    return os;
}

bool date::save(ostream &out) const {
    out.write(rcastcc(&day_), sizeof(day_));
    out.write(rcastcc(&month_), sizeof(month_));
    out.write(rcastcc(&year_), sizeof(year_));
    return out.good();
}

void date::load(istream &in) noexcept(false) {
    int d, m, y;
    in.read(rcastc(&d), sizeof(d));
    in.read(rcastc(&m), sizeof(m));
    in.read(rcastc(&y), sizeof(y));
    set(d, m, y);
}