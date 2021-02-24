//
// Created by alonm on 15/02/2021.
//

#include "elections.h"


elections::~elections() {
    for(auto p: pList)
        delete p;
    pList.clear();

    for(auto p: cList)
        delete p;
    cList.clear();

    for(auto p: dList)
        delete p;
    dList.clear();
}

void elections::setType(ElectionsType t) noexcept(false) {
    if (t != ElectionsType::REGULAR && t != ElectionsType::SIMPLE)
        throw (elections_invalid_type(static_cast<int>(t)));
    type_ = t;
}


void elections::addDistrict(const string &name, int electors, int type) noexcept(false) {
    static int index = -1;
    if (electors < 0)
        throw (input_invalid("electors amount must be a positive number"));
    ++index;
    if(findItem<district>(dList, index).exist())
        throw (district_already_exists(index));
    auto t = static_cast<DistrictType>(type);
    district* d = nullptr;
    if (t == DistrictType::UNIFIED)
        d = new district_unified(name, index, electors);
    else if(t == DistrictType::DIVIDED)
        d = new district_divided(name, index, electors);
    else{
        --index;
        delete d;
        throw (district_invalid_type(type));
    }
    if(!d)
        throw(allocation_error("failed allocating a new district"));
    dList.push_back(d);
}

void elections::addCitizen(const string &name, int citizenID, int birthYear, int districtID) noexcept(false) {
    if (date_.getYear() - birthYear < AGE_LIMIT)
        throw (birth_year_ex());
    if (!(citizenID >= 0 && citizenID <= ID_MAX))
        throw (input_invalid("id must be under digits"));
    district* d = findItem<district>(dList, districtID).get();
    auto* c = new citizen(name, citizenID, birthYear, d);
    addSorted<citizen>(cList, c);
    //cList.push_back(c);
}

void elections::addParty(const string &name, int candidateID) noexcept(false) {
    static int index = -1;
    citizen* c = findItem<citizen>(cList, candidateID).get();
    index++;
    if(findItem<party>(pList, index).exist())
        throw (party_already_exists(index));
    auto* p = new party(name, index, c, dList.size());
    pList.push_back(p);
}

void elections::addRep(int cID, int pID, int dID) noexcept(false) {
    citizen* c = findItem<citizen>(cList, cID).get();
    district* d = findItem<district>(dList, dID).get();
    party* p = findItem<party>(pList, pID).get();
    d->addRep(c, p);
}

void elections::vote(int cID, int pID) noexcept(false) {
    try
    {
       citizen* c = findItem<citizen>(cList, cID).get();
       if(!c)
            throw(citizen_not_found(cID));
       if(!(any_of(pList.begin(),pList.end(), compareID<party>(pID))))
           throw(party_not_found(pID));
       c->castVote(pID);
    }
    catch(const not_found& e){e.show(); return;}
}


void elections::result() noexcept(false) {
    if (dList.empty() || pList.empty() || cList.empty())
        throw (my_exceptions("can't process results"));
    for(auto& i : pList){
        i->resetTotalElectors();
        i->resetTotalVotes();
    }
    for(auto& i : dList)
        i->processResults(&pList);
}


bool elections::save(ostream &out) const {
    out.write(rcastcc(&type_), sizeof(type_));
    if(!date_.save(out)) throw(save_process_ex("saving process failed in: Date."));
    int size;
    size = dList.size();
    out.write(rcastcc(&size), sizeof(size));
    for(auto& d : dList)
        if(!d->save(out)) throw(save_process_ex("saving process failed in: District list."));

    size = cList.size();
    out.write(rcastcc(&size), sizeof(size));
    for(auto& c : cList)
        if(!c->save(out)) throw(save_process_ex("saving process failed in: Citizen list."));

    size = pList.size();
    out.write(rcastcc(&size), sizeof(size));
    for(auto& p : pList)
        if(!p->save(out)) throw(save_process_ex("saving process failed in: Party list."));

    size = dList.size();
    out.write(rcastcc(&size), sizeof(size));
    for(auto& d: dList)
        if(!d->saveReps(out)) throw(save_process_ex("saving process failed in: Reps lists."));

    return out.good();
}


