//
// Created by alonm on 15/02/2021.
//
#include "party.h"

district::district() : id(-1), electors_(0) {
    result_ = new result(this);
}

district::district(istream &in) {
    load(in);
}

district::district(const string &name, int id, int electors) noexcept(false) {
    set(name, id, electors);
    result_ = new result(this);
}

district::district(const string &name, int id, int electors, DistrictType type) noexcept(false) {
    set(name, id, electors);
    setType(type);
    result_ = new result(this);
}

district::~district() {
    delete result_;
}

district &district::operator=(const district &other) {
    if (*this != other) {
        this->name = other.name;
        this->id = other.id;
        this->type_ = other.type_;
        this->electors_ = other.electors_;
        this->result_ = other.result_;
    }
    return *this;
}


void district::set(const string &name, int id, int electors) noexcept(false) {
    setName(name);
    setID(id);
    setElectors(electors);
}


void district::setElectors(int val) noexcept(false) {
    if (val < 0)
        throw (input_invalid("electors amount must be a positive number"));
    electors_ = val;
}

void district::setType(DistrictType type) noexcept(false) {
    if (type != DistrictType::UNIFIED && type != DistrictType::DIVIDED)
        throw (district_invalid_type(static_cast<int>(type)));
    this->type_ = type;
}

void district::addRep(citizen *c, party *p) {
    resizePartyArrays(p->getID());
    if (result_->_partyReps->empty())
    {
        reps* r = new reps(this, p);
        r->addRep(c);
        result_->addRepsList(r, p);
    }
    else
    {
        auto lst = result_->_partyReps;
        auto begin = lst->begin();
        auto end = lst->end();
        auto location = find_if(result_->_partyReps->begin(), result_->_partyReps->end(),
            [&](reps* item) {return (item->getParty() == p && item->getDistrict() == this); });
        if (location == end)
        {
            reps* r = new reps(this, p);
            r->addRep(c);
            result_->addRepsList(r, p);
        }
        else
            (*location)->addRep(c);
    }
}

void district::addVote(int pID) {
    resizePartyArrays(pID);
    if (result_->_sumOfVotes == 0)
        for (int i = 0; i < result_->_amountOfParties; i++)
            result_->_votes[i] = 0;
    result_->_votes[pID] += 1;
    result_->_sumOfVotes++;
}

void district::resizePartyArrays(int p_id) {
    int val = p_id + 1;
    if (result_->_amountOfParties == 0) {
        result_->_votes.resize(val);
        fill(result_->_votes.begin(), result_->_votes.end(), 0);
        result_->_amountOfParties = val;
    }
    if (val > result_->_amountOfParties) {
        result_->_votes.resize(val);
        for (int i = result_->_amountOfParties; i < val; i++) //set newly created lists and vote buckets
            result_->_votes[i] = 0;
        result_->_amountOfParties = val;
    }
}


void district::initElectorsPerParty() {
    result_->_electorsPerParty.resize(result_->_amountOfParties);
    fill(result_->_electorsPerParty.begin(), result_->_electorsPerParty.end(), 0);
}

void district::determineVotesPercentages() {
    result_->_votesPercentage.clear();
    result_->_votesPercentage.resize(result_->_amountOfParties);
    for (int i = 0; i < result_->_amountOfParties; i++)
        result_->_votesPercentage[i] = static_cast<float>(result_->_votes[i] * 100) / static_cast<float>(result_->_sumOfVotes);
}


ostream &operator<<(ostream &os, const district &district) {
    os << left << setw(NameWidth) << setfill(Space) << district.getName()
       << left << setw(NumWidth) << setfill(Space) << district.getID()
       << left << setw(NumWidth) << setfill(Space) << district.getElectorsAmount()
       << left << setw(NameWidth) << setfill(Space) << district.getTypeName();
    return os;
}


void district::processResults(vector<party*> *Plist) {
    result_->_winningParty = distance(result_->_votes.begin(), max_element(result_->_votes.begin(), result_->_votes.end()));
    result_->_percentage = static_cast<float>(result_->_sumOfVotes * 100) / static_cast<float>(citizens_);
    determineVotesPercentages();
    int votesPerElector = result_->_sumOfVotes / electors_;
    determineElectorsPerParty(votesPerElector);
    for(auto& i : *Plist)
    {
        auto id = i->getID();
        if (id < result_->_amountOfParties) {
            i->incrementElectors(result_->_electorsPerParty[id]);
            i->incrementVotes(result_->_votes[id]);
        }
    }
}


bool district::saveReps(ostream &out) const {
    return result_->saveReps(out);
}

bool district::save(ostream &out) const {
    out.write(rcastcc(&type_), sizeof(type_));  // saving district type

    int size = name.size();
    out.write(rcastc(&size), sizeof(size));
    out << name;

    out.write(rcastcc(&id), sizeof(id));
    out.write(rcastcc(&electors_), sizeof(electors_));
    out.write(rcastcc(&citizens_), sizeof(citizens_));


    return result_->save(out);  //saving all show_results information without reps list
}

void district::load(istream &in) {
    int nameSize, id, electors, citizens;
    string name;
    in.read(rcastc(&nameSize), sizeof(nameSize));
    if (nameSize < 0)
        throw (input_invalid("district name size data received is invalid. (size < 0)."));
    name.resize(nameSize);
    in.read(&name[0], nameSize);
    setName(name);
    in.read(rcastc(&id), sizeof(id));
    if (id < 0)
        throw (input_invalid("district id read from file is invalid. (id < 0)"));
    setID(id);
    in.read(rcastc(&electors), sizeof(electors));
    setElectors(electors);
    in.read(rcastc(&citizens), sizeof(citizens));
    incrementCitizens(citizens);

    result_ = new result(this);
    if (!result_)
        throw(allocation_error("error allocating results for a district."));
    result_->load(in);
}

void district_unified::determineElectorsPerParty(int votesPerElector) {
    initElectorsPerParty();
    int i = max_element(result_->_votes.begin(), result_->_votes.end()) - result_->_votes.begin();
    result_->_electorsPerParty[i] = electors_;
}


void district_divided::determineElectorsPerParty(int votesPerElector) {
    initElectorsPerParty();
    vector<float> votesTemp;
    votesTemp.resize(result_->_amountOfParties);
    for (int i = 0; i < result_->_amountOfParties; i++)
        votesTemp[i] = static_cast<float>(result_->_votes[i]);
    int electorsGiven = 0;

    //first round of electors
    for (int i = 0; i < result_->_amountOfParties; i++) {
        if (votesTemp[i] < votesPerElector) //if a party didn't pass the minimal number of votes required
            votesTemp[i] = 0;
        result_->_electorsPerParty[i] += static_cast<int>(votesTemp[i] / votesPerElector);
        electorsGiven += result_->_electorsPerParty[i];
        votesTemp[i] /= votesPerElector;
    }

    //remaining electors to be given
    while (electorsGiven < electors_) {
        int i = max_element(votesTemp.begin(), votesTemp.end()) - votesTemp.begin();
        result_->_electorsPerParty[i] += 1;
        electorsGiven++;
        votesTemp[i] = (-1);
    }
}

