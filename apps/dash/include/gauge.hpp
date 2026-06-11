

#ifndef Gauge
#define Gauge
#include <SFML/Graphics.hpp>
#include "pid.hpp"
#include <cmath>
#include <vector>
#include "common.hpp"
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
struct range{
	float from;
	float to;
};
struct coord{
	float x;
	float y;
};

class gauge{
private:
int dfi;
range curve;
range visible;
coord size;
float pos;
sf::RectangleShape needle;
PID* pid;
std::vector<sf::Text> numbers;
std::vector<sf::RectangleShape> shapes;
public:
gauge(float originX, float originY, int dfi, float curveF, float curveT, float width, PID* pid, int ticks, int vFrom, int vTo, sf::Font* font);
	 void update();
	void makeShape();
	void render(sf::RenderWindow& window);
};

#endif 
