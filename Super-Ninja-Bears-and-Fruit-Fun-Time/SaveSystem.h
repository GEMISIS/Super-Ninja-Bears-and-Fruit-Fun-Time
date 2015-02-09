#pragma once

#include <iostream>
#include <fstream>
#include <string>

#define SAVE_FILE "test.sav"

class SaveSystem
{
public:
	SaveSystem()
	{
		std::ifstream input(SAVE_FILE);
		std::string temp;
		if (input)
		{
			input >> x >> y >> flip >> score >> health >> currentAreaEntry >> currentMap;

			visitedAreas.clear();
			while (input >> temp)
			{
				visitedAreas.push_back(temp);
			}
		}
		x = 0;
		y = 0;
		if (currentMap == "")
		{
			this->reset();
			this->Save();
		}
		input.close();
	}
	void reset()
	{
		currentMap = "level1.json";
		x = 0;
		y = 0;
		score = 0;
		health = 3;
		currentAreaEntry = "X";
		flip = false;
	}
	void Save()
	{
		std::ofstream output(SAVE_FILE);
		output << x << " " << y << " " << flip << " " << score << " " << health << " " << currentAreaEntry << " " << currentMap;
		for (auto iterator = visitedAreas.begin(); iterator != visitedAreas.end(); iterator++)
		{
			output << " " << *iterator;
		}
		output.close();
	}
	float x, y;
	inline void IncrementScore(int amount)
	{
		if (std::find(visitedAreas.begin(), visitedAreas.end(), this->currentMap) == visitedAreas.end())
		{
			this->score += amount;
		}
	}
	inline int GetScore()
	{
		return this->score;
	}
	inline void addAreaToFinished()
	{
		if (std::find(visitedAreas.begin(), visitedAreas.end(), this->currentMap) == visitedAreas.end())
		{
			visitedAreas.push_back(this->currentMap);
		}
	}
	bool flip;
	int health;
	std::string currentAreaEntry;
	std::string currentMap;
private:
	int score;
	std::vector<std::string> visitedAreas;
};