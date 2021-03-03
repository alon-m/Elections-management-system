//
// Created by alonm on 15/02/2021.
//

#ifndef ELECTIONS_PARTY_H
#define ELECTIONS_PARTY_H
#include "district.h"

class party {
private:
    int id{};
    string name;
    citizen *candidate_{};
    repsList *partyReps_{};
    int amountOfDistricts_{};

    int totalElectorsReceived_{};
    int totalVotesReceived_{};

public:
    party() = default;

    explicit party(istream &in) noexcept(false);

    party(const string &name, const int &pID, citizen *can, const int &districts_amount) noexcept(false);

    ~party();

    party(const party &other) { *this = other; }

    party &operator=(const party &other);

    bool operator==(const party &other) {
        return ((this->name == other.name) && (this->id == other.id) &&
                (this->candidate_ == other.candidate_) && (this->partyReps_ == other.partyReps_) &&
                (this->amountOfDistricts_ == other.amountOfDistricts_) &&
                (this->totalElectorsReceived_ == other.totalElectorsReceived_) &&
                (this->totalVotesReceived_ == other.totalVotesReceived_));
    }

    bool operator!=(const party &other) { return !(*this == other); }

    [[nodiscard]] const string & getName() const { return name; }

    [[nodiscard]] int getID() const { return id; }

    [[nodiscard]] int getAmountOfDistricts() const { return amountOfDistricts_; }

    [[nodiscard]] citizen *getCandidate() const { return candidate_; }

    [[nodiscard]] reps *getReps(int dID) const;

    [[nodiscard]] int getTotalVotes() const { return totalVotesReceived_; }

    [[nodiscard]] int getTotalElectors() const { return totalElectorsReceived_; }


    void set(const string &name, int id, citizen *candidate, int districts_amount) noexcept(false);

    void setID(int id) { this->id = id; }

    void setName(const string &name) { this->name = name; }

    void setCandidate(citizen *can) noexcept(false);

    void incrementVotes(int val = 1) { totalVotesReceived_ += val; }

    void incrementElectors(int val = 1) { totalElectorsReceived_ += val; }

    void resetTotalElectors() { totalElectorsReceived_ = 0; }

    void resetTotalVotes() { totalVotesReceived_ = 0; }

    void addRepsList(reps *r, district *d);

    friend ostream &operator<<(ostream &os, const party &party);

    bool save(ostream &out) const;

    void load(istream &in);
};

#endif //ELECTIONS_PARTY_H
