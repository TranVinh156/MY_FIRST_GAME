#ifndef PLAYER_H_
#define PLAYER_H_

#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>

#include "RenderWindow.h"
#include "Entity.h"
#include "Undead.h"
#include "BOSS.h"



class Game_Map;
class Undead;
class Boss;
class Archer;
//class Entity {};

class Player : public Entity
{
public:
	Player(float p_x, float p_y, SDL_Texture* p_text);
	~Player();

	enum SFX
	{
		stepSFX = 0,
		jumpSFX = 1,
		landSFX = 2,
		attackSFX = 3,
		deathSFX = 4,
	};

	void HandleInput(SDL_Event& events, Mix_Chunk* p_playerSFX[]);
	void HandleCamera(SDL_Rect& camera, int& p_levelSTT, bool& p_nextlevel, int& p_skeCount);

	void Update(std::vector<Undead*>& undeadList, std::vector<Archer*>& archerList, std::vector<Game_Map>& levelList, Boss& p_boss, Mix_Chunk* p_playerSFX[]);// 

	void Attack();
	void Jump();
	void Gravity();
	void KnockBack();
	void Roll();

	bool getDead() { return death_; };

	void GetHit(std::vector<Undead*>& undeadList, std::vector<Archer*>& archerList, Boss& p_boss, Mix_Chunk* p_playerSFX[]);

	bool isAttacking();
	void HandleCamera(); // chưa biết làm gì

	void Render(SDL_Rect& camera, Mix_Chunk* p_playerSFX[]);//SDL_Rect* p_camera); // Nhớ thêm camera
	SDL_Rect getCollision() { return Collision; };
	int getLevelSTT() const { return levelSTT; };
	void GoToNextLevel(std::vector<Game_Map> levelList);
	void ResetPlayer(std::vector<Game_Map> levelList);

private:
	//float x_pos_;
	//float y_pos_;

	float x_val_ = 0;
	float y_val_ = 0;

	static const int RUNNING_ANIMATION_FRAME = 10;
	static const int IDLING_ANIMATION_FRAME = 10;
	static const int JUMPING_ANIMATION_FRAME = 3;
	static const int FALLING_ANIMATION_FRAME = 3;
	static const int ATTACKING_ANIMATION_FRAME = 10;
	static const int DEATH_ANIMATION_FRAME = 10;
	
	static const int ROLLING_ANIMATION_FRAME = 12;

	SDL_Rect runningClips[RUNNING_ANIMATION_FRAME];
	SDL_Rect idlingClips[IDLING_ANIMATION_FRAME];
	SDL_Rect jumpingClips[JUMPING_ANIMATION_FRAME];
	SDL_Rect fallingClips[FALLING_ANIMATION_FRAME];
	SDL_Rect attackingClips[ATTACKING_ANIMATION_FRAME];
	SDL_Rect deathClips[DEATH_ANIMATION_FRAME];

	SDL_Rect rollingClips[ROLLING_ANIMATION_FRAME];

	SDL_Rect Collision;

	int idleCounter = 0;
	int runCounter = 0;
	int jumpCounter = 0;
	int fallCounter = 0;
	int attackCounter = 0;
	int deathCounter = 0;
	int beinghitCounter = 0;
	
	int rollCounter = 0;

	bool idle_ = false;
	bool run_ = false;
	bool jump_ = false;
	bool fall_ = false;
	bool attack_ = false;
	bool death_ = false;
	bool beinghit_ = false;
	bool on_ground_ = false;
	bool combo_ = false;
	bool roll_ = false;
	bool down_ = false;
	bool final_ = false;
	bool doublejump_ = false;
	int rollCooldown = 0;

	int groundSTT = 18;
	int levelSTT = 0;
	int wallSTT = 0;

	int player_health_ = 3;

	const int PLAYER_WIDTH = 16;
	const int PLAYER_HEIGHT = 32; // ????
	const int PLAYER_VAL = 6;
	const int FRAME_WIDTH = 112;
	const int FRAME_HEIGHT = 80;
};








#endif // !PLAYER_H_
