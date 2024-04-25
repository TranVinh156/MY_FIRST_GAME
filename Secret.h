#ifndef SECRET_H_
#define SECRET_H_

#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>

#include "RenderWindow.h"
#include "Entity.h"
#include "Skeleton.h"
#include "BOSS.h"


class Secret : public Entity
{
public:
	Secret(float p_x, float p_y, SDL_Texture* p_text);
	~Secret() {};
	void HandleInput(SDL_Event& events);
	void HandleCamera(SDL_Rect& camera, int& p_levelSTT, bool& p_nextlevel, int& p_skeCount);

	void Update(std::vector<Skeleton*>& skeletonList, std::vector<Game_Map>& levelList, Boss& p_boss);// 

	void Attack();
	void Jump();
	void Gravity();
	void KnockBack();
	void Shield();

	bool getDead() { return death_; };

	void GetHit(std::vector<Skeleton*>& skeletonList, Boss& p_boss);

	bool isAttacking();
	bool isParrying() const { return parry_; };
	bool isBuffParrying() const { return shieldBuffParry_; };
	int getShieldCounter() const { return shieldCounter; };
	void HandleCamera(); // chưa biết làm gì

	void Render(SDL_Rect& camera);//SDL_Rect* p_camera); // Nhớ thêm camera
	SDL_Rect getCollision() { return Collision; };
	int getLevelSTT() const { return levelSTT; };
	void setParry(bool p_parry) 
	{
		parry_ = p_parry;
	}
	void setBuffParry(bool p_buffparry)
	{
		shieldBuffParry_ = p_buffparry;
	}
	void GoToNextLevel();
	void ResetSecret();

private:
	float x_val_ = 0;
	float y_val_ = 0;
	//176 96

	static const int IDLING_ANIMATION_FRAME = 6;
	static const int RUNNING_ANIMATION_FRAME = 33;
	static const int ATTACKING_ANIMATION_FRAME = 42;
	static const int JUMPING_ANIMATION_FRAME = 3;
	static const int FALLING_ANIMATION_FRAME = 3;
	static const int DEATH_ANIMATION_FRAME = 26;
	static const int SHIELD_ANIMATION_FRAME = 21; // 7
	static const int SHIELDBUFF_ANIMATION_FRAME = 17;
	static const int REST_ANIMATION_FRAME = 42;

	SDL_Rect idlingClips[IDLING_ANIMATION_FRAME];
	SDL_Rect runningClips[RUNNING_ANIMATION_FRAME];
	SDL_Rect attackingClips[ATTACKING_ANIMATION_FRAME];
	SDL_Rect jumpingClips[JUMPING_ANIMATION_FRAME];
	SDL_Rect fallingClips[FALLING_ANIMATION_FRAME];
	SDL_Rect deathClips[DEATH_ANIMATION_FRAME];
	SDL_Rect shieldClips[SHIELD_ANIMATION_FRAME];
	SDL_Rect shieldBuffClips[SHIELDBUFF_ANIMATION_FRAME];
	SDL_Rect restClips[REST_ANIMATION_FRAME];

	SDL_Rect Collision;

	int idleCounter = 0;
	int runCounter = 0;
	int attackCounter = 0;
	int jumpCounter = 0;
	int fallCounter = 0;
	int deathCounter = 0;
	int shieldCounter = 0;
	int shieldBuffCounter = 0;
	int restCounter = 0;

	bool on_ground_ = false;
	bool idle_ = false;
	bool run_ = false;
	bool attack_ = false;
	bool jump_ = false;
	bool fall_ = false;
	bool death_ = false;
	bool combo_ = false;
	bool shield_ = false;
	bool parry_ = false;
	bool shieldBuff_ = false;
	bool shieldBuffParry_ = false;
	bool rest_ = false;
	bool down_ = false;
	bool final_ = false;
	bool doublejump_ = false;

	const int SECRET_WIDTH = 16;
	const int SECRET_HEIGHT = 32;

	const int FRAME_WIDTH = 176;
	const int FRAME_HEIGHT = 96;
	const int PLAYER_VAL = 6;


	int levelSTT = 0;
	int groundSTT = 0;


	int shieldBuffCooldown = 20;
	
};




#endif // !SECRET_H_
