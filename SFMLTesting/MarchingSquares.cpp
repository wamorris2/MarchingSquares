#include "MarchingSquares.h"
#include <iostream>

float distance(sf::Vector2f a, sf::Vector2f b)
{
	return (float)sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

void MarchingSquares::initVariables()
{
	this->videoMode = sf::VideoMode(width, height);

	this->blobs = std::vector<blob>();
	this->addBlob();
	this->addBlob();
	this->addBlob();
	
	this->field = std::make_shared<arr2d<float>>(width / cellsize + 1, height / cellsize + 1);
	this->calculateFieldValues();
	

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
				paused = !paused;
				break;
			case sf::Keyboard::A:
				if (this->blobs.size() <= 10)
					this->addBlob();
				break;
			case sf::Keyboard::D:
				if (this->blobs.size() > 0)
					this->blobs.pop_back();
				break;
			case sf::Keyboard::K:
				if (paused)
				{
					this->handleCollisions();
					for (blob& b : blobs) b.position += b.velocity * this->deltaTime; // move every blob
					this->calculateFieldValues();
				}
				break;
			case sf::Keyboard::J:
				if (paused)
				{
					this->handleCollisions();
					for (blob& b : this->blobs) b.position += -b.velocity * this->deltaTime; // move every blob back
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
		if (b.position.x > width - b.radius)
		{
			b.position.x = width - b.radius;
			b.velocity.x = -b.velocity.x;
		}
		else if (b.position.x < b.radius)
		{
			b.position.x = b.radius;
			b.velocity.x = -b.velocity.x;
		}

		if (b.position.y > height - b.radius)
		{
			b.position.y = height - b.radius;
			b.velocity.y = -b.velocity.y;
		}
		else if (b.position.y < b.radius)
		{
			b.position.y = b.radius;
			b.velocity.y = -b.velocity.y;
		}

	}
}

sf::Vector2f MarchingSquares::linearInterpolation(sf::Vector2f a, sf::Vector2f b)
{
	float fa = this->field->get_value((int)(a.x / cellsize), (int)(a.y / cellsize));
	float fb = this->field->get_value((int)(b.x / cellsize), (int)(b.y / cellsize));
	float sum = fa + fb;
	float ratio = fb / sum;

	return ratio * (b - a) + a;
}

MarchingSquares::MarchingSquares()
{
	this->initVariables();
	this->initWindow();
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
				this->field->set_value(i, j, this->field->get_value(i, j) + intensity / dist);
			}
		}
}

void MarchingSquares::addBlob()
{
	this->blobs.push_back(blob(50.0f, 
								  sf::Vector2f(random_float_uni() * width, random_float_uni() * height), 
								  sf::Vector2f(random_float_uni() * (2 * max_speed) - max_speed, random_float_uni() * (2 * max_speed) - max_speed)));
}

void MarchingSquares::update()
{
	this->pollEvents();
	if (paused)
		return;
	this->handleCollisions();
	for (blob& b : blobs) b.position += b.velocity * this->deltaTime; // move every blob
	this->calculateFieldValues();
}

void MarchingSquares::render()
{
	this->window->clear();
	std::vector<sf::Vertex> fillZone(6);
	sf::Vertex lineZone[4];
	for (int i = 0; i < this->field->rows - 1; i++)
	{
		for (int j = 0; j < this->field->columns - 1; j++)
		{
			if (drawFillOrLines)
			{
			std::vector<sf::Vector2i> points = { sf::Vector2i(i, j), sf::Vector2i(i, j + 1), sf::Vector2i(i + 1, j + 1), sf::Vector2i(i + 1, j) };
			int vertex = 0;
			for (int k = 0; k < 4; ++k)
			{
				sf::Vector2i p = points[k], p2 = points[(k + 1) % 4];
				sf::Vector2f p_pos((float)(p.x * cellsize), (float)(p.y * cellsize)), p2_pos((float)(p2.x * cellsize), (float)(p2.y * cellsize));
				float f = this->field->get_value(p.x, p.y), f2 = this->field->get_value(p2.x, p2.y);
				if (f >= criticalValue)
				{
					fillZone[vertex].position = p_pos;
					fillZone[vertex].color = sf::Color::White;
					vertex++;

					if (f2 < criticalValue)
					{
						sf::Vector2f linInter = linearInterpolation(p_pos, p2_pos);
						fillZone[vertex].position = linInter;
						fillZone[vertex].color = sf::Color::White;
						vertex++;
					}
				}
				else if (f2 >= criticalValue)
				{
					sf::Vector2f linInter = linearInterpolation(p_pos, p2_pos);
					fillZone[vertex].position = linInter;
					fillZone[vertex].color = sf::Color::White;
					vertex++;
				}
			}
			this->window->draw(&fillZone[0], vertex, sf::TriangleFan);
			}
			else
			{
			std::vector<sf::Vector2i> points = { sf::Vector2i(i, j), sf::Vector2i(i, j + 1), sf::Vector2i(i + 1, j + 1), sf::Vector2i(i + 1, j) };
			sf::VertexArray lines(sf::Lines, 4);
			sf::Uint8 start = 0, end = 1;
			for (int k = 0; k < 4; ++k)
			{
				sf::Vector2i p = points[k], p2 = points[(k + 1) % 4];
				sf::Vector2f p_pos((float)(p.x * cellsize), (float)(p.y * cellsize)), p2_pos((float)(p2.x * cellsize), (float)(p2.y * cellsize));
				float f = this->field->get_value(p.x, p.y), f2 = this->field->get_value(p2.x, p2.y);
				sf::Vector2f linInter = linearInterpolation(p_pos, p2_pos);
				if (f >= criticalValue && f2 < criticalValue) // inside -> outside
				{
					lines[end].color = sf::Color::White;
					lines[end].position = linInter;
					end += 2;
				}
				else if (f < criticalValue && f2 >= criticalValue) // outside -> inside
				{
					if (start == 2)
					{
						auto temp = lines[end].position;
						lines[end].position = lines[1].position;
						lines[1].position = temp;
					}
					lines[start].color = sf::Color::White;
					lines[start].position = linInter;
					start += 2;
				}
			}
			this->window->draw(lines);
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
			if (this->field->get_value(i, j) >= criticalValue)
				circ.setFillColor(sf::Color::Green);
			else
				circ.setFillColor(sf::Color::Red);
			circ.setPosition((float)(i * cellsize), (float)(j * cellsize));
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
