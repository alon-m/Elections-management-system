//
// Created by alonm on 15/02/2021.
//

#include "reps.h"
#include "party.h"

reps::~reps() // this destructor is intentionally not deleting the citizen itself
{
    district_ = nullptr;
    delete district_;
    party_ = nullptr;
    delete party_;
    for(auto& item : lst)
    {
        item = nullptr;
        delete item;
    }
}

bool reps::save(ostream &out) const {
    int dID = district_->getID();
    int pID = party_->getID();
    out.write((char *) &dID, sizeof(dID));
    out.write((char *) &pID, sizeof(pID));
    int _length = lst.size();
    out.write((char *) &_length, sizeof(_length)); //how many reps
    for(auto& i : lst)
    {
        int cID = i->getID();
        out.write((char *) &cID, sizeof(cID));
    }

    return out.good();
}

void reps::addRep(citizen *c) {
    if (lst.empty())
        party_->addRepsList(this, district_);
    lst.push_back(c);
}

ostream &operator<<(ostream &os, const reps &reps) {
    os << "district id: " << reps.district_->getID() << "\tParty id: " << reps.party_->getID() << endl;
    os << left << setw(NameWidth) << setfill(Space) << "Name:"
       << left << setw(NumWidth) << setfill(Space) << "ID:" << endl;
    for(auto& i : reps.lst)
    {
        os << left << setw(NameWidth) << setfill(Space) << i->getName()
           << left << setw(NumWidth) << setfill(Space) << i->getID()
           << endl;
    }
    return os;
}
