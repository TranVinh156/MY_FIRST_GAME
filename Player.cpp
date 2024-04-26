#include "Player.h"

Player::Player(float p_x, float p_y, SDL_Texture* p_text) : Entity(p_x, p_y, p_text)
{
	Collision.x = getX() + 48;
	Collision.y = getY() + 48;
	Collision.w = PLAYER_WIDTH;
	Collision.h = PLAYER_HEIGHT;

	// cần sauwr lại ảnh
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
		deathClips[i] = { i * FRAME_WIDTH, 2 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
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
		attackingClips[i] = { i * FRAME_WIDTH, 4 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
	for (int i = 0; i < ROLLING_ANIMATION_FRAME; i++)
	{
		rollingClips[i] = { i * FRAME_WIDTH, 5 * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT };
	}
}


Player::~Player()
{

}


void Player::HandleInput(SDL_Event& events, Mix_Chunk* p_playerSFX[])
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
					doublejump_ = true;
					Mix_PlayChannel(-1, p_playerSFX[jumpSFX], 0);
				}
				if (on_ground_)
				{
					Jump();
					Mix_PlayChannel(-1, p_playerSFX[jumpSFX], 0);
				}
				
				break;
			//case SDLK_j:
			//	Attack();
				
			//	break;
			case SDLK_LCTRL: // nhảy lên
				if (on_ground_)
				{
					Roll();
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
				x_val_ += PLAYER_VAL;
				break;

			case SDLK_d: // sang phải
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
				//Mix_PlayChannel(-1, p_playerSFX[attackSFX], 0);
				Attack();
				//chưa biết làm gì
			}

		}
		else if (events.type == SDL_MOUSEBUTTONUP && events.key.repeat == 0)
		{
			//attack_ = false;
			// chưa biết làm gì
		}
	}
}

void Player::HandleCamera(SDL_Rect& camera, int& p_levelSTT, bool& p_nextlevel, int& p_skeCount)
{
	if (getX() + 48 >= LEVEL_WIDTH - SCREEN_WIDTH)
	{
		camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
		final_ = true;
	}
	else if (!final_)
	{
		camera.x = (getX() + 48) - SCREEN_WIDTH / 2;		
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.x > LEVEL_WIDTH - camera.w)
		{
			camera.x = LEVEL_WIDTH - camera.w;
		}
	}
	
	camera.y = (getY() + 48) - SCREEN_HEIGHT / 2;
	if (camera.y < 0) 
	{
		camera.y = 0;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}

	if (getX() + 48 >= 4704 && p_levelSTT == 0 && p_skeCount >= 50)
	{
		GoToNextLevel();
		p_levelSTT = levelSTT;
		p_nextlevel = true;
	}
}

void Player::Attack()
{
	//attackCounter = 0;
	if (attack_)
	{
		combo_ = true;
		//Mix_PlayChannel(-1, p_playerSFX[attackSFX], 0);
	}
	else combo_ = false;
	if (!death_ && !run_ && !attack_) attack_ = true;
	

	
}

void Player::Roll()
{
	//rollCounter = 0;
	if (!roll_)
	{
		if (!jump_)
		{
			roll_ = true;
			attack_ = false;
		}
	}
	
}

void Player::Jump()
{
	
	if (on_ground_)
	{
		y_val_ -= 6;
		on_ground_ = false;
	}
	
}

void Player::Gravity()
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

