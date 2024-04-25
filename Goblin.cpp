#include "Goblin.h"

Goblin::Goblin(float p_x, float p_y, SDL_Texture* p_text) : Entity(p_x, p_y, p_text)
{
	collision.x = getX() + 96;
	collision.y = getY() + 32;
	collision.w = BOSS_WIDTH;
	collision.h = BOSS_HEIGHT;

	for (int i = 0; i < IDLING_ANIMATION_FRAME; i++)
	{
		idlingClips[i] = { i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < WALKING_ANIMATION_FRAME; i++)
	{
		walkingClips[i] = { i * FRAME_WIDTH, 1 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < STUNNING_ANIMATION_FRAME; i++)
	{
		stunningClips[i] = { i * FRAME_WIDTH, 2 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < ATTACKING_ANIMATION_FRAME; i++)
	{
		attackingClips[i] = { i * FRAME_WIDTH, 3 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < SP_ATTACKING_ANIMATION_FRAME; i++)
	{
		spAttackingClips[i] = { i * FRAME_WIDTH, 4 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < HURTING_ANIMATION_FRAME; i++)
	{
		hurtingClips[i] = { i * FRAME_WIDTH, 5 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
}

void Goblin::Gravity()
{
	if (!on_ground_)
	{
		y_val_ += GRAVITY;
		if (y_val_ >= MAX_GRAVITY) y_val_ = MAX_GRAVITY;
	}
	else y_val_ = 0;

}

void Goblin::Update(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a)
{
	if (x_val_ > 0) flip_type_ = SDL_FLIP_NONE;
	if (x_val_ < 0) flip_type_ = SDL_FLIP_HORIZONTAL;

	//std::cout << getRect().w << " " << getRect().h << std::endl;
	GetHit(p_player, p_secret);
	Gravity();
	MoveToPlayer(p_player, p_secret, levelList, p_a);


	if (x_val_ == 0 && !death_ && !hurt_ && !attack_ && !spAttack_ && on_ground_)
	{
		idle_ = true;
	}
	else
	{
		idle_ = false;
	}

	if (x_val_ != 0 && !death_ && !hurt_ && !attack_ && on_ground_ && !spAttack_)
	{
		walk_ = true;
	}
	else
	{
		walk_ = false;
	}
	if (y_val_ > 0 && !on_ground_ && !death_ && !hurt_)
	{
		fall_ = true;
	}
	else
	{
		fall_ = false;
	}
	if (x_val_ > 0) flip_type_ = SDL_FLIP_NONE;
	if (x_val_ < 0) flip_type_ = SDL_FLIP_HORIZONTAL;


	if (!attack_ && !death_)
	{
		x_ += x_val_;
		collision.x = getX() + 96;

		if (getX() + 96 < 0)
		{
			x_ = -96;

			collision.x = getX() + 96;
			x_val_ *= -1;

		}
		if (CommonFunc::checkToMap(collision, levelList, levelSTT))
		{
			x_ -= x_val_;
			collision.x = getX() + 96;
			//x_val_ *= -1;
		}

	}

	y_ += y_val_;
	collision.y = getY() + 32;
	if (getY() + 32 < 0)
	{
		y_ = -BOSS_HEIGHT;
		collision.y = getY() + 32;
	}

	if (CommonFunc::checkToMap(collision, levelList, on_ground_, groundSTT, levelSTT))
	{

		if (y_val_ > 0)
		{
			y_ = levelList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 112;
			if (fall_)
			{
				on_ground_ = true;
			}
		}
		if (y_val_ < 0)
		{
			y_ -= y_val_;
			y_val_ = 0;
		}
		collision.y = getY() + 32;
	}
}

void Goblin::MoveToPlayer(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a)
{

	if (!death_ && !p_a)
	{
		distanceToPlayer = sqrt(((p_player.getX() + 48) - (getX() + 96)) * ((p_player.getX() + 48) - (getX() + 96)) + ((p_player.getY() + 48) - (getY() + 32)) * ((p_player.getY() + 48) - (getY() + 32))); // stupid
		if (distanceToPlayer <= 32 * TILE_WIDTH && !spAttack_) // 7 ô
		{
			if ((p_player.getX() + 48) - (getX() + 96) > 32)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 59) x_val_ = 0;
				x_val_ = BOSS_VAL_;
			}
			else if ((p_player.getX() + 48) - (getX() + 96) == 32)
			{
				x_val_ = 0;
				flip_type_ = SDL_FLIP_NONE;
			}

			if ((p_player.getX() + 48) - (getX() + 96) < 0)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() > 59) x_val_ = 0;
				x_val_ = -BOSS_VAL_;
			}
			else if ((p_player.getX() + 48) - (getX() + 96) == 0)
			{
				x_val_ = 0;
				flip_type_ = SDL_FLIP_HORIZONTAL;
			}
			//else x_val_ = 0;
		}
	}
	if (!death_ && p_a)
	{
		distanceToPlayer = sqrt(((p_secret.getX() + 80) - (getX() + 96)) * ((p_secret.getX() + 80) - (getX() + 96)) + ((p_secret.getY() + 64) - (getY() + 32)) * ((p_secret.getY() + 64) - (getY() + 32))); // stupid
		if (distanceToPlayer <= 32 * TILE_WIDTH && !spAttack_) // 7 ô
		{
			if ((p_secret.getX() + 80) - (getX() + 112) > 32)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 59) x_val_ = 0;
				x_val_ = BOSS_VAL_;
			}
			else if ((p_secret.getX() + 80) - (getX() + 96) == 32)
			{
				x_val_ = 0;
				flip_type_ = SDL_FLIP_NONE;
			}

			if ((p_secret.getX() + 80) - (getX() + 96) < 0)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() > 59) x_val_ = 0;
				x_val_ = -BOSS_VAL_;
			}
			else if ((p_secret.getX() + 80) - (getX() + 96) == 0)
			{
				x_val_ = 0;
				flip_type_ = SDL_FLIP_HORIZONTAL;
			}
			//else x_val_ = 0;
		}
	}
	if (attackCoolDown == 0)
	{
		if (((distanceToPlayer <= TILE_WIDTH * 4 && getFlip() == SDL_FLIP_HORIZONTAL) || (getFlip() == SDL_FLIP_NONE && distanceToPlayer <= TILE_WIDTH * 7)) && !death_ && on_ground_ && !hurt_ && !spAttack_)
		{
				attack_ = true;
				spAttack_ = false;
				attackCoolDown = 100;
			
		}
		//else attack_ = false;

	}
	else
	{
		attackCoolDown--;
		//x_val_ = 0;
	}
	if (spAttackCoolDown == 0)
	{
		if (((distanceToPlayer >= TILE_WIDTH * 4 && getFlip() == SDL_FLIP_HORIZONTAL) || (getFlip() == SDL_FLIP_NONE && distanceToPlayer >= TILE_WIDTH * 7)) && !death_ && on_ground_ && !hurt_ && !attack_)
		{
			/*
			if (getFlip() == SDL_FLIP_HORIZONTAL && spAttackCounter / 2 <= 14)
			{
				if (spAttackCounter / 2 <= 14)
				{
					x_ -= 8;
					collision.x = getX() + 96;
				}
				
			}
			
			if (getFlip() == SDL_FLIP_NONE && spAttackCounter / 2 <= 14)
			{
				if (spAttackCounter / 2 <= 14)
				{
					x_ += 8;
					collision.x = getX() + 96;
				}
				
			}
			*/
			//else x_val_ = 0;
			spAttack_ = true;
			attack_ = false;
			spAttackCoolDown = 200;

		}
		//else attack_ = false;

	}
	else
	{
		spAttackCoolDown--;
		//x_val_ = 0;
	}

	if (attackCoolDown != 0 && spAttackCoolDown != 0)
	{
		x_val_ = 0;
	}

}

