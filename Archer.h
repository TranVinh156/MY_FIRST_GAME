#ifndef ARCHER_H_
#define ARCHER_H_

#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>

#include "RenderWindow.h"
#include "Entity.h"

class Game_Map;
class Player;
class Secret;
class Bullet;

class Archer : public Entity
{
public:
	enum SFX
	{
		attackSFX = 0,
		parrySFX = 1,
		deathSFX = 2,
		hurtSFX = 3,
	};
	Archer(float p_x, float p_y, SDL_Texture* p_text);
	~Archer();
	void Update(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a, Mix_Chunk* p_archerSFX[]);
	void AutoMovement(std::vector<Game_Map>& levelList);
	void MoveToPlayer(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a);

	void KnockBack();

	void GetHit(Player& p_player, Secret& p_secret, bool p_a, Mix_Chunk* p_archerSFX[]);

	void Gravity();
	bool isDead();
	bool isAttacking();

	bool SetLoaded(Player& p_player, Secret& p_secret, bool p_a);
	void SetLevelSTT(int p_levelSTT) { levelSTT = p_levelSTT; };
	float getDistance() { return distanceToPlayer; };
	bool getLoaded() const { return loaded_; };
	void render(SDL_Rect& camera, Mix_Chunk* p_archerSFX[]);
	SDL_Rect getCollision() { return collision; };
	void SetBulletList(std::vector<Bullet*> p_bulletList) { this->bulletList = p_bulletList; };
	std::vector<Bullet*> GetBulletList() { return bulletList; };

private:
	float x_val_;
	float y_val_;

	float x_start_;

	const int ARCHER_WIDTH = 16;
	const int ARCHER_HEIGHT = 32;
	const int ARCHER_VAL = 2;
	const int FRAME_WIDTH = 80;
	const int FRAME_HEIGHT = 48;
	const int MAX_DISTANCE = 100;

	static const int IDLING_ANIMATION_FRAME = 4;
	static const int WALKING_ANIMATION_FRAME = 8;
	static const int ATTACKING_ANIMATION_FRAME = 7;
	static const int BEINGHIT_ANIMATION_FRAME = 3;
	static const int DEATH_ANIMATION_FRAME = 8;


	SDL_Rect walkingClips[WALKING_ANIMATION_FRAME];
	SDL_Rect idlingClips[IDLING_ANIMATION_FRAME];
	SDL_Rect attackingClips[ATTACKING_ANIMATION_FRAME];
	SDL_Rect deathClips[DEATH_ANIMATION_FRAME];
	SDL_Rect beinghitClips[BEINGHIT_ANIMATION_FRAME];

	SDL_Rect collision;
	int idleCounter = 0;
	int attackCounter = 0;
	int walkCounter = 0;
	int beinghitCounter = 0;
	int deathCounter = 0;

	bool walk_ = false;
	bool idle_ = false;
	bool attack_ = false;
	bool death_ = false;
	bool beinghit_ = false;
	bool fall_ = false;
	bool on_ground_ = false;

	int groundSTT = 10;
	int levelSTT = 0;

	int archer_health_ = 3;
	int attackCooldown = 30;
	float distanceToPlayer;
	float distanceToSecret;

	bool loaded_ = false;
	std::vector<Bullet*> bulletList;
};



#endif // !Archer_H_


