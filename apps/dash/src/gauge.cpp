#include "gauge.hpp"
#include "common.hpp"
#include <iostream>
#include <string>
#include <map>

void meter::setValue(float value)
{

	this->value = valueMap(value, this->data.from, this->data.to, this->visible.from, this->visible.to);
	std::cout << this->value << "," << value << std::endl;
}

void meter::update() {};
void meter::render(sf::RenderWindow &window) {};
void meter::makeTicks(sf::Font *font) {};

gauge::gauge(float originX, float originY, int size, float curveF, float curveT, float width, int notches, int vFrom, int vTo, sf::Font *font)
{
	origin.x = originX;
	origin.y = originY;
	this->width = width;
	this->notches = notches;
	this->size = size;
	// define start and end angle of gauge
	visible.from = curveF;
	visible.to = curveT;
	// background
	background = sf::CircleShape(size + 3 * width);
	background.setOrigin({background.getRadius(), background.getRadius()});
	background.setPosition({originX, originY});
	background.setFillColor(sf::Color(255, 255, 255));
	// Create needle
	needle = sf::RectangleShape({5.f, size + width});
	needle.setFillColor(sf::Color(255, 0, 0));
	needle.setPosition({originX, originY});
	needle.setRotation(sf::degrees(curveF - 90));

	// assign visible range
	this->data.from = vFrom;
	this->data.to = vTo;
	this->makeTicks(font);
};

void gauge::update() { needle.setRotation(sf::degrees(value - 90)); }

void gauge::makeTicks(sf::Font *font)
{
	for (int i = 0; i < notches; i++)
	{
		float angle = valueMap(i, 0, notches - 1, visible.from, visible.to);
		// Create Word/number
		sf::Text tempWord(*font);
		std::stringstream ss;
		ss << std::fixed << std::setprecision(0) << valueMap(i, 0, notches - 1, data.from, data.to);
		tempWord.setString(ss.str());
		tempWord.setCharacterSize(16);
		auto bounds = tempWord.getLocalBounds();
		tempWord.setOrigin({(bounds.size.x) * valueMap(cos(angle * PI / 180), 1, -1, 0, 1), (bounds.size.y) * valueMap(sin(angle * PI / 180), 1, -1, 0, 2)});
		tempWord.setFillColor(sf::Color(0, 0, 0, 100));
		tempWord.setPosition({origin.x, origin.y});
		tempWord.move({cos(angle * PI / 180) * (size + width), sin(angle * PI / 180) * (size + width)});
		numbers.emplace_back(tempWord);

		// Create Tick
		sf::RectangleShape tempTick({5.f, width});
		bounds = tempTick.getLocalBounds();
		tempTick.setFillColor(sf::Color(0, 0, 0));
		tempTick.setPosition({origin.x, origin.y});
		// std::cout << angle << std::endl;

		tempTick.move({cos(angle * PI / 180) * size, sin(angle * PI / 180) * size});
		tempTick.setRotation(sf::degrees(angle - 90));
		this->ticks.emplace_back(tempTick);
	};
}
void gauge::render(sf::RenderWindow &window)
{
	window.draw(background);
	for (sf::RectangleShape i : ticks)
	{
		window.draw(i);
	};
	for (auto i : numbers)
	{
		window.draw(i);
	};
	window.draw(needle);
}
bar::bar(float originX, float originY,
		 float thick,
		 bool hv,
		 bool neg,
		 int from, int to,
		 int width,
		 sf::Font *font, std::string img)
{
	this->visible.from = 0;
	this->visible.to = width;
	this->thick = thick;
	this->width = width;
	data.from = from;
	data.to = to;
	if (!texture.loadFromFile(img))
	{
		std::cerr << "CRITICAL: Texture FAILED to load from path: " << img << std::endl;
	}
	else
	{
		std::cout << "Texture loaded successfully from: " << img << std::endl;
		sprite = std::make_unique<sf::Sprite>(texture);
	}
	std::cout << "pre-init" << std::endl;
	this->hv = hv;
	origin.x = originX;
	origin.y = originY;
	if (neg)
	{
		this->neg = -1;
	}

	auto bounds = sprite->getLocalBounds();
	sprite->setOrigin({256,256});
	sprite->setScale({thick / bounds.size.x, thick / bounds.size.y});
	int spriteX = (width*.5*(this->neg));
	int spriteY = (thick*.25);
	sprite->setPosition({origin.x, origin.y});
	if(hv){
	sprite->move({spriteY, spriteX });
	}
	else{
	sprite->move({spriteX,spriteY });
	}

	sprite->setColor(sf::Color(255, 255, 0, 255));

	needle = sf::RectangleShape({hv * thick + !hv * 5, !hv * thick + hv * 5});
	sf::Vector2f rectSize = needle.getSize();
	needle.setOrigin({rectSize.x / 2.0f, rectSize.y / 2.0f});
	needle.setPosition({origin.x, origin.y});
	needle.setFillColor(sf::Color(255, 0, 0));
	// Tint the sprite (e.g., pure red overlay)
	std::cout << "post-init" << std::endl;

	this->makeTicks(font);
};