void Player::Update(std::vector<Skeleton*>& skeletonList, std::vector<Game_Map>& levelList, Boss& p_boss, Mix_Chunk* p_playerSFX[])
{
	Gravity();
	//on_ground_ = true;
	if (!death_) GetHit(skeletonList, p_boss, p_playerSFX);

	//x_val_ += PLAYER_VAL;
	if (!death_ && on_ground_ && x_val_ == 0 && !roll_)
	{
		idle_ = true;
	}
	else idle_ = false;

	if (!death_ && on_ground_ && x_val_ != 0 && !roll_)
	{
		run_ = true;
	}
	else run_ = false;

	if (!death_ && !on_ground_ && y_val_ > 0)
	{
		fall_ = true;
	}
	else fall_ = false;

	if (!death_ && !on_ground_ && y_val_ < 0)
	{
		jump_ = true;
	}
	else jump_ = false;

	if (x_val_ > 0) flip_type_ = SDL_FLIP_NONE;
	if (x_val_ < 0) flip_type_ = SDL_FLIP_HORIZONTAL;


	//di chuyển theo x
	if (!death_)
	{
		x_ += x_val_;
		Collision.x = getX() + 48;

		if (final_)
		{
			if (getX() + 48 < LEVEL_WIDTH - SCREEN_WIDTH)
			{
				x_ = LEVEL_WIDTH - SCREEN_WIDTH - 48;
				Collision.x = getX() + 48;
			}
			
		}
		if (getX() + 48 < 0 + levelList.at(levelSTT).getX())
		{
			x_ = levelList.at(levelSTT).getX() - 48;
			Collision.x = getX() + 48;
		}

		if (getX() + 48 + PLAYER_WIDTH >= LEVEL_WIDTH * 10)
		{
			x_ = SCREEN_WIDTH - PLAYER_WIDTH - 48 + levelList.at(levelSTT).getX();
			Collision.x = getX() + 48; // cần sửa
		}



		if (CommonFunc::checkToMap(Collision, levelList, levelSTT))
		{

			x_ -= x_val_;
			Collision.x = getX() + 48;
		}
	}

	//x_ += x_val_;

	//di chuyển theo y

	y_ += y_val_;
	Collision.y = getY() + 48; // đang ko biết vì sao +16 lại đúng :)))
	if (getY() + 48 < 0)
	{
		y_ = -48;
		Collision.y = getY() + 48;
	}
	// đang lỗi cần sửa
	if (CommonFunc::checkToMap(Collision, levelList, on_ground_, groundSTT, levelSTT)) {
		if (y_val_ > 0)
		{
			y_ = levelList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 48 - 32;
			if (fall_)
			{
				on_ground_ = true;
				doublejump_ = false;
				Mix_PlayChannel(-1, p_playerSFX[landSFX], 0);
			}
		}
		else if (y_val_ < 0 && (levelList.at(levelSTT).getTilesList().at(groundSTT - 565 * 2)->getType() <= 60 || levelList.at(levelSTT).getTilesList().at(groundSTT - 565 * 2 + 1)->getType() <= 60))
		{
			//y_ += y_val_ + 16;
			y_ = levelList.at(levelSTT).getTilesList().at(groundSTT - 565 * 2)->getY() - 32;
			y_val_ = 0;
		}
		Collision.y = getY() + 48;
	}

	if (levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() >= 83 && levelList.at(levelSTT).getTilesList().at(groundSTT)->getType() <= 89)
	{
		death_ = true;
		Mix_PlayChannel(-1, p_playerSFX[deathSFX], 0);
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
			Collision.y = getY() + 48;
		}
	}
	//std::cout << attackCounter << std::endl;
	//std::cout << on_ground_ << std::endl;
	//std::cout << groundSTT << std::endl;
	//std::cout << Collision.x << std::endl;
	//std::cout << getX() - levelSTT * LEVEL_WIDTH << std::endl;
}

void Player::KnockBack()
{
	if (beinghit_)
	{
		y_val_ += -3;
		if (flip_type_ = SDL_FLIP_NONE) x_ += -32;
		else if (flip_type_ = SDL_FLIP_HORIZONTAL) x_ += 16;
	}
}

bool Player::isAttacking()
{
	if ((attackCounter / 3 >= 1 && attackCounter / 3 <= 2) || (attackCounter / 3 >= 6 && attackCounter / 3 <= 8))
	{
		return true;
	}
	else return false;
}

void Player::GetHit(std::vector<Skeleton*>& skeletonList, Boss& p_boss, Mix_Chunk* p_playerSFX[])
{
	
	for (int i = 0; i < skeletonList.size(); i++)
	{
		if (skeletonList.at(i)!= NULL && skeletonList.at(i)->isAttacking() && skeletonList.at(i)->getFlip() == SDL_FLIP_NONE && !roll_)
		{
			SDL_Rect a = { skeletonList.at(i)->getCollision().x + 16, skeletonList.at(i)->getCollision().y, 32, 32 };
			if (CommonFunc::checkCollision(a, getCollision()))
			{
				death_ = true;
				Mix_PlayChannel(-1, p_playerSFX[deathSFX], 0);
			}
		}

		if (skeletonList.at(i) != NULL && skeletonList.at(i)->isAttacking() && skeletonList.at(i)->getFlip() == SDL_FLIP_HORIZONTAL && !roll_)
		{
			SDL_Rect a = { skeletonList.at(i)->getCollision().x - 32, skeletonList.at(i)->getCollision().y, 32, 32 };
			if (CommonFunc::checkCollision(a, getCollision()))
			{
				death_ = true;
				Mix_PlayChannel(-1, p_playerSFX[deathSFX], 0);
			}
		}
	}

	if (p_boss.isAttacking() && !roll_ && p_boss.getFlip() == SDL_FLIP_NONE)
	{
		SDL_Rect a = { p_boss.getCollision().x + 64, p_boss.getCollision().y, 80, 80 };
		if (CommonFunc::checkCollision(a, getCollision()))
		{
			death_ = true;
			Mix_PlayChannel(-1, p_playerSFX[deathSFX], 0);
		}
	}
	if (p_boss.isAttacking() && !roll_ && p_boss.getFlip() == SDL_FLIP_HORIZONTAL)
	{
		SDL_Rect a = { p_boss.getCollision().x - 80, p_boss.getCollision().y, 80, 80 };
		if (CommonFunc::checkCollision(a, getCollision()))
		{
			death_ = true;
			Mix_PlayChannel(-1, p_playerSFX[deathSFX], 0);
		}
	}

	if (p_boss.isSmashing())
	{
		SDL_Rect a = { p_boss.getCollision().x, p_boss.getCollision().y + 64,80, 16 };
		if (CommonFunc::checkCollision(a, getCollision()))
		{
			death_ = true;
			Mix_PlayChannel(-1, p_playerSFX[deathSFX], 0);
		}
	}

	for (int i = 0; i < p_boss.GetBulletList().size(); i++)
	{
		if (p_boss.GetBulletList().at(i) != NULL)
		{
			if (CommonFunc::checkCollision(p_boss.GetBulletList().at(i)->getCollision(), getCollision()))
			{
				death_ = true;
				Mix_PlayChannel(-1, p_playerSFX[deathSFX], 0);
				p_boss.GetBulletList().at(i)->setMove(false);
			}
		}
		
	}
}

