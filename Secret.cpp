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
	for (int i = 0; i < SPECIAL_ATTACKING_ANIMATION_FRAME; i++)
	{
		specialAttackingClips[i] = { i * FRAME_WIDTH, 8 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < BEINGHIT_ANIMATION_FRAME; i++)
	{
		beinghitClips[i] = { i * FRAME_WIDTH, 9 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < SPELL_ANIMATION_FRAME; i++)
	{
		spellClips[i] = { i * FRAME_WIDTH, 10 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < HEALING_ANIMATION_FRAME; i++)
	{
		healingClips[i] = { i * FRAME_WIDTH, 11 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
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
				if (!attack_ && !specialAttack_ && !spell_ && !heal_ && shieldBuffCooldown == 0)
				{
					shieldBuff_ = true;
					shieldBuffCooldown = 100;
					Mix_PlayChannel(-1, p_secretSFX[spellSFX], 0);
				}
				break;
			case SDLK_e:
				SpecialAttack();

				break;
			case SDLK_r: // nhảy lên
				
				if (idle_)
				{
					rest_ = true;
				}
				break;
			case SDLK_h:
				rest_ = false;
				if (!death_ && !heal_ && !run_ && !attack_ && !shield_ && !shieldBuff_&& !spell_ && !specialAttack_)
				{
					heal_ = true;
				}
				break;
			case SDLK_c:
				Spell();
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
				//restCounter = 0;
				if (!attack_ && !specialAttack_ && !spell_)
				{
					Shield();
				}
				
			}

		}
		else if (events.type == SDL_MOUSEBUTTONUP && events.key.repeat == 0)
		{

		}
	}
}

void Secret::HandleCamera(SDL_Rect& camera, int& p_levelSTT, bool& p_nextlevel, int& p_skeCount)
{
	
	if (getX() + 80 >= LEVEL_WIDTH - SCREEN_WIDTH && levelSTT == 1)
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
	if (getX() + 80 >= 8944 && p_levelSTT == 0 && p_skeCount >= 0)
	{
		p_levelSTT = levelSTT;
		p_nextlevel = true;
	}
}

void Secret::Attack()
{
	//attackCounter = 0;
	rest_ = false;
	//restCounter = 0;
	if (attack_)
	{
		combo_ = true;
	}
	else combo_ = false;
	if (!death_ && !run_ && !attack_ && !specialAttack_ && !spell_) attack_ = true;



}

void Secret::SpecialAttack()
{
	rest_ = false;
	//restCounter = 0;
	if (!death_ && !specialAttack_ && !attack_ && !spell_) specialAttack_ = true;
}

void Secret::Spell()
{
	rest_ = false;
	if (!death_ && !specialAttack_ && !attack_ && !run_ && !spell_ && spellCooldown == 0)
	{
		spell_ = true;
	}
	//if (!death_ && !)
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

void Secret::Update(std::vector<Undead*>& undeadList, std::vector<Archer*>& archerList, std::vector<Game_Map>& levelList, Boss& p_boss, Mix_Chunk* p_secretSFX[])
{
	Gravity();

	if (!death_) GetHit(undeadList, archerList, p_boss, p_secretSFX);

	//x_val_ += PLAYER_VAL;
	if (!death_ && on_ground_ && x_val_ == 0 && !shield_ && !shieldBuff_ && !attack_ && !spell_ && !specialAttack_ && !heal_)
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
	if (!death_ && !shield_ && !shieldBuff_ && !specialAttack_ && !spell_ && !heal_)
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
			y_ = levelList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 96;
			if (fall_)
			{
				on_ground_ = true;
				doublejump_ = false;
				Mix_PlayChannel(-1, p_secretSFX[landSFX], 0);
			}
		}
		else if (y_val_ < 0)
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
	if ((levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() >= 76 && levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() < 83) || (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() >= 76 && levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() < 80))
	{
		if (!jump_)
		{
			y_ = levelList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 96;
		}

		if (fall_)
		{
			on_ground_ = true;
			doublejump_ = false;
			Mix_PlayChannel(-1, p_secretSFX[landSFX], 0);
		}
	}
	if (down_)
	{
		if ((levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() >= 76 && levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() <= 82) && (levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() >= 76 && levelList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() <= 82)) 
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

bool Secret::isSpecialAttacking()
{
	if (specialAttackCounter >= 18 && specialAttackCounter <= 20)
	{
		return true;
	}
	else return false;
}

bool Secret::isSpelling()
{
	if (spellCounter / 3 >= 26 && spellCounter / 3 <= 29)
	{
		return true;
	}
	else return false;
}

void Secret::GetHit(std::vector<Undead*>& undeadList, std::vector<Archer*>& archerList, Boss& p_boss, Mix_Chunk* p_secretSFX[])
{
	for (int i = 0; i < undeadList.size(); i++)
	{
		if (undeadList.at(i) != NULL && undeadList.at(i)->isAttacking() && undeadList.at(i)->getFlip() == SDL_FLIP_NONE)
		{
			SDL_Rect a = { undeadList.at(i)->getCollision().x + 16, undeadList.at(i)->getCollision().y, 32, 32 };
			if (CommonFunc::checkCollision(a, getCollision()))
			{
				
				if (shieldCounter / 2 >= 1 && shieldCounter / 2 <= 7)
				{
					parry_ = true;
					shieldCounter = 7 * 2;
					//death_ = false;
				}
				else if (!parry_ && !shieldBuffParry_ && !spell_ && !beinghit_ && !death_)
				{
					beinghit_ = true;
					Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
				}
				

				
			}
		}

		if (undeadList.at(i) != NULL && undeadList.at(i)->isAttacking() && undeadList.at(i)->getFlip() == SDL_FLIP_HORIZONTAL)
		{
			SDL_Rect a = { undeadList.at(i)->getCollision().x - 32, undeadList.at(i)->getCollision().y, 32, 32 };
			if (CommonFunc::checkCollision(a, getCollision()))
			{
				//death_ = true;
				
				if (shieldCounter / 2 >= 1 && shieldCounter / 2 <= 7)
				{
					parry_ = true;
					shieldCounter = 7 * 2;
					//death_ = false;
				}
				else if (!parry_ && !shieldBuffParry_ && !spell_ && !beinghit_ && !death_)
				{
					beinghit_ = true;
					Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
				}
				
				
			}
		}
	}
	for (int i = 0; i < archerList.size(); i++)
	{
		for (int j = 0; j < archerList.at(i)->GetBulletList().size(); j++)
		{
			if (archerList.at(i)->GetBulletList().at(j) != NULL)
			{
				if (CommonFunc::checkCollision(getCollision(), archerList.at(i)->GetBulletList().at(j)->getCollision()))
				{
					if (!shield_ && !shieldBuffParry_ && !spell_ && !beinghit_ && !death_)
					{
						beinghit_ = true;
						Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
					}
					else if(shieldBuffParry_)
					{
						shieldBuffParry_ = false;
						Bullet* bullet = new Bullet(getCollision().x + SECRET_WIDTH, getCollision().y, NULL);
						if (archerList.at(i)->GetBulletList().at(j)->getFlip() == SDL_FLIP_NONE)
						{
							bullet->setFlip(SDL_FLIP_HORIZONTAL);
						}
						else
						{
							bullet->setFlip(SDL_FLIP_NONE);
						}
						
						bullet->setSize_Position(getCollision().x, 16);
						bullet->setType(Bullet::NORMAL);
						bullet->setMove(true);
						bulletList.push_back(bullet);
					}
					else if (shield_)
					{
						if (getFlip() != archerList.at(i)->GetBulletList().at(j)->getFlip())
						{
							Bullet* bullet = new Bullet(getCollision().x + SECRET_WIDTH, getCollision().y, NULL);
							if (archerList.at(i)->GetBulletList().at(j)->getFlip() == SDL_FLIP_NONE)
							{
								bullet->setFlip(SDL_FLIP_HORIZONTAL);
							}
							else
							{
								bullet->setFlip(SDL_FLIP_NONE);
							}

							bullet->setSize_Position(getCollision().x, 16);
							bullet->setType(Bullet::NORMAL);
							bullet->setMove(true);
							bulletList.push_back(bullet);
						}
						else
						{
							beinghit_ = true;
							Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
						}
					}
					archerList.at(i)->GetBulletList().at(j)->setMove(false);
				}
			}
		}
	}
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
			else if (!parry_ && !shieldBuffParry_ && !spell_ && !beinghit_ && !death_)
			{
				beinghit_ = true;
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
			else if (!parry_ && !shieldBuffParry_ && !spell_ && !beinghit_ && !death_)
			{
				beinghit_ = true;
				Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
			}
		}
	}

	if (p_boss.isSmashing())
	{
		SDL_Rect a = { p_boss.getCollision().x, p_boss.getCollision().y + 64,80, 16 };
		if (CommonFunc::checkCollision(a, getCollision()) && !spell_ && !beinghit_ && !death_)
		{
			beinghit_ = true;
			Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
		}
	}

	for (int i = 0; i < p_boss.GetBulletList().size(); i++)
	{
		if (p_boss.GetBulletList().at(i) != NULL)
		{
			if (CommonFunc::checkCollision(p_boss.GetBulletList().at(i)->getCollision(), getCollision()) && !spell_ && !beinghit_ && !death_)
			{
				beinghit_ = true;
				Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
				p_boss.GetBulletList().at(i)->setMove(false);
			}
		}

	}

	if (health_ <= 0)
	{
		death_ = true;
		Mix_PlayChannel(-1, p_secretSFX[deathSFX], 0);
	}
}

void Secret::Render(SDL_Rect& camera, Mix_Chunk* p_secretSFX[])
{
	if (idle_ && !attack_ && !death_ && !rest_ && !specialAttack_ && !beinghit_ && !spell_ && !heal_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &idlingClips[idleCounter / 4], &camera, 0, NULL, getFlip());
		idleCounter++;
		if (idleCounter / 4 >= IDLING_ANIMATION_FRAME) idleCounter = 0;
	}
	else idleCounter = 0;
	if (run_ && !attack_ && !death_ && !shieldBuff_ && !specialAttack_ && !beinghit_ && !spell_ && !heal_)
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

	if (jump_ && !attack_ && !shield_ && !shieldBuff_ && !specialAttack_ && !beinghit_ && !spell_ && !heal_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &jumpingClips[jumpCounter / 4], &camera, 0, NULL, getFlip());
		jumpCounter++;
		if (jumpCounter / 4 >= JUMPING_ANIMATION_FRAME)
		{
			jumpCounter = 0;
		}
	}
	else jumpCounter = 0;
	if (fall_ && !attack_ && !shield_ && !shieldBuff_ && !specialAttack_ && !beinghit_ && !spell_ && !heal_)
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

	if (specialAttack_ && !shield_ && !shieldBuff_ && !death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &specialAttackingClips[specialAttackCounter], &camera, 0, NULL, getFlip());
		specialAttackCounter++;
		if (specialAttackCounter == 2) Mix_PlayChannel(-1, p_secretSFX[spellSFX], 0);
		if (specialAttackCounter == 18) Mix_PlayChannel(-1, p_secretSFX[attackSFX], 0);
		if (specialAttackCounter >= SPECIAL_ATTACKING_ANIMATION_FRAME)
		{
			specialAttack_ = false;
			specialAttackCounter = 0;
		}
	}
	else specialAttackCounter = 0;

	if (spell_ && !shield_ && !shieldBuff_ && !death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &spellClips[spellCounter / 3], &camera, 0, NULL, getFlip());
		spellCounter++;
		if (spellCounter / 3 >= 4 && spellCounter / 3 <= 29) Mix_PlayChannel(-1, p_secretSFX[spellSFX], 0);
		if (spellCounter / 3 >= SPELL_ANIMATION_FRAME)
		{
			spell_ = false;
			spellCounter = 0;
			spellCooldown = 1000;
			health_ = 3;
		}
	}
	else
	{
		spellCounter = 0;
		if (spellCooldown != 0)
		{
			spellCooldown--;
		}
	}

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
	if (beinghit_ && !attack_ && !specialAttack_ && !shield_ && !shieldBuff_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &beinghitClips[beinghitCounter / 2], &camera, 0, NULL, getFlip());
		beinghitCounter++;
		if (beinghitCounter / 2 >= BEINGHIT_ANIMATION_FRAME)
		{
			beinghit_ = false;
			health_--;
			beinghitCounter = 0;
		}
	}
	else beinghitCounter = 0;
	if (heal_ && !death_ && !shield_ && !shieldBuff_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &healingClips[healCounter / 2], &camera, 0, NULL, getFlip());
		healCounter++;
		if (healCounter / 2 == 2) Mix_PlayChannel(-1, p_secretSFX[spellSFX], 0);
		if (healCounter / 2 >= HEALING_ANIMATION_FRAME)
		{
			heal_ = false;
			healCounter = 0;
			if (health_ < 3)
			{
				health_++;
			}
		}
	}
	else healCounter = 0;
}

