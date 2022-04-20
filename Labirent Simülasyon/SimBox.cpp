#include "SimBox.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <FL/FL.h>

void drawWalls(SimBox* simBox) {
	std::ifstream WallsFile(simBox->mapName);


	std::string line;
	int y = 0;
	while (std::getline(WallsFile, line)) {
		for (int x = 0; x < simBox->boxCount - 1; x++)
		{
			if (line[x] == '1' &&  y < simBox->boxCount)
			{
				SimBox::Wall wall(Position2D{x, y}, 0);
				simBox->walls.push_back(wall);
				simBox->draw_Wall(wall, FL_WHITE);
				
			}
			else if (line[x] == '1' && y >= simBox->boxCount)
			{
				SimBox::Wall wall(Position2D{ y - simBox->boxCount , x }, 1);
				simBox->walls.push_back(wall);
				simBox->draw_Wall(wall, FL_WHITE);
				
			}
		}
		y++;
	}

	WallsFile.close();


}

void SimBox::draw()
{

	//4 tane for döngüsü sýnýrlarýn çizilmesi için
	draw_box(FL_FLAT_BOX, 0, 0, boxCount * SimBox::Wall::wallHeight + Wall::wallWidth, boxCount * SimBox::Wall::wallHeight + Wall::wallWidth, FL_WHITE);
	draw_box(FL_FLAT_BOX, SimBox::Wall::wallWidth, SimBox::Wall::wallWidth, boxCount * SimBox::Wall::wallHeight - SimBox::Wall::wallWidth, boxCount * SimBox::Wall::wallHeight - SimBox::Wall::wallWidth, FL_BLACK);
	drawWalls(this);
	drawRobot();

}



SimBox::SimBox(const char* mapFileName, int boxCounts, int robotDirection, Position2D robotPosition, float simSpeed)
	:SimBox(0, 0, 0, 0)
{
	this->simulationSpeed = simSpeed;
	this->robotDirection = robotDirection;
	this->robotPos = robotPosition;
	boxCount = boxCounts;
	mapName = mapFileName;
	this->resize(0, 0, boxCount * Wall::wallHeight, boxCount * Wall::wallHeight);

	simBoxWidth = boxCount * Wall::wallHeight;
	simBoxHeight = boxCount * Wall::wallHeight;
}


SimBox::SimBox(int x, int y, int w, int h)
	:Fl_Box(x, y, w, h), robotDirection(2)
{
	this->robotPos = Position2D{ 0, 0 };
}

void SimBox::draw_box(Fl_Boxtype b, int x, int y, int w, int h, Fl_Color c)
{
	fl_draw_box(b, this->x() + x, this->y() + y, w, h, c);
}



void SimBox::setRobotPos(Position2D pos2D)
{
	this->robotPos = pos2D;
}

void SimBox::setRobotDirection(int dir) 
{
	SimBox::robotDirection = dir;//same as "this"
}

void SimBox::drawRobot()
{

	int posX = this->robotPos.x * Wall::wallHeight + 
		(((Wall::wallHeight - Wall::wallWidth)- robotWidth) / 2) ;
	int posY = this->robotPos.y * Wall::wallHeight +
		(((Wall::wallHeight - Wall::wallWidth) - robotWidth) / 2);

	SimBox::draw_box(FL_FLAT_BOX, posX, posY, robotWidth, robotWidth, FL_WHITE);

	switch (this->robotDirection)
	{
	case 0:
		SimBox::draw_box(FL_FLAT_BOX, posX + 10, posY, 5, 10, FL_RED);
		break;

	case 1:
		SimBox::draw_box(FL_FLAT_BOX, posX + 15, posY + 10, 10, 5, FL_RED);
		break;

	case 2:
		SimBox::draw_box(FL_FLAT_BOX, posX + 10, posY + 15, 5, 10, FL_RED);
		break;

	case 3:
		SimBox::draw_box(FL_FLAT_BOX, posX, posY + 10, 10, 5, FL_RED);
		break;
	}
}

