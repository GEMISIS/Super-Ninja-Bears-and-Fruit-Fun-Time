#include <iostream>
#include <fstream>
#include <sstream>

#include "map.h"
#include "npc1.h"
#include "npc2.h"

#include "enemy.h"
#include "mob1.h"
#include "mob2.h"

#include "collectible.h"
#include "guava.h"
#include "bronze_coin.h"
#include "silver_coin.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

Map::Map(EntityManager* entityManager)
{
	this->backgroundTexture = new sf::Texture();
	this->tiles = std::unordered_map<int, sf::Image*>();
	this->entityManager = entityManager;
	this->background = new sf::Sprite();
}

void Map::Load(std::string filename, std::string backgroundFile, Speech* speech)
{
	this->backgroundTexture->loadFromFile("Graphics/Backgrounds/" + backgroundFile);
	this->backgroundTexture->setRepeated(true);
	this->background->setTexture(*this->backgroundTexture);
	this->background->scale(2.0f, 2.0f);

	std::ifstream mapFile("Graphics/maps/" + filename);
	std::string mapFileData((std::istreambuf_iterator<char>(mapFile)),
		std::istreambuf_iterator<char>());
	Document mapFileDoc;
	mapFileDoc.Parse(mapFileData.c_str());

	this->width = mapFileDoc["width"].GetInt();
	this->height = mapFileDoc["height"].GetInt();
	this->tileWidth = mapFileDoc["tilewidth"].GetInt();
	this->tileHeight = mapFileDoc["tileheight"].GetInt();

	Value& dataArray = mapFileDoc["layers"];
	if (dataArray.IsArray())
	{
		for (int i = 0; i < dataArray.Capacity(); i += 1)
		{
			Value& data = dataArray[i]["data"];
			int* tempData = new int[this->width * this->height];

			if (dataArray[i].HasMember("actives") && dataArray[i]["actives"].IsArray())
			{
				Value& actives = dataArray[i]["actives"];

				for (int y = 0; y < this->height; y += 1)
				{
					for (int x = 0; x < this->width; x += 1)
					{
						if (actives.Capacity() > (x + y * this->width) && !actives[x + y * this->width].GetBool())
						{
							tempData[x + y * this->width] = 0;
						}
						else
						{
							tempData[x + y * this->width] = data[x + y * this->width].GetInt();
						}
					}
				}
			}
			else
			{
				for (int y = 0; y < this->height; y += 1)
				{
					for (int x = 0; x < this->width; x += 1)
					{
						tempData[x + y * this->width] = data[x + y * this->width].GetInt();
					}
				}
			}

			std::cout << i << std::endl;
			std::pair<std::string, int*> tempPair(dataArray[i]["name"].GetString(), tempData);
			this->data.insert(tempPair);
		}
	}

	int groundTileSet = -1;
	int npcTileSet = -1;
	int mobsTileSet = -1;
	int collectTileSet = -1;

	Value& tileList = mapFileDoc["tilesets"];
	if (tileList.IsArray())
	{
		for (int i = 0; i < tileList.Capacity(); i += 1)
		{
			std::string name = mapFileDoc["tilesets"][i]["name"].GetString();
			if (name == "Ground")
			{
				groundTileSet = i;
			}
			if (name == "NPC")
			{
				npcTileSet = i;
			}
			if (name == "Mobs")
			{
				mobsTileSet = i;
			}
			if (name == "Collectibles")
			{
				collectTileSet = i;
			}

			if (i == groundTileSet)
			{
				Value& tileData = mapFileDoc["tilesets"][i]["tiles"];
				int firstgid = mapFileDoc["tilesets"][i]["firstgid"].GetInt();
				for (Value::ConstMemberIterator iterator = tileData.MemberBegin(); iterator != tileData.MemberEnd(); iterator += 1)
				{
					sf::Image* temp = new sf::Image();
					temp->loadFromFile("Graphics/" + std::string(iterator->value["image"].GetString()));

					std::string name = iterator->name.GetString();
					std::pair<int, sf::Image*> tempPair(std::stoi(name) + 0 + firstgid, temp);
					this->tiles.insert(tempPair);

					tile_properties_t tempProps = { std::stoi(name) + 0 + firstgid, NO_GROUP, false, "", "" };
					if (mapFileDoc["tilesets"][i]["tileproperties"].HasMember(name.c_str()))
					{
						if (mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()].HasMember("wavy"))
						{
							tempProps.wavy = true;
						}
						if (mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()].HasMember("nextArea") &&
							mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()].HasMember("nextAreaEntry"))
						{
							tempProps.nextArea = mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()]["nextArea"].GetString();
							tempProps.nextAreaEntry = mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()]["nextAreaEntry"].GetString();
						}
						if (mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()].HasMember("group"))
						{
							std::string strValue = mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()]["group"].GetString();
							int intValue = std::stoi(strValue);
							tempProps.group = (tile_groups)intValue;
						}
					}
					std::pair<int, tile_properties_t> tempPropsPair(std::stoi(name) + 0 + firstgid, tempProps);
					this->tileProperties.insert(tempPropsPair);

					if (tempProps.wavy)
					{
						sf::Image* temp2 = new sf::Image();
						temp2->loadFromFile("Graphics/" + std::string(iterator->value["image"].GetString()));
						temp2->flipHorizontally();
						std::pair<int, sf::Image*> tempPair2(std::stoi(name) + 0 + firstgid, temp2);
						this->flippedTiles.insert(tempPair2);
					}
				}
			}
		}
	}

	this->groundTileStart = mapFileDoc["tilesets"][groundTileSet]["firstgid"].GetInt();
	if (npcTileSet > -1)
	{
		this->npcTileStart = mapFileDoc["tilesets"][npcTileSet]["firstgid"].GetInt();
	}
	if (mobsTileSet > -1)
	{
		this->mobsTileStart = mapFileDoc["tilesets"][mobsTileSet]["firstgid"].GetInt();
	}
	if (collectTileSet > -1)
	{
		this->collectTileStart = mapFileDoc["tilesets"][collectTileSet]["firstgid"].GetInt();
	}

	for (auto iterator = this->data.begin(); iterator != this->data.end(); iterator++)
	{
		std::cout << iterator->first;
		int* layerData = iterator->second;
		sf::Texture* tempTexture = new sf::Texture();
		tempTexture->create(this->width * this->tileWidth, this->height * this->tileHeight);
		std::pair<std::string, sf::Texture*> tempPairTex(iterator->first, tempTexture);
		this->tileTextures.insert(tempPairTex);

		for (int y = 0; y < this->height; y += 1)
		{
			for (int x = 0; x < this->width; x += 1)
			{
				if (layerData[x + y * this->width] > 0 && layerData[x + y * this->width] < mapFileDoc["tilesets"][npcTileSet]["firstgid"].GetInt()
					&& layerData[x + y * this->width] < mapFileDoc["tilesets"][mobsTileSet]["firstgid"].GetInt())
				{
					this->tileTextures[iterator->first]->update(*this->tiles[layerData[x + y * this->width]], x * this->tileWidth, y * this->tileHeight);
					if (saveSystem.currentAreaEntry != "" && this->tileProperties[layerData[x + y * this->width]].nextAreaEntry != "")
					{
						if (saveSystem.currentAreaEntry == this->tileProperties[layerData[x + y * this->width]].nextAreaEntry)
						{
							saveSystem.x = x * this->tileWidth;
							saveSystem.y = y * this->tileHeight - this->tileHeight / 2;
						}
					}
				}
				else if (layerData[x + y * this->width] >= mapFileDoc["tilesets"][npcTileSet]["firstgid"].GetInt() && layerData[x + y * this->width] < mapFileDoc["tilesets"][mobsTileSet]["firstgid"].GetInt())
				{
					Value& tileSet = mapFileDoc["tilesets"][npcTileSet];
					int tile = layerData[x + y * this->width] - tileSet["firstgid"].GetInt();

					std::string characterName = "hilly";
					if (tileSet.HasMember("tileproperties"))
					{
						if (tileSet["tileproperties"].HasMember(std::to_string(tile).c_str()))
						{
							if (tileSet["tileproperties"][std::to_string(tile).c_str()].HasMember("characterName"))
							{
								characterName = tileSet["tileproperties"][std::to_string(tile).c_str()]["characterName"].GetString();
							}
						}
					}
					base_npc* temp = new base_npc(speech, tileSet["tiles"][std::to_string(tile).c_str()]["image"].GetString(), this,
						characterName, tileSet["tileproperties"][std::to_string(tile).c_str()]["text"].GetString(), 
						x * this->tileWidth, y * this->tileHeight);
					this->entityManager->Add("hilly", temp);
					temp->offset(-(temp->getGlobalBounds().width - (float)this->tileWidth), -(temp->getGlobalBounds().height - (float)this->tileHeight));
					layerData[x + y * this->width] = 0;
				}
				else if (layerData[x + y * this->width] >= mapFileDoc["tilesets"][mobsTileSet]["firstgid"].GetInt() && layerData[x + y * this->width] < mapFileDoc["tilesets"][collectTileSet]["firstgid"].GetInt())
				{
					Value& tileSet = mapFileDoc["tilesets"][mobsTileSet];
					int tile = layerData[x + y * this->width] - tileSet["firstgid"].GetInt();

					int type = 0;
					int health = 1;
					int damage = 1;
					if (tileSet.HasMember("tileproperties"))
					{
						if (tileSet["tileproperties"].HasMember(std::to_string(tile).c_str()))
						{
							if (tileSet["tileproperties"][std::to_string(tile).c_str()].HasMember("health"))
							{
								health = tileSet["tileproperties"][std::to_string(tile).c_str()]["health"].GetInt();
							}
							if (tileSet["tileproperties"][std::to_string(tile).c_str()].HasMember("damage"))
							{
								damage = tileSet["tileproperties"][std::to_string(tile).c_str()]["damage"].GetInt();
							}
							if (tileSet["tileproperties"][std::to_string(tile).c_str()].HasMember("type"))
							{
								type = tileSet["tileproperties"][std::to_string(tile).c_str()]["type"].GetInt();
							}
						}
					}

					enemy* temp = NULL;

					switch ((type == 0) ? tile : type)
					{
					default:
						temp = new Mob1(this, x * this->tileWidth, y * this->tileHeight, health, damage);
						break;
					}
					this->entityManager->Add("mobby", temp);
					temp->offset(-(temp->getGlobalBounds().width - (float)this->tileWidth), -(temp->getGlobalBounds().height - (float)this->tileHeight));
					layerData[x + y * this->width] = 0;
				}
				else if (layerData[x + y * this->width] >= mapFileDoc["tilesets"][collectTileSet]["firstgid"].GetInt())
				{
					Value& tileSet = mapFileDoc["tilesets"][collectTileSet];
					int tile = layerData[x + y * this->width] - tileSet["firstgid"].GetInt();

					int type = 0;
					if (tileSet.HasMember("tileproperties"))
					{
						if (tileSet["tileproperties"].HasMember(std::to_string(tile).c_str()))
						{
							if (tileSet["tileproperties"][std::to_string(tile).c_str()].HasMember("type"))
							{
								type = tileSet["tileproperties"][std::to_string(tile).c_str()]["type"].GetInt();
							}
						}
					}

					collectible* temp = NULL;
					switch ((type == 0) ? tile : type)
					{
					case 1:
						temp = new bronze_coin(this, x * this->tileWidth, y * this->tileHeight);
						break;
					case 3:
						temp = new silver_coin(this, x * this->tileWidth, y * this->tileHeight);
						break;
					default:
						temp = new guava(this, x * this->tileWidth, y * this->tileHeight);
						break;
					}
					this->entityManager->Add("collectible", temp);
					temp->offset(-(temp->getGlobalBounds().width - (float)this->tileWidth), -(temp->getGlobalBounds().height - (float)this->tileHeight));
					layerData[x + y * this->width] = 0;
				}
			}
		}

		std::pair<std::string, sf::Sprite*> tempPairSpr(iterator->first, new sf::Sprite());
		this->mapTiles.insert(tempPairSpr);

		this->mapTiles[iterator->first]->setTexture(*this->tileTextures[iterator->first]);
	}

	mapFile.close();
}

