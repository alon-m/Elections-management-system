//
// Created by alonm on 15/02/2021.
//

#ifndef ELECTIONS_EXCEPTIONS_H
#define ELECTIONS_EXCEPTIONS_H
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>
using namespace std;

const int AGE_LIMIT = 18;
const int ID_MAX = 999999999;
const char Space = ' ';
const int NameWidth = 20;
const int NumWidth = 10;

class district;// forward declaration
class citizen;
class party;
class reps;

typedef vector<district*> districtList;
typedef vector<citizen*> citizenList;
typedef vector<party*> partyList;
typedef vector<reps*> repsList;

enum class ElectionsType {
    REGULAR = 1, SIMPLE
};

enum ErrorReport {
    UNHANDLED_EXCEPTION, FILE_HANDLING_ERROR, ALLOCATION_ERROR
};

enum class DistrictType {
    UNDEFINED, UNIFIED, DIVIDED
};

class my_exceptions : public exception {
protected:
    string _msg;
public:
    explicit my_exceptions(string msg) : _msg(std::move(msg)) {}

    my_exceptions() : _msg("unknown exception") {}

    virtual void show() const { cout << _msg << endl; };
};

class allocation_error : public my_exceptions {
public:
    explicit allocation_error(string msg) : my_exceptions(std::move(msg)) {}

    allocation_error() : my_exceptions("allocation error occurred. exiting") {}
};

class input_invalid : public my_exceptions {
public:
    explicit input_invalid(string msg) : my_exceptions(std::move(msg)) {}
};

class nullptr_ex : public my_exceptions {
public:
    nullptr_ex() : my_exceptions("null pointer received exception") {}

    explicit nullptr_ex(string msg) : my_exceptions(std::move(msg)) {}
};

class citizen_already_voted : public my_exceptions {
public:
    citizen_already_voted() : my_exceptions("citizen already voted") {}
};

class invalid_menu_option : public my_exceptions {
public:
    invalid_menu_option() : my_exceptions("This is not a valid menu option") {}
};

class not_found : public my_exceptions {
protected:
    int input;
public:
    explicit not_found(int val) : input(val) {}

    void show() const override { cout << "item was not found" << endl; }
};

class district_not_found : public not_found {

public:
    explicit district_not_found(int v) : not_found(v) {}

    void show() const override {
        cout << "a district with the ID: " << "'" << input << "'" << " could not be found" << endl;
    }
};

class party_not_found : public not_found {
public:
    explicit party_not_found(int v) : not_found(v) {}

    void show() const override {
        cout << "a party with the ID: " << "'" << input << "'" << " could not be found" << endl;
    }
};

class citizen_not_found : public not_found {
public:
    explicit citizen_not_found(int v) : not_found(v) {}

    void show() const override {
        cout << "a citizen with the ID: " << "'" << input << "'" << " could not be found" << endl;
    }
};

class already_exists : public my_exceptions {
protected:
    int data_id;
public:
    explicit already_exists(int v) : data_id(v), my_exceptions("Data already exists") {}
};

class citizen_already_exists : public already_exists {
public:
    explicit citizen_already_exists(int v) : already_exists(v) {}

    void show() const override {
        cout << "a citizen with the ID: " << "'" << data_id << "'" << " already exists" << endl;
    }
};

class district_already_exists : public already_exists {
public:
    explicit district_already_exists(int v) : already_exists(v) {}

    void show() const override {
        cout << "a citizen with the ID: " << "'" << data_id << "'" << " already exists" << endl;
    }
};

class party_already_exists : public already_exists {
public:
    explicit party_already_exists(int v) : already_exists(v) {}

    void show() const override {
        cout << "a citizen with the ID: " << "'" << data_id << "'" << " already exists" << endl;
    }
};

class invalid_type : public my_exceptions {
protected:
    int v;
public:
    explicit invalid_type(int val) : v(val), my_exceptions("invalid type entered") {}
};

class elections_invalid_type : public invalid_type {
public:
    explicit elections_invalid_type(int v) : invalid_type(v) {}

    void show() const override {
        cout << v << " is invalid. Elections type can only be:\n1. Regular\n2. Simple" << endl;
    }
};

class district_invalid_type : public invalid_type {
public:
    explicit district_invalid_type(int v) : invalid_type(v) {}

    void show() const override {
        cout << v << " is invalid. district type can only be:\n1. Unified\n2. Divided" << endl;
    }
};

class file_handling_ex : public my_exceptions {
public:
    file_handling_ex() : my_exceptions("unknown file handling exception") {}

    explicit file_handling_ex(string msg) : my_exceptions(std::move(msg)) {}
};

class file_open_ex : public file_handling_ex {
    string file_name;
public:
    explicit file_open_ex(string name) : file_name(std::move(name)) {}

    void show() const override { cout << "'" << file_name << "' couldn't open successfully" << endl; }
};

class save_process_ex : public file_handling_ex {
public:
    explicit save_process_ex(string msg) : file_handling_ex(std::move(msg)) {}

    save_process_ex() : file_handling_ex("saving process failed") {}
};

class load_process_ex : public file_handling_ex {
public:
    explicit load_process_ex(string msg) : file_handling_ex(std::move(msg)) {}

    load_process_ex() : file_handling_ex("loading process failed") {}
};

class [[maybe_unused]] add_district_ex : public my_exceptions {
public:
    add_district_ex() : my_exceptions("unknown error occurred while trying to add a district") {}

    [[maybe_unused]] explicit add_district_ex(string msg) : my_exceptions(std::move(msg)) {}
};

class add_citizen_ex : public my_exceptions {
public:
    add_citizen_ex() : my_exceptions("unknown error occurred while trying to add a citizen") {}

    explicit add_citizen_ex(string msg) : my_exceptions(std::move(msg)) {}
};

class birth_year_ex : public add_citizen_ex {
public:
    birth_year_ex() : add_citizen_ex(
            "birth year is invalid. citizen must be over 18 and birth year must be a positive number") {}

    [[maybe_unused]] explicit birth_year_ex(string msg) : add_citizen_ex(std::move(msg)) {}

};

class [[maybe_unused]] add_party_ex : public my_exceptions {
public:
    add_party_ex() : my_exceptions("unknown error occurred while trying to add a citizen") {}

    [[maybe_unused]] explicit add_party_ex(string msg) : my_exceptions(std::move(msg)) {}
};


template<class T> class compareID{
public:
    int ID;
    explicit compareID(int id) : ID(id) {}

    bool operator()(const T* t)
    {
        return t->getID() == this->ID;
    }
    bool operator()(const T& t)
    {
        return t.getID() == this->ID;
    }
};

template<class T> class findItem{
public:
    vector<T*> V;
    int ID;
    findItem(vector<T*> v, int id) : V(v), ID(id) {}

    bool exist()
    {
        return (any_of(V.begin(),V.end(), compareID<T>(ID)));
    }

    T* get()
    {
        auto p = find_if(V.begin(),V.end(), compareID<T>(ID));
        if (p == V.end())
        {
            if (typeid(T) == typeid(citizen))
                throw(citizen_not_found(ID));
            if (typeid(T) == typeid(district))
                throw(district_not_found(ID));
            if (typeid(T) == typeid(party))
                throw(party_not_found(ID));
            return nullptr;
        }
        else
            return (*p);
    }
};

template<class T> void addSorted(vector<T*>& lst, T* data)
{
    auto start = lst.begin();
    auto finish = lst.end();
    auto item = find_if(start, finish, [&](T* t) {return(t->getID() > data->getID()); });
    lst.insert(item, data);
}

#endif //ELECTIONS_EXCEPTIONS_H
