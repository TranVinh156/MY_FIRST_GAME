#include "Undead.h"

Undead::Undead(float p_x, float p_y, SDL_Texture* p_text) : Entity(p_x, p_y, p_text)
{
	collision.x = getX() + 32;
	collision.y = getY() + 16;
	collision.w = UNDEAD_WIDTH;
	collision.h = UNDEAD_HEIGHT;

	x_start_ = p_x;

	for (int i = 0; i < IDLING_ANIMATION_FRAME; i++)
	{
		idlingClips[i] = { i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < WALKING_ANIMATION_FRAME; i++)
	{
		walkingClips[i] = { i * FRAME_WIDTH, 1 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < DEATH_ANIMATION_FRAME; i++)
	{
		deathClips[i] = { i * FRAME_WIDTH, 4 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < BEINGHIT_ANIMATION_FRAME; i++)
	{
		beinghitClips[i] = { i * FRAME_WIDTH, 3 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < ATTACKING_ANIMATION_FRAME; i++)
	{
		attackingClips[i] = { i * FRAME_WIDTH, 2 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
}

Undead::~Undead()
{
	
}

void Undead::Gravity()
{
	if (!on_ground_)
	{
		y_val_ += GRAVITY;
		if (y_val_ >= MAX_GRAVITY) y_val_ = MAX_GRAVITY;
	}
	else y_val_ = 0;
}

void Undead::Update(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a, Mix_Chunk* p_undeadSFX[])
{
	

	if (x_val_ < 0) flip_type_ = SDL_FLIP_HORIZONTAL;
	if (x_val_ > 0) flip_type_ = SDL_FLIP_NONE;
	Gravity();
	GetHit(p_player, p_secret, p_a, p_undeadSFX);
	AutoMovement(levelList);
	MoveToPlayer(p_player, p_secret, levelList, p_a);
	KnockBack();
	
	if (!death_ && on_ground_ && x_val_ == 0 && !beinghit_ && !attack_)
	{
		idle_ = true;
	}
	else idle_ = false;

	if (!death_ && on_ground_ && x_val_ != 0 && !beinghit_ && !attack_)
	{
		walk_ = true;
	}
	else walk_ = false;

	if (y_val_ > 0 && !on_ground_ && !death_ && !beinghit_)
	{
		fall_ = true;
	}
	else fall_ = false;
	//x_val_ = 5;
	if (!attack_ && !death_)
	{
		x_ += x_val_;
		collision.x = getX() + 32;
		if (getX() + 16 <= 0 + levelList.at(levelSTT).getX())
		{
			x_ = levelList.at(levelSTT).getX() - UNDEAD_WIDTH;
			collision.x = getX() + 32;
			x_val_ *= -1; // đi ngược lại
		}

		if (getX() + 32 + 32  >= LEVEL_WIDTH + levelList.at(levelSTT).getX())
		{
			x_ -= 8;
			collision.x = getX() + 32;
			x_val_ *= -1; // đi ngược lại
		}
		if (CommonFunc::checkToMap(collision, levelList, levelSTT))
		{
			x_ -= x_val_;
			collision.x = getX() + 32;
			if (!p_a && distanceToPlayer > 10 * TILE_WIDTH)
			{
				x_val_ *= -1;
			}
			if (p_a && distanceToSecret > 10 * TILE_WIDTH)
			{
				x_val_ *= -1;
			}
			
		}
	}




	y_ += y_val_;  
	collision.y = getY() + 16;
	if (getY() + 2 * UNDEAD_HEIGHT < 0)
	{
		y_ = -UNDEAD_HEIGHT;
		collision.y = getY() + 16;
	}
	if (CommonFunc::checkToMap(collision, levelList, on_ground_, groundSTT, levelSTT))
	{
		if (y_val_ > 0)
		{
			y_ = levelList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 48;
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
		collision.y = getY() + 16;
	}
	if ((levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() >= 76 && levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() < 83) || (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() >= 76 && levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() < 80))
	{
		y_ = levelList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 48;
		if (fall_)
		{
			on_ground_ = true;
		}
	}

	if (levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() >= 83 && levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() <= 89)
	{
		death_ = true;
		Mix_PlayChannel(-1, p_undeadSFX[deathSFX], 0);
	}
	// cần sửa tấn công

}

void Undead::AutoMovement(std::vector<Game_Map>& levelList)
{
	if (on_ground_ && !beinghit_ && !death_)
	{
		if (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 82 && levelList.at(levelSTT).getTilesList().at(groundSTT - 1)->getType() > 82) x_val_ = 0; // để nhân vật không nhảy	
		else if (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 82 && x_val_ > 0)
		{
			x_val_ = -UNDEAD_VAL * 0.5;
		}
		else if (levelList.at(levelSTT).getTilesList().at(groundSTT - 1)->getType() > 82 && x_val_ < 0)
		{
			x_val_ = UNDEAD_VAL * 0.5;
		}
		else if (levelList.at(levelSTT).getTilesList().at(groundSTT + 2)->getType() > 82 && levelList.at(levelSTT).getTilesList().at(groundSTT - 2)->getType() > 82) x_val_ = 0;
		else if (getFlip() == SDL_FLIP_NONE) x_val_ = UNDEAD_VAL * 0.5;
		else if (getFlip() == SDL_FLIP_HORIZONTAL) x_val_ = -UNDEAD_VAL * 0.5;
	}
}

void Undead::MoveToPlayer(Player& p_player, Secret& p_secret, std::vector<Game_Map>& levelList, bool p_a)
{
	if (!death_ && !beinghit_ && !p_a)
	{
		distanceToPlayer = sqrt(((p_player.getX() + 48) - (getX() + 32)) * ((p_player.getX() + 48) - (getX() + 32)) + ((p_player.getY() + 48) - (getY() + 16)) * ((p_player.getY() + 48) - (getY() + 16))); // stupid
		if (distanceToPlayer <= 10 * TILE_WIDTH) // 7 ô
		{
			if ((p_player.getX() + 48) - (getX() + 32) > 0)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 82) x_val_ = 0;
				else x_val_ = UNDEAD_VAL;
			}

			if ((p_player.getX() + 48) - (getX() + 32) < 0)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT - 1)->getType() > 82) x_val_ = 0;
				else x_val_ = -UNDEAD_VAL;
			}

		}
		else
		{
			if (x_ - x_start_ > MAX_DISTANCE)
			{
				x_val_ = -0.5 * UNDEAD_VAL;
			}
			else if (x_ - x_start_ < -MAX_DISTANCE)
			{
				x_val_ = 0.5 * UNDEAD_VAL;
			}
		}
		if (p_player.getCollision().x - 8 <= collision.x && p_player.getCollision().x + 8 >= collision.x) x_val_ = 0;
		//if (((distanceToPlayer <= TILE_WIDTH * 2 && getFlip() == SDL_FLIP_HORIZONTAL) || (distanceToPlayer <= 2 * TILE_WIDTH && getFlip() == SDL_FLIP_NONE)) && !death_ && on_ground_ && !beinghit_) attack_ = true; // thiếu điều kiện
		if (attackCooldown == 0)
		{
			if (((distanceToPlayer <= TILE_WIDTH * 2 && getFlip() == SDL_FLIP_HORIZONTAL) || (distanceToPlayer <= 2 * TILE_WIDTH && getFlip() == SDL_FLIP_NONE)) && !death_ && on_ground_ && !beinghit_)
			{
				attack_ = true;
				attackCooldown = 60;
			}
		}
		else
		{
			attackCooldown--;
		}
	}
	
	if (!death_ && p_a && !beinghit_)
	{
		distanceToSecret = sqrt(((p_secret.getX() + 80) - (getX() + 32)) * ((p_secret.getX() + 80) - (getX() + 32)) + ((p_secret.getY() + 64) - (getY() + 16)) * ((p_secret.getY() + 64) - (getY() + 16))); // stupid
		if (distanceToSecret <= 10 * TILE_WIDTH) // 7 ô
		{
			if ((p_secret.getX() + 80) - (getX() + 32) > 0)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 82) x_val_ = 0;
				else x_val_ = UNDEAD_VAL;
			}

			if ((p_secret.getX() + 80) - (getX() + 32) < 0)
			{
				if (levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() > 82) x_val_ = 0;
				else x_val_ = -UNDEAD_VAL;
			}
		}
		else
		{
			if (x_ - x_start_ > MAX_DISTANCE)
			{
				x_val_ = -0.5 * UNDEAD_VAL;
			}
			else if (x_ - x_start_ < -MAX_DISTANCE)
			{
				x_val_ = 0.5 * UNDEAD_VAL;
			}
		}
		if (p_secret.getCollision().x - 8 <= collision.x && p_secret.getCollision().x + 8 >= collision.x) x_val_ = 0;

		if (attackCooldown == 0)
		{
			if (((distanceToSecret <= TILE_WIDTH * 2 && getFlip() == SDL_FLIP_HORIZONTAL) || (distanceToSecret <= 2 * TILE_WIDTH && getFlip() == SDL_FLIP_NONE)) && !death_ && on_ground_ && !beinghit_)
			{
				attack_ = true;
				attackCooldown = 60;
			}
		}
		else
		{
			attackCooldown--;
		}
	}

}

