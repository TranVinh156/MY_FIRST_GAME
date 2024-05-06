#include "Gamemap.h"

Game_Map::Game_Map(float p_x, float p_y, const char* p_path, SDL_Texture* p_text)
	:x_(p_x), y_(p_y)
{
	//Success flag
	bool tilesLoaded = true;

	int x = getX(), y = getY();

	std::ifstream map(p_path);

	if (map.fail()) {
		tilesLoaded = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_TILE; ++i) 
		{
			int tileType = -1;
			map >> tileType;
			if (map.fail()) {
				tilesLoaded = false;
				break;
			}

			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITE)) {
				Tile* tile = new Tile(x, y, p_text, tileType);
				tilesList.push_back(tile);
			}
			else
			{
				tilesLoaded = false;
				break;
			}

			if (tileType == 46)
			{
				undeadPos.push_back(i);
			}

			if (tileType == 47)
			{
				archerPos.push_back(i);
			}

			if (tileType == 48)
			{
				playerPos = i;
			}

			if (tileType == 243)
			{
				gatePos = i;
			}

			x += TILE_WIDTH;

			if (x >= getX() + LEVEL_WIDTH) {
				x = getX();
				y += TILE_HEIGHT;
			}
		}
	}
	map.close();
}

void Game_Map::SetTileType(const char* p_path)
{
	bool tilesLoaded = true;
	int x = getX(), y = getY();
	std::ifstream map(p_path);

	if (map.fail()) 
	{
		tilesLoaded = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_TILE; ++i) 
		{
			int tileType = -1;

			map >> tileType;

			if (map.fail()) {
				tilesLoaded = false;
				break;
			}

			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITE)) {
				tilesList.at(i)->setType(tileType);
			}
			else
			{
				tilesLoaded = false;
				break;
			}

			x += TILE_WIDTH;

			if (x >= getX() + LEVEL_WIDTH) {
				x = getX();
				y += TILE_HEIGHT;
			}
		}
	}
	map.close();
}

void Game_Map::SetLevelX(Game_Map& map)
{
	x_ = map.getX();
	for (int i = 0; i < tilesList.size(); i++) {
		int row = i / 45;
		int colum = (i - row * 45) * TILE_WIDTH + getX();
		tilesList.at(i)->setX(colum);
	}
}

void Game_Map::SetLevelX(float p_x)
{
	x_ = p_x;
	for (int i = 0; i < tilesList.size(); i++) {
		int row = i / 45;
		int colum = (i - row * 45) * TILE_WIDTH + getX();
		tilesList.at(i)->setX(colum);
		//tilesList.at(i)->setY(TILE_HEIGHT);
	}
}

void Game_Map::render(SDL_Rect p_tileClips[], SDL_Rect* p_camera)
{
	for (int i = 0; i < tilesList.size(); i++)
	{	
		SDL_Rect dst = { tilesList.at(i)->getX(), tilesList.at(i)->getY(), TILE_WIDTH, TILE_HEIGHT };
		CommonFunc::renderTile(*tilesList.at(i), &p_tileClips[tilesList.at(i)->getType() - 1], p_camera);
	}
}