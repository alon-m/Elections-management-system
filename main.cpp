#include "main.h"

const bool TEST = false;

void unhandledException() {
    cout << "unhandled exception occurred, exiting" << endl;
    exit(UNHANDLED_EXCEPTION);
}

int main() {
    set_terminate(unhandledException);
    run();
}

void run() {
    elections *_elections;
    if (TEST == true)
        _elections = test();
    else
        _elections = initMenu();
    menu(_elections);
}

elections *initMenu() {
    elections *_elections = nullptr;
    while (!_elections) {
        showInitMenu();
        _elections = switchInitMenu(static_cast<initMenuItem>(getNumberInput("Your choice: ")));
    }
    return _elections;
}

elections *switchInitMenu(const initMenuItem &choice) {
    try {
        if (choice == initMenuItem::NEW)
            return createNewElections();
        else if (choice == initMenuItem::LOAD)
            return load();
        else if (choice == initMenuItem::EXIT) {
            printHeader("EXITING ELECTION SYSTEM");
            exit(static_cast<int>(initMenuItem::EXIT));
        } else
            throw (invalid_menu_option());
    }
    catch (const invalid_menu_option &e) { e.show(); }
    catch (const my_exceptions &e) { e.show(); }
    return nullptr;
}

void menu(elections *_elections) {
    int choice;
    while (true) {
        showMenu();
        _elections = switchMenu(static_cast<menuItem>(getNumberInput("Your choice: ")), _elections);
        clearScreen();
    }
}

void clearScreen()
{
    cout << "Clear Screen? (y)" << endl;
    cout << "Your choice: ";
    string clear;
    cin >> clear;
    if (clear == "y")
        system("CLS");
}

void setDate(elections *_elections) {
    bool valid = false;
    while (!valid) {
        try {
            string dateString;
            cout << "Please enter elections date: day/month/year " << endl;
            getline(cin >> ws, dateString);
            _elections->setDate(dateString);
            valid = true;
        }
        catch (const my_exceptions &e) { e.show(); }
        catch (...) {
            cout << "Unknown error" << endl;
            exit(UNHANDLED_EXCEPTION);
        }
    }
}

void showInitMenu() {
    printHeader("Initiating elections System");
    cout << "Please choose an item" << endl;
    cout << "1  : Create a new elections system" << endl;
    cout << "2  : Load an existing elections system" << endl;
    cout << "3  : exit program" << endl;
}

elections *createNewElections() {
    elections *_elections;
    try {
        _elections = new elections();
        if (!_elections)
            throw (allocation_error("Failed to allocated new elections"));
    }
    catch (const allocation_error &e) {
        e.show();
        return nullptr;
    }
    bool valid = false;
    while (!valid) {
        try {
            int input = getNumberInput(
                    "Please choose your elections system type:\n1. Regular (multi district system)\n2. Simple (Single district system)\n");
            _elections->setType(static_cast<ElectionsType>(input));
            valid = true;
        }
        catch (const elections_invalid_type &e) { e.show(); }
    }
    setDate(_elections);
    if (_elections->getType() == ElectionsType::SIMPLE) {
        cout << "Please name your country:" << endl;
        string name;
        getline(cin >> ws, name);
        int electors = getNumberInput("Please enter how many electors are there in your country:\n");
        _elections->addDistrict(name, electors, static_cast<int>(DistrictType::DIVIDED));
    }
    return _elections;
}

elections *load(elections *_elections) noexcept(false) {
    if (_elections) {
        string input;
        cout << "Would you like to save current elections before loading another one? (y)" << endl;
        cin >> input;
        if (input == "y")
            save(_elections);
    }

    ifstream infile;
    cout << "Please enter the name of the file you would like to load: \n(.bin file format will be appended automatically)"<< endl;
    string file_name;
    getline(cin >> ws, file_name);
    file_name += ".bin";
    infile.open(file_name, ios::binary | ios::in);
    try {
        if (!infile)
            throw (file_open_ex(file_name));
        delete _elections;
        _elections = new elections();
        if (!_elections)
            throw (allocation_error("Failed to allocated new _elections"));
        _elections->load(infile);
        infile.close();
        cout << "loading process finished successfully" << endl;
        return _elections;
    }
    catch (const allocation_error &e) {
        e.show();
        exit(ALLOCATION_ERROR);
    }
    catch (const file_open_ex &e) {
        e.show();
        return nullptr;
    }
    catch (const my_exceptions &e) {
        e.show();
        exit(FILE_HANDLING_ERROR);
    }
    catch (...) { exit(FILE_HANDLING_ERROR); }

}