void elections::load(istream &in) {
    //elections type loading
    ElectionsType type;
    in.read(rcastc(&type), sizeof(type));
    setType(type);
    int size;

    //date loading
    date_.load(in);

    //districts loading
    in.read(rcastc(&size), sizeof(size));
    if (size < 0)
        throw (load_process_ex("invalid data received for district list size (size < 0)"));
    for (int i = 0; i < size; i++)
        load_district(in);

    //citizens loading
    in.read(rcastc(&size), sizeof(size));
    if (size < 0)
        throw (input_invalid("invalid data received when loading from file"));
    for (int i = 0; i < size; i++)
        load_citizen(in);

    //Plist loading
    in.read(rcastc(&size), sizeof(size));
    if (size < 0)
        throw (input_invalid("invalid data received when loading from file"));
    for (int i = 0; i < size; i++)
        load_party(in);

    //reps_list loading
    int amountOfDistricts, amountOfRepsListsInDistrict, cID, dID, pID;
    in.read(rcastc(&amountOfDistricts), sizeof(amountOfDistricts));
    if (amountOfDistricts < 0)
        throw (input_invalid("invalid data received when loading from file"));
    for (int i = 0; i < amountOfDistricts; i++) {
        in.read(rcastc(&amountOfRepsListsInDistrict), sizeof(amountOfRepsListsInDistrict));
        if (amountOfRepsListsInDistrict < 0)
            throw (input_invalid("invalid data received when loading from file"));
        for (int j = 0; j < amountOfRepsListsInDistrict; j++) {
            in.read(rcastc(&pID), sizeof(pID));
            in.read(rcastc(&dID), sizeof(dID));
            in.read(rcastc(&size), sizeof(size));
            if (size < 0)
                throw (input_invalid("invalid data received when loading from file"));
            for (int k = 0; k < size; k++) {
                in.read(rcastc(&cID), sizeof(cID));
                addRep(cID, pID, dID);
            }
        }
    }

}

void elections::addCitizen(istream &in) noexcept(false) {
    try {
        auto* c = new citizen(in);
        if (!c)
            throw(allocation_error("error allocating a new citizen while loading from file."));
        cList.push_back(c);
    }
    catch (const allocation_error &e) {
        e.show();
        exit(ALLOCATION_ERROR);
    }
    catch (const my_exceptions &e) {
        cout << "error while loading from file:\n";
        e.show();
        exit(FILE_HANDLING_ERROR);
    }

}

void elections::load_district(istream &in) {
    addDistrict(in);
}

void elections::load_citizen(istream &in) {
    int dID, cID, pID;
    addCitizen(in);
    in.read(rcastc(&dID), sizeof(dID)); //districtID
    district *d = findItem<district>(dList, dID).get();
    if (!d)
        throw (nullptr_ex("error getting a district pointer"));
    cList.back()->setDistrict(d);
}

void elections::load_party(istream &in) {
    int cID;
    addParty(in); //loading party without candidate and partyReps pointers
    in.read(rcastc(&cID), sizeof(cID)); //candidate's ID
    citizen *c = findItem<citizen>(cList, cID).get();
    pList.back()->setCandidate(c); //now party has a pointer to it's candidate
    //reps loading will happen later
}

void elections::addDistrict(istream &in) noexcept(false) {
    district *d;
    DistrictType typeOfDistrict;
    in.read(rcastc(&typeOfDistrict), sizeof(typeOfDistrict));
    if (typeOfDistrict == DistrictType::UNIFIED)
        d = new district_unified(in);
    else if (typeOfDistrict == DistrictType::DIVIDED)
        d = new district_divided(in);
    else
        throw (district_invalid_type(static_cast<int>(typeOfDistrict)));
    if(!d)
        throw(nullptr_ex());
    dList.push_back(d);
}

void elections::addParty(istream &in) noexcept(false) {
    try {
        party * p = new party(in);
        if (!p)
            throw (allocation_error("a new party couldn't be allocated while loading from file"));
        pList.push_back(p);
    }
    catch (const allocation_error &e) {
        e.show();
        exit(ALLOCATION_ERROR);
    }
    catch (const my_exceptions &e) {
        cout << "error while loading from file:\n";
        e.show();
        exit(FILE_HANDLING_ERROR);
    }
}

partyList & elections::getSortedParties() {
    partyList* sorted = new partyList;
    sorted->resize(pList.size());
    for (int i = 0; i < pList.size(); ++i)
        (*sorted)[i] = pList[i];
    std::sort((*sorted).begin(), (*sorted).end(),
              [](party *a, party *b) -> bool {
                  if (a->getTotalElectors() > b->getTotalElectors())
                      return true;
                  else
                      return false;
              });
    return *sorted;
}

void elections::printPartyElectionResultsToTerminal() {
    partyList& sortedParties = getSortedParties();
    //Printing candidates sorted by their received electors
    for (auto &i : sortedParties) {
        if (i == *sortedParties.begin())
            cout << "THE WINNING PARTY IS:" << endl;
        cout << "PARTY: " << i->getName() << "\tWITH THE CANDIDATE: "
             << i->getCandidate()->getName() << endl;
        cout << "TOTAL ELECTORS RECEIVED: " << i->getTotalElectors() << "\tTOTAL VOTES RECEIVED: "
             << i->getTotalVotes() << "\n\n";
    }
}


