//
// Created by alonm on 15/02/2021.
//

#include "district.h"

citizen::citizen(istream &in) {
    load(in);
    district_ = nullptr;
}

citizen &citizen::operator=(const citizen &other) {

    if (*this != other) {
        this->name_ = other.name_;
        this->id_ = other.id_;
        this->birthYear_ = other.birthYear_;
        delete this->district_;
        this->district_ = other.district_;
    }
    return *this;
}

void citizen::set(const string &name, int id, int birth_year, district *pDistrict) noexcept(false) {
    setName(name);
    setID(id);
    setBirthYear(birth_year);
    setDistrict(pDistrict);
}

void citizen::setID(int id) noexcept(false) {
    if (!(id >= 0 && id <= ID_MAX))
        throw (input_invalid("citizen's id can't be longer than 9 digits"));
    this->id_ = id;
}

void citizen::setBirthYear(int birth_year) noexcept(false) {
    if (birth_year < 0)
        throw (input_invalid("birth year must be a positive number"));
    this->birthYear_ = birth_year;
}

void citizen::setDistrict(district *pDistrict) noexcept(false) {
    if (!pDistrict)
        throw (nullptr_ex());
    this->district_ = pDistrict;
    district_->incrementCitizens();
}

void citizen::castVote(const int pID) noexcept(false) {
    if (vote_)
        throw (citizen_already_voted());
    vote_ = true;
    district_->addVote(pID);
}

bool citizen::save(ostream &out) const {

    int size = name_.size();
    out.write(rcastcc(&size), sizeof(size));
    out << name_;

    out.write(rcastcc(&id_), sizeof(id_));
    out.write(rcastcc(&birthYear_), sizeof(birthYear_));
    out.write(rcastcc(&vote_), sizeof(vote_));

    int districtID = district_->getID();
    out.write(rcastcc(&districtID), sizeof(districtID));

    return out.good();
}

void citizen::load(istream &in) {
    int nameSize, id, birthYear;
    string name;
    in.read(rcastc(&nameSize), sizeof(nameSize));
    if (nameSize < 0)
        throw (input_invalid("invalid data received when loading from file"));
    name.resize(nameSize);
    in.read(&name[0], nameSize);
    setName(name);


    in.read(rcastc(&id), sizeof(id));
    setID(id);
    in.read(rcastc(&birthYear), sizeof(birthYear));
    setBirthYear(birthYear);
    in.read(rcastc(&this->vote_), sizeof(this->vote_));
}


ostream &operator<<(ostream &os, const citizen &c) {
    os << left << setw(NameWidth) << setfill(Space) << c.getName()
       << left << setw(NumWidth) << setfill(Space) << c.getID()
       << left << setw(NumWidth) << setfill(Space) << c.getBirthYear()
       << left << setw(NumWidth) << setfill(Space) << "   " << c.getDistrict()->getID();
    return os;
}