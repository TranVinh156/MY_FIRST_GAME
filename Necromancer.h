#ifndef NECROMANCER_H_
#define NECROMANCER_H_

#pragma once

#include "RenderWindow.h"


class Necromancer : public Entity
{
public:
	Necromancer(float p_x, float p_y, SDL_Texture* p_text);
	~Necromancer(){};
	void Update();
	void Render(SDL_Rect& camera);

private:

	//SDL_Rect
};



#endif // !NECROMANCER_H_


