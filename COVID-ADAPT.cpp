// COVID-ADAPT
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <tgmath.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>
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
    double total_prob;
};

class Room
{
    //Access specifier
    public:

    //Data members
    string identifier;
    double Total_Virus_level;
    double Average_Virus_level;
    int Number_cells;
};

//fill maps with all the user settings
void initSettings(std::map<std::string, int> &intSettings, std::map<std::string, double> &doubleSettings){
    // map the names of every setting to a default value
    // some settings are decimal and some are integer
    doubleSettings= {
        {"steepness_exposure", 1},
        {"steepness_infectious", 1},
        {"midpoint_infectious", 7200},
        {"steepness_recovery", 1},
        {"midpoint_recovery", 14400},
        {"virus_decay_rate", 0.001},
        {"vaccine_efficacy",0.75}
    };

    intSettings= {
        {"number_infectious_unmasked", 4},
        {"number_susceptible_vaccinated_unmasked", 6},
        {"number_susceptible_unvaccinated_unmasked",6},
        {"number_infectious_masked", 0},
        {"number_susceptible_vaccinated_masked", 0},
        {"number_susceptible_unvaccinated_masked",0},
        {"gridsize_x", 43},
        {"gridsize_y", 43},
        {"max_time", 30000},
        {"midpoint_exposure", 50}
    };
}

//Replace the default values in the maps with settings from the JSON file which are set by the user through the GUI
void configSettings(std::map<std::string, int> &intSettings, std::map<std::string, double> &doubleSettings){
    std::ifstream config("settings.json");

    // if a setting appears in the settings.json file, it will be overwritten in the map
    std::string variable, value;

    std::getline(config, variable); //get past first line of JSON file

    while(std::getline(config, variable, ':')){
        
        if(variable != "}"){ //ignore last line of JSON file
            std::getline(config, value);
            //clean the data
            value.erase(value.begin()); //remove whitespace
            value.erase(value.begin()); //remove quote
            if(value[value.length() - 1] == ','){ //remove comma from every line but the last
                value.erase(value.end() - 1);
            }
            value.erase(value.end() - 1); //remove quote

            variable.erase(variable.begin()); //remove quote
            variable.erase(variable.end() - 1); //remove quote

            // determine if the setting should be a double or an int
            if(doubleSettings.count(variable) == 1){
                doubleSettings[variable] = std::stod(value);
            }
            else if(intSettings.count(variable) == 1){
                intSettings[variable] = std::stoi(value);
            }
        }
    }
}

