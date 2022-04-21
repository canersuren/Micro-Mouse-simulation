# Micro-Mouse-simulation
This simulation is for testing maze solving algorithms.

## How to use

Maze solving algorithms should be writed inside the function below to the scope marked with "//Write your algorithm" comment which is in the "Sim.cpp" file.

```
start->initialize(simBox, [](SimBox* simBox, int eventCode)
{
  if (eventCode == FL_RELEASE && !simBox->hasStarted)
	{
		simBox->hasStarted = true;

		//Write your algorithm here

		return 1;
   }
	  
    return 0;
});

```

Above this function, there is statement as

```
	this->simBox = new SimBox("map1.txt", 13, 2, {12, 0}, 0.2);
```
this creates the simBox object which does everything for controlling simulation.
In the constructor first arguement is your map files name. Map file must be in "Micro-Mouse-simulation/Labirent Simülasyon/" direction. Else you have to write full adress of map file like "C:\Users\caner\Desktop\c++\Micro-Mouse-simulation\Labirent Simülasyon\map1.txt". How you can create map files will be mentioned below. Second arguement is maze map's dimension as boxes as int type. Third arguement is robot's starting direction. Forth arguement is robot's starting position. Fifth algorithm is simulation's animation speed. 
### Using simulation functions

simBox object provides

```
bool readSensorFront();
bool readSensorRight();
bool readSensorLeft();

bool moveForward();
//return 1 if move is succesfull, 0 if move is blocked 

void turnRight();
void turnLeft();

```
functions and 2 values

```
Position2D robotPos;
int robotDirection;
```

Sensor reading functions returns 1 if there is a wall returns 0 if there is no wall. robotDirection value returns 0 for up 1 for right 2 for down 3 for left. robotPos returns Position2D object that whas 2 attributes: "x" and "y". x value starts from 0; increments 1 by each box going from left to right. y values starts from 0; increments 1 by each box going from up to down. 

## creating map files

For creating map files; start from top left box and start going right by adding "1" if there is a vertical wall "0" if there is no vertical wall. When the row is over move to the nex row and continue until vertical walls are over. After vertical rows are over go to the top left corner again and start going down by adding "1" if there is a horizontal wall "0" if there is no horizontal wall. When the column is over move to the nex column and continue until horizontal walls are over.
##### For example
<img src="https://user-images.githubusercontent.com/34722832/164459213-e41e11e6-4094-44e6-9ddf-847c56f5a53f.jpeg" width="350" />

this maps file would be like this:

```
11110
00100
00000
00100
10110
00010
00110
01110
00000
01100
11100
00100
```

