#include "MarchingSquares.h"
#include <iostream>

void MarchingSquares::initVariables()
{
	this->width = 802;
	this->height = 802;
	this->videoMode = sf::VideoMode(this->width, this->height);
	this->cellsize = 5;

	this->intensity = 50.0f;
	float max_speed = 200.0f;
	this->blobs = { blob(50.0f, sf::Vector2f(random_float_uni() * width, random_float_uni() * height), sf::Vector2f(random_float_uni() * max_speed - max_speed / 2, random_float_uni() * max_speed - max_speed / 2)),
					blob(50.0f, sf::Vector2f(random_float_uni() * width, random_float_uni() * height), sf::Vector2f(random_float_uni() * max_speed - max_speed / 2, random_float_uni() * max_speed - max_speed / 2)),
					blob(50.0f, sf::Vector2f(random_float_uni() * width, random_float_uni() * height), sf::Vector2f(random_float_uni() * max_speed - max_speed / 2, random_float_uni() * max_speed - max_speed / 2)) };
	this->field = std::make_shared<arr2d<float>>(width / cellsize + 1, height / cellsize + 1);
	
	this->drawPoints = true;
	this->drawLines = false;
	this->drawFill = true;
	this->drawBlobs = false;

	this->paused = false;
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
			case sf::Keyboard::Space:
				this->paused = !this->paused;
				break;
			case sf::Keyboard::D:
				if (paused)
				{
					this->moveBlobs();
					this->calculateFieldValues();
				}
				break;
			}
			break;
		}
	}
}

void MarchingSquares::handleCollisions()
{
	for (blob& b : blobs)
	{
		if (b.position.x > this->width - b.radius)
		{
			b.position.x = this->width - b.radius;
			b.velocity.x = -b.velocity.x;
		}
		else if (b.position.x < b.radius)
		{
			b.position.x = b.radius;
			b.velocity.x = -b.velocity.x;
		}

		if (b.position.y > this->height - b.radius)
		{
			b.position.y = this->height - b.radius;
			b.velocity.y = -b.velocity.y;
		}
		else if (b.position.y < b.radius)
		{
			b.position.y = b.radius;
			b.velocity.y = -b.velocity.y;
		}

	}
}

float distance(sf::Vector2f a, sf::Vector2f b)
{
	return (float)sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

sf::Vector2f MarchingSquares::linearInterpolation(sf::Vector2f a, sf::Vector2f b)
{

	float fb = this->field->get_value((int)(b.x / cellsize), (int)(b.y / cellsize));
	float sum = this->field->get_value((int)(a.x / cellsize), (int)(a.y / cellsize)) + fb;
	float ratio = fb / sum;

	return ratio * (b - a) + a;
}

MarchingSquares::MarchingSquares()
{
	this->initVariables();
	this->initWindow();
}

void MarchingSquares::moveBlobs()
{
	for (blob& b : blobs) b.position += b.velocity * this->deltaTime; // move every blob
}

void MarchingSquares::calculateFieldValues()
{
	for (int i = 0; i < this->field->rows; i++)
		for (int j = 0; j < this->field->columns; j++)
		{
			this->field->set_value(i, j, 0.0f);
			for (const blob& b : blobs)
			{
				sf::Vector2f point((float)(i * cellsize), (float)(j * cellsize));
				float dist = distance(point, b.position);
				this->field->set_value(i, j, this->field->get_value(i, j) + this->intensity / dist);
			}
		}
}
void MarchingSquares::update()
{
	this->pollEvents();
	this->handleCollisions();
	if (paused)
		return;
	this->moveBlobs();
	this->calculateFieldValues();
}

void MarchingSquares::render()
{
	this->window->clear();
	std::vector<sf::Vertex> fillZone(6);
	sf::Vertex lineZone[4];
	if (drawFill || drawLines)
	for (int i = 0; i < this->field->rows - 1; i++)
	{
		for (int j = 0; j < this->field->columns - 1; j++)
		{
			if (drawFill)
			{
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
			if (drawLines)
			{
				
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
	if (drawBlobs)
	{
	sf::CircleShape circ;
	circ.setOutlineColor(sf::Color::Blue);
	circ.setOutlineThickness(2.0f);
	circ.setFillColor(sf::Color::Transparent);
	for (const blob& b : blobs)
	{
		circ.setRadius(b.radius);
		circ.setOrigin(b.radius, b.radius);
		circ.setPosition(b.position);
		this->window->draw(circ);
	}
	}
	this->window->display();
	this->deltaTime = this->clock.restart().asSeconds();
}
