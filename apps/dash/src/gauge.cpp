#include "gauge.hpp"
#include "common.hpp"
#include <iostream>


gauge::gauge(float originX, float originY, int dfi, float curveF, float curveT, float width, PID* pid, int ticks, int vFrom, int vTo, sf::Font* font){
	//define start and end angle of gauge
	curve.from = curveF;
	curve.to = curveT;
	
	//Create needle
	needle = sf::RectangleShape({5.f, dfi+width});
	needle.setFillColor(sf::Color(255, 0, 0));
	needle.setPosition({originX,originY});
	needle.setRotation(sf::degrees(curveF-90));
	
	//Assign pid
	this->pid = pid;
	// assign visible range
	this->visible.from = vFrom;
	this->visible.to = vTo;
	
	for (int i = 0; i < ticks; i++){
		float angle = valueMap(i,0,ticks-1,curve.from,curve.to);
		//Create Word/number
		sf::Text tempWord(*font);
		std::stringstream ss;
		ss << std::fixed << std::setprecision(0) << valueMap(i,0,ticks-1,visible.from,visible.to);
		tempWord.setString(ss.str());
		tempWord.setCharacterSize(16);
		auto bounds = tempWord.getLocalBounds();
		tempWord.setOrigin( {(bounds.size.x)*valueMap(cos(angle*PI/180),1,-1,0,1) , (bounds.size.y)*valueMap(sin(angle*PI/180),1,-1,0,2) });				
		tempWord.setFillColor(sf::Color(255,255,255));
		tempWord.setPosition({originX,originY});
		tempWord.move({cos(angle*PI/180)*(dfi+2*width),sin(angle*PI/180)*(dfi+2*width)});
		numbers.emplace_back(tempWord);
		
		//Create Tick
		sf::RectangleShape tempTick({5.f, width});
		tempTick.setFillColor(sf::Color(255, 255, 255));
		tempTick.setPosition({originX,originY});
		//std::cout << angle << std::endl;
		
		tempTick.move({cos(angle*PI/180)*dfi,sin(angle*PI/180)*dfi});
		tempTick.setRotation(sf::degrees(angle-90));
		shapes.emplace_back(tempTick);
		
		
	
		
	};
	
};	


void gauge::update(){
	
	
	    if (pid->ID == 0) {
        switch (pid->offset) {
            case 2: // Coolant (CLT)
				if(pid->data < visible.from){
					needle.setFillColor(sf::Color(0, 0, 255));
					needle.setRotation(sf::degrees(curve.from-90));
				}
				else{
					needle.setFillColor(sf::Color(255, 0, 0));
					needle.setRotation(sf::degrees(valueMap(pid->data, visible.from, visible.to, curve.from, curve.to)-90));
				}
				break;
			default:
				needle.setFillColor(sf::Color(255, 0, 0));
				needle.setRotation(sf::degrees(valueMap(pid->data, visible.from, visible.to, curve.from, curve.to)-90));
				break;
        

        }
    } 
    else if (pid->ID == 1) {
        switch (pid->offset) {
			default:
				needle.setFillColor(sf::Color(255, 0, 0));
				needle.setRotation(sf::degrees(valueMap(pid->data, visible.from, visible.to, curve.from, curve.to)-90));
				break;
        }
    }
};


void gauge::render(sf::RenderWindow& window){
for(sf::RectangleShape i : shapes){
	window.draw(i);
};
for(sf::Text i : numbers){
	window.draw(i);
};
	window.draw(needle);
};
