//
// Created by alonm on 15/02/2021.
//

#include "result.h"
#include "district.h"
#include "party.h"

result::result(district *d) {
    _parent = d;
    _partyReps = new repsList;
    _amountOfParties = 0;
    _sumOfVotes = 0;
    _percentage = 0;
    _winningParty = -1;
}

result::~result() {
    _parent = nullptr;
    delete _partyReps;
}

bool result::operator==(const result &other) const {
    return (this->_parent == other._parent) && (this->_amountOfParties == other._amountOfParties) &&
           (this->_sumOfVotes == other._sumOfVotes) &&
           (this->_percentage == other._percentage) && (this->_winningParty == other._winningParty) &&
           (this->_votes == other._votes)
           && (this->_electorsPerParty == other._electorsPerParty) &&
           (this->_votesPercentage == other._votesPercentage) && (this->_partyReps == other._partyReps);
}

result &result::operator=(const result &other) {
    if (*this != other) {
        this->_parent = other._parent;
        this->_amountOfParties = other._amountOfParties;
        this->_sumOfVotes = other._sumOfVotes;
        this->_percentage = other._percentage;
        this->_winningParty = other._winningParty;
        this->_votes = other._votes;
        this->_electorsPerParty = other._electorsPerParty;
        this->_votesPercentage = other._votesPercentage;
        this->_partyReps = other._partyReps;
    }
    return *this;
}

bool result::saveReps(ostream &out) const
{
    int length = _partyReps->size();
    out.write(rcastcc(&length), sizeof(length));

    for (auto& item : *_partyReps)
    {
        int pID = item->getParty()->getID();
        int dID = item->getDistrict()->getID();
        out.write(rcastcc(&pID), sizeof(pID));
        out.write(rcastcc(&dID), sizeof(dID));
        const citizenList& lst = item->getList();
        length = lst.size();
        out.write(rcastcc(&length), sizeof(length));
        for(auto& c : lst)
        {
            int cID = c->getID();
            out.write(rcastcc(&cID), sizeof(cID));
        }
    }
    return out.good();
}

bool result::save(ostream &out) const {
    int size = 0;
    out.write(rcastcc(&_amountOfParties), sizeof(_amountOfParties));
    out.write(rcastcc(&_sumOfVotes), sizeof(_sumOfVotes));
    out.write(rcastcc(&_percentage), sizeof(_percentage));
    out.write(rcastcc(&_winningParty), sizeof(_winningParty));

    size = _votes.size();
    out.write(rcastcc(&size), sizeof(size));
    for (int i = 0; i < size; i++)
        out.write(rcastcc(&(_votes[i])), sizeof(_votes[i]));

    size = _votesPercentage.size();
    out.write(rcastcc(&size), sizeof(size));
    for (int i = 0; i < size; i++)
        out.write(rcastcc(&(_votesPercentage[i])), sizeof(_votesPercentage[i]));

    size = _electorsPerParty.size();
    out.write(rcastcc(&size), sizeof(size));
    for (int i = 0; i < size; i++)
        out.write(rcastcc(&(_electorsPerParty[i])), sizeof(_electorsPerParty[i]));
    return out.good();
}

void result::load(istream &in) {
    int tmpSize;
    in.read(rcastc(&_amountOfParties), sizeof(_amountOfParties));
    if (_amountOfParties < 0)
        throw (input_invalid("invalid data received when loading from file"));
    in.read(rcastc(&_sumOfVotes), sizeof(_sumOfVotes));
    if (_sumOfVotes < 0)
        throw (input_invalid("invalid data received when loading from file"));
    in.read(rcastc(&_percentage), sizeof(_percentage));
    if (_percentage < 0)
        throw (input_invalid("invalid data received when loading from file"));
    in.read(rcastc(&_winningParty), sizeof(_winningParty));
    if (_winningParty < -1 || _winningParty > _amountOfParties)
        throw (input_invalid("invalid data received when loading from file"));

    in.read(rcastc(&tmpSize), sizeof(tmpSize));
    if (tmpSize < 0)
        throw (input_invalid("invalid data received when loading from file"));
    _votes.resize(tmpSize);
    for (int i = 0; i < tmpSize; i++)
        in.read(rcastc(&(_votes[i])), sizeof(_votes[i]));

    in.read(rcastc(&tmpSize), sizeof(tmpSize));
    if (tmpSize < 0)
        throw (input_invalid("invalid data received when loading from file"));
    _votesPercentage.resize(tmpSize);
    for (int i = 0; i < tmpSize; i++)
        in.read(rcastc(&(_votesPercentage[i])), sizeof(_votesPercentage[i]));

    in.read(rcastc(&tmpSize), sizeof(tmpSize));
    if (tmpSize < 0)
        throw (input_invalid("invalid data received when loading from file"));
    _electorsPerParty.resize(tmpSize);
    for (int i = 0; i < tmpSize; i++)
        in.read(rcastc(&(_electorsPerParty[i])), sizeof(_electorsPerParty[i]));
}

