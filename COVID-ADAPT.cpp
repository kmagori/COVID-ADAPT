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
    bool susceptible; //0
    bool exposed;     //1
    bool infected;    
    bool infectious;  //2
    bool recovered;   //3
    int status;
    bool masked;
    bool vaccinated;
    int age;
    int location;
    double movement_rate;
    double exposure_probability;
    double exposure_rate;
    double exposure_time;
    double infectious_probability;
    double infectious_rate;
    double sum_prob;
    double infectious_time;  
    double recovery_probability;
    double recovery_rate;
    double recovery_time;  

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
        status=1;
        exposure_time=simtime;
        exposure_probability=0;
    }

    void becoming_infectious(double simtime)
    {
        susceptible=false;
        exposed=false;
        infectious=true;
        status=2;
        infectious_probability=0;
        infectious_time=simtime;
    }

    void recovering(double simtime)
    {
        susceptible=false;
        exposed=false;
        infectious=false;
        recovered=true;
        status=3;
        recovery_probability=0;
        recovery_time=simtime;
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
    srand ((unsigned) time(NULL));
    int number_infectious=2;
    int number_susceptible=3;
    Person person[number_infectious+number_susceptible];
    int gridsize=10;
    Place places[gridsize*gridsize];
    double simtime,sum_prob;
    int max_time=30000;
    double steepness_exposure=1;
    int midpoint_exposure=50;
    double steepness_infectious=1;
    double midpoint_infectious=7200;
    double steepness_recovery=1;
    double midpoint_recovery=14400;
    double virus_decay_rate=0.001;
    double current_sum;
    int PlaceBefore,PlaceAfter;
    ofstream record;
    record.open("virus_levels.csv");
    
    record << "time,";
    for (int i=0;i<=(gridsize*gridsize)-1;i++) record << ",Virus_Closet"+to_string(i);
    record.close();

    

    for (int i=0;i<gridsize;i++)
    {
        for (int j=0;j<gridsize;j++)
        {
            places[gridsize*i+j].identifier="Closet" + to_string(gridsize*i+j+1);
            places[gridsize*i+j].xposition=j+1;
            places[gridsize*i+j].yposition=i+1;
            places[gridsize*i+j].Virus_level=0;
            places[gridsize*i+j].Occupied=false;
            places[gridsize*i+j].person_in_there=-9999;
            places[gridsize*i+j].west_prob=25;
            places[gridsize*i+j].east_prob=25;
            places[gridsize*i+j].north_prob=25;
            places[gridsize*i+j].south_prob=25;
            if (i==0&&j==0)
                {
                    places[gridsize*i+j].west_prob=0;
                    places[gridsize*i+j].east_prob=50;
                    places[gridsize*i+j].north_prob=0;
                    places[gridsize*i+j].south_prob=50;
                }
            if (i==(gridsize-1)&&j==0)
                {
                    places[gridsize*i+j].west_prob=0;
                    places[gridsize*i+j].east_prob=50;
                    places[gridsize*i+j].north_prob=50;
                    places[gridsize*i+j].south_prob=0;
                }
            if (i==0&&j==(gridsize-1))
                {
                    places[gridsize*i+j].west_prob=50;
                    places[gridsize*i+j].east_prob=0;
                    places[gridsize*i+j].north_prob=0;
                    places[gridsize*i+j].south_prob=50;
                }
            if (i==(gridsize-1)&&j==(gridsize-1))
                {
                    places[gridsize*i+j].west_prob=50;
                    places[gridsize*i+j].east_prob=0;
                    places[gridsize*i+j].north_prob=50;
                    places[gridsize*i+j].south_prob=0;
                }
            if (i==0&&j!=0&&j!=(gridsize-1))
                {
                    places[gridsize*i+j].west_prob=1.0/3*100;
                    places[gridsize*i+j].east_prob=1.0/3*100;
                    places[gridsize*i+j].north_prob=0;
                    places[gridsize*i+j].south_prob=1.0/3*100;
                }
            if (i!=0&&i!=(gridsize-1)&&j==0)
                {
                    places[gridsize*i+j].west_prob=0;
                    places[gridsize*i+j].east_prob=1.0/3*100;
                    places[gridsize*i+j].north_prob=1.0/3*100;
                    places[gridsize*i+j].south_prob=1.0/3*100;
                }
            if (i==(gridsize-1)&&j!=0&&j!=(gridsize-1))
                {
                    places[gridsize*i+j].west_prob=1.0/3*100;
                    places[gridsize*i+j].east_prob=1.0/3*100;
                    places[gridsize*i+j].north_prob=1.0/3*100;
                    places[gridsize*i+j].south_prob=0;
                }
            if (i!=0&&i!=(gridsize-1)&&j==(gridsize-1))
                {
                    places[gridsize*i+j].west_prob=1.0/3*100;
                    places[gridsize*i+j].east_prob=0;
                    places[gridsize*i+j].north_prob=1.0/3*100;
                    places[gridsize*i+j].south_prob=1.0/3*100;
                }
        }
    }

    record.open("people.csv");
    record << "time, person_id, position, status";
    record.close();
    //accessing data member
    for (int i=0;i<number_infectious;i++)
    {
    person[i].identifier="Toga";
    
    do
    {
    person[i].location=floor(((float) rand()/RAND_MAX)*(gridsize*gridsize-1));
    }
    while (places[person[i].location].Occupied);
    person[i].xposition=(person[i].location % gridsize) + 1 ;
    person[i].yposition=((int) person[i].location/gridsize) + 1;
    places[person[i].location].Occupied=true;
    places[person[i].location].person_in_there=i;

    person[i].susceptible=false;
    person[i].exposed=false;
    person[i].infected=false;
    person[i].infectious=true;
    person[i].recovered=false;
    person[i].masked=false;
    person[i].vaccinated=false;
    person[i].age=16;
    person[i].movement_rate=0.01;
    person[i].exposure_probability=0;
    person[i].exposure_rate=0.1;
    person[i].sum_prob=0;
    person[i].exposure_time=-9999;
    person[i].infectious_probability=0;
    person[i].infectious_rate=0.1;
    person[i].infectious_time=0;
    person[i].recovery_probability=0;
    person[i].recovery_rate=0.1;
    person[i].recovery_time=-9999;
    person[i].status=2;
    }

    for (int i=number_infectious;i<(number_infectious+number_susceptible);i++)
    {
    person[i].identifier="Eri";
    do
    {
        person[i].location=floor(((float) rand()/RAND_MAX)*(gridsize*gridsize-1));
    }
    while (places[person[i].location].Occupied);
    person[i].xposition=(person[i].location % gridsize) + 1;
    person[i].yposition=((int) person[i].location/gridsize) + 1;
    places[person[i].location].Occupied=true;
    places[person[i].location].person_in_there=i;

    person[i].susceptible=true;
    person[i].exposed=false;
    person[i].infected=false;
    person[i].infectious=false;
    person[i].recovered=false;
    person[i].masked=false;
    person[i].vaccinated=false;
    person[i].age=7;
    person[i].movement_rate=0.01;
    person[i].exposure_probability=0;
    person[i].exposure_rate=0.1;
    person[i].sum_prob=0;
    person[i].exposure_time=-9999;
    person[i].infectious_probability=0;
    person[i].infectious_rate=0.1;
    person[i].infectious_time=-9999;
    person[i].recovery_probability=0;
    person[i].recovery_rate=0.1;
    person[i].recovery_time=-9999;
    person[i].status=0;
    }

    //accessing member function
    //person1.printname();

    //run the simulation until a specified time

    int newx,newy,current_person;
    double v1,sojourn_time;

    do
    {

        //calculating probability of becoming exposed
        for (int i=0;i<(number_infectious+number_susceptible);i++) {if (person[i].susceptible) person[i].exposure_probability=1/(1+exp(-steepness_exposure*(places[person[i].location].Virus_level-midpoint_exposure)));}

        //calculating probability of becoming infectious
        for (int i=0;i<(number_infectious+number_susceptible);i++) {
            if (person[i].exposed) {
            person[i].infectious_probability=1/(1+exp(-steepness_infectious*((simtime-person[i].exposure_time)-midpoint_infectious)));
            }
        }

        //calculating probability of recovering
        for (int i=0;i<(number_infectious+number_susceptible);i++) {
            if (person[i].infectious) {
            person[i].recovery_probability=1/(1+exp(-steepness_recovery*((simtime-person[i].infectious_time)-midpoint_recovery)));
            }
        }

        //Gillespie algorithm
        //calculating sum of probabilities across persons, with movement rate, exposure rate
        for (int i=0;i<(number_infectious+number_susceptible);i++) {person[i].sum_prob=person[i].movement_rate+person[i].exposure_rate*person[i].exposure_probability+person[i].infectious_rate*person[i].infectious_probability+person[i].recovery_rate*person[i].recovery_probability;}

        //calculating sum of probabilities overall
        sum_prob=0;
        for (int i=0;i<(number_infectious+number_susceptible);i++) {sum_prob=sum_prob+person[i].sum_prob;}

    //get the sojourn time
    
    v1=(float) rand()/RAND_MAX;
    sojourn_time=(1/sum_prob)*log(1/v1);
    simtime=simtime+sojourn_time;
    //record << "\nThe time is " << simtime;

     //increase virus level by some value for all occupied places; and decrease the Virus level
    for (int i=0; i<=(gridsize*gridsize-1); i++)
    {    
    if (places[i].Occupied)
        {
                if (person[places[i].person_in_there].infectious==true) places[i].Virus_level=places[i].Virus_level+sojourn_time;
        }
    places[i].Virus_level=places[i].Virus_level-(places[i].Virus_level*sojourn_time*virus_decay_rate);
    }

    //movement
    //lets see who will do something
    v1=sum_prob*((float) rand()/RAND_MAX) ;
    current_person=number_infectious+number_susceptible;
    current_sum=sum_prob;
    do
    {
        current_person--;
        current_sum=current_sum-person[current_person].sum_prob;
    } while (v1<current_sum);
    
    //adjust v1 to v1 - sum_prob if current_person=1;
    v1=v1-current_sum;

    //so what does the person do
    if (v1<person[current_person].movement_rate) 
    { 
    person[current_person].move(places[person[current_person].location].west_prob,places[person[current_person].location].east_prob,places[person[current_person].location].north_prob,places[person[current_person].location].south_prob,&newx,&newy);
    
    //newx=person[current_person].xposition;
    //newy=person[current_person].yposition;
    
    //find the place where the person was before
    
    int i=-1;
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
         
    else 
    if (v1<(person[current_person].movement_rate+(person[current_person].exposure_rate*person[current_person].exposure_probability)+(person[current_person].infectious_rate*person[current_person].infectious_probability)))
        person[current_person].becoming_infectious(simtime);
        else person[current_person].recovering(simtime);


    record.open("virus_levels.csv",std::fstream::app);


    record << "\n" << simtime << "," ;
    
    for (int i=0;i<=(gridsize*gridsize-1);i++)
    {
    record << "," << places[i].Virus_level;
    }

    record.close();

    record.open("people.csv",std::fstream::app);

    for (int i=0;i<(number_infectious+number_susceptible);i++)
    {
        int j=-1;
        do
        {
            j++;
        }while ((places[j].xposition!=person[i].xposition)||(places[j].yposition!=person[i].yposition));
        record << "\n" << simtime << "," << i << "," << places[j].identifier << "," << person[i].status;
    }
    record.close();

    }
    while(simtime < max_time);

   
    

    return 0;
}