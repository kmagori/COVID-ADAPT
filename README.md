# COVID-ADAPT v.0.1.0

COVID-ADAPT is an individual-based stochastic discrete-space simulation model of airborne disease transmission and infection control practices. It was inspired by the COVID-19 pandemic in 2020-21, but can be used to model any disease transmitted by airborne droplets between hosts with a Susceptible-Exposed-Infectious-Recovered transmission cycle. 

This software is licensed under a Creative Commons BY-NC-SA license, which allows copying and redistributing the material in any medium or format, and adapt, remix, transform, and build upon the material. However, you must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use. You may not use the material for commercial purposes, without my express permission, especially not selling the software or any derivatives of it. If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original. See more details here:

https://creativecommons.org/licenses/by-nc-sa/4.0/

Disease transmission occurs in the model by infectious individuals exhaling viral particles (as a unit of quanta) which contaminate a location. Susceptible individuals who enter the location can become exposed with a specific probability as a function of the amount of quanta in the location they reside in. Exposed individuals become infectious some time after they became exposed. Infectious individuals recover at a specific time after they became infectious. Quanta of particles accumulate in each cell as a function of the time an infectious individual spent in each cell, and decay exponentially with time in each cell. The model assumes discrete space, meaning that the facility being modeled is broken up into a grid of rectangular cells, with a size of either 6 feet by 6 feet, or 2 feet by 2 feet, or any other pre-defined size. Individuals can move according to the von Neumann neighborhood, either west, east, north or south. If an individual moves, the probability that the individual will move to one of the four cardinal directions is determined by four probabilities assigned to each cell. Cells on the edges are defined as having a probability of zero for individuals attempting to go off the map, while cells in the corners are defined by having a probability of zero for going to two of the four cardinal directions off the map.  

The model is an event-based stochastic model, with the following four events occuring, based on their propensity functions: (1) movement; (2) susceptible individuals becoming exposed; (3) exposed individuals becoming infectious; (4) infectious individuals recovering. Viral particle quanta are recalculated at each event in each cell, in a deterministic fashion.

