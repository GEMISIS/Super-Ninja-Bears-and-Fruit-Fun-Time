#pragma once

#include "base_npc.h"

class Npc1 : public base_npc
{
public:
	Npc1(Speech* speech, Map* map, std::string text, float x, float y) : base_npc(speech, "sprites/hilly.png", map, text, x, y)
	{
	}
};