void save(elections *_elections) noexcept(false) {
    cout
            << "Please enter the name of the file you would like to save: \n(.bin file format will be appended automatically)"
            << endl;
    string file_name;
    getline(cin >> ws, file_name);
    file_name += ".bin";
    ofstream outfile(file_name, ios::binary | ios::trunc);
    try {
        if (!outfile)
            throw (file_open_ex(file_name));
        _elections->save(outfile);
        outfile.close();
        cout << "saving process finished successfully" << endl;
    }
    catch (file_open_ex &e) {
        e.show();
        return;
    }
    catch (const my_exceptions &e) {
        cout << "error while saving to file:\n";
        e.show();
        outfile.close();
        exit(FILE_HANDLING_ERROR);
    }
}

void showMenu() {
    printHeader("ELECTION'S MENU");
    cout << "Please choose an item" << endl;
    cout << "1  : Add a district" << endl;
    cout << "2  : Add a citizen" << endl;
    cout << "3  : Add a party" << endl;
    cout << "4  : Add a party representative" << endl;
    cout << "5  : Print all districts" << endl;
    cout << "6  : Print all citizens" << endl;
    cout << "7  : Print all parties" << endl;
    cout << "8  : Add a vote" << endl;
    cout << "9  : Print election results" << endl;
    cout << "10 : exit program" << endl;
    cout << "11 : save current elections system" << endl;
    cout << "12 : load an existing elections system" << endl;
}

elections *switchMenu(menuItem choice, elections *_elections) {
    try {
        if (choice == menuItem::ADD_DISTRICT)
            addDistrict(_elections);
        else if (choice == menuItem::ADD_CITIZEN)
            addCitizen(_elections);
        else if (choice == menuItem::ADD_PARTY)
            addParty(_elections);
        else if (choice == menuItem::ADD_REP)
            addRep(_elections);
        else if (choice == menuItem::PRINT_DISTRICT)
            showDistricts(_elections);
        else if (choice == menuItem::PRINT_CITIZENS)
            showCitizens(_elections);
        else if (choice == menuItem::PRINT_PARTIES)
            showParties(_elections);
        else if (choice == menuItem::VOTE)
            vote(_elections);
        else if (choice == menuItem::RESULTS)
            showResults(_elections);
        else if (choice == menuItem::EXIT) {
            printHeader("EXITING ELECTIONS SYSTEM");
            exit(static_cast<int>(menuItem::EXIT));
        } else if (choice == menuItem::SAVE)
            save(_elections);
        else if (choice == menuItem::LOAD)
            _elections = load(_elections);
        else
            throw (invalid_menu_option());
    }
    catch (const invalid_menu_option &e) { e.show(); }
    catch (const my_exceptions &e) { e.show(); }
    return _elections;
}

void addDistrict(elections *_elections) {
    if (_elections->getType() == ElectionsType::SIMPLE) {
        cout << "can't add a new district in a simple elections system" << endl;
        return;
    }
    printHeader("CREATING A NEW DISTRICT");
    cout << "Please enter a name for your district" << endl;
    string name;
    getline(cin >> ws, name);
    try {
        int type = getNumberInput(
                "What type of district would you like that to be?\n1. Unified district\n2. Divided district\n");
        int electors = getNumberInput("Please enter amount of representatives for the district\n");
        _elections->addDistrict(name, electors, type);
    }
    catch (const my_exceptions &e) {
        e.show();
        return;
    }
    cout << "district was added successfully" << endl;
}

void addCitizen(elections *_elections) {
    printHeader("CREATING A NEW CITIZEN");
    int district_id = 0;
    cout << "Please enter citizen name: " << endl;
    string name;
    getline(cin >> ws, name);
    try {
        int id = getNumberInput("Please enter the citizen's id:\n");
        int BirthYear = getNumberInput("Please enter the citizen's birth year:\n");
        if (_elections->getType() == ElectionsType::REGULAR) {
            cout << "Please choose " << name << "'s district id:" << endl;
            printAvailableItems<district>(_elections->getDistrictList());
            district_id = getNumberInput("Your choice: ");
        }
        _elections->addCitizen(name, id, BirthYear, district_id);
    }
    catch (const my_exceptions &e) {
        e.show();
        return;
    }
    cout << "citizen was added successfully" << endl;
}

