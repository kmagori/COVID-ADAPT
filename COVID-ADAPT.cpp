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
    int location;

    //Member functions
    void printname()
    {
        // cout << "Person name is:" << identifier << " at xposition " << xposition << " and yposition " << yposition << " who is susceptible " << susceptible << " or exposed " << exposed << " or infected " << infected << " or infectious " << infectious << " or recovered " << recovered << " and is masked " << masked << " and is vaccinated " << vaccinated;
        cout << "Person name is:" << identifier << " at xposition " << xposition << " and yposition " << yposition << " who is susceptible " << susceptible << " or exposed " << exposed << " or infected " << infected << " or infectious " << infectious << " or recovered " << recovered << " and is masked " << masked << " and is vaccinated " << vaccinated << " and is age " << age;
    }

    void move(double west_prob, double east_prob, double north_prob, double south_prob, int* newx, int* newy)
    {
        int v1;
        //int newx, newy;
        v1 = rand() % 100;
        if (v1<west_prob) {*newx=xposition-1;*newy=yposition;} 
        else if (v1<(west_prob+north_prob)) {*newx=xposition;*newy=yposition-1;}
        else if (v1<(west_prob+north_prob+east_prob)) {*newx=xposition+1;*newy=yposition;}
        else {*newx=xposition;*newy=yposition+1;}
        
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
    int person_in_there;
    double west_prob;
    double north_prob;
    double east_prob;
    double south_prob;
};

