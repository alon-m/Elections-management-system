//
// Created by alonm on 15/02/2021.
//

#include "party.h"

party::party(istream &in) noexcept(false) {
    load(in);
    partyReps_ = new repsList;
}

party::party(const string &name, const int &pID, citizen *can, const int &districts_amount) noexcept(false) {
    set(name, pID, can, districts_amount);
    totalElectorsReceived_ = totalVotesReceived_ = 0;
}

party::~party() {
    candidate_ = nullptr;
    delete partyReps_;
}

party &party::operator=(const party &other) {
    if (*this != other) {
        this->name_ = other.name_;
        this->id_ = other.id_;
        this->candidate_ = other.candidate_;
        this->partyReps_ = other.partyReps_;
        this->amountOfDistricts_ = other.amountOfDistricts_;
        this->totalElectorsReceived_ = other.totalElectorsReceived_;
        this->totalVotesReceived_ = other.totalVotesReceived_;
    }
    return *this;
}


void party::set(const string &name, int id, citizen *candidate, int districts_amount) noexcept(false) {
    setCandidate(candidate);
    setName(name);
    setID(id);
    amountOfDistricts_ = districts_amount;
    partyReps_ = new repsList;
}

void party::setCandidate(citizen *can) noexcept(false) {
    if (!can)
        throw (nullptr_ex());
    candidate_ = can;
}


reps *party::getReps(int dID) const {
    return (*partyReps_)[dID];
}

void party::addRepsList(reps *r, district *d) {
    if (amountOfDistricts_ <= d->getID())
        amountOfDistricts_ = d->getID() + 1;

    int dID = d->getID();
    if (partyReps_->empty())
    {
        partyReps_->push_back(r);
        return;
    }
    auto begin = (*partyReps_).begin();
    auto end = (*partyReps_).end();
    auto location = find_if(begin, end,
        [&](reps* item) {return (item->getDistrict()->getID() > dID); });
    partyReps_->insert(location, r);
}

ostream &operator<<(ostream &os, const party &party) {

    os << left << setw(NameWidth) << setfill(Space) << party.getName()
       << left << setw(NumWidth) << setfill(Space) << party.getID()
       << left << setw(NameWidth) << setfill(Space) << party.candidate_->getName() << endl;
    os << "PRINTING REPRESENTATIVES OF THIS PARTY IN ALL DISTRICTS:" << "\n\n";
    for(auto& i: *(party.partyReps_))
        os << *i;
    cout << endl;
    return os;
}


bool party::save(ostream &out) const {

    int size = name_.size();
    out.write(rcastcc(&size), sizeof(size));
    out << name_;

    out.write(rcastcc(&id_), sizeof(id_));
    out.write(rcastcc(&amountOfDistricts_), sizeof(amountOfDistricts_));
    out.write(rcastcc(&totalElectorsReceived_), sizeof(totalElectorsReceived_));
    out.write(rcastcc(&totalVotesReceived_), sizeof(totalVotesReceived_));

    int citizenID = candidate_->getID();                 //
    out.write(rcastcc(&citizenID), sizeof(citizenID));  // saving only the ID of the candidate

    return out.good();
}

void party::load(istream &in) {
    int size;
    string name;
    in.read(rcastc(&size), sizeof(size));
    if (size < 0)
        throw (input_invalid("invalid data received when loading from file"));
    in.read(&name[0], size);
    setName(name);

    in.read(rcastc(&id_), sizeof(id_));
    if (id_ < 0)
        throw (input_invalid("invalid data received when loading from file"));
    in.read(rcastc(&amountOfDistricts_), sizeof(amountOfDistricts_));
    if (amountOfDistricts_ < 0)
        throw (input_invalid("invalid data received when loading from file"));
    in.read(rcastc(&totalElectorsReceived_), sizeof(totalElectorsReceived_));
    in.read(rcastc(&totalVotesReceived_), sizeof(totalVotesReceived_));
}

