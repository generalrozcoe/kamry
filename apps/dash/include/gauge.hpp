#ifndef Gauge
#define Gauge
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "common.hpp"
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
static std::map<std::string, sf::Texture> textureCache;

struct range
{
	float from;
	float to;
};
struct coord
{
	float x;
	float y;
};

class meter
{
protected:
	coord origin;
	int width;
	range data;
	range visible;
	sf::RectangleShape needle;
	std::vector<sf::Text> numbers;
	std::vector<sf::RectangleShape> ticks;
	int notches;
	float value = 0;
	sf::Texture texture;
	std::unique_ptr<sf::Sprite> sprite;

public:
	void virtual update();
	void virtual render(sf::RenderWindow &window);
	void virtual setValue(float value);
	void virtual makeTicks(sf::Font *font);
};

class gauge : public meter
{
private:
int size;
	int dfi;
	range curve;
	float pos;
	sf::CircleShape background;

public:
	gauge(float originX, float originY, int dfi, float curveF, float curveT, float width, int ticks, int vFrom, int vTo, sf::Font *font);
	void update();
	void render(sf::RenderWindow &window);
	void makeTicks(sf::Font *font);
		void  setValue(float value)override;

};
class bar : public meter
{
	// hv=false horizontal, hv =true vertical

private:
	int notches = 5;
	bool hv;
	int neg = 1;
	int thick;
	coord size;
	sf::RectangleShape background;

public:
	bar(float originX, float originY, float thick, bool hv, bool neg, int from, int to, int width, sf::Font *font, std::string img);
	void update();
	void render(sf::RenderWindow &window);
	void makeTicks(sf::Font *font);
		void  setValue(float value) override;

};

#endif
