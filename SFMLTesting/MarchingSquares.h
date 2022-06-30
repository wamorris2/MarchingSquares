#pragma once
#ifndef _MARCHINGSQUARES_H_
#define _MARCHINGSQUARES_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "arr2d.h"
#include "blob.h"
#include "random_number.h"


class MarchingSquares {
private:
	sf::ContextSettings settings;
	std::unique_ptr<sf::RenderWindow> window;
	sf::VideoMode videoMode;
	sf::Event ev;
	sf::Clock clock;
	float deltaTime;
	bool paused;

	int width, height, cellsize;
	void calculateFieldValues();
	std::shared_ptr<arr2d<float>> field;

	float intensity;
	void moveBlobs();
	std::vector<blob> blobs;

	bool drawPoints;
	bool drawLines;
	bool drawFill;
	bool drawBlobs;

	void initVariables();
	void initWindow();
	void pollEvents();
	void handleCollisions();
	sf::Vector2f linearInterpolation(sf::Vector2f a, sf::Vector2f b);

public:
	MarchingSquares();

	inline bool running() { return this->window->isOpen(); }
	void update();
	void render();
};

#endif