int main()
{
    //////////////////////////////////////////////////////////////////////////////////
    //read in the settings from the JSON file which is written in the python UI script
    //////////////////////////////////////////////////////////////////////////////////

    //create maps to hold integer and decimal settings
    std::map<std::string, int> intSettings;
    std::map<std::string, double> doubleSettings;

    //fill the maps with the settings
    initSettings(intSettings, doubleSettings);

    //set values of settings based on settings.JSON
    configSettings(intSettings, doubleSettings);

    //set variables based on the values in the maps
    int number_infectious_unmasked = intSettings["number_infectious_unmasked"];
    int number_susceptible_vaccinated_unmasked = intSettings["number_susceptible_vaccinated_unmasked"];
    int number_susceptible_unvaccinated_unmasked = intSettings["number_susceptible_unvaccinated_unmasked"];
    int number_infectious_masked = intSettings["number_infectious_masked"];
    int number_susceptible_vaccinated_masked = intSettings["number_susceptible_vaccinated_masked"];
    int number_susceptible_unvaccinated_masked = intSettings["number_susceptible_unvaccinated_masked"];
    int gridsize_x = intSettings["gridsize_x"];
    int gridsize_y = intSettings["gridsize_y"];
    int max_time = intSettings["max_time"];
    int midpoint_exposure = intSettings["midpoint_exposure"];

    double steepness_exposure = doubleSettings["steepness_exposure"];
    double steepness_infectious = doubleSettings["steepness_infectious"];
    double midpoint_infectious = doubleSettings["midpoint_infectious"];
    double steepness_recovery = doubleSettings["steepness_recovery"];
    double midpoint_recovery = doubleSettings["midpoint_recovery"];
    double virus_decay_rate = doubleSettings["virus_decay_rate"];
    double vaccine_efficacy = doubleSettings["vaccine_efficacy"];
    //////////////////////////////////////////////////////////////////////////////////


    //Declare an object
    srand ((unsigned) time(NULL));
    int total_number_people=number_infectious_unmasked+number_susceptible_vaccinated_unmasked+number_susceptible_unvaccinated_unmasked+number_infectious_masked+number_susceptible_vaccinated_masked+number_susceptible_unvaccinated_masked;
    Person person[total_number_people];
    Place places[gridsize_x*gridsize_y];
    double simtime,sum_prob;
    double current_sum;
    int PlaceBefore,PlaceAfter;
    double quanta_rate=14.28;
    ofstream record;
    fstream fin;
    string example;
    vector<string> RoomIDs; 
    int something;
   

    record.open("virus_levels.csv");
    
    record << fixed << setprecision(9) << "time,";
    for (int i=0;i<=(gridsize_x*gridsize_y)-1;i++) record << ",Virus_Closet"+to_string(i);
    record.close();

    

    for (int i=0;i<gridsize_y;i++)
    {
        for (int j=0;j<gridsize_x;j++)
        {
            places[gridsize_x*i+j].identifier="Closet" + to_string(gridsize_x*i+j+1);
            places[gridsize_x*i+j].xposition=j+1;
            places[gridsize_x*i+j].yposition=i+1;
            places[gridsize_x*i+j].Virus_level=0;
            places[gridsize_x*i+j].Occupied=false;
            places[gridsize_x*i+j].person_in_there=-9999;
            places[gridsize_x*i+j].west_prob=25;
            places[gridsize_x*i+j].east_prob=25;
            places[gridsize_x*i+j].north_prob=25;
            places[gridsize_x*i+j].south_prob=25;
            places[gridsize_x*i+j].total_prob=100;
           
        }
    }

    fin.open("layout.csv",ios::in);

    for (int i=0;i<(gridsize_x*gridsize_y);i++)
    {
        char* val;
        getline(fin,example);
        std::vector<char> v(example.begin(),example.end());
        places[i].west_prob=v[1]-48;
        places[i].east_prob=v[5]-48;
        places[i].north_prob=v[9]-48;
        places[i].south_prob=v[13]-48;
        places[i].identifier=v[17];
        for (int j=18;j<(v.size()-1);j++) places[i].identifier+=v[j];
        RoomIDs.push_back(places[i].identifier);
    }
    fin.close();

    for (int i=0;i<(gridsize_x*gridsize_y);i++)
    {
        places[i].total_prob=places[i].west_prob+places[i].east_prob+places[i].north_prob+places[i].south_prob;
        places[i].west_prob=places[i].west_prob/places[i].total_prob*100;
        places[i].east_prob=places[i].east_prob/places[i].total_prob*100;
        places[i].north_prob=places[i].north_prob/places[i].total_prob*100;
        places[i].south_prob=places[i].south_prob/places[i].total_prob*100;
   }

   //making Rooms
   std::sort(RoomIDs.begin(),RoomIDs.end());
   std::unique(RoomIDs.begin(),RoomIDs.end());
    auto vi = RoomIDs.begin();
    std::advance(vi,38);
   RoomIDs.erase(vi,RoomIDs.end());
   //Room Rooms[RoomIDs.size()];
   Room Rooms[38];
    for (int i=0;i<RoomIDs.size();i++) Rooms[i].identifier=RoomIDs.at(i); 
    for (int i=0;i<RoomIDs.size();i++) Rooms[i].Total_Virus_level=0;
    for (int i=0;i<RoomIDs.size();i++) Rooms[i].Average_Virus_level=0;
    for (int i=0;i<RoomIDs.size();i++) Rooms[i].Number_cells=0;

    //count up how many cells in each Room
   for (int i=0;i<(gridsize_x*gridsize_y);i++)
   {
       string this_room=places[i].identifier;
       int this_room_number=0;
       while(Rooms[this_room_number].identifier!=this_room) this_room_number++;
       Rooms[this_room_number].Number_cells++;
   }

    

    record.open("people.csv");
    record << fixed << setprecision(9) << "time, person_id, position, status, masked, vaccinated";
    record.close();
    //accessing data member
    //making infectious people
    for (int i=0;i<number_infectious_unmasked;i++)
    {
    person[i].identifier="Toga";
    
    
    do
    {
    person[i].location=floor(((float) rand()/RAND_MAX)*(gridsize_x*gridsize_y-1));
    person[i].xposition=(person[i].location % gridsize_x) + 1 ;
    person[i].yposition=((int) person[i].location/gridsize_x) + 1;
    }
    while (places[person[i].location].Occupied||(person[i].xposition<10&&person[i].yposition<10)||(person[i].xposition<10&&person[i].yposition>33)||(person[i].xposition>33&&person[i].yposition<10)||(person[i].xposition>33&&person[i].yposition>33)||(person[i].xposition>12&&person[i].xposition<33&&person[i].yposition>12&&person[i].yposition<33));
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
    person[i].movement_rate=0.1;
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

    //making vaccinated susceptible people
    for (int i=number_infectious_unmasked;i<(number_infectious_unmasked+number_susceptible_vaccinated_unmasked);i++)
    {
    person[i].identifier="Eri";
    do
    {
        person[i].location=floor(((float) rand()/RAND_MAX)*(gridsize_x*gridsize_y-1));
        person[i].xposition=(person[i].location % gridsize_x) + 1;
        person[i].yposition=((int) person[i].location/gridsize_x) + 1;
    }
    while (places[person[i].location].Occupied||(person[i].xposition<10&&person[i].yposition<10)||(person[i].xposition<10&&person[i].yposition>33)||(person[i].xposition>33&&person[i].yposition<10)||(person[i].xposition>33&&person[i].yposition>33)||(person[i].xposition>12&&person[i].xposition<33&&person[i].yposition>12&&person[i].yposition<33));
    places[person[i].location].Occupied=true;
    places[person[i].location].person_in_there=i;

    person[i].susceptible=true;
    person[i].exposed=false;
    person[i].infected=false;
    person[i].infectious=false;
    person[i].recovered=false;
    person[i].masked=false;
    person[i].vaccinated=true;
    person[i].age=7;
    person[i].movement_rate=0.1;
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

    //making susceptible unvaccinated people
    for (int i=(number_infectious_unmasked+number_susceptible_vaccinated_unmasked);i<(number_infectious_unmasked+number_susceptible_vaccinated_unmasked+number_susceptible_unvaccinated_unmasked);i++)
    {
    person[i].identifier="Eri";
    do
    {
        person[i].location=floor(((float) rand()/RAND_MAX)*(gridsize_x*gridsize_y-1));
        person[i].xposition=(person[i].location % gridsize_x) + 1;
        person[i].yposition=((int) person[i].location/gridsize_x) + 1;
    }
    while (places[person[i].location].Occupied||(person[i].xposition<10&&person[i].yposition<10)||(person[i].xposition<10&&person[i].yposition>33)||(person[i].xposition>33&&person[i].yposition<10)||(person[i].xposition>33&&person[i].yposition>33)||(person[i].xposition>12&&person[i].xposition<33&&person[i].yposition>12&&person[i].yposition<33));
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
    person[i].movement_rate=0.1;
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
    //masked infectious people
    for (int i=(number_infectious_unmasked+number_susceptible_vaccinated_unmasked+number_susceptible_unvaccinated_unmasked);i<(number_infectious_unmasked+number_susceptible_vaccinated_unmasked+number_susceptible_unvaccinated_unmasked+number_infectious_masked);i++)
    {
    person[i].identifier="Toga";
    
    
    do
    {
    person[i].location=floor(((float) rand()/RAND_MAX)*(gridsize_x*gridsize_y-1));
    person[i].xposition=(person[i].location % gridsize_x) + 1 ;
    person[i].yposition=((int) person[i].location/gridsize_x) + 1;
    }
    while (places[person[i].location].Occupied||(person[i].xposition<10&&person[i].yposition<10)||(person[i].xposition<10&&person[i].yposition>33)||(person[i].xposition>33&&person[i].yposition<10)||(person[i].xposition>33&&person[i].yposition>33)||(person[i].xposition>12&&person[i].xposition<33&&person[i].yposition>12&&person[i].yposition<33));
    places[person[i].location].Occupied=true;
    places[person[i].location].person_in_there=i;

    person[i].susceptible=false;
    person[i].exposed=false;
    person[i].infected=false;
    person[i].infectious=true;
    person[i].recovered=false;
    person[i].masked=true;
    person[i].vaccinated=false;
    person[i].age=16;
    person[i].movement_rate=0.1;
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

    //making vaccinated susceptible masked people
    for (int i=(number_infectious_unmasked+number_susceptible_vaccinated_unmasked+number_susceptible_unvaccinated_unmasked+number_infectious_masked);i<(number_infectious_unmasked+number_susceptible_vaccinated_unmasked+number_susceptible_unvaccinated_unmasked+number_infectious_masked+number_susceptible_vaccinated_masked);i++)
    {
    person[i].identifier="Eri";
    do
    {
        person[i].location=floor(((float) rand()/RAND_MAX)*(gridsize_x*gridsize_y-1));
        person[i].xposition=(person[i].location % gridsize_x) + 1;
        person[i].yposition=((int) person[i].location/gridsize_x) + 1;
    }
    while (places[person[i].location].Occupied||(person[i].xposition<10&&person[i].yposition<10)||(person[i].xposition<10&&person[i].yposition>33)||(person[i].xposition>33&&person[i].yposition<10)||(person[i].xposition>33&&person[i].yposition>33)||(person[i].xposition>12&&person[i].xposition<33&&person[i].yposition>12&&person[i].yposition<33));
    places[person[i].location].Occupied=true;
    places[person[i].location].person_in_there=i;

    person[i].susceptible=true;
    person[i].exposed=false;
    person[i].infected=false;
    person[i].infectious=false;
    person[i].recovered=false;
    person[i].masked=true;
    person[i].vaccinated=true;
    person[i].age=7;
    person[i].movement_rate=0.1;
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

    //making susceptible masked unvaccinated people
    for (int i=(number_infectious_unmasked+number_susceptible_vaccinated_unmasked+number_susceptible_unvaccinated_unmasked+number_infectious_masked+number_susceptible_vaccinated_masked);i<(total_number_people);i++)
    {
    person[i].identifier="Eri";
    do
    {
        person[i].location=floor(((float) rand()/RAND_MAX)*(gridsize_x*gridsize_y-1));
        person[i].xposition=(person[i].location % gridsize_x) + 1;
        person[i].yposition=((int) person[i].location/gridsize_x) + 1;
    }
    while (places[person[i].location].Occupied||(person[i].xposition<10&&person[i].yposition<10)||(person[i].xposition<10&&person[i].yposition>33)||(person[i].xposition>33&&person[i].yposition<10)||(person[i].xposition>33&&person[i].yposition>33)||(person[i].xposition>12&&person[i].xposition<33&&person[i].yposition>12&&person[i].yposition<33));
    places[person[i].location].Occupied=true;
    places[person[i].location].person_in_there=i;

    person[i].susceptible=true;
    person[i].exposed=false;
    person[i].infected=false;
    person[i].infectious=false;
    person[i].recovered=false;
    person[i].masked=true;
    person[i].vaccinated=false;
    person[i].age=7;
    person[i].movement_rate=0.1;
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

        //zeroing out the probability of becoming exposed
        for (int i=0;i<(total_number_people);i++) person[i].exposure_probability=0;

        //calculating probability of becoming exposed
        for (int i=0;i<(total_number_people);i++) {if (person[i].susceptible) if(places[person[i].location].Virus_level>0) person[i].exposure_probability=1/(1+exp(-steepness_exposure*(places[person[i].location].Virus_level-midpoint_exposure)));}

        //multiplying exposure probability with mask modifier
        for (int i=0;i<(total_number_people);i++) {if (person[i].masked) person[i].exposure_probability=0.05*person[i].exposure_probability;}

        //multiplying exposure probability with vaccine efficacy
        for (int i=0;i<(total_number_people);i++) {if (person[i].vaccinated) person[i].exposure_probability=(1-vaccine_efficacy)*person[i].exposure_probability;}

        //calculating probability of becoming infectious
        for (int i=0;i<(total_number_people);i++) {
            if (person[i].exposed) {
            person[i].infectious_probability=1/(1+exp(-steepness_infectious*((simtime-person[i].exposure_time)-midpoint_infectious)));
            }
        }

        //calculating probability of recovering
        for (int i=0;i<(total_number_people);i++) {
            if (person[i].infectious) {
            person[i].recovery_probability=1/(1+exp(-steepness_recovery*((simtime-person[i].infectious_time)-midpoint_recovery)));
            }
        }

        //Gillespie algorithm
        //calculating sum of probabilities across persons, with movement rate, exposure rate
        for (int i=0;i<(total_number_people);i++) {person[i].sum_prob=person[i].movement_rate+person[i].exposure_rate*person[i].exposure_probability+person[i].infectious_rate*person[i].infectious_probability+person[i].recovery_rate*person[i].recovery_probability;}

        //calculating sum of probabilities overall
        sum_prob=0;
        for (int i=0;i<(total_number_people);i++) {sum_prob=sum_prob+person[i].sum_prob;}

    //get the sojourn time
    
    v1=(float) rand()/RAND_MAX;
    if (v1==0) v1=0.00000000000001;
    sojourn_time=(1/sum_prob)*log(1/v1);
    simtime=simtime+sojourn_time;
    //record << "\nThe time is " << simtime;

     //increase virus level by some value for all occupied places; and decrease the Virus level
    for (int i=0; i<=(gridsize_x*gridsize_y-1); i++)
    {    
    if (places[i].Occupied)
        {
                if (person[places[i].person_in_there].infectious==true) 
                    {
                        if (person[places[i].person_in_there].masked) 
                        {
                            places[i].Virus_level=places[i].Virus_level+sojourn_time*quanta_rate*0.05;
                        }
                        else
                        {
                            places[i].Virus_level=places[i].Virus_level+sojourn_time*quanta_rate;                        
                        }
                    }
        }
    places[i].Virus_level=places[i].Virus_level-(places[i].Virus_level*sojourn_time*virus_decay_rate);
    }

    //homogenize virus levels between places with the same identifier

    for (int i=0;i<RoomIDs.size();i++) Rooms[i].Total_Virus_level=0;

    for (int i=0;i<(gridsize_x*gridsize_y);i++)
    {   
       string this_room=places[i].identifier;
       int this_room_number=0;
       while(Rooms[this_room_number].identifier!=this_room) this_room_number++;
       Rooms[this_room_number].Total_Virus_level+=places[i].Virus_level;    
    }

    for (int i=0;i<RoomIDs.size();i++) Rooms[i].Average_Virus_level=Rooms[i].Total_Virus_level/Rooms[i].Number_cells;

    for (int i=0;i<(gridsize_x*gridsize_y);i++)
    {   
       string this_room=places[i].identifier;
       int this_room_number=0;
       while(Rooms[this_room_number].identifier!=this_room) this_room_number++;
       places[i].Virus_level=Rooms[this_room_number].Average_Virus_level;    
    }


    //movement
    //lets see who will do something
    v1=sum_prob*((float) rand()/RAND_MAX) ;
    current_person=total_number_people;
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
    
    for (int i=0;i<=(gridsize_x*gridsize_y-1);i++)
    {
    record << "," << places[i].Virus_level;
    }

    record.close();

    record.open("people.csv",std::fstream::app);

    for (int i=0;i<(total_number_people);i++)
    {
        int j=-1;
        do
        {
            j++;
        }while ((places[j].xposition!=person[i].xposition)||(places[j].yposition!=person[i].yposition));
        record << "\n" << simtime << "," << i << "," << j << "," << person[i].status << "," << person[i].masked << "," << person[i].vaccinated;
    }
    record.close();

    }
    while(simtime < max_time);

   
    

    return 0;
}