void Goblin::GetHit(Player& p_player, Secret& p_secret)
{
	if (p_player.isAttacking() && p_player.getFlip() == SDL_FLIP_NONE)
	{
		SDL_Rect a = { p_player.getCollision().x + 16, p_player.getCollision().y, 48, 32 };
		//std::cout << CommonFunc::checkCollision(a, getCollision()) << std::endl;
		//std::cout << a.x << std::endl;
		//std::cout << a.y << std::endl;
		if (CommonFunc::checkCollision(a, getCollision()))
		{
			hurt_ = true;
		}
	}

	if (p_player.isAttacking() && p_player.getFlip() == SDL_FLIP_HORIZONTAL)
	{
		SDL_Rect a = { p_player.getCollision().x - 48, p_player.getCollision().y, 48, 32 };
		if (CommonFunc::checkCollision(a, getCollision()))
		{
			hurt_ = true;
		}
	}

	if (p_secret.isAttacking() && p_secret.getFlip() == SDL_FLIP_NONE)
	{
		SDL_Rect a = { p_secret.getCollision().x + 16, p_secret.getCollision().y, 48, 32 };
		//std::cout << CommonFunc::checkCollision(a, getCollision()) << std::endl;
		//std::cout << a.x << std::endl;
		//std::cout << a.y << std::endl;
		if (CommonFunc::checkCollision(a, getCollision()))
		{
			hurt_ = true;
		}
	}

	if (p_secret.isAttacking() && p_secret.getFlip() == SDL_FLIP_HORIZONTAL)
	{
		SDL_Rect a = { p_secret.getCollision().x - 48, p_secret.getCollision().y, 48, 32 };
		if (CommonFunc::checkCollision(a, getCollision()))
		{
			hurt_ = true;
		}
	}

	if (isAttacking() && p_secret.isParrying())
	{
		hurt_ = true;
		attack_ = false;

		attackCounter = 0;
		//p_secret.setParry(true);
		//death_ = true;
	}

	if (isAttacking() && p_secret.isBuffParrying())
	{
		hurt_ = true;
		attack_ = false;

		attackCounter = 0;
		p_secret.setBuffParry(false);
		//death_ = true;
	}

	if (boss_health_ <= 0)
	{
		death_ = true;
		hurt_ = false;
	}
}