void bar::update()
{
	std::cout << value << std::endl;
	needle.setPosition({origin.x + (!hv * value * (neg)), origin.y + (hv * value * (neg))});
	std::cout << !hv * needle.getPosition().x << "," << hv * needle.getPosition().y << std::endl;
};

void bar::render(sf::RenderWindow &window)
{

	// std::cout << "Rendering bar at: " << sprite->getPosition().x << " " << sprite->getPosition().x << std::endl;
	window.draw(*sprite);
	/*sf::RectangleShape debugBox;
	debugBox.setPosition(sprite->getPosition());
	debugBox.setSize(sprite->getLocalBounds().size);
	debugBox.setFillColor(sf::Color::Transparent);
	debugBox.setOutlineColor(sf::Color::Red);
	debugBox.setOutlineThickness(2.0f);
	window.draw(debugBox);*/

	for (sf::RectangleShape i : ticks)
	{
		window.draw(i);
	};
	for (sf::Text i : numbers)
	{
		window.draw(i);
	};
	window.draw(needle);
}

void bar::makeTicks(sf::Font *font)
{
	for (int i = 0; i < notches; i++)
	{
		/*
		// Create Word/number
		sf::Text tempWord(*font);
		std::stringstream ss;
		ss << std::fixed << std::setprecision(0) << valueMap(i, 0, ticks - 1, visible.from, visible.to);
		tempWord.setString(ss.str());
		tempWord.setCharacterSize(width/10);
		tempWord.setFillColor(sf::Color(255, 255, 255));
		tempWord.setPosition({originX +!hv*i*(width/ticks)*neg, originY +hv*i*(width/ticks)*neg});

		numbers.emplace_back(tempWord);
*/
		// Create Tick
		sf::RectangleShape tempTick({hv * thick * (((i % 2 == 0) + 1) * .5) + !hv * thick/10, (!hv) * thick * (((i % 2 == 0) + 1) * .5) + hv * thick/10});
		if (hv)
		{
			//	tempTick.rotate(sf::degrees(90));
		}
		sf::Vector2f rectSize = tempTick.getSize();
		if(hv){
					tempTick.setOrigin({rectSize.x / 2.0f, rectSize.y * (i+1)/notches});

		}
		else{
		tempTick.setOrigin({rectSize.x/2.0f, rectSize.y / 2.0f});
		}
		tempTick.setFillColor(sf::Color(255, 255, 255));
		tempTick.setPosition({origin.x + (!this->hv) * i * (width / notches) * this->neg, origin.y + this->hv * i * (width / notches) * this->neg });
		// std::cout << (!hv)*i*(width/(ticks-1))*neg << std::endl;
		this->ticks.emplace_back(tempTick);
	};
}
void bar::setValue(float val)
{
	meter::setValue(val);
}
void gauge::setValue(float val)
{
	meter::setValue(val);
};