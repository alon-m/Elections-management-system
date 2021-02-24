//
// Created by alonm on 15/02/2021.
//

#ifndef ELECTIONS_MAIN_H
#define ELECTIONS_MAIN_H
#include "elections.h"

enum class initMenuItem {
    NEW = 1, LOAD, EXIT
};
enum class menuItem {
    ADD_DISTRICT = 1,
    ADD_CITIZEN,
    ADD_PARTY,
    ADD_REP,
    PRINT_DISTRICT,
    PRINT_CITIZENS,
    PRINT_PARTIES,
    VOTE,
    RESULTS,
    EXIT,
    SAVE,
    LOAD
};

elections *createNewElections();

elections *load(elections *_elections = nullptr) noexcept(false);

void save(elections *_elections) noexcept(false);

void addDistrict(elections *_elections);

void addCitizen(elections *_elections);

void addParty(elections *_elections);

void addRep(elections *_elections);

void showDistricts(elections *_elections);

void showCitizens(elections *_elections);

void showParties(elections *_elections);

void vote(elections *_elections);

void showResults(elections *_elections);

void run();

elections *initMenu();

void menu(elections *_elections);

void showInitMenu();

void showMenu();

elections *switchInitMenu(const initMenuItem &choice) noexcept(false);

elections *switchMenu(menuItem choice, elections *_elections) noexcept(false);

void printHeader(const string &st);

void setDate(elections *_elections);

int getNumberInput(const string &msg);

int getNumberInput();

void clearScreen();

//Testing purposes
[[maybe_unused]] elections *test();

[[maybe_unused]] elections *test1();

[[maybe_unused]] elections *test2();

#endif //ELECTIONS_MAIN_H
