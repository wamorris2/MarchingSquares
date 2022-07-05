#include "MarchingSquares.h"
#include <iostream>

float distance(sf::Vector2f a, sf::Vector2f b)
{
	return (float)sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

void MarchingSquares::initVariables()
{
	this->width = 1002;
	this->height = 902;
	this->optionsWidth = 200;
	this->videoMode = sf::VideoMode(width + optionsWidth, height);

	this->menu = Menu("Options", {
		Option("Critical Value", type::decimal, 1.0f, 0.0f, 5.0f, 0.10f),	// [0]
		Option("Intensity", type::decimal, 50.0f, 10.0f, 1000.0f, 10.0f),	// [1]
		Option("Speed", type::decimal, 40.0f, 0.0f, 500.0f, 5.0f),			// [2]
		Option("Cellsize", type::decimal, 10.0f, 1.0f, 100.0f, 1.0f),		// [3]
		Option("Draw Points", type::boolean, 0, {false, true}),				// [4]
		Option("Draw Fill/Lines", type::boolean, 1, {false, true}),			// [5] true = drawFill, false = drawLines
		Option("Draw Blobs", type::boolean, 0, {false, true}), 				// [6]
		Option("Blob Red", type::decimal, 255.0f, 0.0f, 255.0f, 5.0f),		// [7]
		Option("Blob Green", type::decimal, 255.0f, 0.0f, 255.0f, 5.0f),	// [8]
		Option("Blob Blue", type::decimal, 255.0f, 0.0f, 255.0f, 5.0f)		// [9]
		});
		
	this->blobs = std::vector<blob>();
	this->addBlob();
	this->addBlob();
	this->addBlob();
	
	this->resetField();
	this->paused = false;
}

void MarchingSquares::initWindow()
{
	this->settings.antialiasingLevel = 8u;
	this->window = std::make_unique<sf::RenderWindow>(this->videoMode, "Marching Squares", sf::Style::Default ^ sf::Style::Resize, this->settings);
	this->window->setVerticalSyncEnabled(true);
}

MarchingSquares::MarchingSquares()
{
	this->initVariables();
	this->initWindow();
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
					for (blob& b : blobs) b.position += menu[2].current() * b.velocity * this->deltaTime; // move every blob
					this->calculateFieldValues();
				}
				break;
			case sf::Keyboard::J:
				if (paused)
				{
					this->handleCollisions();
					for (blob& b : this->blobs) b.position += menu[2].current() * (-b.velocity) * this->deltaTime; // move every blob back
					this->calculateFieldValues();
				}
				break;
			case sf::Keyboard::Up:
				// currOptions--
				menu.up();
				break;
			case sf::Keyboard::Down:
				// currOption++
				menu.down();
				break;
			case sf::Keyboard::Left:
				// optionNum--
				menu[menu.current()].left();
				if (menu.current() == 3)
					this->resetField();
				break;
			case sf::Keyboard::Right:
				// optionNum++
				menu[menu.current()].right();
				if (menu.current() == 3)
					this->resetField();
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
	float fa = this->field->get_value((int)(a.x / menu[3].current()), (int)(a.y / menu[3].current()));
	float fb = this->field->get_value((int)(b.x / menu[3].current()), (int)(b.y / menu[3].current()));
	float sum = fa + fb;
	float ratio = fb / sum;

	return ratio * (b - a) + a;
}

void MarchingSquares::resetField()
{
	this->field = std::make_shared<arr2d<float>>(width / menu[3].current() + 1, height / menu[3].current() + 1);
	this->calculateFieldValues();
}

void MarchingSquares::calculateFieldValues()
{
	for (int i = 0; i < this->field->rows; i++)
		for (int j = 0; j < this->field->columns; j++)
		{
			this->field->set_value(i, j, 0.0f);
			for (const blob& b : blobs)
			{
				sf::Vector2f point((float)(i * menu[3].current()), (float)(j * menu[3].current()));
				float dist = distance(point, b.position);
				this->field->set_value(i, j, this->field->get_value(i, j) + menu[1].current() / dist);
			}
		}
}

void MarchingSquares::addBlob()
{
	float angle = random_float_uni() * 2 * PI;
	this->blobs.push_back(blob(50.0f, 
							   sf::Vector2f(random_float_uni() * width, random_float_uni() * height), 
							   sf::Vector2f(cos(angle), sin(angle))));
}

void MarchingSquares::update()
{
	this->pollEvents();
	if (paused)
		return;
	this->handleCollisions();
	for (blob& b : blobs) b.position += menu[2].current() * b.velocity * this->deltaTime; // move every blob
	this->calculateFieldValues();
}