ostream &operator<<(ostream &os, const result &result) {
    const string typeName = result._parent->getTypeName();
    party* winningParty = find_if(result._partyReps->begin(), result._partyReps->end(),
        [&](reps* r) {return r->getParty()->getID() == result._winningParty; }).operator*()->getParty();
    os << "SHOWING RESULTS FOR DISTRICT: " << result._parent->getName() << endl;
    os << left << setw(NameWidth) << setfill(Space) << "ID:"
       << left << setw(NameWidth) << setfill(Space) << "DISTRICT TYPE:"
       << left << setw(NameWidth) << setfill(Space) << "ELECTORS:"
       << left << setw(NameWidth) << setfill(Space) << "TOTAL VOTES:"
       << left << setw(NameWidth) << setfill(Space) << "PERCENTAGE OF VOTERS:"
       << left << setw(NameWidth) << setfill(Space) << "   WINNING PARTY:"
       << left << setw(NameWidth) << setfill(Space) << "ELECTED CANDIDATE:" << endl;

    os << left << setw(NameWidth) << setfill(Space) << result._parent->getID()
       << left << setw(NameWidth) << setfill(Space) << result._parent->getTypeName()
       << left << setw(NameWidth) << setfill(Space) << result._parent->getElectorsAmount()
       << left << setw(NameWidth) << setfill(Space) << result._sumOfVotes
       << left << setw(NameWidth) << setfill(Space) << result._percentage
       << left << setw(NameWidth) << setfill(Space) << winningParty->getName()
       << left << setw(NameWidth) << setfill(Space) << winningParty->getCandidate()->getName() << endl << endl;

    //printing the elected representatives for each party acknowledged by the district
    // (a district may not know of a certain party if a representative was never set there. or if a vote to that party was never cast)
    for(auto& i : *result._partyReps)
    {
        const party *p = i->getParty();
        int pID = p->getID();

        os << left << setw(NameWidth) << setfill(Space) << "PARTY:"
           << left << setw(NameWidth) << setfill(Space) << "ID:"
           << left << setw(NameWidth) << setfill(Space) << "VOTES RECEIVED:"
           << left << setw(NameWidth) << setfill(Space) << "PERCENTAGE OF VOTERS:"
           << left << setw(NameWidth) << setfill(Space) << "   TOTAL ELECTORS IN DISTRICT:" << endl;

        os << left << setw(NameWidth) << setfill(Space) << p->getName()
           << left << setw(NameWidth) << setfill(Space) << pID
           << left << setw(NameWidth) << setfill(Space) << result._votes[pID]
           << left << setw(NameWidth) << setfill(Space) << result._votesPercentage[pID]
           << left << setw(NameWidth) << setfill(Space) << "   " << result._electorsPerParty[pID] << endl << endl;

        os << "ELECTED REPRESENTATIVES:";
        int k = 0;
        const citizenList& lst = (*result._partyReps)[pID]->getList();
        for(auto& j: lst)
        {
            if(k >= result._electorsPerParty[pID])
                break;
            if (k % 3 == 0)
                os << endl;
            os << j->getName() << ";\t";
            k++;
        }
        os << "\n\n";
    }
    return os;
}

void result::addRepsList(reps *r, party *p) const
{
    int pID = p->getID();
    if (_partyReps->empty())
    {
        _partyReps->push_back(r);
        return;
    }
    auto begin = (*_partyReps).begin();
    auto end = (*_partyReps).end();
    auto location = find_if(begin, end,
        [&](reps* item) {return (item->getParty()->getID() > pID); });
    _partyReps->insert(location, r);
}

void result::addRep(citizen *c, party *p) const {
    auto pID = p->getID();
    for(auto & item : *_partyReps)
    {
        auto currID = item->getParty()->getID();
        if(currID == pID){
            item->addRep(c);
            return;
        }
    }
}
