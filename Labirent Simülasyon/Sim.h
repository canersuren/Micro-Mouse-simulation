#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include "SimBox.h"

#ifndef Sim_H
#define Sim_H



class Sim
{
public:
	Sim();
	static void button_callback(Fl_Widget* widget, void* data)
	{
		std::cout << widget->label();
	}
private:
	Fl_Window* window;
	SimBox* simBox;
};


#endif
