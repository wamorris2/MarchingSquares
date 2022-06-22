#pragma once
#ifndef _MARCHINGSQUARES_H_
#define _MARCHINGSQUARES_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "arr2d.h"
#include "random_number.h"


class MarchingSquares {
private:
	std::unique_ptr<sf::RenderWindow> window;
	sf::VideoMode videoMode;
	sf::Event ev;
	sf::Clock clock;
	int width, height, cellsize;
	std::shared_ptr<arr2d<float>> field;

	bool drawPoints;
	bool drawLines;
	bool drawFill;

	void initVariables();
	void initWindow();
	void pollEvents();
	void handleCollisions();
	float deltaTime();
	sf::Vector2f linearInterpolation(sf::Vector2f a, sf::Vector2f b);

public:
	MarchingSquares();

	bool running();
	void update();
	void render();
};

#endif