bool Undead::isAttacking()
{
	if (!beinghit_ && !death_)
	{
		if (attackCounter / 4 == 4)
		{
			return true;
		}
		
	}
	return false;
}

void Undead::KnockBack()
{
	if (beinghit_ && !death_)
	{
		
		y_ += -2;
		if (flip_type_ == SDL_FLIP_NONE)
		{
			x_ += -2;
		}
		else if (flip_type_ == SDL_FLIP_HORIZONTAL)
		{
			x_ += 2;
		}
		collision.x = getX() + 32;
		collision.y = getY() + 32;
	}
}

void Undead::GetHit(Player& p_player, Secret& p_secret, bool p_a, Mix_Chunk* p_undeadSFX[])
{
	if (!p_a)
	{
		if (p_player.isAttacking() && p_player.getFlip() == SDL_FLIP_NONE)
		{
			SDL_Rect a = { p_player.getCollision().x + 16, p_player.getCollision().y, 48, 32 };
			//std::cout << CommonFunc::checkCollision(a, getCollision()) << std::endl;
			//std::cout << a.x << std::endl;
			//std::cout << a.y << std::endl;
			if (CommonFunc::checkCollision(a, getCollision()))
			{
				beinghit_ = true;
				Mix_PlayChannel(-1, p_undeadSFX[hurtSFX], 0);

			}
		}

		if (p_player.isAttacking() && p_player.getFlip() == SDL_FLIP_HORIZONTAL)
		{
			SDL_Rect a = { p_player.getCollision().x - 48, p_player.getCollision().y, 48, 32 };
			if (CommonFunc::checkCollision(a, getCollision()))
			{
				beinghit_ = true;
				Mix_PlayChannel(-1, p_undeadSFX[hurtSFX], 0);

			}
		}


	}
	else
	{
		if (p_secret.isAttacking() && p_secret.getFlip() == SDL_FLIP_NONE)
		{
			SDL_Rect a = { p_secret.getCollision().x + 16, p_secret.getCollision().y, 48, 32 };

			if (CommonFunc::checkCollision(a, getCollision()))
			{
				beinghit_ = true;
				Mix_PlayChannel(-1, p_undeadSFX[hurtSFX], 0);

			}
		}

		if (p_secret.isAttacking() && p_secret.getFlip() == SDL_FLIP_HORIZONTAL)
		{
			SDL_Rect a = { p_secret.getCollision().x - 48, p_secret.getCollision().y, 48, 32 };
			if (CommonFunc::checkCollision(a, getCollision()))
			{
				beinghit_ = true;
				Mix_PlayChannel(-1, p_undeadSFX[hurtSFX], 0);

			}
		}

		if (p_secret.isSpecialAttacking() && p_secret.getFlip() == SDL_FLIP_NONE)
		{
			SDL_Rect a = { p_secret.getCollision().x + 16, p_secret.getCollision().y - 64, 80, 96 };

			if (CommonFunc::checkCollision(a, getCollision()))
			{
				death_ = true;
				Mix_PlayChannel(-1, p_undeadSFX[deathSFX], 0);

			}
		}

		if (p_secret.isSpecialAttacking() && p_secret.getFlip() == SDL_FLIP_HORIZONTAL)
		{
			SDL_Rect a = { p_secret.getCollision().x - 80, p_secret.getCollision().y - 64, 80, 96 };
			if (CommonFunc::checkCollision(a, getCollision()))
			{
				death_ = true;
				Mix_PlayChannel(-1, p_undeadSFX[deathSFX], 0);

			}
		}
		
		if (loaded_ && p_secret.isSpelling())
		{
			death_ = true;
			Mix_PlayChannel(-1, p_undeadSFX[deathSFX], 0);
		}

		if (isAttacking() && p_secret.isParrying() && distanceToSecret <= 32)
		{
			beinghit_ = true;
			attack_ = false;
			attackCounter = 0;
			Mix_PlayChannel(-1, p_undeadSFX[parrySFX], 0);
		}

		if (isAttacking() && p_secret.isBuffParrying() && distanceToSecret <= 32)
		{
			beinghit_ = true;
			attack_ = false;
			Mix_PlayChannel(-1, p_undeadSFX[parrySFX], 0);
			attackCounter = 0;
			p_secret.setBuffParry(false);
		}
		for (int i = 0; i < p_secret.GetBulletList().size(); i++)
		{
			if (p_secret.GetBulletList().at(i) != NULL)
			{
				if (CommonFunc::checkCollision(getCollision(), p_secret.GetBulletList().at(i)->getCollision()))
				{
					beinghit_ = true;
					Mix_PlayChannel(-1, p_undeadSFX[hurtSFX], 0);
				}
			}
		}
	}
	

	if (undead_health_ <= 0)
	{
		death_ = true;
		
		beinghit_ = false;
	}
}

