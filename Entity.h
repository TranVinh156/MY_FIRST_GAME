#ifndef ENTITY_H_
#define ENTITY_H_

#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
//#include "Tile.h"



class Entity
{
public:
	Entity(float p_x, float p_y, SDL_Texture* p_text);
	~Entity();
	float getX() const;
	float getY() const;
	SDL_Texture* getText() const;
	SDL_RendererFlip getFlip() const;
	SDL_Rect getRect() const;

protected:
	SDL_Texture* text_ = NULL;
	SDL_Rect rect_;
	SDL_RendererFlip flip_type_ = SDL_FLIP_NONE;
	float x_, y_;
private:

};



#endif // !ENTITY_H_

