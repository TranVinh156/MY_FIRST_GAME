#ifndef TILE_H_
#define TILE_H_

#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "RenderWindow.h"
#include "Entity.h"

class Entity;

class Tile : public Entity
{
public:
	Tile(float p_x, float p_y, SDL_Texture* p_text, int p_type);
	~Tile() {};
	int getType() { return tile_type_; };
	void setType(int p_type) { tile_type_ = p_type; };
	void setX(int p_x);
	SDL_Rect getCollision() const { return collision; };

	
private:
	int tile_type_;
	SDL_Rect collision;
};


#endif // !TILE_H_

