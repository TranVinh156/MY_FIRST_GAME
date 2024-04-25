#include "BOSS.h"

Boss::Boss(float p_x, float p_y, SDL_Texture* p_text) : Entity(p_x, p_y, p_text)
{
	collision.x = getX() + + 112;
	collision.y = getY() + 80;
	collision.w = BOSS_WIDTH;
	collision.h = BOSS_HEIGHT;

	for (int i = 0; i < IDLING_ANIMATION_FRAME; i++)
	{
		idlingClips[i] = { i * getRect().w / 22, 0 * getRect().h / 7, getRect().w / 22, getRect().h / 7 }; 
	}
	for (int i = 0; i < RUNNING_ANIMATION_FRAME; i++)
	{
		runningClips[i] = { i * getRect().w / 22, 1 * getRect().h / 7, getRect().w / 22, getRect().h / 7 };
	}
	for (int i = 0; i < ATTACKING_ANIMATION_FRAME; i++)
	{
		attackingClips[i] = { i * getRect().w / 22, 2 * getRect().h / 7, getRect().w / 22, getRect().h / 7 };
	}
	for (int i = 0; i < HURTING_ANIMATION_FRAME; i++)
	{
		hurtingClips[i] = { i * getRect().w / 22, 3 * getRect().h / 7, getRect().w / 22, getRect().h / 7 };
	}
	for (int i = 0; i < DEATH_ANIMATION_FRAME; i++)
	{
		deathClips[i] = { i * getRect().w / 22, 4 * getRect().h / 7, getRect().w / 22, getRect().h / 7 };
	}
	for (int i = 0; i < SMASH_ANIMATION_FRAME; i++)
	{
		smashClips[i] = { i * getRect().w / 22, 5 * getRect().h / 7, getRect().w / 22, getRect().h / 7 };
	}
	for (int i = 0; i < SPELL_ANIMATION_FRAME; i++)
	{
		spellClips[i] = {i * getRect().w / 22, 6 * getRect().h / 7, getRect().w / 22, getRect().h / 7};
	}
}

void Boss::Gravity()
{
	if (!on_ground_)
	{
		y_val_ += GRAVITY;
		if (y_val_ >= MAX_GRAVITY) y_val_ = MAX_GRAVITY;
	}
	else y_val_ = 0;

}