void MarchingSquares::render()
{

	this->window->clear();
	sf::Color col((sf::Uint8)(menu[7].current()), (sf::Uint8)(menu[8].current()), (sf::Uint8)(menu[9].current()));
	std::vector<sf::Vertex> fillZone(6);
	sf::Vertex lineZone[4];
	for (int i = 0; i < this->field->rows - 1; i++)
	{
		for (int j = 0; j < this->field->columns - 1; j++)
		{
			if (menu[5].current())
			{
			std::vector<sf::Vector2i> points = { sf::Vector2i(i, j), sf::Vector2i(i, j + 1), sf::Vector2i(i + 1, j + 1), sf::Vector2i(i + 1, j) };
			int vertex = 0;
			for (int k = 0; k < 4; ++k)
			{
				sf::Vector2i p = points[k], p2 = points[(k + 1) % 4];
				sf::Vector2f p_pos((float)(p.x * menu[3].current()), (float)(p.y * menu[3].current())), p2_pos((float)(p2.x * menu[3].current()), (float)(p2.y * menu[3].current()));
				float f = this->field->get_value(p.x, p.y), f2 = this->field->get_value(p2.x, p2.y);
				if (f >= menu[0].current())
				{
					fillZone[vertex].position = p_pos;
					fillZone[vertex].color = col;
					vertex++;

					if (f2 < menu[0].current())
					{
						sf::Vector2f linInter = linearInterpolation(p_pos, p2_pos);
						fillZone[vertex].position = linInter;
						fillZone[vertex].color = col;
						vertex++;
					}
				}
				else if (f2 >= menu[0].current())
				{
					sf::Vector2f linInter = linearInterpolation(p_pos, p2_pos);
					fillZone[vertex].position = linInter;
					fillZone[vertex].color = col;
					vertex++;
				}
			}
			this->window->draw(&fillZone[0], vertex, sf::TriangleFan);
			}
			else // drawLines
			{
			std::vector<sf::Vector2i> points = { sf::Vector2i(i, j), sf::Vector2i(i, j + 1), sf::Vector2i(i + 1, j + 1), sf::Vector2i(i + 1, j) };
			sf::VertexArray lines(sf::Lines, 4);
			sf::Uint8 start = 0, end = 1;
			for (int k = 0; k < 4; ++k)
			{
				sf::Vector2i p = points[k], p2 = points[(k + 1) % 4];
				sf::Vector2f p_pos((float)(p.x * menu[3].current()), (float)(p.y * menu[3].current())), p2_pos((float)(p2.x * menu[3].current()), (float)(p2.y * menu[3].current()));
				float f = this->field->get_value(p.x, p.y), f2 = this->field->get_value(p2.x, p2.y);
				sf::Vector2f linInter = linearInterpolation(p_pos, p2_pos);
				if (f >= menu[0].current() && f2 < menu[0].current()) // inside -> outside
				{
					lines[end].color = col;
					lines[end].position = linInter;
					end += 2;
				}
				else if (f < menu[0].current() && f2 >= menu[0].current()) // outside -> inside
				{
					if (start == 2)
					{
						auto temp = lines[end].position;
						lines[end].position = lines[1].position;
						lines[1].position = temp;
					}
					lines[start].color = col;
					lines[start].position = linInter;
					start += 2;
				}
			}
			this->window->draw(lines);
			}
		}
	}
	if (menu[4].current())
	{
	sf::CircleShape circ;
	circ.setRadius(2.0f);
	circ.setOrigin(1.0f, 1.0f);
	for (int i = 0; i < this->field->rows; i++)
	{
		for (int j = 0; j < this->field->columns; j++)
		{
			if (this->field->get_value(i, j) >= menu[0].current())
				circ.setFillColor(sf::Color::Green);
			else
				circ.setFillColor(sf::Color::Red);
			circ.setPosition((float)(i * menu[3].current()), (float)(j * menu[3].current()));
			this->window->draw(circ);
		}
	}
	}
	if (menu[6].current())
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

	// Draw Options
	{
	sf::RectangleShape rect(sf::Vector2f(optionsWidth, (float)(height)));
	rect.setPosition((float)(width), 0.0f);
	rect.setFillColor(sf::Color::White);
	this->window->draw(rect);

	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) std::cerr << "Error loading font!" << std::endl;
	sf::Text text(menu.getName(), font, 20);
	text.setFillColor(sf::Color::Black);

	text.setPosition((float)(width + 2), 2.0f);
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	this->window->draw(text);
	int startingHeight = 26;
	text.setCharacterSize(12);

	for (int i = 0; i < menu.getSize(); ++i)
	{
		text.setStyle((i == menu.current()) ? sf::Text::Bold : sf::Text::Regular);
		text.setPosition((float)(width + 2), (float)(startingHeight + 16 * i));
		text.setString(menu[i].getName() + " " + std::to_string(menu[i].current()));
		this->window->draw(text);
	}
	}
	
	
	
	


	this->window->display();
	this->deltaTime = this->clock.restart().asSeconds();
}