void Player::Render(SDL_Rect& camera, Mix_Chunk* p_playerSFX[])
{
	if (idle_ && !attack_ && !beinghit_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &idlingClips[idleCounter / 4], &camera, 0, NULL, getFlip());
		idleCounter++;
		if (idleCounter / 4 >= IDLING_ANIMATION_FRAME) idleCounter = 0;
	}
	else idleCounter = 0;
	
	if (run_ && !attack_ && !beinghit_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &runningClips[runCounter / 4], &camera, 0, NULL, getFlip());
		if (runCounter % 4 == 0)
		{
			Mix_PlayChannel(-1, p_playerSFX[stepSFX], 0);
		}
		runCounter++;
		if (runCounter / 4 >= RUNNING_ANIMATION_FRAME) runCounter = 0;
	}
	else runCounter = 0;

	if (jump_ && !attack_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &jumpingClips[jumpCounter / 4], &camera, 0, NULL, getFlip());
		jumpCounter++;
		if (jumpCounter / 4 >= JUMPING_ANIMATION_FRAME)
		{
			jumpCounter = 0;
			//rollCounter = 0;
			roll_ = false;
		}
	}
	else jumpCounter = 0;

	if (fall_ && !attack_ && !roll_)
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
	if (beinghit_)
	{
		//CommonFunc::renderAnimation(text_, x_, y_, &beinghit)
	}

	if (attack_ && !death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &attackingClips[attackCounter / 3], &camera, 0, NULL, getFlip());
		attackCounter++;
		if (attackCounter == 1 || attackCounter == 13) Mix_PlayChannel(-1, p_playerSFX[attackSFX], 0);
		if (attackCounter / 3 >= 4)
		{
			if (!combo_)
			{
				attack_ = false;
				attackCounter = 0;
			}
			else
			{
				//attackCounter++;
				if (attackCounter / 3 >= ATTACKING_ANIMATION_FRAME)
				{
					combo_ = false;
					attack_ = false;
					attackCounter = 0;
				}
			}
			//attack_ = false;
			//attackCounter = 0;
			roll_ = false;
			//rollCounter = 0;
		}
	}
	else attackCounter = 0;

	if (death_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &deathClips[deathCounter / 4], &camera, 0, NULL, getFlip());
		deathCounter++;
		if (deathCounter / 4 >= DEATH_ANIMATION_FRAME)
		{
			deathCounter = 36;
		}
	}
	else deathCounter = 0;

	if (roll_ && !jump_ && !death_ && !attack_)
	{
		CommonFunc::renderAnimation(text_, x_, y_, &rollingClips[rollCounter / 2], &camera, 0, NULL, getFlip());
		rollCounter++;
		if (rollCounter / 2 >= ROLLING_ANIMATION_FRAME)
		{

			rollCounter = 0;
			roll_ = false;
		}
		//attackCounter = 0;
	}
	else rollCounter = 0;

}

void Player::GoToNextLevel()
{
	x_ = 16 * 16;
	y_ = 63 * 16;
	levelSTT = 1;
	//x_val_ = 0;
	//y_val_ = 0;
	death_ = false;
	final_ = false;
	deathCounter = 0;
	flip_type_ = SDL_FLIP_NONE;
}


void Player::ResetPlayer()
{
	x_ = 16 * 16;
	y_ = 21 * 16;
	x_val_ = 0;
	y_val_ = 0;
	death_ = false;
	final_ = false;
	down_ = false;
	deathCounter = 0;
	flip_type_ = SDL_FLIP_NONE;
	levelSTT = 0;
}