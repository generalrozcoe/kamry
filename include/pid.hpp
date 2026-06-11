#ifndef Pid
#define Pid
#include <SFML/Graphics.hpp>
#include <iostream>


class PID{
public:
	int ID;
	int offset;
	float data;
	std::pair<int, int> range;
	sf::Texture texture;

	
	PID(int ID, int offset, int low, int high, std::string txt);

};


#endif