bool Undead::isDead()
{
	if (deathCounter == 56)
	{
		return death_;
	}
	else return false;
}

void Undead::render(SDL_Rect& camera, Mix_Chunk* p_undeadSFX[])
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

	if (beinghit_ && !death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &beinghitClips[beinghitCounter], &camera, 0, NULL, getFlip());
		beinghitCounter++;
		if (beinghitCounter >= BEINGHIT_ANIMATION_FRAME) 
		{
			undead_health_--;
			beinghitCounter = 0;
			beinghit_ = false;
		}
		
	}

	if (death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &deathClips[deathCounter / 4], &camera, 0, NULL, getFlip());
		deathCounter++;
		if (deathCounter == 1) Mix_PlayChannel(-1, p_undeadSFX[deathSFX], 0);
		if (deathCounter / 4 >= DEATH_ANIMATION_FRAME)
		{
			
			deathCounter = 56;
		}
	}

	if (attack_ && !beinghit_ && !death_ && !walk_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &attackingClips[attackCounter / 4], &camera, 0, NULL, getFlip());
		if (attackCounter / 4 == 3) Mix_PlayChannel(-1, p_undeadSFX[attackSFX], 0);
		attackCounter++;
		if (attackCounter / 4 >= ATTACKING_ANIMATION_FRAME)
		{
			attack_ = false;
			attackCounter = 0;
		}
	}
	else attackCounter = 0;
}

bool Undead::SetLoaded(Player& p_player, Secret& p_secret, bool p_a)
{
	if (!p_a)
	{
		if (collision.x >= (p_player.getCollision().x - SCREEN_WIDTH * 2 / 3) && collision.x <= (p_player.getCollision().x + SCREEN_WIDTH * 2 / 3) && collision.y >= (p_player.getCollision().y - SCREEN_HEIGHT / 2) && collision.y <= (p_player.getCollision().y + SCREEN_HEIGHT / 2))
		{
			loaded_ = true;
		}
		else loaded_ = false;
	}
	else
	{
		if (collision.x >= (p_secret.getCollision().x - SCREEN_WIDTH * 2 / 3) && collision.x <= (p_secret.getCollision().x + SCREEN_WIDTH * 2 / 3) && collision.y >= (p_secret.getCollision().y - SCREEN_HEIGHT / 2) && collision.y <= (p_secret.getCollision().y + SCREEN_HEIGHT / 2))
		{
			loaded_ = true;
		}
		else loaded_ = false;
	}
	return loaded_;
}