sf::FloatRect Map::getGlobalBounds()
{
	return this->mapTiles["Ground"]->getGlobalBounds();
}

void Map::move(float x, float y)
{
	for (auto it = this->mapTiles.begin(); it != mapTiles.end(); it++)
	{
		it->second->move(x, y);
	}
	this->background->move(x * 0.5f, y * 0.5f);
}

void Map::move(sf::Vector2f amount)
{
	this->move(amount.x, amount.y);
}


bool flip[] = { false, false, false, false, false, false };

void Map::Update(sf::RenderWindow* window)
{
	sf::Time elapsed = this->timer.getElapsedTime();
	if (elapsed.asMilliseconds() > 16 * 15)
	{
		int count = 0;
		for (auto it = this->data.begin(); it != this->data.end(); it++)
		{
			int* layerData = this->data[it->first];
			for (int y = 0; y < this->height; y += 1)
			{
				for (int x = 0; x < this->width; x += 1)
				{
					int tile = layerData[x + y * this->width];
					if (tile > 0)
					{
						if (this->tileProperties[tile].wavy)
						{
							if (flip[count])
							{
								this->tileTextures[it->first]->update(*this->flippedTiles[tile], x * this->tileWidth, y * this->tileHeight);
							}
							else
							{
								this->tileTextures[it->first]->update(*this->tiles[tile], x * this->tileWidth, y * this->tileHeight);
							}
						}
					}
				}
				if (y < this->height - 1)
				{
					flip[count] = !flip[count];
				}
			}
			count += 1;
		}
		timer.restart();
	}
}

