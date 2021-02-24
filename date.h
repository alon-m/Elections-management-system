//
// Created by alonm on 15/02/2021.
//

#ifndef ELECTIONS_DATE_H
#define ELECTIONS_DATE_H


#include "Exceptions.h"

class date {
private:
    int day_{0}, month_{0}, year_{0};
public:
    date() = default;

    date(int day, int month, int year) noexcept(false) { set(day, month, year); }

    [[maybe_unused]] explicit date(string& s) noexcept(false) { set(s); }
    [[maybe_unused]] explicit date(istream &in) noexcept(false) { load(in); }

    date(const date &other) { *this = other; }

    ~date() = default;

    date &operator=(const date &other) {
        if (*this != other) {
            this->day_ = other.day_;
            this->month_ = other.month_;
            this->year_ = other.year_;
        }
        return *this;
    }

    bool operator==(const date &other) const {
        return (this->day_ == other.day_) && (this->month_ == other.month_) && (this->year_ == other.year_);
    }

    bool operator!=(const date &other) const { return !(*this == other); }

    [[nodiscard]] int getDay() const {return day_;}

    [[nodiscard]] int getMonth() const {return month_;}

    [[nodiscard]] int getYear() const {return year_;}

    void set(int d, int m, int y) noexcept(false);

    void set(string &date) noexcept(false);

    void setDay(int val) noexcept(false);

    void setMonth(int val) noexcept(false);

    void setYear(int val) noexcept(false);

    bool save(ostream &out) const;

    void load(istream &in) noexcept(false);

    friend ostream &operator<<(ostream &os, const date &date);
};

#endif //ELECTIONS_DATE_H
