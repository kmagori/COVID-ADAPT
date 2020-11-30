// COVID-ADAPT
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
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

    void move(int* newx, int* newy)
    {
        int v1;
        //int newx, newy;
        v1 = rand() % 100;
        if (xposition==1&yposition==1) 
        {
           if (v1<50) {*newx=xposition+1;*newy=yposition;} else {*newx=xposition; *newy=yposition+1;}
        }
        if (xposition==1&yposition==3) 
        {
           if (v1<50) {*newx=xposition+1;*newy=yposition;} else {*newx=xposition; *newy=yposition-1;}
        }
        if (xposition==3&yposition==1) 
        {
           if (v1<50) {*newx=xposition-1;*newy=yposition;} else {*newx=xposition; *newy=yposition+1;}
        }
        if (xposition==3&yposition==3) 
        {
           if (v1<50) {*newx=xposition-1;*newy=yposition;} else {*newx=xposition; *newy=yposition-1;}
        }
        if (xposition==1&yposition!=1&yposition!=3) 
        {
           if (v1<33) {*newx=xposition;*newy=yposition-1;} else if (v1<66) {*newx=xposition+1; *newy=yposition;} else {*newx=xposition;*newy=yposition+1;}
        }
        if (xposition==3&yposition!=1&yposition!=3) 
        {
           if (v1<33) {*newx=xposition;*newy=yposition-1;} else if (v1<66) {*newx=xposition-1; *newy=yposition;} else {*newx=xposition;*newy=yposition+1;}
        }
        if (yposition==1&xposition!=1&xposition!=3) 
        {
           if (v1<33) {*newx=xposition-1;*newy=yposition;} else if (v1<66) {*newx=xposition; *newy=yposition+1;} else {*newx=xposition+1;*newy=yposition;}
        }
        if (yposition==3&xposition!=1&xposition!=3) 
        {
           if (v1<33) {*newx=xposition-1;*newy=yposition;} else if (v1<66) {*newx=xposition; *newy=yposition-1;} else {*newx=xposition+1;*newy=yposition;}
        }
        if (xposition!=1&xposition!=3&yposition!=1&yposition!=3)
        {
            if (v1<25) {*newx=xposition+1;*newy=yposition;}
            if (v1>25&v1<50) {*newx=xposition;*newy=yposition+1;}
            if (v1>50&v1<75) {*newx=xposition-1;*newy=yposition;}
            if (v1>75) {*newx=xposition;*newy=yposition-1;}
        }
        //return newx,newy;
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
    srand (time(NULL));
    Person person1;
    Place places[9];
    double simtime;
    int max_time=1000;
    int PlaceBefore,PlaceAfter,i;
    ofstream record;
    record.open("record.txt");
    

    //accessing data member
    person1.identifier="Toga";
    person1.xposition=1;
    person1.yposition=1;
    person1.susceptible=false;
    person1.exposed=false;
    person1.infected=false;
    person1.infectious=true;
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
    //person1.printname();

    //run the simulation until a specified time

    int newx,newy;

    do
    {
 
    //movement
    person1.move(&newx,&newy);
    
    //find the place where the person was before
    
    i=-1;
    do
    {
        i++;
    } while ((places[i].xposition!=person1.xposition)||(places[i].yposition!=person1.yposition));
    PlaceBefore=i;

    record << "\n" << person1.identifier << " was at " << places[PlaceBefore].identifier << " which is at " << places[PlaceBefore].xposition << " and " << places[PlaceBefore].yposition;

    person1.xposition=newx;
    person1.yposition=newy;

    //let's see where he-she is now
    i=-1;
    do
    {
        i++;
    } while ((places[i].xposition!=person1.xposition)||(places[i].yposition!=person1.yposition));
    PlaceAfter=i;


    record << "\n" << person1.identifier << " is now at " << places[PlaceAfter].identifier << " which is at " << person1.xposition << " and " << person1.yposition;
    //std::printf("\nThe new position is %d and %d",person1.xposition,person1.yposition);   

    //change occupied status
    places[PlaceBefore].Occupied=false;
    places[PlaceAfter].Occupied=true;

    //elapse a random amount of time
    int v1;
    v1=rand() %100;
    simtime=simtime+v1;
    record << "\nThe time is " << simtime;
 
    //increase virus level by some value in occupied place
    if (person1.infectious==true) places[PlaceBefore].Virus_level=places[PlaceBefore].Virus_level+v1;
    cout << "\nVirus level: " << places[PlaceBefore].Virus_level << " at place " << places[PlaceBefore].identifier;
    }
    while(simtime < max_time);

    record.close();

    return 0;
}