bool Goblin::isAttacking()
{
	if ((attackCounter / 3 >= 5 && attackCounter / 3 <= 6) || (attackCounter / 3 >= 11 && attackCounter / 3 <= 11) || (attackCounter / 3 >= 20 && attackCounter / 3 <= 23))
	{
		return true;
	}
	return false;
}

void Goblin::Render(SDL_Rect& camera)
{

	if (idle_ || fall_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &idlingClips[idleCounter / 4], &camera, 0, NULL, getFlip());
		idleCounter++;
		if (idleCounter / 4 >= IDLING_ANIMATION_FRAME) idleCounter = 0;
	}

	if (walk_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &walkingClips[walkCounter / 4], &camera, 0, NULL, getFlip());
		walkCounter++;
		if (walkCounter / 4 >= WALKING_ANIMATION_FRAME) walkCounter = 0;
	}

	if (hurt_ && !death_ && !attack_ && !spAttack_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &hurtingClips[hurtCounter / 2], &camera, 0, NULL, getFlip());
		hurtCounter++;
		if (hurtCounter / 2 >= HURTING_ANIMATION_FRAME)
		{
			boss_health_--;
			hurtCounter = 0;
			hurt_ = false;
		}

	}
	/*
	if (death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &deathClips[deathCounter / 4], &camera, 0, NULL, getFlip());
		deathCounter++;
		if (deathCounter / 4 >= DEATH_ANIMATION_FRAME)
		{

			deathCounter = 84;
		}
	}
	*/
	if (attack_ && !death_ && !walk_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &attackingClips[attackCounter / 3], &camera, 0, NULL, getFlip());
		attackCounter++;
		if (attackCounter / 3 >= ATTACKING_ANIMATION_FRAME)
		{
			attack_ = false;
			attackCounter = 0;
		}
	}
	else attackCounter = 0;

	if (spAttack_ && !death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &spAttackingClips[spAttackCounter / 2], &camera, 0, NULL, getFlip());
		spAttackCounter++;
		if (spAttackCounter / 2 <= 14)
		{
			if (getFlip() == SDL_FLIP_HORIZONTAL)
			{
				x_val_ = -6;
			}
			if (getFlip() == SDL_FLIP_NONE)
			{
				x_val_ = 6;
			}
		}
		else
		{
			x_val_ = 0;
		}
		if (spAttackCounter / 2 >= SP_ATTACKING_ANIMATION_FRAME)
		{
			
			spAttack_ = false;
			spAttackCounter = 0;
		}
	}
	else spAttackCounter = 0;

	
}
