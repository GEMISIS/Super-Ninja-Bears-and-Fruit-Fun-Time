#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "entity_manager.h"
#include "speech.h"

#include "SaveSystem.h"

typedef enum
{
	NONE = -1,
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	TOP_LEFT = 4,
	TOP_RIGHT = 5,
	BOTTOM_LEFT = 6,
	BOTTOM_RIGHT = 7
}Direction;

typedef enum
{
	NO_GROUP = 0,
	GROUND = 1,
	WATER = 2,
	LAVA = 3,
	EXIT = 4,
	FINISHED = 5
}tile_groups;

typedef struct
{
	int id;
	tile_groups group;
	bool wavy;
	std::string nextArea, nextAreaEntry;
}tile_properties_t;

extern SaveSystem saveSystem;

class Map
{
public:
	Map(EntityManager* entityManager);

	void Load(std::string filename, std::string backgroundFile, Speech* speech);

	virtual void Update(sf::RenderWindow* window);

	tile_properties_t CheckCollision(Entity* entity, Direction direction);

	void Render(sf::RenderWindow* window);

	void move(float x, float y);
	void move(sf::Vector2f amount);

	sf::FloatRect getGlobalBounds();

	std::string topArea, bottomArea, leftArea, rightArea;

	~Map();
protected:
	std::string tileSet;
	int width = 0;
	int height = 0;
	int tileWidth = 0;
	int tileHeight = 0;
	std::unordered_map<std::string, int*> data;
private:
	int groundTileStart;
	int npcTileStart;
	int mobsTileStart;
	int collectTileStart;

	sf::Texture* backgroundTexture;
	std::unordered_map<int, tile_properties_t> tileProperties;
	std::unordered_map<int, sf::Image*> tiles;
	std::unordered_map<int, sf::Image*> flippedTiles;
	EntityManager* entityManager;
	std::unordered_map<std::string, sf::Sprite*> mapTiles;
	std::unordered_map<std::string, sf::Texture*> tileTextures;
	sf::Sprite* background;

	sf::Clock timer;
};

