# Elections-management-system
A simple terminal based management system for elections. Written in C++. Academy project in a course called: "Object Oriented Programming and C++ programming language".


Project Summary:

This project was initially inteded for the student to properly study Object Oriented Programming.
Therefore putting a lot of emphasize on those concepts: Classes, Encapsulation, Polymorphism, Inheritance.
Another goal of the course was to teach us the use of C++ programming language.
After two other courses in C programming language (a basic and an advanced one) - this C++ course had a high standard for code writing.
Thereofre another emphasize was to be able to write elegant, efficient (both in runtime speed and memory usage) and smart code.
Using STL, algorithms and more...


Project Description and guidelines:

In an annonymous country it has been decided to digitize the system which manages the elections.
The country is devided into DISTRICTS.
Each DISTRICT has:
  1. a set amount of delegates/electors.
Each PARTY which runs in the elections has:
  1. a single Candidate to be the head of the country
  2. a seperate list of representatives which it sends to every district
Each CITIZEN can only vote once.

Once results are calculated, the respective amount of electors in each district are given to the party that won them.

Every elections round must hold this data:
  1. the date in which the elections take place (day, month, year)
  2. a list of eligible voters (every citizen on this list is an eligible voter. Non-eligible citizens can not be added)
  3. a list of all districts
  4. a list of all parties
  
Each district hold this data:
  1. Name
  2. serial number (ID)
  3. Number of delegates it can send (electors)
  4. amount of citizens registered in the district
  5. list of representatives sent to it from each party
  6. results:
    1. winning party
    2. votes
    3. percentages

Each citizens holds this data:
  1. Name
  2. ID
  3. Birth year
  4. The district where he can vote

Each party holds this data:
  1. Name
  2. serial number (ID)
  3. party's candidate to 'head of country' (Prime minister, President etc.)
  4. list of representatives it sends to each district

An interactive terminal based menu is presented to the user throuout the usage of the system.
There are two menus.

An initial menu offers these options: (only appears on initial entrance)
  1. start a new elections system
  2. load an existing system from a binary file
  3. exit

The usage menu offers these options:
  1. create a new district
  2. create a new citizen
  3. create a new party
  4. set a citizen to be a party representative
  5. print all district
  6. print all citizens
  7. print all parties
  8. cast a vote
  9. show elections results
  10. exit
  11. save current elections data to a binary file
  12. load a saved system from a binary file

General guidelines:
  1. party and district serial numbers will be generated automatically.
     In case of a draw between two parties - the winner will be the one with the lower serial number.
  2. There can be two types of districts: Unified and Divided.
     1. Unified: the winning party (with mose votes) receives all the electors the district holds
     2. Divided: each party receives a respective amount of electors based on the amount of votes
  3. There can be two types of Elections systems: Simple and Regular
     1. Regular: can have multiple districts, each district can be either unified or divided.
     2. Simple: a one district system. This can be thought of as a regular elections system with one Divided District
  4. citizen id can't be longer than 9 digits
  5. citizen has to be at lease 18 (only by birth year in respect to the elections date.year, day and month don't matter)
  6. general system logic must be tested (for example, citizens can only vote once...)
  7. data validation should happen at the very least within the internal classes.


Code writing guidedlines:
  1. Object Oriented programming conecepts must be used properly.
     constructors and destructors, setters and getters, encapsulation, data privacy and more...
  2. Operator<< must be implemented for citizen, party, district.
  3. cannot request input within internal classes. Input and output must be handled from the main.
  4. code writing must aspire to be efficient, elegant and with minimal 'copy-paste' and code duplication
  5. use inheritance when desigining the different districts. The elections systems type management can be either by inheritance or by a type variable.
  6. design entities as 'black boxes'
  7. saving and loading using binary files
  8. use exceptions to handle errors
  9. use STL and algorithms when possible and relevant
  10.check that every memory allocation works
  11.every input must be tested and correct. can't rely on proper input from user. (example: if numbers are expected, make sure only numbers are inputted)
  12.design a sytem which is not strongly-reliant on that specific main, but can work with various user interfaces that adhere to the classes' logic.

