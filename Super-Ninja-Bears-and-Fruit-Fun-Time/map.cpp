#include <iostream>
#include <fstream>
#include <sstream>

#include "map.h"
#include "npc1.h"
#include "npc2.h"
#include "mob1.h"
#include "mob2.h"
#include "enemy.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

Map::Map(EntityManager* entityManager)
{
	this->texture = new sf::Texture();
	this->backgroundTexture = new sf::Texture();
	this->tiles = std::unordered_map<std::string, sf::Image*>();
	this->mapTiles = new sf::Sprite();
	this->entityManager = entityManager;
	this->background = new sf::Sprite();
}

void Map::Load(std::string filename, std::string backgroundFile, Speech* speech)
{
	this->backgroundTexture->loadFromFile(backgroundFile);
	this->background->setTexture(*this->backgroundTexture);

	std::ifstream mapFile("Graphics/maps/" + filename);
	std::string mapFileData((std::istreambuf_iterator<char>(mapFile)),
		std::istreambuf_iterator<char>());
	Document mapFileDoc;
	mapFileDoc.Parse(mapFileData.c_str());

	this->width = mapFileDoc["width"].GetInt();
	this->height = mapFileDoc["height"].GetInt();
	this->tileWidth = mapFileDoc["tilewidth"].GetInt();
	this->tileHeight = mapFileDoc["tileheight"].GetInt();

	Value& data = mapFileDoc["layers"][0]["data"];
	this->data = new int[this->width * this->height];
	for (int y = 0; y < this->height; y += 1)
	{
		for (int x = 0; x < this->width; x += 1)
		{
			this->data[x + y * this->width] = data[x + y * this->width].GetInt();
		}
	}

	this->texture->create(this->width * this->tileWidth, this->height * this->tileHeight);

	Value& tileData = mapFileDoc["tilesets"][0]["tiles"];
	for (Value::ConstMemberIterator iterator = tileData.MemberBegin(); iterator != tileData.MemberEnd(); iterator += 1)
	{
		sf::Image* temp = new sf::Image();
		temp->loadFromFile(iterator->value["image"].GetString());
		std::pair<std::string, sf::Image*> tempPair(iterator->name.GetString(), temp);
		this->tiles.insert(tempPair);
	}

	for (int y = 0; y < this->height; y += 1)
	{
		for (int x = 0; x < this->width; x += 1)
		{
			switch (this->data[x + y * this->width])
			{
			case 0:
				break;
			case 1:
				this->texture->update(tile1, x * 64, y * 64);
				break;
			case 2:
				this->texture->update(tile2, x * 64, y * 64);
				break;
			case 3:
				this->texture->update(tile3, x * 64, y * 64);
				break;
			case 4:
				this->texture->update(tile4, x * 64, y * 64);
				break;
			case 5:
				this->texture->update(tile5, x * 64, y * 64);
				break;
			case 6:
				this->entityManager->Add("npc1", new Npc1(speech, this, x * 64, y * 64 + 32));
				this->data[x + y * this->width] = 0;
				break;
			case 7:
				this->entityManager->Add("npc2", new Npc2(speech, this, x * 64, y * 64 + 32));
				this->data[x + y * this->width] = 0;
				break;
			case 8:
				this->entityManager->Add("mob1", new Mob1(this, x * 64, y * 64 + 32));
				this->data[x + y * this->width] = 0;
				break;
			case 9:
				this->entityManager->Add("mob2", new Mob2(this, x * 64, y * 64 + 32));
				this->data[x + y * this->width] = 0;
				break;
			}
		}
	}

	this->mapTiles->setTexture(*this->texture);
	mapFile.close();
}

sf::FloatRect Map::getGlobalBounds()
{
	return this->mapTiles->getGlobalBounds();
}

void Map::move(float x, float y)
{
	this->mapTiles->move(x, y);
	this->background->move(x * 0.5f, y * 0.5f);
}

void Map::move(sf::Vector2f amount)
{
	this->move(amount.x, amount.y);
}


void Map::Update(sf::RenderWindow* window)
{
}

void Map::Render(sf::RenderWindow* window)
{
	window->draw(*this->background);
	window->draw(*this->mapTiles);
}

int Map::CheckCollision(Entity* entity, Direction direction)
{
	entity->move(Entity::scroll);
	int x = (int)(entity->getPosition().x + entity->getGlobalBounds().width / 2) / this->tileWidth;
	int y = (int)(entity->getPosition().y + entity->getGlobalBounds().height / 2) / this->tileHeight;
	switch (direction)
	{
	case LEFT:
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height / 2) / this->tileHeight;
		break;
	case RIGHT:
		x = (int)(entity->getPosition().x) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height / 2) / this->tileHeight;
		break;
	case UP:
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width / 2) / this->tileWidth;
		y = (int)(entity->getPosition().y) / this->tileHeight;
		break;
	case DOWN:
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width / 2) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height) / this->tileHeight;
		break;
	case TOP_LEFT:
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width) / this->tileWidth;
		y = (int)(entity->getPosition().y) / this->tileHeight;
		break;
	case TOP_RIGHT:
		x = (int)(entity->getPosition().x) / this->tileWidth;
		y = (int)(entity->getPosition().y) / this->tileHeight;
		break;
	case BOTTOM_LEFT:
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height) / this->tileHeight;
		break;
	case BOTTOM_RIGHT:
		x = (int)(entity->getPosition().x) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height) / this->tileHeight;
		break;
	}
	entity->move(-Entity::scroll);
	return this->data[x + y * this->width];
}


Map::~Map()
{
	this->tiles.clear();
	delete this->texture;
	delete this->mapTiles;
	delete this->data;
	delete this->background;
}