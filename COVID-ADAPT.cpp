// COVID-ADAPT
#include <iostream>
using namespace std;

class Person
{
    //Access specifier
    public:

    //Data Members
    string identifier;
    int xposition;
    int yposition;
    bool susceptible;
    bool exposed;
    bool infected;
    bool infectious;
    bool recovered;
    bool masked;
    bool vaccinated;
    int age;

    //Member functions
    void printname()
    {
        cout << "Person name is:" << identifier << " at xposition " << xposition << " and yposition " << yposition << " who is susceptible " << susceptible << " or exposed " << exposed << " or infected " << infected << " or infectious " << infectious << " or recovered " << recovered << " and is masked " << masked << " and is vaccinated " << vaccinated << " and is age " << age;
    }
};

class Place
{
    //Access specifier
    public:

    //Data members
    string identifier;
    int xposition;
    int yposition;
    double Virus_level;
    bool Occupied;
};

int main()
{
    //Declare an object
    Person person1;
    Place place1;

    //accessing data member
    person1.identifier="Toga";
    person1.xposition=1;
    person1.yposition=1;
    person1.susceptible=true;
    person1.exposed=false;
    person1.infected=false;
    person1.infectious=false;
    person1.recovered=false;
    person1.masked=false;
    person1.vaccinated=false;
    person1.age=18;

    place1.identifier="Closet";
    place1.xposition=1;
    place1.yposition=1;
    place1.Virus_level=0;
    place1.Occupied=true;

    //accessing member function
    person1.printname();

    return 0;
}