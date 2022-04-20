#include "Position2D.h"
#include "FL/Fl_Box.H"
#include "FL/fl_draw.H"
#include <vector>

#ifndef SimBox_H
#define SimBox_H

class SimBox : Fl_Box
{
public:
	class Wall
	{
	public:
		Wall(Position2D wallPosition, bool wallSide)
			: wallPos(wallPosition), side(wallSide) {}

		Position2D wallPos;//koordinat sol alttan baþlatýlýyor
		bool side;//0 ise dikey duvar 1 ise yatay duvar
		//Geniþlik ve uzunluk deðerleri dik duran bir duvar için
		static const int wallWidth =4;
		static const int wallHeight = 48;
	};


	SimBox(int x, int y, int w, int h);

	SimBox(const char* mapFileName, int boxCounts, int robotDirection, Position2D robotPosition, float simSpeed);

	void draw() override;

	void draw_box(Fl_Boxtype b, int x, int y, int w, int h, Fl_Color c);

	void draw_Wall(Wall wall, Fl_Color c);

	void setRobotPos(Position2D pos2D);
	
	void drawRobot();

	void setRobotDirection(int dir);
	//0 up, 1 right, 2 down, 3 left

	void waitForAnimation();

	void drawAgain()
	{
		Fl_Box::redraw();
	}

	bool isDrawn = false;
	bool hasStarted = false;

	bool readSensorFront();
	bool readSensorRight();
	bool readSensorLeft();

	bool moveForward();
	//return 1 if move is succesfull, 0 if move is blocked 
	
	void turnRight();
	void turnLeft();

	int boxCount;
	const char* mapName;

	int simBoxWidth;
	int simBoxHeight;

	float simulationSpeed;

	Position2D robotPos;
	int robotDirection;

	const int robotWidth = 25;


	std::vector<Wall> walls;

};

#endif