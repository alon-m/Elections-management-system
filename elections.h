//
// Created by alonm on 15/02/2021.
//

#ifndef ELECTIONS_ELECTIONS_H
#define ELECTIONS_ELECTIONS_H
#include "party.h"

class elections {
private:
    date date_;
    districtList dList;
    citizenList cList;
    partyList pList;
    ElectionsType type_{};

public:
    elections() : type_(ElectionsType::REGULAR) {}

    elections(int day, int month, int year, ElectionsType type) : date_(day, month, year), type_(type) {}

    elections(const elections &other) { *this = other; }

    ~elections();

    //operator overloading
    elections &operator=(const elections &other) {
        if (*this != other) {
            this->date_ = other.date_;
            this->dList = other.dList;
            this->cList = other.cList;
            this->pList = other.pList;
            this->type_ = other.type_;
        }
        return *this;
    }

    bool operator==(const elections &other) {
        return
                ((this->date_ == other.date_) && (this->dList == other.dList)
                && (this->cList == other.cList) && (this->pList == other.pList)
                && (this->type_ == other.type_));
    }

    bool operator!=(const elections &other) { return !(*this == other); }

    //getters
    const districtList& getDistrictList() {return dList;}

    const citizenList& getCitizenList() {return cList;}

    const partyList& getPartyList() {return pList;}

    const date &getDate() { return date_; }

    [[nodiscard]] ElectionsType getType() const { return type_; }

    //setters
    void setType(ElectionsType t) noexcept(false);

    void setDate(int d, int m, int y) {date_.set(d,m,y);}

    void setDate(string& s) {date_.set(s);}

    //district tools
    void addDistrict(const string &name, int electors, int type) noexcept(false);

    void addDistrict(istream &in) noexcept(false);

    void load_district(istream& in);

    //citizen tools
    void addCitizen(const string &name, int citizenID, int birthYear, int districtID) noexcept(false);

    void addCitizen(istream &in) noexcept(false);

    void load_citizen(istream& in);

    //party tools
    void addParty(const string &name, int candidateID) noexcept(false);

    void addParty(istream& in) noexcept(false);

    void printPartyElectionResultsToTerminal();

    partyList & getSortedParties();

    void load_party(istream& in);

    //general tools
    void addRep(int cID, int pID, int dID) noexcept(false);

    void vote(int cID, int pID) noexcept(false);

    void result() noexcept(false);

    bool save(ostream &out) const;

    void load(istream &in);

};


template < class T >
inline std::ostream& operator << (std::ostream& os, const std::vector<T*>& v)
{
    if(typeid(T) == typeid(citizen)){
        os << left << setw(NameWidth) << setfill(Space) << "Name:"
           << left << setw(NumWidth) << setfill(Space) << "ID:"
           << left << setw(NumWidth) << setfill(Space) << "Birth Year:"
           << left << setw(NumWidth) << setfill(Space) << "   district ID:" << endl;
    }
    if(typeid(T) == typeid(district)){
        os << left << setw(NameWidth) << setfill(Space) << "Name:"
           << left << setw(NumWidth) << setfill(Space) << "ID:"
           << left << setw(NumWidth) << setfill(Space) << "Electors:"
           << left << setw(NameWidth) << setfill(Space) << "district Type:" << endl;
    }
    if(typeid(T) == typeid(party)){
        os << left << setw(NameWidth) << setfill(Space) << "Name:"
           << left << setw(NumWidth) << setfill(Space) << "ID:"
           << left << setw(NameWidth) << setfill(Space) << "Candidate name:" << endl;
    }
    for (auto item : v)
        os << *item << endl;
    return os;
}

template<class T> void printAvailableItems(vector<T*> v)
{
    for (auto i : v)
        cout << i->getID() << " : " << i->getName() << endl;
}

#endif //ELECTIONS_ELECTIONS_H
