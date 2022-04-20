#include <iostream>
#include "Sim.h"
#include "FL/Fl_Button.H"
#include <FL/fl_draw.H>
#include "SimBox.h"
#include <vector>

class CustomButton : Fl_Button
{
public:
	CustomButton(int a, int c, int b, int d, const char *label)
		: Fl_Button(a, c, b, d, label)
	{}

	void initialize(SimBox *simBox, int(*func)(SimBox*, int))
	{
		this->simBox = simBox;
		this->Handler = func;
	}
	
	int handle(int event) override
	{
		Fl_Button::handle(event);
		return Handler(simBox, event);
	}

private:
	SimBox *simBox;
	
	int(*Handler)(SimBox*, int);
};


Position2D* boxAround(SimBox* sim)
{
	Position2D boxes[4];

	int x = 0;
	int y = 0;

	x = sim->robotPos.x;
	y = sim->robotPos.y - 1;
	boxes[0] = Position2D{ x, y };

	x = sim->robotPos.x + 1;
	y = sim->robotPos.y;
	boxes[1] = Position2D{ x, y };

	x = sim->robotPos.x;
	y = sim->robotPos.y + 1;
	boxes[2] = Position2D{ x, y };

	x = sim->robotPos.x - 1;
	y = sim->robotPos.y;
	boxes[3] = Position2D{ x, y };


	return boxes;
}

void step(SimBox* sim, int** allBoxes)
{
	Position2D* list = boxAround(sim);
	Position2D boxes[4] = { list[0], list[1], list[2], list[3] };


	int values[4] = { 10000, 10000, 10000, 10000 };
	for (int i = 0; i < 4; i++)
	{

		if (boxes[i].x < 0 || boxes[i].y < 0)
		{
			values[i] == 10000;

		}
		else
		{

			values[i] = allBoxes[boxes[i].x][boxes[i].y];
		}
	}



	int front = sim->readSensorFront();
	int right = sim->readSensorRight();
	int left = sim->readSensorLeft();

	switch (sim->robotDirection)
	{
	case 0:
		if (front)
			values[0] = 10000;

		if (right)
			values[1] = 10000;

		if (left)
			values[3] = 10000;
		break;
	case 1:
		if (front)
			values[1] = 10000;

		if (right)
			values[2] = 10000;

		if (left)
			values[0] = 10000;
		break;
	case 2:
		if (front)
			values[2] = 10000;

		if (right)
			values[3] = 10000;

		if (left)
			values[1] = 10000;
		break;
	case 3:
		if (front)
			values[3] = 10000;

		if (right)
			values[0] = 10000;

		if (left)
			values[2] = 10000;
		break;
	}


	int min = 10000;
	int index = 0;

	for (int i = 0; i < 4; i++)
	{
		if (min > values[i])
		{
			min = values[i];
			index = i;
			/*if (index == sim->robotDirection)
				break;*///Arbitrary
		}
	}

	int turnCount = sim->robotDirection - index;
	if (turnCount > 0)
	{
		while (true)
		{
			if (index == sim->robotDirection)
				break;
			else
				sim->turnLeft();
		}
	}
	else
	{
		while (true)
		{
			if (index == sim->robotDirection)
				break;
			else
				sim->turnRight();
		}
	}





	/*if (front && right && left)
	{
		std::cout << "Dead-end";
		allBoxes[sim->robotPos.x][sim->robotPos.y]++;
	}*/

	sim->moveForward();
	allBoxes[sim->robotPos.x][sim->robotPos.y]++;

}
Sim::Sim()
{
	window = new Fl_Window(1000, 800);

	//First arguement is map's filename
	//Second arguement is dimension of map as bo count
	//Third arguement is robot's direction (0 for up, 1 for right, 2 for down, 3 for left)
	//Forth arguement is robot's position (starting from 0)
	//fifth arguement is simulation speed, waiting time for each animation(moving and turning)
	this->simBox = new SimBox("UORG.txt", 13, 2, {12, 0}, 0.2);


	CustomButton* start = new CustomButton(simBox->simBoxWidth + 50, 100, 100, 50, "Start");
	start->initialize(simBox, [](SimBox* simBox, int eventCode) {
		if (eventCode == FL_RELEASE && !simBox->hasStarted)
		{
			simBox->hasStarted = true;

			


			int** array;
			array = new int* [simBox->boxCount+1];
			for (int i = 0; i < simBox->boxCount + 1; i++)
				array[i] = new int[simBox->boxCount + 1];
			
			for (int i = 0; i < simBox->boxCount + 1; i++)
			{
				for (int j = 0; j < simBox->boxCount + 1; j++)
				{
					array[i][j] = 0;
				}
			}

			array[simBox->robotPos.x][simBox->robotPos.y] == 1;

			while (true)
				step(simBox, array);


			

			return 1;
		}
		return 0;
		});

	/*Fl_Button* button = new Fl_Button(20, 40, 300, 100, "Button gibi button");
	button->color(FL_GRAY);
	button->callback(Sim::button_callback, nullptr);
	button->color(FL_WHITE);*/

	window->end();
	window->show();
	Fl::run();
}