void Boss::Update(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a)
{
	if (x_val_ > 0) flip_type_ = SDL_FLIP_NONE;
	if (x_val_ < 0) flip_type_ = SDL_FLIP_HORIZONTAL;
	
	//std::cout << getRect().w << " " << getRect().h << std::endl;
	GetHit(p_player, p_secret);
	Gravity();
	MoveToPlayer(p_player, p_secret, levelList, p_a);
	

	if (x_val_ == 0 && !death_ && !hurt_ && !attack_ && !smash_ && !spell_ && on_ground_)
	{
		idle_ = true;
	}
	else
	{
		idle_ = false;
	}

	if (x_val_ != 0 && !death_ && !hurt_ && !attack_ && !smash_ && !spell_ && on_ground_)
	{
		run_ = true;
	}
	else
	{
		run_ = false;
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


	if (!attack_ && !death_ && !smash_ && !spell_)
	{
		x_ += x_val_;
		collision.x = getX() + 112;
		
		if (getX() + 112 < 0)
		{
			x_ = -112;
			
			collision.x = getX() + 112;
			x_val_ *= -1;
			
		}
		if (CommonFunc::checkToMap(collision, levelList, levelSTT))
		{
			x_ -= x_val_;
			collision.x = getX() + 112;
			//x_val_ *= -1;
		}
		
	}

	y_ += y_val_;
	collision.y = getY() + 80;
	if (getY() + 80 < 0)
	{
		y_ = -BOSS_HEIGHT;
		collision.y = getY() + 80;
	}
	
	if (CommonFunc::checkToMap(collision, levelList, on_ground_, groundSTT, levelSTT))
	{
		
		if (y_val_ > 0)
		{
			y_ = levelList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 160;
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
		collision.y = getY() + 80;
	}
	//on_ground_ = true;
	//std::cout << on_ground_ << std::endl;
	//std::cout << groundSTT << std::endl;
	//std::cout << distanceToPlayer << std::endl;
	//std::cout << attack_ << std::endl;
	//std::cout << levelSTT << std::endl;
}

void Boss::MoveToPlayer(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a)
{
	//distanceToPlayer = sqrt(((p_player.getX() + 48) - (getX() + 32)) * ((p_player.getX() + 48) - (getX() + 32)) + ((p_player.getY() + 48) - (getY() + 32)) * ((p_player.getY() + 48) - (getY() + 32)));

	/*
	if (!death_ && on_ground_ && !attack_)
	{
		
		if (p_player.getX() + 48 - (getX() + 112) > 0) x_val_ = BOSS_VAL_;
		if (p_player.getX() + 48 - (getX() + 112) < 0) x_val_ = -BOSS_VAL_;

		if (distanceToPlayer <= 100) attack_ = true;
		else attack_ = false;
	}
	*/

	if (!death_)
	{
		distanceToPlayer = sqrt(((p_player.getX() + 48) - (getX() + 112)) * ((p_player.getX() + 48) - (getX() + 112)) + ((p_player.getY() + 48) - (getY() + 80)) * ((p_player.getY() + 48) - (getY() + 80))); // stupid
		if (distanceToPlayer <= 32 * TILE_WIDTH) // 7 ô
		{
			if ((p_player.getX() + 48) - (getX() + 112) > 32)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 59) x_val_ = 0;
				x_val_ = BOSS_VAL_;
			}
			else if ((p_player.getX() + 48) - (getX() + 112) == 32)
			{
				x_val_ = 0;
				flip_type_ = SDL_FLIP_NONE;
			}

			if ((p_player.getX() + 48) - (getX() + 112) < 0)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() > 59) x_val_ = 0;
				x_val_ = -BOSS_VAL_;
			}
			else if ((p_player.getX() + 48) - (getX() + 112) == 0)
			{
				x_val_ = 0;
				flip_type_ = SDL_FLIP_HORIZONTAL;
			}
			//else x_val_ = 0;
		}					
	}
	if (!death_ && p_a)
	{
		distanceToPlayer = sqrt(((p_secret.getX() + 80) - (getX() + 112)) * ((p_secret.getX() + 80) - (getX() + 112)) + ((p_secret.getY() + 64) - (getY() + 80)) * ((p_secret.getY() + 64) - (getY() + 80))); // stupid
		if (distanceToPlayer <= 32 * TILE_WIDTH) // 7 ô
		{
			if ((p_secret.getX() + 80) - (getX() + 112) > 32)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 59) x_val_ = 0;
				x_val_ = BOSS_VAL_;
			}
			else if ((p_secret.getX() + 80) - (getX() + 112) == 32)
			{
				x_val_ = 0;
				flip_type_ = SDL_FLIP_NONE;
			}

			if ((p_secret.getX() + 80) - (getX() + 112) < 0)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() > 59) x_val_ = 0;
				x_val_ = -BOSS_VAL_;
			}
			else if ((p_secret.getX() + 80) - (getX() + 112) == 0)
			{
				x_val_ = 0;
				flip_type_ = SDL_FLIP_HORIZONTAL;
			}
			//else x_val_ = 0;
		}
	}
	if (attackCoolDown == 0)
	{
		if (((distanceToPlayer <= TILE_WIDTH * 4 && getFlip() == SDL_FLIP_HORIZONTAL) || (getFlip() == SDL_FLIP_NONE && distanceToPlayer <= TILE_WIDTH * 7)) && !death_ && on_ground_ && !hurt_)
		{
			if (!smash_)
			{
				attack_ = true;
				attackCoolDown = 100;
			}
			//attack_ = true; // thiếu điều kiện
			//attackCoolDown = 100;
		}
		//else attack_ = false;
		
	}
	else
	{
		attackCoolDown--;
		x_val_ = 0;
	}
	
	//do smash

	if (CommonFunc::checkCollision(p_player.getCollision(), collision) && smashCoolDown == 0 && !attack_)
	{
		smash_ = true;
		attack_ = false;
		attackCounter = 0;
		smashCoolDown = 100;
	}
	else
	{
		if (smashCoolDown > 0)
		{
			smashCoolDown--;
			//smash_ = false;
		}
		else smashCoolDown = 0;
		
	}

	if (CommonFunc::checkCollision(p_secret.getCollision(), collision) && smashCoolDown == 0 && !attack_)
	{
		smash_ = true;
		attack_ = false;
		attackCounter = 0;
		smashCoolDown = 100;
	}
	else
	{
		if (smashCoolDown > 0)
		{
			smashCoolDown--;
			//smash_ = false;
		}
		else smashCoolDown = 0;

	}


	// do spell
	if (boss_health_ % 10 == 0 && boss_health_ < 100 && boss_health_ != 0)
	{
		
		smashCounter = 0;
		attackCounter = 0;
		spell_ = true;
		//SpellCoolDown = 0;
		Spell();
		smash_ = false;
		attack_ = false;
	}
	

}


