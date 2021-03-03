//
// Created by alonm on 15/02/2021.
//

#ifndef ELECTIONS_CITIZEN_H
#define ELECTIONS_CITIZEN_H
#include "date.h"

class citizen {
private:
    string name;
    int id{0};
    int birthYear{0};
    district *district_{nullptr};
    bool vote_{false};

public:
    citizen() = default;

    citizen(const string &name, int id, int birthYear, district *pDistrict) noexcept(false) {set(name, id, birthYear, pDistrict);}

    citizen(const string &name, const string &id, int birthYear, district *pDistrict) noexcept(false) {set(name, stoi(id), birthYear, pDistrict);}

    explicit citizen(istream &in) noexcept(false);

    citizen(const citizen &other) { *this = other; }

    ~citizen() { district_ = nullptr; }

    citizen &operator=(const citizen &other);

    bool operator==(const citizen &other) {
        return (this->name == other.name) && (this->id == other.id) && (this->birthYear == other.birthYear) &&
               (this->district_ == other.district_) && (this->vote_ == other.vote_);
    }

    bool operator!=(const citizen &other) { return !(*this == other); }

    [[nodiscard]] const string & getName() const { return name; }

    [[nodiscard]] int getID() const { return id; }

    [[nodiscard]] int  getBirthYear() const { return birthYear; }

    [[maybe_unused]] [[nodiscard]] bool getVote() const { return vote_; }

    [[nodiscard]] const district * getDistrict() const {return district_;}

    void set(const string &name, int id, int birth_year, district *pDistrict) noexcept(false);

    void setName(const string &name) { this->name = name; }

    void setID(int id) noexcept(false);

    void setBirthYear(int birth_year) noexcept(false);

    void setDistrict(district *pDistrict) noexcept(false);

    void castVote(int pID) noexcept(false);

    bool save(ostream &out) const noexcept(false);

    void load(istream &in) noexcept(false);

    friend ostream &operator<<(ostream &os, const citizen &c);
};

#endif //ELECTIONS_CITIZEN_H