int main()
{
    //Declare an object
    srand (time(NULL));
    Person person[2];
    Place places[9];
    double simtime;
    int max_time=1000;
    int PlaceBefore,PlaceAfter,i,TogaPlace,EriPlace;
    ofstream record;
    record.open("record.csv");
    
    record << "time,Toga_position,Eri_position,Virus_Closet1,Virus_Closet2,Virus_Closet3,Virus_Closet4,Virus_Closet5,Virus_Closet6,Virus_Closet7,Virus_Closet8,Virus_Closet9";
    record.close();
    //accessing data member
    person[0].identifier="Toga";
    person[0].xposition=1;
    person[0].yposition=1;
    person[0].location=0;
    person[0].susceptible=false;
    person[0].exposed=false;
    person[0].infected=false;
    person[0].infectious=true;
    person[0].recovered=false;
    person[0].masked=false;
    person[0].vaccinated=false;
    person[0].age=16;
    
    person[1].identifier="Eri";
    person[1].xposition=3;
    person[1].yposition=3;
    person[1].location=8;
    person[1].susceptible=false;
    person[1].exposed=false;
    person[1].infected=false;
    person[1].infectious=true;
    person[1].recovered=false;
    person[1].masked=false;
    person[1].vaccinated=false;
    person[1].age=7;

    for (int i=0;i<3;i++)
    {
        for (int j=0;j<3;j++)
        {
            places[3*i+j].identifier="Closet" + to_string(3*i+j+1);
            places[3*i+j].xposition=j+1;
            places[3*i+j].yposition=i+1;
            places[3*i+j].Virus_level=0;
            places[3*i+j].Occupied=false;
            places[3*i+j].person_in_there=-9999;
            places[3*i+j].west_prob=25;
            places[3*i+j].east_prob=25;
            places[3*i+j].north_prob=25;
            places[3*i+j].south_prob=25;
            if (i==0&&j==0)
                {
                    places[3*i+j].west_prob=0;
                    places[3*i+j].east_prob=50;
                    places[3*i+j].north_prob=0;
                    places[3*i+j].south_prob=50;
                }
            if (i==2&&j==0)
                {
                    places[3*i+j].west_prob=0;
                    places[3*i+j].east_prob=50;
                    places[3*i+j].north_prob=50;
                    places[3*i+j].south_prob=0;
                }
            if (i==0&&j==2)
                {
                    places[3*i+j].west_prob=50;
                    places[3*i+j].east_prob=0;
                    places[3*i+j].north_prob=0;
                    places[3*i+j].south_prob=50;
                }
            if (i==2&&j==2)
                {
                    places[3*i+j].west_prob=50;
                    places[3*i+j].east_prob=0;
                    places[3*i+j].north_prob=50;
                    places[3*i+j].south_prob=0;
                }
            if (i==0&&j!=0&&j!=2)
                {
                    places[3*i+j].west_prob=1.0/3*100;
                    places[3*i+j].east_prob=1.0/3*100;
                    places[3*i+j].north_prob=0;
                    places[3*i+j].south_prob=1.0/3*100;
                }
            if (i!=0&&i!=2&&j==0)
                {
                    places[3*i+j].west_prob=0;
                    places[3*i+j].east_prob=1.0/3*100;
                    places[3*i+j].north_prob=1.0/3*100;
                    places[3*i+j].south_prob=1.0/3*100;
                }
            if (i==2&&j!=0&&j!=2)
                {
                    places[3*i+j].west_prob=1.0/3*100;
                    places[3*i+j].east_prob=1.0/3*100;
                    places[3*i+j].north_prob=1.0/3*100;
                    places[3*i+j].south_prob=0;
                }
            if (i!=0&&i!=2&&j==2)
                {
                    places[3*i+j].west_prob=1.0/3*100;
                    places[3*i+j].east_prob=0;
                    places[3*i+j].north_prob=1.0/3*100;
                    places[3*i+j].south_prob=1.0/3*100;
                }
        }
    }

    places[0].Occupied=true;
    places[8].Occupied=true;
    places[0].person_in_there=0;
    places[8].person_in_there=8;

    //accessing member function
    //person1.printname();

    //run the simulation until a specified time

    int newx,newy,v1,current_person;

    do
    {



    //movement
    //lets see who will move
    v1=rand() %100;
    if (v1<50) current_person=0; else current_person=1;
    
    person[current_person].move(places[person[current_person].location].west_prob,places[person[current_person].location].east_prob,places[person[current_person].location].north_prob,places[person[current_person].location].south_prob,&newx,&newy);
    //newx=person[current_person].xposition;
    //newy=person[current_person].yposition;
    
    //find the place where the person was before
    
    i=-1;
    do
    {
        i++;
    } while ((places[i].xposition!=person[current_person].xposition)||(places[i].yposition!=person[current_person].yposition));
    PlaceBefore=i;

    //record << "\n" << person1.identifier << " was at " << places[PlaceBefore].identifier << " which is at " << places[PlaceBefore].xposition << " and " << places[PlaceBefore].yposition;

    //check if the new place is occupied: if it is, stay where they are

    //let's see where he-she is now
    i=-1;
    do
    {
        i++;
    } while ((places[i].xposition!=newx)||(places[i].yposition!=newy));
    PlaceAfter=i;

    if (places[PlaceAfter].Occupied==true) {newx=person[current_person].xposition; newy=person[current_person].yposition;PlaceAfter=PlaceBefore;}

    person[current_person].xposition=newx;
    person[current_person].yposition=newy;
    person[current_person].location=PlaceAfter;

    //record << "\n" << person1.identifier << " is now at " << places[PlaceAfter].identifier << " which is at " << person1.xposition << " and " << person1.yposition;
    //std::printf("\nThe new position is %d and %d",person1.xposition,person1.yposition);   

    //elapse a random amount of time
    
    v1=rand() %100;
    simtime=simtime+v1;
    //record << "\nThe time is " << simtime;
 
    //increase virus level by some value for all occupied places
    for (i=0; i<=9; i++)
    {    
    if (places[i].Occupied)
        {
                if (person[places[i].person_in_there].infectious==true) places[i].Virus_level=places[i].Virus_level+v1;
        }
    }

    //change occupied status
    places[PlaceBefore].Occupied=false;
    places[PlaceBefore].person_in_there=-9999;
    places[PlaceAfter].Occupied=true;
    places[PlaceAfter].person_in_there=current_person;

    
    //cout << "\nVirus level: " << places[PlaceBefore].Virus_level << " at place " << places[PlaceBefore].identifier;

    //check where is Toga is
    i=-1;
    do
    {
        i++;
    } while ((places[i].xposition!=person[0].xposition)||(places[i].yposition!=person[0].yposition));
    TogaPlace=i;

    //check where is Eri
    i=-1;
    do
    {
        i++;
    } while ((places[i].xposition!=person[1].xposition)||(places[i].yposition!=person[1].yposition));
    EriPlace=i;

    record.open("record.csv",std::fstream::app);


    record << "\n" << simtime << "," << places[TogaPlace].identifier << "," << places[EriPlace].identifier << "," << places[0].Virus_level << "," << places[1].Virus_level << "," << places[2].Virus_level << "," << places[3].Virus_level << "," << places[4].Virus_level << "," << places[5].Virus_level << "," << places[6].Virus_level << "," << places[7].Virus_level << "," << places[8].Virus_level;

    record.close();
    }
    while(simtime < max_time);

   
    

    return 0;
}