#include "pid.hpp"

PID::PID(int ID, int offset, int low, int high, std::string txt){
	this->ID = ID;
	this->offset = offset;
	this->data = 0;
	this->range.first = low;
	this->range.second = high;
	this->texture = sf::Texture(txt);
};
