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

    //Member functions
    void printname()
    {
        cout << "Person name is:" << identifier << " at xposition " << xposition << " and yposition " << yposition << " who is susceptible " << susceptible << " or exposed " << exposed << " or infected " << infected << " or infectious " << infectious << " or recovered " << recovered << " and is masked " << masked << " and is vaccinated " << vaccinated;
    }
};

int main()
{
    //Declare an object
    Person person1;

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

    //accessing member function
    person1.printname();

    return 0;
}