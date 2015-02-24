#pragma once

#include "base_npc.h"

class Npc2 : public base_npc
{
public:
	Npc2(Speech* speech, Map* map, std::string text, float x, float y) : base_npc(speech, "sprites/ninja a.png", map, "ninja", text, x, y)
	{
	}
};