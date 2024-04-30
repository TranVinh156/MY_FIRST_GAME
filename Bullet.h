#ifndef BULLET_H_
#define BULLET_H_

#pragma once
#include "RenderWindow.h"

const int BULLET_SPEED = 1;

class Bullet : public Entity
{
public:
	Bullet(float p_x, float p_y, SDL_Texture* p_text);
	~Bullet();

	void Update(std::vector<Game_Map>& levelList);

	int GetType() const { return bulletType; }
	bool isMoving() const { return moving; }
	SDL_Rect getCollision() const { return collision; }

	void setFlip(SDL_RendererFlip p_flip);
	void setMove(bool p_move) { moving = p_move; }
	void setType(const int& p_type) { bulletType = p_type; }
	void setSize_Position(const int& p_playerX, int p_height = 32);
	void setXVal(float p_x_val) { x_val_ = p_x_val; };
	void Render(SDL_Rect& camera, SDL_Texture* p_bulletText);


	enum bulletType
	{
		NONE = 0,
		NORMAL = 1,
	};

private:
	const int BULLET_WIDTH = 32;
	const int BULLET_HEIGHT = 32;
	
	
	float x_val_ = 1;
	bool moving = false;
	int bulletType;
	SDL_Rect collision;

	static const int BULLET_ANIMATION_FRAME = 4;
	SDL_Rect bulletClips[BULLET_ANIMATION_FRAME];

	int bulletCounter = 0;
	int levelSTT = 1;
	const int delay = 4;
};



#endif // !BULLET_H_


#pragma once
