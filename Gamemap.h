#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#pragma once

#include <vector>
#include <fstream>
#include "RenderWindow.h"
#include "Tile.h"



class Tile;

class Game_Map
{
public:
	Game_Map(float p_x, float p_y, const char* p_path, SDL_Texture* p_text);
	~Game_Map() {};
	void SetTileType(const char* p_path);
	void SetSkeletonPos(std::vector<int> p_skeletonPos) { skeletonPos = p_skeletonPos; };

	void SetLevelX(Game_Map& map);
	void SetLevelX(float p_x);
	void render(SDL_Rect p_tileClips[], SDL_Rect* p_camera);

	std::vector<Tile*> getTilesList() { return tilesList; };
	std::vector<int> GetSkeletonPos() { return skeletonPos; };
	float getX() const { return x_; };
	float getY() const { return y_; };
private:
	float x_;
	float y_;
	std::vector<Tile*> tilesList; //tileset
	std::vector<int> skeletonPos;


};


#endif // !GAME_MAP_H_