void addParty(elections *_elections) {
    printHeader("CREATING A NEW PARTY");
    cout << "Please enter a name for your party" << endl;
    string name;
    getline(cin >> ws, name);
    try {
        int canID = getNumberInput("Please enter ID of party's candidate\n");
        _elections->addParty(name, canID);
    }
    catch (const my_exceptions &e) {
        e.show();
        return;
    }
    cout << "party was added successfully" << endl;
}

void addRep(elections *_elections) {
    printHeader("ADDING A PARTY REPRESENTATIVE");
    try {
        int cID, pID, dID = 0;
        cID = getNumberInput("Please enter your citizen's ID:\n");
        cout << "Please enter the party ID for which this citizen will be the representative: " << endl;
        printAvailableItems<party>(_elections->getPartyList());
        pID = getNumberInput("Your choice: ");
        if (_elections->getType() == ElectionsType::REGULAR) {
            cout << "Please enter district ID in which the citizen will be the representative: " << endl;
            printAvailableItems<district>(_elections->getDistrictList());
            dID = getNumberInput("Your choice: ");
        }
        _elections->addRep(cID, pID, dID);
        cout << "citizen " << cID << "was added as party " << pID << " representative in district " << dID << " successfully"
            << endl;
    }
    catch (const my_exceptions &e) {
        e.show();
        return;
    }
}

void showDistricts(elections *_elections) {
    printHeader("PRINTING ALL DISTRICTS");
    cout << _elections->getDistrictList();
}

void showCitizens(elections *_elections) {
    printHeader("PRINTING ALL CITIZENS");
    cout << _elections->getCitizenList();
}

void showParties(elections *_elections) {
    printHeader("PRINTING ALL PARTIES");
    cout << _elections->getPartyList();
}

void vote(elections *_elections) {
    int cID, pID;
    printHeader("CASTING A VOTE");
    try {
        cID = getNumberInput("Please enter citizen's ID:\n");
        cout << "Please enter desired party ID: " << endl;
        printAvailableItems<party>(_elections->getPartyList());
        pID = getNumberInput("Your choice: ");
        _elections->vote(cID, pID);
    }
    catch (const my_exceptions &e) {
        e.show();
        return;
    }
    cout << "vote successful" << endl;
}

void showResults(elections *_elections) {
    printHeader("PRINTING ELECTIONS RESULTS");
    cout << _elections->getDate();
    try {
        _elections->result(); // goes through all districts and processes results for elections
        districtList lst = _elections->getDistrictList();
        for (auto& item : lst)
        {
            cout << *(item->getResult());
        }
        _elections->printPartyElectionResultsToTerminal();
    }
    catch (const my_exceptions &e) {
        e.show();
        return;
    }
}

void printHeader(const string &st) {
    cout << " ";
    for (int i = 0; i < 40; i++)
        cout << "*";
    cout << endl;
    cout << "\t" << st << "\t" << endl;
    cout << " ";
    for (int i = 0; i < 40; i++)
        cout << "*";
    cout << endl;
}

int getNumberInput(const string &msg) {
    bool valid = false;
    int val;
    while (!valid) {
        try {
            cout << msg;
            val = getNumberInput();
            valid = true;
        }
        catch (const input_invalid &e) { e.show(); }
    }
    return val;
}

int getNumberInput() {
    string input;
    cin >> input;
    if (input.size() > 9)
        throw (input_invalid("maximum input length for numbers is 9 digits"));
    if (!all_of(input.begin(), input.end(), isdigit))
        throw (input_invalid("invalid input received: " + input));

    return stoi(input);
}


//Made for test purposes

elections *test() {
    try {
        //return test1();
        return test2();
        return nullptr;
    }
    catch (const my_exceptions &e) { e.show(); exit(UNHANDLED_EXCEPTION); }
}

