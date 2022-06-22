#include "MarchingSquares.h"
#include <iostream>

void MarchingSquares::initVariables()
{
	this->width = 802;
	this->height = 802;
	this->videoMode = sf::VideoMode(this->width, this->height);
	this->cellsize = 400;
	this->field = std::make_shared<arr2d<float>>(width / cellsize + 1, height / cellsize + 1);
	std::cerr << width / cellsize << " " << height / cellsize;
	for (int i = 0; i < this->field->rows; i++)
		for (int j = 0; j < this->field->columns; j++)
			field->set_value(i, j, random_float());

	this->drawPoints = false;
	this->drawLines = false;
	this->drawFill = true;
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
	float fa = this->field->get_value((int)(a.x), (int)(a.y));
	float fb = this->field->get_value((int)(b.x), (int)(b.y));
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
	if (drawPoints)
	{
		sf::CircleShape circ;
		circ.setRadius(2.0f);
		circ.setOrigin(1.0f, 1.0f);
		for (int i = 0; i < this->field->rows; i++)
			for (int j = 0; j < this->field->columns; j++)
			{
				if (this->field->get_value(i, j) >= 1.0)
					circ.setFillColor(sf::Color::Green);
				else
					circ.setFillColor(sf::Color::Red);
				circ.setPosition((float)(i * this->cellsize), (float)(j * this->cellsize));
				this->window->draw(circ);
			}
	}
	
	if (drawFill)
	{
		std::vector<sf::Vertex> fillZone(6);
		for (int i = 0; i < this->field->rows - 1; i++)
			for (int j = 0; j < this->field->columns - 1; j++)
			{
				// start at top-left and go clockwise around points
				// if point is inside, add to Vertex Array
				// if next point is different from this point, add linear interpolated to Vertex Array
				std::vector<sf::Vector2i> points = { sf::Vector2i(i, j), sf::Vector2i(i, j + 1), sf::Vector2i(i + 1, j + 1), sf::Vector2i(i + 1, j) };
				int vertex = 0;
				for (int i = 0; i < points.size(); ++i)
				{
					sf::Vector2i p = points[i];
					float f = this->field->get_value(p.x, p.y);
					if (f >= 1.0f)
					{
						fillZone[vertex].position = sf::Vector2f((float)(p.x * cellsize), (float)(p.y * cellsize));
						fillZone[vertex].color = sf::Color::White;
						vertex++;
					}
					sf::Vector2i p2 = points[(i + 1) % 4];
					float f2 = this->field->get_value(p2.x, p2.y);
					if ((f >= 1.0f && f2 < 1.0f) || (f < 1.0f && f2 >= 1.0f))
					{
						sf::Vector2f linInter = linearInterpolation(sf::Vector2f(p), sf::Vector2f(p2));
						fillZone[vertex].position = linInter;
						fillZone[vertex].color = sf::Color::Red;
						vertex++;
					}
				}
				this->window->draw(&fillZone[0], vertex - 1, sf::TriangleFan);
			}
	}
	this->window->display();
	this->clock.restart();
}
