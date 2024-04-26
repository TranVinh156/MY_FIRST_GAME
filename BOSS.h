#ifndef BOSS_H_
#define BOSS_H_


#pragma once

#include "Entity.h"
#include "RenderWindow.h"

class Player;
class Game_Map;
class Bullet;
class Secret;

class Boss : public Entity
{
public:
	enum
	{
		attackSFX = 0,
		parrySFX = 1,
		spellSFX = 2,
		hurtSFX = 3,
		deathSFX = 4,
	};
	Boss(float p_x, float p_y, SDL_Texture* p_text);
	~Boss() {};
	void Update(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a, Mix_Chunk* p_bossSFX[]);
	void MoveToPlayer(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a, Mix_Chunk* p_bossSFX[]);
	void Attack();
	void Smash();
	void Spell();
	bool isAttacking();
	bool isSmashing();
	bool isSpelling() { return spell_; };
	SDL_Rect getCollision() { return collision; };

	void Gravity();
	void Render(SDL_Rect& camera, Mix_Chunk* p_bossSFX[]);
	float getDistance() { return distanceToPlayer; };
	void GetHit(Player& p_player, Secret& p_secret, Mix_Chunk* p_bossSFX[]);

	void ResetBoss()
	{
		//x_ = 16 * 20, 256;
		x_ = 16 * (20331 % 565) - 112;
		y_ = 16 * (20331 / 565) - 80 - 80;
		death_ = false;
		deathCounter = 0;
		spell_ = false;
		boss_health_ = 100;
		for (int i = 0; i < bulletList.size(); i++)
		{
			delete bulletList.at(i);
			bulletList.at(i) = NULL;
			bulletList.erase(bulletList.begin() + i);
		}

	}
	bool GetDead() const { if (deathCounter >= 84) return death_; else return false; };
	void SetBulletList(std::vector<Bullet*> p_bulletList) { this->bulletList = p_bulletList; };
	std::vector<Bullet*> GetBulletList() { return bulletList; };

private:
	float x_val_; 
	float y_val_;

	const int BOSS_WIDTH = 64;
	const int BOSS_HEIGHT = 80;
	const int BOSS_VAL_ = 5;

	static const int IDLING_ANIMATION_FRAME = 6;
	static const int RUNNING_ANIMATION_FRAME = 12;
	static const int ATTACKING_ANIMATION_FRAME = 15;
	static const int HURTING_ANIMATION_FRAME = 5;
	static const int DEATH_ANIMATION_FRAME = 22;
	static const int SMASH_ANIMATION_FRAME = 18;
	static const int SPELL_ANIMATION_FRAME = 18;

	SDL_Rect idlingClips[IDLING_ANIMATION_FRAME];
	SDL_Rect runningClips[RUNNING_ANIMATION_FRAME];
	SDL_Rect attackingClips[ATTACKING_ANIMATION_FRAME];
	SDL_Rect hurtingClips[HURTING_ANIMATION_FRAME];
	SDL_Rect deathClips[DEATH_ANIMATION_FRAME];
	SDL_Rect smashClips[SMASH_ANIMATION_FRAME];
	SDL_Rect spellClips[SPELL_ANIMATION_FRAME];

	SDL_Rect collision;

	int idleCounter = 0;
	int runCounter = 0;
	int attackCounter = 0;
	int hurtCounter = 0;
	int deathCounter = 0;
	int smashCounter = 0;
	int spellCounter = 0;
	                                                                                                                                                                      

	bool idle_ = false;
	bool run_ = false;
	bool attack_ = false;
	bool hurt_ = false;
	bool death_ = false;
	bool fall_ = true;
	bool smash_ = false;
	bool spell_ = false;
	
	bool on_ground_ = false;
	int groundSTT = 1;
	int levelSTT = 1;

	float distanceToPlayer;
	float distanceToSecret;

	int boss_health_ = 100;

	int attackCoolDown = 0;
	int smashCoolDown = 0;
	int SpellCoolDown = 1;

	int bullet_y_ = 0;

	int bulletNum = 0;

	std::vector<Bullet*> bulletList;
};


#endif // !BOSS_H_
