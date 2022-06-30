#pragma once

#ifndef _RANDOM_NUMBER_H_
#define _RANDOM_NUMBER_H_

#include <random>
#include <SFML/System/Vector2.hpp>
#include "constants.h"

static std::random_device rd;
static std::mt19937 rnd;



static unsigned int random_int_in_range(unsigned int min, unsigned int max)
{
	std::uniform_int_distribution<unsigned int> dist(min, max);
	return dist(rnd);
}

static unsigned int random_int()
{
	std::uniform_int_distribution<unsigned int> dist(0, std::numeric_limits<unsigned int>::max());
	return dist(rnd);
}

static float random_float_in_range(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rnd);
}

static float random_float_exp()
{
	std::exponential_distribution<float> dist(1.0);
	return dist(rnd);
}

static float random_float_uni()
{
	std::uniform_real_distribution<float> dist(0.0, 1.0);
	return dist(rnd);
}

static sf::Vector2f random_direction(float magnitude = 1.0f)
{

	float num = random_float_in_range(0.0, 2 * PI);
	return magnitude * sf::Vector2f(sin(num), cos(num));
}

static sf::Vector2f random_position(int width, int height)
{
	float x = random_float_in_range(0.0f, width * 1.0f);
	float y = random_float_in_range(0.0f, height * 1.0f);
	return sf::Vector2f(x, y);
}

#endif // !_RANDOM_NUMBER_H_
