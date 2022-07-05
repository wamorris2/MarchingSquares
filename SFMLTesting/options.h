#pragma once
#ifndef _OPTIONS_H_
#define _OPTIONS_H_
#include <iostream>

namespace type
{
	enum
	{
		boolean = 0,
		integer = 1,
		decimal = 2

	};
}

class Option {
private:
	std::string name;
	
	std::vector<float> optionRange;
	std::size_t size;
	float min, max, step;
	
	int currOption;
	float currValue;
	
	typedef std::vector<float>::const_iterator const_iterator;
	typedef std::vector<float>::iterator iterator;

public:
	Option() : name(""), type(0), optionRange({}), size(0), currOption(0), min(-1), max(-1), step(-1), currValue(0.0) {}

	Option(std::string n, int t, int dO, std::vector<float> oR) : min(-1.0), max(-1.0)
	{
		name = n;
		type = t;
		if (type = type::boolean)
			optionRange = { 0.0f, 1.0f };
		else
			optionRange = oR;
		size = optionRange.size();
		assert(dO >= 0 && dO < size);
		currOption = dO;
	}

	Option(std::string n, int t, float dO, float mi, float ma, float st) : optionRange({}), size(0)
	{
		name = n;
		type = t;
		assert(type != type::boolean);
		min = mi;
		max = ma;
		step = st;
		assert(dO >= min && dO <= max && step > 0.0);
		currValue = dO;
	}

	iterator begin()
	{
		return optionRange.begin();
	}
	const_iterator begin() const
	{
		return optionRange.begin();
	}
	iterator end()
	{
		return optionRange.end();
	}
	const_iterator end() const
	{
		return optionRange.end();
	}

	const std::string range() const
	{
		if (max == -1)
			return "range(" + std::to_string(optionRange[0]) + ", " + std::to_string(optionRange[optionRange.size() - 1]) + ")";
		return "range(" + std::to_string(min) + ", " + std::to_string(max) + ")";
	}

	const float& current() const
	{
		if (max == -1.0)
			return optionRange[currOption];
		return currValue;
	}

	const std::string getName() const
	{
		return this->name;
	}

	void left()
	{
		if (max == -1 && currOption > 0)
			--currOption; 
		else if (currValue - step >= min) 
			currValue -= step;
	}

	void right()
	{
		if (max == -1 && currOption < optionRange.size() - 1) 
			++currOption;
		else if (currValue + step <= max) 
			currValue += step;
	}
	int type;
};

class Menu {
	std::string name;
	int currentOption;
	std::vector<Option> menu;
	std::size_t size;

	typedef std::vector<Option>::iterator iterator;
	typedef std::vector<Option>::const_iterator const_iterator;

public:
	Menu(std::string n, std::vector<Option> m) : name(n), menu(m), size(m.size()), currentOption(0) {}
	Menu() : name(""), menu({}), size(0), currentOption(0) {}

	iterator begin()
	{
		return menu.begin();
	}
	const_iterator begin() const
	{
		return menu.begin();
	}
	iterator end()
	{
		return menu.end();
	}
	const_iterator end() const
	{
		return menu.end();
	}

	const std::string getName() const
	{
		return this->name;
	}

	const std::size_t getSize() const
	{
		return this->size;
	}

	int current()
	{
		return currentOption;
	}

	void up()
	{
		if (currentOption > 0) currentOption--;
	}

	void down()
	{
		if (currentOption < menu.size() - 1) currentOption++;
	}

	Option& operator[](int index)
	{
		assert(index >= 0 && index < menu.size());
		return menu[index];
	}
};
#endif