[[maybe_unused]] elections *test1() {
    elections *pElections = new elections(1, 1, 2021, ElectionsType::REGULAR);
    int start, finish;
    //3 districts
    pElections->addDistrict("New York", 3, static_cast<int>(DistrictType::DIVIDED));
    pElections->addDistrict("California", 4, static_cast<int>(DistrictType::DIVIDED));
    pElections->addDistrict("Texas", 4, static_cast<int>(DistrictType::UNIFIED));

    //create 8 citizens in d0
    for (int i = 1000; i < 1008; i++)
        pElections->addCitizen("citizen" + to_string(i), i, 1900, 0);
    //create 12 citizens in d1
    for (int i = 2000; i < 2012; i++)
        pElections->addCitizen("citizen" + to_string(i), i, 1900, 1);
    //create 15 citizens in d2
    for (int i = 3000; i < 3015; i++)
        pElections->addCitizen("citizen" + to_string(i), i, 1900, 2);

    //2 parties
    pElections->addParty("Democratic party", 1000);
    pElections->addParty("Republican party", 2000);

    //E.addRep(cID, pID, dID);
    //add 3 reps from p0 to d0
    start = 1000;
    finish = start + 3;
    for (int i = start; i < finish; i++)
        pElections->addRep(i, 0, 0);
    //add 4 reps from p0 to d1
    start = 2000;
    finish = start + 4;
    for (int i = start; i < finish; i++)
        pElections->addRep(i, 0, 1);

    //add 4 reps from p0 to d2
    start = 3000;
    finish = start + 4;
    for (int i = start; i < finish; i++)
        pElections->addRep(i, 0, 2);

    //add 3 reps from p1 to d0
    start = 1003;
    finish = start + 3;
    for (int i = start; i < finish; i++)
        pElections->addRep(i, 1, 0);

    //add 4 reps from p1 to d1
    start = 2004;
    finish = start + 4;
    for (int i = start; i < finish; i++)
        pElections->addRep(i, 1, 1);

    //add 4 reps from p1 to d2
    start = 3004;
    finish = start + 4;
    for (int i = start; i < finish; i++)
        pElections->addRep(i, 1, 2);

    //add 2 votes in d0 to p0
    start = 1000;
    finish = start + 2;
    for (int i = start; i < finish; i++)
        pElections->vote(i, 0);
    //add 3 votes in d0 to p1
    start = 1003;
    finish = start + 3;
    for (int i = start; i < finish; i++)
        pElections->vote(i, 1);

    //add 6 votes in d1 to p0
    start = 2000;
    finish = start + 6;
    for (int i = start; i < finish; i++)
        pElections->vote(i, 0);
    //add 3 votes in d1 to p1
    start = 2006;
    finish = start + 3;
    for (int i = start; i < finish; i++)
        pElections->vote(i, 1);

    //add 5 votes in d2 to p0
    start = 3000;
    finish = start + 5;
    for (int i = start; i < finish; i++)
        pElections->vote(i, 0);
    //add 7 votes in d2 to p1
    start = 3005;
    finish = start + 7;
    for (int i = start; i < finish; i++)
        pElections->vote(i, 1);

    return pElections;
}

[[maybe_unused]] elections *test2() {
    elections *pElections = new elections(1, 1, 2021, ElectionsType::SIMPLE);
    int start, finish;
    //3 districts
    pElections->addDistrict("Country", 54, static_cast<int>(DistrictType::DIVIDED));


    //create citizens

    start = 0;
    finish = start + 1000;
    for (int i = start; i < finish; i++)
        pElections->addCitizen("citizen" + to_string(i), i, 1900, 0);

    //4 parties
    pElections->addParty("Party0", (rand() % finish));
    pElections->addParty("Party1", (rand() % finish));
    pElections->addParty("Party2", (rand() % finish));
    pElections->addParty("Party3", (rand() % finish));


    //E.addRep(cID, pID, dID);
    //add 54 reps from Party0
    start = 0;
    finish = start + 54;
    for (int i = start; i < finish; i++)
        pElections->addRep(i, 0, 0);
    //add 54 reps from Party1
    start = 100;
    finish = start + 54;
    for (int i = start; i < finish; i++)
        pElections->addRep(i, 1, 0);
    //add 54 reps from Party2
    start = 200;
    finish = start + 54;
    for (int i = start; i < finish; i++)
        pElections->addRep(i, 2, 0);
    //add 54 reps from Party3
    start = 300;
    finish = start + 54;
    for (int i = start; i < finish; i++)
        pElections->addRep(i, 3, 0);

    //add votes
    start = 0;
    finish = start + 500;
    for (int i = start; i < finish; i++)
        pElections->vote(i, rand() % 4);

    return pElections;
}