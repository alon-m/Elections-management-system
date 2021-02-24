//
// Created by alonm on 15/02/2021.
//

#ifndef ELECTIONS_REPS_H
#define ELECTIONS_REPS_H

#include "citizen.h"

class reps {
protected:
    district *district_;
    party *party_;
    citizenList lst;
public:
    reps(district *d, party *p) noexcept(false): district_(d), party_(p) {}

    ~reps();

    [[nodiscard]] party * getParty() const { return party_; }

    [[nodiscard]] district * getDistrict() const { return district_; }

    [[nodiscard]] const citizenList& getList() const {return lst;}

    void addRep(citizen* r);

    bool save(ostream &out) const;

    bool operator==(const reps &other)
    {
        return (this->district_ == other.district_) && (this->party_ == other.party_) && (this->lst == other.lst);
    }

    bool operator!=(const reps &other) { return !(*this == other); }

    friend ostream &operator<<(ostream &os, const reps &reps);
};


#endif //ELECTIONS_REPS_H
