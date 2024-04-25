#include "Tile.h"

Tile::Tile(float p_x, float p_y, SDL_Texture* p_text, int p_type) : Entity(p_x, p_y, p_text)
{
	collision.x = p_x;
	collision.y = p_y;
	collision.w = TILE_WIDTH;
	collision.h = TILE_HEIGHT;

	tile_type_ = p_type;
}

void Tile::setX(int p_x)
{
	x_ = p_x;
	collision.x = getX();
}