bool SimBox::readSensorFront()
{
	Position2D wallToSearch;
	bool side;
	
	if (robotDirection == 1 || robotDirection == 3)
	{
		if (robotDirection == 1)
		{
			if (robotPos.x == boxCount - 1)
				return 1;
			else
				wallToSearch = Position2D{ this->robotPos.x, this->robotPos.y };
		}
		else if (robotDirection == 3)
		{
			if (robotPos.x == 0)
				return 1;
			else
				wallToSearch = Position2D{ this->robotPos.x - 1, this->robotPos.y };
		}
		side = 0;
	}
	else if (robotDirection == 0 || robotDirection == 2)
	{
		if (robotDirection == 0)
		{
			if (robotPos.y == 0)
				return 1;
			else
				wallToSearch = Position2D{ this->robotPos.x, this->robotPos.y - 1 };
		}
			

		else if (robotDirection == 2)
		{
			if (robotPos.y == boxCount - 1)
				return 1;
			else
				wallToSearch = Position2D{ this->robotPos.x, this->robotPos.y };
		}
		side = 1;
	}

	for (const Wall& wall : walls)
	{
		if (wall.wallPos == wallToSearch && wall.side == side)
		{
			//std::cout << wall.wallPos.x << " " << wall.wallPos.y << std::endl;
			return 1;
		}
	}
	return 0;
}

bool SimBox::readSensorRight()
{
	int newDir = (this->robotDirection + 1) % 4;
	robotDirection = newDir;

	bool isThereWall = readSensorFront();
	
	newDir = (this->robotDirection + 3) % 4;
	robotDirection = newDir;

	return isThereWall;
}
bool SimBox::readSensorLeft()
{
	int newDir = (this->robotDirection + 3) % 4;
	robotDirection = newDir;

	bool isThereWall = readSensorFront();

	newDir = (this->robotDirection + 1) % 4;
	robotDirection = newDir;

	return isThereWall;
}


void SimBox::waitForAnimation()
{
	Fl_Timeout_Handler handler = [](void* a)
	{
		SimBox* simBox = (SimBox*)a;
		//std::cout << "Timeout function";
		simBox->isDrawn = true;
	};


	Fl::add_timeout(simulationSpeed, handler, this);

	this->drawAgain();

	//std::this_thread::sleep_for(std::chrono::milliseconds(150));
	while (!isDrawn)
	{
		Fl::wait();
	}
	isDrawn = false;
}

bool SimBox::moveForward()
{
	if(readSensorFront())
	{
		//std::cout << "AAA";

		//drawAgain();
		return 0;
	}


	switch (this->robotDirection) 
	{
	case 0:
		robotPos = Position2D{ robotPos.x, robotPos.y - 1 };
		break;

	case 1:
		robotPos = Position2D{ robotPos.x + 1, robotPos.y};
		break;

	case 2:
		robotPos = Position2D{ robotPos.x, robotPos.y + 1};
		break;

	case 3:
		robotPos = Position2D{ robotPos.x - 1, robotPos.y};
		break;
	}

	

	//std::cout << this->robotPos.x << " " << this->robotPos.y << std::endl;

	
	waitForAnimation();

	return 1;
}


void SimBox::turnRight()
{
	int newDir = (this->robotDirection + 1) % 4;
	this->robotDirection = newDir;

	waitForAnimation();
}
void SimBox::turnLeft()
{
	int newDir = (this->robotDirection + 3) % 4;
	this->robotDirection = newDir;
	
	waitForAnimation();
}

void SimBox::draw_Wall(Wall wall, Fl_Color c)
{
	if (wall.side)
	{//Yatay duvar
		int posX = (wall.wallPos.x) * wall.wallHeight;
		int posY = (wall.wallPos.y + 1) * wall.wallHeight;
		draw_box(FL_FLAT_BOX, posX, posY, wall.wallHeight, wall.wallWidth, c);
	}
	else
	{//Dikey duvar
		int posX = (wall.wallPos.x + 1) * wall.wallHeight;
		int posY = ((wall.wallPos.y) * wall.wallHeight);
		draw_box(FL_FLAT_BOX, posX, posY, wall.wallWidth, wall.wallHeight, c);
	}
}