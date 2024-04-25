#include "Entity.h"


Entity::Entity(float p_x, float p_y, SDL_Texture* p_text)
	:x_(p_x), y_(p_y), text_(p_text)
{
	rect_.x = 0;
	rect_.y = 0;
	SDL_QueryTexture(p_text, NULL, NULL, &rect_.w, &rect_.h);
}

Entity::~Entity()
{

}

float Entity::getX() const
{
	return x_;
}

float Entity::getY() const
{
	return y_;
}

SDL_Texture* Entity::getText() const
{
	return text_;
}

SDL_Rect Entity::getRect() const
{
	return rect_;
}

SDL_RendererFlip Entity::getFlip() const
{
	return flip_type_;
}