void Secret::GoToNextLevel(std::vector<Game_Map> levelList)
{
	levelSTT = 1;
	x_ = TILE_WIDTH * (levelList.at(levelSTT).GetPlayerPos() % 565) - 80;
	y_ = TILE_HEIGHT* (levelList.at(levelSTT).GetPlayerPos() / 565) - 96;
	
	shieldBuff_ = false;
	specialAttack_ = false;
	spell_ = false;
	shield_ = false;
	death_ = false;
	final_ = false;
	deathCounter = 0;
	shieldCounter = 0;
	shieldBuffCounter = 0;
	spellCooldown = 0;
	flip_type_ = SDL_FLIP_NONE;
}

void Secret::ResetSecret(std::vector<Game_Map> levelList)
{
	levelSTT = 0;
	x_ = TILE_WIDTH * (levelList.at(levelSTT).GetPlayerPos() % 565) - 80;
	y_ = TILE_HEIGHT * (levelList.at(levelSTT).GetPlayerPos() / 565) - 96;
	health_ = 3;
	x_val_ = 0;
	y_val_ = 0;
	shieldBuff_ = false;
	specialAttack_ = false;
	shield_ = false;
	death_ = false;
	final_ = false;
	down_ = false;
	deathCounter = 0;
	shieldCounter = 0;
	shieldBuffCounter = 0;
	spellCooldown = 0;
	flip_type_ = SDL_FLIP_NONE;
}