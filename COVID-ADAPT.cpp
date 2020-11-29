// COVID-ADAPT
#include <iostream>
using namespace std;

class Person
{
    //Access specifier
    public:

    //Data Members
    string identifier;

    //Member functions
    void printname()
    {
        cout << "Person name is:" << identifier;
    }
};

int main()
{
    //Declare an object
    Person person1;

    //accessing data member
    person1.identifier="Toga";

    //accessing member function
    person1.printname();

    return 0;
}