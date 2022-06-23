#include "MarchingSquares.h"
#include <iostream>

void MarchingSquares::initVariables()
{
	this->width = 802;
	this->height = 802;
	this->videoMode = sf::VideoMode(this->width, this->height);
	this->cellsize = 40;
	this->field = std::make_shared<arr2d<float>>(width / cellsize + 1, height / cellsize + 1);
	for (int i = 0; i < this->field->rows; i++)
		for (int j = 0; j < this->field->columns; j++)
			field->set_value(i, j, random_float());
	this->drawPoints = true;
	this->drawLines = false;
	this->drawFill = true;
}

void MarchingSquares::initWindow()
{
	this->settings.antialiasingLevel = 8u;
	this->window = std::make_unique<sf::RenderWindow>(this->videoMode, "Marching Squares", sf::Style::Default ^ sf::Style::Resize, this->settings);
	this->window->setVerticalSyncEnabled(true);
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

float distance(sf::Vector2f a, sf::Vector2f b)
{
	return (float)sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

sf::Vector2f MarchingSquares::linearInterpolation(sf::Vector2f a, sf::Vector2f b)
{

	float fa = this->field->get_value((int)(a.x / cellsize), (int)(a.y / cellsize));
	float fb = this->field->get_value((int)(b.x / cellsize), (int)(b.y / cellsize));
	float sum = fa + fb;
	float ratio = fb / sum;
	// a + ratio * (b - a)

	return ratio * (b - a) + a;
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
	if (drawFill) 
	{
	std::vector<sf::Vertex> fillZone(6);
	for (int i = 0; i < this->field->rows - 1; i++)
	{
		for (int j = 0; j < this->field->columns - 1; j++)
		{
			// start at top-left and go clockwise around points
			// if point is inside, add to Vertex Array
			// if next point is different from this point, add linear interpolated to Vertex Array
			std::vector<sf::Vector2i> points = { sf::Vector2i(i, j), sf::Vector2i(i, j + 1), sf::Vector2i(i + 1, j + 1), sf::Vector2i(i + 1, j) };
			int vertex = 0;
			for (int k = 0; k < 4; ++k)
			{
				sf::Vector2i p = points[k], p2 = points[(k + 1) % 4];
				sf::Vector2f p_pos((float)(p.x * this->cellsize), (float)(p.y * this->cellsize)), p2_pos((float)(p2.x * this->cellsize), (float)(p2.y * this->cellsize));
				float f = this->field->get_value(p.x, p.y), f2 = this->field->get_value(p2.x, p2.y);

				if (f >= 1.0f)
				{
					fillZone[vertex].position = p_pos;
					fillZone[vertex].color = sf::Color::White;
					vertex++;

					if (f2 < 1.0f)
					{
						sf::Vector2f linInter = linearInterpolation(p_pos, p2_pos);
						fillZone[vertex].position = linInter;
						fillZone[vertex].color = sf::Color::White;
						vertex++;
					}
				}
				else if (f2 >= 1.0f)
				{
					sf::Vector2f linInter = linearInterpolation(p_pos, p2_pos);
					fillZone[vertex].position = linInter;
					fillZone[vertex].color = sf::Color::White;
					vertex++;
				}
			}
			this->window->draw(&fillZone[0], vertex, sf::TriangleFan);
		}
	}
	}
	if (drawPoints)
	{
	sf::CircleShape circ;
	circ.setRadius(2.0f);
	circ.setOrigin(1.0f, 1.0f);
	for (int i = 0; i < this->field->rows; i++)
	{
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
	}
	this->window->display();
	this->clock.restart();
}
