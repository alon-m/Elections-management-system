//
// Created by alonm on 15/02/2021.
//

#ifndef ELECTIONS_RESULT_H
#define ELECTIONS_RESULT_H

#include "reps.h"

class result {
public:
    district *_parent{};
    int _amountOfParties{};
    int _sumOfVotes{};
    float _percentage{};
    int _winningParty{};

    vector<int> _votes;
    vector<int> _electorsPerParty;
    vector<float> _votesPercentage;

    repsList *_partyReps{};

    result(const result &other) { *this = other; }

    explicit result(district *d);

    ~result();

    result &operator=(const result &other);

    bool operator==(const result &other) const;

    bool operator!=(const result &other) const { return !(*this == other); }

    void addRepsList(reps* r, party* p) const;

    void addRep(citizen* c, party* p) const;

    bool saveReps(ostream &out) const;

    bool save(ostream &out) const;

    void load(istream &in);

    friend ostream &operator<<(ostream &os, const result &result);
};

#endif //ELECTIONS_RESULT_H
