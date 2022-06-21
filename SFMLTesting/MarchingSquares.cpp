#include "MarchingSquares.h"
#include <iostream>

void MarchingSquares::initVariables()
{
	this->width = 802;
	this->height = 802;
	this->videoMode = sf::VideoMode(this->width, this->height);
	this->cellsize = 20;
	this->field = std::make_shared<arr2d<float>>(width / cellsize + 1, height / cellsize + 1);
	std::cerr << width / cellsize << " " << height / cellsize;
	for (int i = 0; i < this->field->rows; i++)
		for (int j = 0; j < this->field->columns; j++)
			field->set_value(i, j, random_float());
		
}

void MarchingSquares::initWindow()
{
	this->window = std::make_unique<sf::RenderWindow>(sf::VideoMode(this->videoMode), "Marching Squares", sf::Style::Default ^ sf::Style::Resize);
}

void MarchingSquares::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			switch (this->ev.key.code)
			{
			case sf::Keyboard::Escape:
				this->window->close();
				break;
			}
			break;
		}
	}
}

void MarchingSquares::handleCollisions()
{
}

float MarchingSquares::deltaTime()
{
	return this->clock.getElapsedTime().asSeconds();
}

sf::Vector2f MarchingSquares::linearInterpolation(sf::Vector2f a, sf::Vector2f b)
{
	float fa = this->field->get_value(a.x, a.y);
	float fb = this->field->get_value(b.x, b.y);
	float m = (fb - fa) / cellsize;

	return m * (b - a) + a;
}

MarchingSquares::MarchingSquares()
{
	this->initVariables();
	this->initWindow();
}

bool MarchingSquares::running()
{
	return this->window->isOpen();
}

void MarchingSquares::update()
{
	this->pollEvents();
	this->handleCollisions();

}

void MarchingSquares::render()
{
	this->window->clear();
	sf::CircleShape circ;
	circ.setRadius(2.0f);
	circ.setOrigin(1.0f, 1.0f);
	for (int i = 0; i < this->field->rows; i++)
		for (int j = 0; j < this->field->columns; j++)
		{
			if ((*field)[i][j] >= 1.0)
				circ.setFillColor(sf::Color::Green);
			else
				circ.setFillColor(sf::Color::Red);
			circ.setPosition((float)(i * this->cellsize), (float)(j * this->cellsize));
			this->window->draw(circ);
		}

	for (int i = 0; i < this->field->rows - 1; i++)
		for (int j = 0; j < this->field->columns - 1; j++)
		{
			if (this->field->get_value(i, j) >= 1.0 && this->field->get_value(i, j + 1) < 1.0 ||
				this->field->get_value(i, j) < 1.0 && this->field->get_value(i, j + 1) >= 1.0)
			{

			}
			if (this->field->get_value(i + 1, j) >= 1.0 && this->field->get_value(i + 1, j + 1) < 1.0 ||
				this->field->get_value(i + 1, j) < 1.0 && this->field->get_value(i + 1, j + 1) >= 1.0)
			{

			}
			if (this->field->get_value(i, j) >= 1.0 && this->field->get_value(i + 1, j) < 1.0 ||
				this->field->get_value(i, j) < 1.0 && this->field->get_value(i + 1, j) >= 1.0)
			{

			}
			if (this->field->get_value(i, j + 1) >= 1.0 && this->field->get_value(i + 1, j + 1) < 1.0 ||
				this->field->get_value(i, j + 1) < 1.0 && this->field->get_value(i + 1, j + 1) >= 1.0)
			{

			}
		}


	this->window->display();
	this->clock.restart();
}