void Map::Render(sf::RenderWindow* window)
{
	window->draw(*this->background);
	for (auto it = this->mapTiles.begin(); it != this->mapTiles.end(); it++)
	{
		sf::Vector2f position = it->second->getPosition();
		if (position.x + it->second->getGlobalBounds().width >= 0 &&
			position.x <= window->getSize().x &&
			position.y + it->second->getGlobalBounds().height >= 0 &&
			position.y <= window->getSize().y)
		{
			window->draw(*it->second);
		}
	}
}

tile_properties_t Map::CheckCollision(Entity* entity, Direction direction, float offsetX, float offsetY)
{
	entity->move(Entity::scroll);
	int x = (int)(entity->getPosition().x + offsetX + entity->getGlobalBounds().width / 2) / this->tileWidth;
	int y = (int)(entity->getPosition().y + offsetY + entity->getGlobalBounds().height / 2) / this->tileHeight;
	switch (direction)
	{
	case LEFT:
		x = (int)(entity->getPosition().x - offsetX + entity->getGlobalBounds().width) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height / 2) / this->tileHeight;
		break;
	case RIGHT:
		x = (int)(entity->getPosition().x + offsetX) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height / 2) / this->tileHeight;
		break;
	case UP:
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width / 2) / this->tileWidth;
		y = (int)(entity->getPosition().y - offsetY) / this->tileHeight;
		break;
	case DOWN:
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width / 2) / this->tileWidth;
		y = (int)(entity->getPosition().y + offsetY + entity->getGlobalBounds().height) / this->tileHeight;
		break;
	case TOP_LEFT:
		x = (int)(entity->getPosition().x - offsetX + entity->getGlobalBounds().width) / this->tileWidth;
		y = (int)(entity->getPosition().y - offsetY) / this->tileHeight;
		break;
	case TOP_RIGHT:
		x = (int)(entity->getPosition().x + offsetX) / this->tileWidth;
		y = (int)(entity->getPosition().y - offsetY) / this->tileHeight;
		break;
	case BOTTOM_LEFT:
		x = (int)(entity->getPosition().x - offsetX + entity->getGlobalBounds().width) / this->tileWidth;
		y = (int)(entity->getPosition().y + offsetY + entity->getGlobalBounds().height) / this->tileHeight;
		break;
	case BOTTOM_RIGHT:
		x = (int)(entity->getPosition().x + offsetX) / this->tileWidth;
		y = (int)(entity->getPosition().y + offsetY + entity->getGlobalBounds().height) / this->tileHeight;
		break;
	}
	entity->move(-Entity::scroll);
	if (this->data["Ground"][x + y * this->width] == 0)
	{
		tile_properties_t temp = { 0, NO_GROUP, false };
		return temp;
	}
	else
	{
		return this->tileProperties[this->data["Ground"][x + y * this->width]];
	}
}

Map::~Map()
{
	for (auto iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++)
	{
		delete iterator->second;
	}
	for (auto iterator = this->flippedTiles.begin(); iterator != this->flippedTiles.end(); iterator++)
	{
		delete iterator->second;
	}
	for (auto iterator = this->data.begin(); iterator != this->data.end(); iterator++)
	{
		delete iterator->second;
	}
	for (auto iterator = this->tileTextures.begin(); iterator != this->tileTextures.end(); iterator++)
	{
		delete iterator->second;
	}
	for (auto iterator = this->mapTiles.begin(); iterator != this->mapTiles.end(); iterator++)
	{
		delete iterator->second;
	}
	this->data.clear();
	this->tiles.clear();
	this->tileProperties.clear();

	delete this->backgroundTexture;
	delete this->background;
}