Input to the model is currently at the code level. Output is generated as a form of two CSV files. Both files are added to following each event. One of the files (people.csv) contain the time of the event; the location of all the individuals in the grid at that time; the infection status of all the individuals; and whether or not they are masked. Another file (viral_levels.csv) contains for each timestep the viral quanta levels in each of the cells, enumerated from the top left corner, starting with location 0, going to the right and down. Visualization of the map, and the dynamics of the location and infection status of the two individuals and the viral levels in each cell is done in R, and saved into an animated GIF (https://github.com/kmagori/COVID-ADAPT/blob/main/raster.gif). The movie shows a specific run of the model, showing the grid at each timepoint with susceptible individuals depicted as green, exposed individuals as orange, infectious individuals as red, and recovered individuals as blue. Unmasked and masked individuals are denoted by different symbols. The level of viral quanta in each cell is shown as the shade of red, relative to the legend bar on the right hand side. 

**Version 0.3.0 (4/26/2021)**

This version incorporates "rooms", which are sets of gridcells with the same Room attributes. Viral quanta is averaged between cells belonging to the same "room" at the end of each event. 

**Version 0.2.0 (1/7/2021)**

Incorporated more than 2 people, both into core C++ code as well as to R front-end visualization. Number of people is limited to the number of cells in the grid, as initialization won't allow more than one person per grid cell. There's no error handling for this, it would just result in infinite loop.

**Prototype v0.1.0 (12/21/2020)**

This prototype version of the code is functional, but neither easily useable, realistic, or computationally efficient. It uses the Gillespie algorithm to model stochastic events (https://en.wikipedia.org/wiki/Gillespie_algorithm). This means that at every event two random numbers are generated. One random number is used to calculate the sojourn time, telling us the time at which the next event occurs, calculated as an exponentionally distributed random variable, proportional to the overall propensity of events occuring. This means that when the probability of events occuring over unit time is overall higher, the sojourn time will be shorter. The second random number is used to pick the specific event that occurs, and the individual to whom this event occurs, by adding up the probability of each event occuring across all individuals, and selecting the event where the random number times the sum of the probabilities falls.  

Issues with and assumptions of this version:

1. Only two individuals 

2. Probability for a susceptible individual being exposed is calculated using the virus level in her/his current location at the current time. This assumes that virus levels in that cell will stay constant until the next event, which is unlikely to be correct. It is possible to calculate the probability of an event with non-homogeneous propensity function, but it requires the use of the next reaction method instead of the Gillespie algorithm (https://pubs.acs.org/doi/10.1021/jp993732q), which is not yet implemented.

3. Probability for an exposed individual to become infectious is calculated using the current time. This potentially overshoots the time when they become infectious as it only occurs after a different event occurs after the incubation period. There are methods to incorporate delayed reactions into stochastic algorithms, based on the next reaction method, but they have not been implemented yet (https://www.math.wisc.edu/~anderson/papers/AndNRM.pdf).

4. Probability for an infectious individual to recover is calculated using the current time. There is a similar concern here as #3, with a similar solution.

5. The number of viral particles contaminating each cell is the same as the number of minutes spent in the cell for an infectious host, irrespective of mask wearing or how long the person was infectious.

6. The probability of being exposed is a logistic function (https://en.wikipedia.org/wiki/Logistic_function) of viral particles with a steepness of 1 and a midpoint of 50 viral particles. 

7. The probability of becoming infectious is a logistic function of the time since being exposed with a steepness of 1 and a midpoint of 7200 minutes (5 days).

8. The probability of recovering is a logistic function of the time since becoming infectious with a steepness of 1 and a midpoint of 14,400 minutes (10 days).

9. The rate at which viral particles decay is 0.001 per minute, and the new virus level at a time point is calculated as the prior virus level plus the new viruses exhaled by an infectious individual, minus the prior virus level times the sojourn time times the decay rate. The viral decay rate is the same at each location at this point.

10. At this point, there is no diffusion process of viral particles implemented in the model, and therefore viral particles will not spread from the cell where an infectious individual was present. This means that if there is no movement, a susceptible individual will never get infected by an infectious individual until that person recovers. There is also no airflow incorporated into the model. 

11. There is no death/birth or migration implemented in the model. Individuals who become hospitalized could be modeled as leaving the model as an event. New individuals could be added to the model as an event.

12. There is no facility at this point to import a map into the model. This would have to be done by hand, defining the probabilities of permitted directions of movement for each cell. Obstacles such as walls could be defined as zero probability of moving in that direction from adjoining cells. Inaccessible spaces and larger barrier (such as the inside of a grocery aisle) could be modelled as cells that have zero probability of movement from all directions.

13. For now, individuals move according to Brownian motion, such that they have equal probability of moving to any of the permitted directions. This can be relaxed by making the probabilities unequal for different directions for specific cells, making directional flow possible, such as through grocery isles. Individual stochasticity can be added by implementing a modifier that can be different for each individual that can change how much they obey the pre-determined traffic flow of the cells. Finally, movement probability can be modified based on the distance of other individuals, either through line-of-sight or Euclidean, where individuals are either more or less likely towards others. However, these are not yet implemented.

14. For now, two individuals are not allowed to be present in the same cell, and if they would end up in the same cell, the movement will be aborted, and they will stay in the same cell. However, this can be relaxed in subsequent versions. This could include direct transmission when an infectious and a susceptible individual are in the same cell, but this is not implemented.

15. Noone is vaccinated yet. However, vaccination status will likely change the probability of being exposed, or the level of viral particles needed to get exposed, or whether or not someone becomes infectious, or the viral particle levels that they contaminate their location with. 

16. Mask status is not yet implemented. Mask status will be implemented in changing the probability of being exposed, or the level of viral particles needed to get exposed, or the viral particle levels an infectious individual contaminates their location with.

17. The model currently only allows movement on a flat level, leaving the possibility of upwards or downwards movement for later implementation of the model with multiple stories and/or stairs.



