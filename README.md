# Embedded-Systems-Lab3
Assignment provided by Embedded Systems (ECGR 3101)
This project was performed using a board EK-TM4C123GXL and Code Composer Studio

LYNX Blue Line Extension 
The 9.3-mile LYNX Rapid Transit Service Blue Line Extension provides light rail that and the University City area. It was planned to carry its first paying passengers in [August 2012] [August 2017] [March 16th, 2018] end of Spring 2018.  (It is about to celebrate its three year anniversary.) At a total cost of about $1.2 billion, city leaders hope that the new leg of the Blue Line will do more than move people from Point A to Point B. Let's do a project based on this new infrastructure.

 

Objective
The objective of this lab is two-fold. First, it introduces the students to the general-purpose Timer/Counters on the TI TM4C123GXL board. Second, it integrates multiple peripherals including both inputs and outputs of the GPIO peripherals. The hardware aspects are not challenging; for most students, most of the effort will be the software required to integrate the system as a whole.

Materials and Resources
For this lab you will need:
-TM4C123GXL Development Board
-solderless breadboard (at least half-size, 400 tie-points)
-six LEDs (ideally, 4 blue, 1 red, and 1 green)
-assorted resistors (six approximately 100  to 1k ; at least one 100k  
-M-to-F or M-to-M Jumper Wires
-one slide switch (instructor has some to loan)

Procedure
The last four stops on the LYNX Blue Line Extension are University City Blvd, McCollough, JW Clay, and UNC-Charlotte Main. We are going to model the train's movement through these four stations.  The blue LEDs are lit when the train is stopped at a specific station.  Pedestrians crossing at McCollough do not have a bridge so it needs a Walk/Don't Walk'' signal which is represented by the red and green LEDs.  For simplicity, the train moves from UNC-Charlotte Main to University City Blvd and then back (visiting University City Blvd, McCollough, JW Clay, along the way).  Whenever the train is approaching McCollough station (at half the distance between the adjacent stations) the "Don't Walk" red LED is illuminated.  Otherwise, the "Walk" green LED is illuminated.

The train moves at constant speed and, again, to simplyify we'll use a fixed, 2 second travel time between each station.

The train will spend 1 second motionless at each station.
(PNG of the train station map will be called TrainMap.png)

However, the train conductor may notice congestion and whilenat a station, the conductor can keep the train at the station until all passengers have disembarked and boarded the train.

To control this, a slide switch is used to hold the train. When the switch is engaged, the train will not leave the station and remain motionless.  When the switch is not engaged, the train will proceed at the scheduled rate.

After completing the Pre-Lab, the following steps are recommended. There are other approaches that meet the requirements but if you need it, here is some guidance.

-Create a plan. (This might include a schematic, net list, physical layout for the external circuits. It would also include an approach to the software solution.)

-Place four blue transistors on the breadboard with the appropriate sized resistors in series. Likewise, place the red and green LED-resistor pairs.

-Locate the GPIO pins on the developer board (recommended: PORTB) and jumper the I/O pins to the breadboard.

-Add the switch to the breadboard and connect via jumper wires to developer board.

-Write the software.  The recommended approach is to work incrementally: test one LED (as described in the pre-lab) and test the switch.  Next, develop the control logic that moves the train, forwards and backwards, and take into account the hold switch.

-Review the software for complete documentation and consistency.

Requirements
-The train moves back and forth from the two end points.
-The train pauses with the switch.
-Crosswalk LEDs function correctly with approaching/departing train.
