#ifndef SKELETON_H_
#define SKELETON_H_

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

class Skeleton : public Entity
{
public:
	Skeleton(float p_x, float p_y, SDL_Texture* p_text);
	~Skeleton();
	void Update(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a);
	void AutoMovement(std::vector<Game_Map>& levelList);
	void MoveToPlayer(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a);
	
	void KnockBack();

	void GetHit(Player& p_player, Secret& p_secret, bool p_a);
	
	void Gravity();
	bool isDead();
	bool isAttacking();

	bool SetLoaded(Player& p_player, Secret& p_secret, bool p_a);
	void SetLevelSTT(int p_levelSTT) { levelSTT = p_levelSTT; };
	float getDistance() { return distanceToPlayer; };
	bool getLoaded() const { return loaded_; };
	void render(SDL_Rect& camera);
	SDL_Rect getCollision() { return collision; };
	
private:
	float x_val_;
	float y_val_;

	const int SKELETON_WIDTH = 16;
	const int SKELETON_HEIGHT = 16;
	const int SKELETON_VAL = 2;

	static const int IDLING_ANIMATION_FRAME = 11;
	static const int WALKING_ANIMATION_FRAME = 13;
	static const int ATTACKING_ANIMATION_FRAME = 15;
	static const int BEINGHIT_ANIMATION_FRAME = 8;
	static const int DEATH_ANIMATION_FRAME = 15;
	

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

	int skeleton_health_ = 3;
	int attackCooldown = 30;
	float distanceToPlayer;
	float distanceToSecret;

	bool loaded_ = false;
};



#endif // !SKELETON_H_

