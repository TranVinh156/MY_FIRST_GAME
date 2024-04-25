#ifndef GOBLIN_H_
#define GOBLIN_H_

#pragma once


#include "Entity.h"
#include "RenderWindow.h"


class Goblin:public Entity
{
public:
	Goblin(float p_x, float p_y, SDL_Texture* p_text);
	~Goblin() {};
	void Update(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a);
	void MoveToPlayer(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a);
	void Attack();
	bool isAttacking();
	SDL_Rect getCollision() { return collision; };

	void Gravity();
	void Render(SDL_Rect& camera);
	float getDistance() { return distanceToPlayer; };
	void GetHit(Player& p_player, Secret& p_secret);

	void ResetBoss()
	{
		//x_ = 16 * 20, 256;
		x_ = 9 * 720 + 16 * 20;
		y_ = 256;
		death_ = false;
		boss_health_ = 100;
	}
	bool GetDead() const { return death_; };
private:
	
	float x_val_;
	float y_val_;
	
	float distanceToPlayer;

	const int FRAME_WIDTH = 256;
	const int FRAME_HEIGHT = 112;

	const int BOSS_WIDTH = 64;
	const int BOSS_HEIGHT = 80;
	const int BOSS_VAL_ = 3;

	static const int IDLING_ANIMATION_FRAME = 14;
	static const int WALKING_ANIMATION_FRAME = 26;
	static const int STUNNING_ANIMATION_FRAME = 22;
	static const int ATTACKING_ANIMATION_FRAME = 24;
	static const int SP_ATTACKING_ANIMATION_FRAME = 28;
	static const int HURTING_ANIMATION_FRAME = 4;

	SDL_Rect idlingClips[IDLING_ANIMATION_FRAME];
	SDL_Rect walkingClips[WALKING_ANIMATION_FRAME];
	SDL_Rect stunningClips[STUNNING_ANIMATION_FRAME];
	SDL_Rect attackingClips[ATTACKING_ANIMATION_FRAME];
	SDL_Rect spAttackingClips[SP_ATTACKING_ANIMATION_FRAME];
	SDL_Rect hurtingClips[HURTING_ANIMATION_FRAME];

	SDL_Rect collision;

	int idleCounter = 0;
	int walkCounter = 0;
	int stunCounter = 0;
	int attackCounter = 0;
	int spAttackCounter = 0;
	int hurtCounter = 0;

	bool idle_ = false;
	bool walk_ = false;
	bool attack_ = false;
	bool spAttack_ = false;
	bool stun_ = false;
	bool hurt_ = false;
	bool fall_ = false;
	bool death_ = false;

	bool on_ground_ = false;
	int levelSTT = 1;
	int groundSTT = 0;

	int boss_health_ = 100;

	int attackCoolDown = 0;
	int spAttackCoolDown = 0;
};


#endif // !GOBLIN_H_H

