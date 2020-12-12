// COVID-ADAPT
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <tgmath.h>
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
    double movement_rate;
    double exposure_probability;
    double exposure_rate;
    double infectious_probability;
    double infectious_rate;
    double sum_prob;
    double exposure_time;
    double infectious_time;    

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

    void exposure(double simtime)
    {
        susceptible=false;
        exposed=true;
        infectious=false;
        exposure_time=simtime;
        exposure_probability=0;
    }

    void becoming_infectious(double simtime)
    {
        susceptible=false;
        exposed=false;
        infectious=true;
        infectious_probability=0;
        infectious_time=simtime;
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
    double simtime,sum_prob;
    int max_time=10000;
    double steepness_exposure=1;
    int midpoint_exposure=50;
    double steepness_infectious=1;
    double midpoint_infectious=10;
    int PlaceBefore,PlaceAfter,i,TogaPlace,EriPlace;
    ofstream record;
    record.open("record.csv");
    
    record << "time,Toga_position,Eri_position,Eri_susceptible,Eri_exposed,Eri_infectious,Virus_Closet1,Virus_Closet2,Virus_Closet3,Virus_Closet4,Virus_Closet5,Virus_Closet6,Virus_Closet7,Virus_Closet8,Virus_Closet9";
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
    person[0].movement_rate=0.01;
    person[0].exposure_probability=0;
    person[0].exposure_rate=0.1;
    person[0].sum_prob=0;
    person[0].exposure_time=-9999;
    person[0].infectious_probability=0;
    person[0].infectious_rate=0.1;
    person[0].infectious_time=0;
    
    person[1].identifier="Eri";
    person[1].xposition=3;
    person[1].yposition=3;
    person[1].location=8;
    person[1].susceptible=true;
    person[1].exposed=false;
    person[1].infected=false;
    person[1].infectious=false;
    person[1].recovered=false;
    person[1].masked=false;
    person[1].vaccinated=false;
    person[1].age=7;
    person[1].movement_rate=0.01;
    person[1].exposure_probability=0;
    person[1].exposure_rate=0.1;
    person[1].sum_prob=0;
    person[1].exposure_time=-9999;
    person[1].infectious_probability=0;
    person[1].infectious_rate=0.1;
    person[1].infectious_time=-9999;

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
    places[8].person_in_there=1;

    //accessing member function
    //person1.printname();

    //run the simulation until a specified time

    int newx,newy,current_person;
    double v1,sojourn_time;

    do
    {

        //calculating probability of becoming exposed
        for (int i=0;i<2;i++) {if (person[i].susceptible) person[i].exposure_probability=1/(1+exp(-steepness_exposure*(places[person[i].location].Virus_level-midpoint_exposure)));}

        //calculating probability of becoming infectious
        for (int i=0;i<2;i++) {
            if (person[i].exposed) {
            person[i].infectious_probability=1/(1+exp(-steepness_infectious*((simtime-person[i].exposure_time)-midpoint_infectious)));
            }
        }

        //Gillespie algorithm
        //calculating sum of probabilities across persons, with movement rate, exposure rate
        for (int i=0;i<2;i++) {person[i].sum_prob=person[i].movement_rate+person[i].exposure_rate*person[i].exposure_probability+person[i].infectious_rate*person[i].infectious_probability;}

        //calculating sum of probabilities overall
        sum_prob=0;
        for (int i=0;i<2;i++) {sum_prob=sum_prob+person[i].sum_prob;}

    //get the sojourn time
    
    v1=(float) rand()/RAND_MAX;
    sojourn_time=(1/sum_prob)*log(1/v1);
    simtime=simtime+sojourn_time;
    //record << "\nThe time is " << simtime;

     //increase virus level by some value for all occupied places
    for (i=0; i<=8; i++)
    {    
    if (places[i].Occupied)
        {
                if (person[places[i].person_in_there].infectious==true) places[i].Virus_level=places[i].Virus_level+sojourn_time;
        }
    }

    //movement
    //lets see who will do something
    v1=sum_prob*((float) rand()/RAND_MAX) ;
    if (v1<person[0].sum_prob) current_person=0; else current_person=1;

    //adjust v1 to v1 - sum_prob if current_person=1;
    if (current_person==1) v1=v1-person[0].sum_prob;

    //so what does the person do
    if (v1<person[current_person].movement_rate) 
    { 
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


       //change occupied status
    places[PlaceBefore].Occupied=false;
    places[PlaceBefore].person_in_there=-9999;
    places[PlaceAfter].Occupied=true;
    places[PlaceAfter].person_in_there=current_person;


    }
    else 
    if(v1<(person[current_person].movement_rate+(person[current_person].exposure_rate*person[current_person].exposure_probability)))
         person[current_person].exposure(simtime);
         else person[current_person].becoming_infectious(simtime);
    //record << "\n" << person1.identifier << " is now at " << places[PlaceAfter].identifier << " which is at " << person1.xposition << " and " << person1.yposition;
    //std::printf("\nThe new position is %d and %d",person1.xposition,person1.yposition);   

    
 
   

 
    
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


    record << "\n" << simtime << "," << places[TogaPlace].identifier << "," << places[EriPlace].identifier << "," << person[1].susceptible << "," << person[1].exposed << "," << person[1].infectious_probability << "," << places[0].Virus_level << "," << places[1].Virus_level << "," << places[2].Virus_level << "," << places[3].Virus_level << "," << places[4].Virus_level << "," << places[5].Virus_level << "," << places[6].Virus_level << "," << places[7].Virus_level << "," << places[8].Virus_level;

    record.close();
    }
    while(simtime < max_time);

   
    

    return 0;
}