void Boss::Smash()
{

}

void Boss::Spell()
{
	if ((spellCounter  == 0 || spellCounter == 27))
	{
		Bullet* bullet = new Bullet(getCollision().x + BOSS_WIDTH + 80, getCollision().y + bullet_y_, NULL);
		bullet->setFlip(getFlip());
		bullet->setSize_Position(getX());
		bullet->setType(Bullet::NORMAL);
		bullet->setMove(true);
		bulletList.push_back(bullet);
		bulletNum++;
		bullet_y_ += 48;
		if (bullet_y_ > 48)
		{
			bullet_y_ = 0;
		}
		if (bulletNum > 10)
		{
			spell_ = false;
			bulletNum = 0;
			boss_health_--;
		}
	}
	

}

void Boss::GetHit(Player& p_player, Secret& p_secret)
{
	/*
	if (distanceToPlayer <= 48 && p_player.isAttacking() && !death_ && !attack_ && ((p_player.getFlip() == SDL_FLIP_NONE && p_player.getX() - getX() - 64 <= 0) || (p_player.getFlip() == SDL_FLIP_HORIZONTAL && p_player.getX() - getX() - 64 >= 0)))
	{
		hurt_ = true;
	}
	*/
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

bool Boss::isAttacking()
{
	if (attackCounter / 3 >= 9 && attackCounter / 3 <= 11)
	{
		return true;
	}
	return false;
}

bool Boss::isSmashing()
{
	if (smashCounter / 2 >= 11 && smashCounter / 2 <= 13)
	{
		return true;
	}
	return false;
}

void Boss::Render(SDL_Rect& camera)
{
	
	if (idle_ || fall_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &idlingClips[idleCounter / 4], &camera, 0, NULL, getFlip());
		idleCounter++;
		if (idleCounter / 4 >= IDLING_ANIMATION_FRAME) idleCounter = 0;
	}

	if (run_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &runningClips[runCounter / 4], &camera, 0, NULL, getFlip());
		runCounter++;
		if (runCounter / 4 >= RUNNING_ANIMATION_FRAME) runCounter = 0;
	}

	if (hurt_ && !death_ && !attack_ && !spell_ && !smash_ && !spell_)
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

	if (death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &deathClips[deathCounter / 4], &camera, 0, NULL, getFlip());
		deathCounter++;
		if (deathCounter / 4 >= DEATH_ANIMATION_FRAME)
		{

			deathCounter = 84;
		}
	}

	if (attack_ && !death_ && !run_)
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

	if (smash_ && !death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &smashClips[smashCounter / 2], &camera, 0, NULL, getFlip());
		smashCounter++;
		if (smashCounter / 2 >= SMASH_ANIMATION_FRAME)
		{
			smashCounter = 0;
			smash_ = false;
		}
	}

	if (spell_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &spellClips[spellCounter / 3], &camera, 0, NULL, getFlip());
		spellCounter++;
		if (spellCounter / 3 >= SPELL_ANIMATION_FRAME)
		{
			spellCounter = 0;
			SpellCoolDown = 0;
			//spell_ = false;
		}
	}
}