#include "Bullet.h"

Bullet::Bullet(float p_x, float p_y, SDL_Texture* p_text) : Entity(p_x, p_y, p_text)
{
	collision.x = x_;
	collision.y = y_;
	collision.w = BULLET_WIDTH;
	collision.h = BULLET_HEIGHT;
	// set animation nếu mà có
	for (int i = 0; i < BULLET_ANIMATION_FRAME; i++)
	{
		bulletClips[i] = { i * BULLET_WIDTH, 0, BULLET_WIDTH, BULLET_HEIGHT };
	}

}

Bullet::~Bullet()
{

}

void Bullet::Update(std::vector<Game_Map>& levelList)
{
	if (bulletCounter / 5 < BULLET_ANIMATION_FRAME)
	{
		x_val_ += BULLET_SPEED;
		if (getFlip() == SDL_FLIP_NONE)
		{
			x_ += x_val_;
		}

		collision.x = getX();

		if (getFlip() == SDL_FLIP_HORIZONTAL)
		{
			x_ -= x_val_;
		}

		if (getX() < 0)
		{
			x_ = 0;
			collision.x = getX();
		}

		if (CommonFunc::checkToMap(collision, levelList, levelSTT))
		{
			if (getFlip() == SDL_FLIP_NONE)
			{
				x_ -= x_val_;
			}
			if (getFlip() == SDL_FLIP_HORIZONTAL)
			{
				x_ += x_val_;
			}
		}
	}
	else if (bulletCounter / 5 == BULLET_ANIMATION_FRAME)
	{
		setMove(false);
	}
	
	
}

void Bullet::setFlip(SDL_RendererFlip p_flip)
{
	flip_type_ = p_flip;
}

void Bullet::setSize_Position(const int& p_playerX)
{
	if (getFlip() == SDL_FLIP_HORIZONTAL)
	{
		x_ = p_playerX;
	}
}


void Bullet::Render(SDL_Rect& camera, SDL_Texture* p_bulletText)
{
	
		CommonFunc::renderAnimation(p_bulletText, x_, y_, &bulletClips[bulletCounter / 5], &camera, 0, NULL, getFlip());
		bulletCounter++;
		if (bulletCounter / 5 == BULLET_ANIMATION_FRAME)
		{
			//bulletCounter = 0;
			//setMove(false);
		}
	
}