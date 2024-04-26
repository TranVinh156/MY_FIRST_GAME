#include "Secret.h"


Secret::Secret(float p_x, float p_y, SDL_Texture* p_text) : Entity(p_x, p_y, p_text)
{
	Collision.x = getX() + 80;
	Collision.y = getY() + 64;
	Collision.w = SECRET_WIDTH;
	Collision.h = SECRET_HEIGHT;

	for (int i = 0; i < IDLING_ANIMATION_FRAME; i++)
	{
		idlingClips[i] = { i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < RUNNING_ANIMATION_FRAME; i++)
	{
		runningClips[i] = { i * FRAME_WIDTH, 1 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < DEATH_ANIMATION_FRAME; i++)
	{
		deathClips[i] = { i * FRAME_WIDTH, 4 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < JUMPING_ANIMATION_FRAME; i++)
	{
		jumpingClips[i] = { i * FRAME_WIDTH, 3 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < FALLING_ANIMATION_FRAME; i++)
	{
		fallingClips[i] = { (i + 3) * FRAME_WIDTH, 3 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < ATTACKING_ANIMATION_FRAME; i++)
	{
		attackingClips[i] = { i * FRAME_WIDTH, 2 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < SHIELD_ANIMATION_FRAME; i++)
	{
		shieldClips[i] = { i * FRAME_WIDTH, 5 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < SHIELDBUFF_ANIMATION_FRAME; i++)
	{
		shieldBuffClips[i] = { i * FRAME_WIDTH, 6 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < REST_ANIMATION_FRAME; i++)
	{
		restClips[i] = { i * FRAME_WIDTH, 7 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
}

void Secret::HandleInput(SDL_Event& events, Mix_Chunk* p_secretSFX[])
{
	if (!death_)
	{
		if (events.type == SDL_KEYDOWN && events.key.repeat == 0)
		{
			switch (events.key.keysym.sym)
			{
			case SDLK_a: // sang trái
				x_val_ -= PLAYER_VAL;
				break;

			case SDLK_d: // sang phải
				x_val_ += PLAYER_VAL;
				break;

			case SDLK_s: // cúi xuống
				down_ = true;
				break;

			case SDLK_w: // nhảy lên
				if (!on_ground_ && !doublejump_)
				{
					y_val_ -= 6;
					Mix_PlayChannel(-1, p_secretSFX[jumpSFX], 0);
					doublejump_ = true;
				}
				if (on_ground_)
				{
					Mix_PlayChannel(-1, p_secretSFX[jumpSFX], 0);
					Jump();
				}
				break;
			case SDLK_q:
				rest_ = false;
				restCounter = 0;
				if (!attack_ && shieldBuffCooldown == 0)
				{
					shieldBuff_ = true;
					shieldBuffCooldown = 100;
					Mix_PlayChannel(-1, p_secretSFX[spellSFX], 0);
				}
				break;
			case SDLK_j:
				Attack();

				break;
			case SDLK_r: // nhảy lên
				
				if (idle_)
				{
					rest_ = true;
				}
				break;
			default:
				break;
			}
		}
		else if (events.type == SDL_KEYUP && events.key.repeat == 0)
		{
			switch (events.key.keysym.sym)
			{
			case SDLK_a: // sang trái
				//x_val_ = 0;
				x_val_ += PLAYER_VAL;
				break;

			case SDLK_d: // sang phải
				//x_val_ = 0;
				x_val_ -= PLAYER_VAL;
				break;

			case SDLK_s: // cúi xuống
				down_ = false;
				break;

			case SDLK_w: // nhảy lên
				if (!on_ground_ && jump_)
				{
					y_val_ *= .5f;
				}
				break;
			default:
				break;
			}
		}
		else if (events.type == SDL_MOUSEBUTTONDOWN && events.key.repeat == 0) {
			if (events.button.button == SDL_BUTTON_LEFT)
			{

				Attack();
				//chưa biết làm gì
			}
			else if (events.button.button == SDL_BUTTON_RIGHT)
			{
				rest_ = false;
				restCounter = 0;
				if (!attack_)
				{
					Shield();
				}
				
			}

		}
		else if (events.type == SDL_MOUSEBUTTONUP && events.key.repeat == 0)
		{
			//attack_ = false;
			/*
			if (events.button.button == SDL_BUTTON_RIGHT)
			{
				shield_ = false;
				shieldCounter = 0;
			}
			*/
			// chưa biết làm gì
		}
	}
}

void Secret::HandleCamera(SDL_Rect& camera, int& p_levelSTT, bool& p_nextlevel, int& p_skeCount)
{
	
	if (getX() + 80 >= LEVEL_WIDTH - SCREEN_WIDTH)
	{
		camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
		final_ = true;
	}
	else if (!final_)
	{
		camera.x = (getX() + 80) - SCREEN_WIDTH / 2;
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.x > LEVEL_WIDTH - camera.w)
		{
			camera.x = LEVEL_WIDTH - camera.w;
		}
	}

	camera.y = (getY() + 64) - SCREEN_HEIGHT / 2;
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
	if (getX() + 80 >= 4704 && p_levelSTT == 0 && p_skeCount >= 50)
	{
		GoToNextLevel();
		p_levelSTT = levelSTT;
		p_nextlevel = true;
	}
}

void Secret::Attack()
{
	//attackCounter = 0;
	rest_ = false;
	restCounter = 0;
	if (attack_)
	{
		combo_ = true;
	}
	else combo_ = false;
	if (!death_ && !run_ && !attack_) attack_ = true;



}

void Secret::Shield()
{
	if (!death_ && x_val_ == 0)
	{
		shield_ = true;
		//x_val_ = 0;
	}
}

void Secret::Jump()
{
	if (on_ground_)
	{
		y_val_ -= 6;
		on_ground_ = false;
	}
}

void Secret::Gravity()
{
	if (!on_ground_)
	{
		y_val_ += GRAVITY;
		if (y_val_ >= MAX_GRAVITY)
		{
			y_val_ = MAX_GRAVITY;
		}
	}
	else y_val_ = GRAVITY;
}

void Secret::Update(std::vector<Skeleton*>& skeletonList, std::vector<Game_Map>& levelList, Boss& p_boss, Mix_Chunk* p_secretSFX[])
{
	Gravity();

	if (!death_) GetHit(skeletonList, p_boss, p_secretSFX);

	//x_val_ += PLAYER_VAL;
	if (!death_ && on_ground_ && x_val_ == 0 && !shield_ && !shieldBuff_)
	{
		idle_ = true;
	}
	else idle_ = false;

	if (!death_ && on_ground_ && x_val_ != 0 && !shield_)
	{
		run_ = true;
		rest_ = false;
		restCounter = 0;
	}
	else run_ = false;

	if (!death_ && !on_ground_ && y_val_ > 0)
	{
		fall_ = true;
		rest_ = false;
		restCounter = 0;
	}
	else fall_ = false;

	if (!death_ && !on_ground_ && y_val_ < 0)
	{
		jump_ = true;
		rest_ = false;
		restCounter = 0;
	}
	else jump_ = false;
	
	
	if (shieldBuffCooldown != 0)
	{
		shieldBuffCooldown--;
	}

	if (x_val_ > 0) flip_type_ = SDL_FLIP_NONE;
	if (x_val_ < 0) flip_type_ = SDL_FLIP_HORIZONTAL;


	//di chuyển theo x
	if (!death_ && !shield_ && !shieldBuff_)
	{
		x_ += x_val_;
		Collision.x = getX() + 80;

		if (final_)
		{
			if (getX() + 80 < LEVEL_WIDTH - SCREEN_WIDTH)
			{
				x_ = LEVEL_WIDTH - SCREEN_WIDTH - 80;
				Collision.x = getX() + 80;
			}
		}

		if (getX() + 80 < 0 + levelList.at(levelSTT).getX())
		{
			x_ = levelList.at(levelSTT).getX() - 80;
			Collision.x = getX() + 80;
		}

		if (getX() + 80 + SECRET_WIDTH >= LEVEL_WIDTH * 10)
		{
			x_ = SCREEN_WIDTH - SECRET_WIDTH - 80 + levelList.at(levelSTT).getX();
			Collision.x = getX() + 80; // cần sửa
		}



		if (CommonFunc::checkToMap(Collision, levelList, levelSTT))
		{

			x_ -= x_val_;
			Collision.x = getX() + 80;
		}
	}

	//x_ += x_val_;

	//di chuyển theo y

	y_ += y_val_;
	Collision.y = getY() + 64; // đang ko biết vì sao +16 lại đúng :)))
	if (getY() + 64 < 0)
	{
		y_ = -64;
		Collision.y = getY() + 64;
	}
	// đang lỗi cần sửa
	if (CommonFunc::checkToMap(Collision, levelList, on_ground_, groundSTT, levelSTT)) {
		if (y_val_ > 0)
		{
			y_ = levelList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 64 - 32;
			if (fall_)
			{
				on_ground_ = true;
				doublejump_ = false;
				Mix_PlayChannel(-1, p_secretSFX[landSFX], 0);
			}
		}
		else if (y_val_ < 0 && (levelList.at(levelSTT).getTilesList().at(groundSTT - 565 * 2)->getType() <= 60 || levelList.at(levelSTT).getTilesList().at(groundSTT - 565 * 2 + 1)->getType() <= 60))
		{
			y_ = levelList.at(levelSTT).getTilesList().at(groundSTT - 565 * 2)->getY() - 48;
			y_val_ = 0;
		}
		Collision.y = getY() + 64;
	}

	if (levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() >= 83 && levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() <= 89)
	{
		death_ = true;
		Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
	}

	if (down_)
	{
		if (levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() >= 76 && levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() <= 82 && levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() >= 76 && levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() <= 82)
		{
			fall_ = true;
			on_ground_ = false;
			idle_ = false;
			run_ = false;
			y_ += 16;
			Collision.y = getY() + 64;
		}
	}
	//std::cout << groundSTT << std::endl;
}


bool Secret::isAttacking()
{
	if ((attackCounter >= 6 && attackCounter <= 9) || (attackCounter >= 24 && attackCounter <= 25) || (attackCounter >= 35 && attackCounter <= 37))
	{
		return true;
	}
	else return false;
}

void Secret::GetHit(std::vector<Skeleton*>& skeletonList, Boss& p_boss, Mix_Chunk* p_secretSFX[])
{
	for (int i = 0; i < skeletonList.size(); i++)
	{
		/*
		if (skeletonList.at(i)->getDistance() <= 32 && skeletonList.at(i)->isAttacking() && !roll_)
		{
			death_ = true;
		}
		*/
		if (skeletonList.at(i) != NULL && skeletonList.at(i)->isAttacking() && skeletonList.at(i)->getFlip() == SDL_FLIP_NONE)
		{
			SDL_Rect a = { skeletonList.at(i)->getCollision().x + 16, skeletonList.at(i)->getCollision().y, 32, 32 };
			if (CommonFunc::checkCollision(a, getCollision()))
			{
				
				if (shieldCounter / 2 >= 1 && shieldCounter / 2 <= 7)
				{
					parry_ = true;
					shieldCounter = 7 * 2;
					//death_ = false;
				}
				else if (!parry_ && !shieldBuffParry_)
				{
					death_ = true;
					Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
				}
				

				
			}
		}

		if (skeletonList.at(i) != NULL && skeletonList.at(i)->isAttacking() && skeletonList.at(i)->getFlip() == SDL_FLIP_HORIZONTAL)
		{
			SDL_Rect a = { skeletonList.at(i)->getCollision().x - 32, skeletonList.at(i)->getCollision().y, 32, 32 };
			if (CommonFunc::checkCollision(a, getCollision()))
			{
				//death_ = true;
				
				if (shieldCounter / 2 >= 1 && shieldCounter / 2 <= 7)
				{
					parry_ = true;
					shieldCounter = 7 * 2;
					//death_ = false;
				}
				else if (!parry_ && !shieldBuffParry_)
				{
					death_ = true;
					Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
				}
				
				
			}
		}
	}
	/*
		if (p_boss.isAttacking() && ((p_boss.getFlip() == SDL_FLIP_HORIZONTAL && p_boss.getDistance() <= 6 * TILE_WIDTH && (getX() + 48 - (p_boss.getX() + 112) <= 64 )) || (p_boss.getFlip() == SDL_FLIP_NONE && p_boss.getDistance() <= 9 * TILE_WIDTH && (getX() + 48 - (p_boss.getX() + 112) >= 0))) && !roll_)
		{
			death_ = true;
		}
		*/
	if (p_boss.isAttacking() && p_boss.getFlip() == SDL_FLIP_NONE)
	{
		SDL_Rect a = { p_boss.getCollision().x + 64, p_boss.getCollision().y, 80, 80 };
		if (CommonFunc::checkCollision(a, getCollision()))
		{
			//death_ = true;
			if (shieldCounter / 2 >= 1 && shieldCounter / 2 <= 8)
			{
				parry_ = true;
				shieldCounter = 7 * 2;
			}
			else if (!parry_ && !shieldBuffParry_)
			{
				death_ = true;
				Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
			}
		}
	}
	if (p_boss.isAttacking() && p_boss.getFlip() == SDL_FLIP_HORIZONTAL)
	{
		SDL_Rect a = { p_boss.getCollision().x - 80, p_boss.getCollision().y, 80, 80 };
		if (CommonFunc::checkCollision(a, getCollision()))
		{
			//death_ = true;
			if (shieldCounter / 2 >= 1 && shieldCounter / 2 <= 8)
			{
				parry_ = true;
				shieldCounter = 7 * 2;
			}
			else if (!parry_ && !shieldBuffParry_)
			{
				death_ = true;
				Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
			}
		}
	}

	if (p_boss.isSmashing())
	{
		SDL_Rect a = { p_boss.getCollision().x, p_boss.getCollision().y + 64,80, 16 };
		if (CommonFunc::checkCollision(a, getCollision()))
		{
			death_ = true;
			Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
		}
	}

	for (int i = 0; i < p_boss.GetBulletList().size(); i++)
	{
		if (p_boss.GetBulletList().at(i) != NULL)
		{
			if (CommonFunc::checkCollision(p_boss.GetBulletList().at(i)->getCollision(), getCollision()))
			{
				death_ = true;
				Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
				p_boss.GetBulletList().at(i)->setMove(false);
			}
		}

	}
}

void Secret::Render(SDL_Rect& camera, Mix_Chunk* p_secretSFX[])
{
	if (idle_ && !attack_ && !death_ && !rest_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &idlingClips[idleCounter / 4], &camera, 0, NULL, getFlip());
		idleCounter++;
		if (idleCounter / 4 >= IDLING_ANIMATION_FRAME) idleCounter = 0;
	}
	else idleCounter = 0;
	if (run_ && !attack_ && !death_ && !shieldBuff_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &runningClips[runCounter / 2], &camera, 0, NULL, getFlip());
		if (runCounter % 4 == 0)
		{
			Mix_PlayChannel(-1, p_secretSFX[stepSFX], 0);
		}
		runCounter++;
		if (runCounter / 2 >= RUNNING_ANIMATION_FRAME) runCounter = 0;
	}
	else runCounter = 0;

	if (jump_ && !attack_ && !shield_ && !shieldBuff_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &jumpingClips[jumpCounter / 4], &camera, 0, NULL, getFlip());
		jumpCounter++;
		if (jumpCounter / 4 >= JUMPING_ANIMATION_FRAME)
		{
			jumpCounter = 0;
		}
	}
	else jumpCounter = 0;
	if (fall_ && !attack_ && !shield_ && !shieldBuff_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &fallingClips[fallCounter / 4], &camera, 0, NULL, getFlip());
		fallCounter++;
		if (fallCounter / 4 >= FALLING_ANIMATION_FRAME)
		{
			fallCounter = 0;
			//rollCounter = 0;
			//roll_ = false;
		}
	}
	else fallCounter = 0;
	if (attack_ && !death_ && !shield_ && !shieldBuff_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &attackingClips[attackCounter], &camera, 0, NULL, getFlip());
		attackCounter++;
		if (attackCounter == 1 || attackCounter == 24 || attackCounter == 35) Mix_PlayChannel(-1, p_secretSFX[attackSFX], 0);
		if (attackCounter >= 18)
		{
			if (!combo_)
			{
				attack_ = false;
				attackCounter = 0;
			}
			else
			{
				//attackCounter++;
				if (attackCounter >= ATTACKING_ANIMATION_FRAME)
				{
					combo_ = false;
					attack_ = false;
					attackCounter = 0;
				}
			}
			//attack_ = false;
			//attackCounter = 0;
		}
	}
	else attackCounter = 0;
	if (death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &deathClips[deathCounter / 3], &camera, 0, NULL, getFlip());
		deathCounter++;
		if (deathCounter / 3 >= DEATH_ANIMATION_FRAME)
		{
			deathCounter = 77;
		}
	}
	else deathCounter = 0;
	if (shield_ && !death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &shieldClips[shieldCounter / 2], &camera, 0, NULL, getFlip());
		shieldCounter++;
		if (shieldCounter / 2 >= 7)
		{
			if (!parry_)
			{
				shieldCounter = 0;
				shield_ = false;
			}
			else
			{
				shieldCounter++;
				if (shieldCounter / 2 >= SHIELD_ANIMATION_FRAME)
				{
					parry_ = false;
					shield_ = false;
					shieldCounter = 0;
				}
			}
		}
	}
	else shieldCounter = 0;
	if (shieldBuff_ && !death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &shieldBuffClips[shieldBuffCounter / 2], &camera, 0, NULL, getFlip());
		shieldBuffCounter++;
		if (shieldBuffCounter / 2 >= SHIELDBUFF_ANIMATION_FRAME)
		{
			shieldBuffCounter = 0;
			shieldBuff_ = false;
			shieldBuffParry_ = true;
		}

	}
	else shieldBuffCounter = 0;
	if (rest_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &restClips[restCounter / 4], &camera, 0, NULL, getFlip());
		restCounter++;
		if (restCounter / 4 >= REST_ANIMATION_FRAME)
		{
			restCounter = 80;
		}
	}
	else restCounter = 0;
}

void Secret::GoToNextLevel()
{
	x_ = 16 * 16;
	y_ = 62 * 16;
	levelSTT = 1;
	//x_val_ = 0;
	//y_val_ = 0;
	shieldBuff_ = false;
	shield_ = false;
	death_ = false;
	final_ = false;
	deathCounter = 0;
	shieldCounter = 0;
	shieldBuffCounter = 0;
	flip_type_ = SDL_FLIP_NONE;
}

void Secret::ResetSecret()
{
	x_ = 16 * 16;
	y_ = 20 * 16;
	x_val_ = 0;
	y_val_ = 0;
	shieldBuff_ = false;
	shield_ = false;
	death_ = false;
	final_ = false;
	down_ = false;
	deathCounter = 0;
	shieldCounter = 0;
	shieldBuffCounter = 0;
	flip_type_ = SDL_FLIP_NONE;
	levelSTT = 0;
}