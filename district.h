//
// Created by alonm on 15/02/2021.
//

#ifndef ELECTIONS_DISTRICT_H
#define ELECTIONS_DISTRICT_H
#include "result.h"

class district {
protected:
    string name;
    int id{};
    int electors_{};
    int citizens_ = 0;
    result *result_{};
    DistrictType type_{DistrictType::UNDEFINED};

public:
    district();

    district(const string &name, int id, int electors) noexcept(false);

    district(const string &name, int id, int electors, DistrictType type) noexcept(false);

    district(const district &other) { *this = other; }

    explicit district(istream &in) noexcept(false);

    virtual ~district();

    district &operator=(const district &other);

    bool operator==(const district &other) {
        return (this->name == other.name) && (this->id == other.id) &&
               (this->electors_ == other.electors_) && (this->result_ == other.result_)
               && (this->type_ == other.type_) && (this->citizens_ == other.citizens_);
    }

    bool operator!=(const district &other) { return !(*this == other); }

    [[maybe_unused]] [[nodiscard]] DistrictType getType() const { return type_; }

    [[nodiscard]] virtual string getTypeName() const { return "Unknown"; }

    [[nodiscard]] const string & getName() const { return name; }

    [[nodiscard]] int getID() const { return id; }

    [[nodiscard]] int getElectorsAmount() const { return electors_; }

    [[nodiscard]] const result *getResult() { return result_; }

    void set(const string &name, int id, int electors) noexcept(false);

    void setName(const string& name) { this->name = name; }

    void setID(int id) { this->id = id; }

    void setElectors(int val) noexcept(false);

    void setType(DistrictType type) noexcept(false);

    void addRep(citizen *c, party *p);

    void incrementCitizens(int val = 1) { citizens_+=val;}

    void addVote(int pID);

    void processResults(vector<party*> *Plist);

    void determineVotesPercentages();

    virtual void determineElectorsPerParty(int votesPerElector) {}

    void initElectorsPerParty();

    void resizePartyArrays(int p_id);

    friend ostream &operator<<(ostream &os, const district &district);

    bool save(ostream &out) const;

    bool saveReps(ostream &out) const;

    void load(istream &in);
};


class district_unified : public district {
public:
    district_unified(const string& name, const int &id, const int &electors)
    : district(name, id, electors, DistrictType::UNIFIED) {}

    district_unified() { type_ = DistrictType::UNIFIED;}

    explicit district_unified(istream &in) : district(in) { type_ = DistrictType::UNIFIED;}

    ~district_unified() override = default;

    [[nodiscard]] string getTypeName() const override  { return "Unified"; }

    void determineElectorsPerParty(int votesPerElector) override;
};

class district_divided : public district {
public:
    district_divided(const string &name, const int &id, const int &electors)
    : district(name, id, electors,DistrictType::DIVIDED) {}

    district_divided() { type_ = DistrictType::DIVIDED; }

    explicit district_divided(istream &in) : district(in) { type_ = DistrictType::DIVIDED; }

    ~district_divided() override = default;

    [[nodiscard]] string getTypeName() const override  { return "Divided"; }

    void determineElectorsPerParty(int votesPerElector) override;
};

#endif //ELECTIONS_DISTRICT_H
