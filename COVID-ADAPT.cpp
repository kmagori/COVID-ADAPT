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
        // cout << "Person name is:" << identifier << " at xposition " << xposition << " and yposition " << yposition << " who is susceptible " << susceptible << " or exposed " << exposed << " or infected " << infected << " or infectious " << infectious << " or recovered " << recovered << " and is masked " << masked << " and is vaccinated " << vaccinated;
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
    Place places[9];


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
    person1.age=16;

    places[0].identifier="Closet1";
    places[0].xposition=1;
    places[0].yposition=1;
    places[0].Virus_level=0;
    places[0].Occupied=true;

    places[1].identifier="Closet2";
    places[1].xposition=2;
    places[1].yposition=1;
    places[1].Virus_level=0;
    places[1].Occupied=false;

    places[2].identifier="Closet3";
    places[2].xposition=3;
    places[2].yposition=1;
    places[2].Virus_level=0;
    places[2].Occupied=false;

    places[3].identifier="Closet4";
    places[3].xposition=1;
    places[3].yposition=2;
    places[3].Virus_level=0;
    places[3].Occupied=false;

    places[4].identifier="Closet5";
    places[4].xposition=2;
    places[4].yposition=2;
    places[4].Virus_level=0;
    places[4].Occupied=false;

    places[5].identifier="Closet6";
    places[5].xposition=3;
    places[5].yposition=2;
    places[5].Virus_level=0;
    places[5].Occupied=false;

    places[6].identifier="Closet7";
    places[6].xposition=1;
    places[6].yposition=3;
    places[6].Virus_level=0;
    places[6].Occupied=false;

    places[7].identifier="Closet8";
    places[7].xposition=2;
    places[7].yposition=3;
    places[7].Virus_level=0;
    places[7].Occupied=false;

    places[8].identifier="Closet9";
    places[8].xposition=3;
    places[8].yposition=3;
    places[8].Virus_level=0;
    places[8].Occupied=false;

    //accessing member function
    person1.printname();

    return 0;
}