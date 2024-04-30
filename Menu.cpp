#include "Menu.h"

Menu::Menu(SDL_Texture* p_button, SDL_Texture* p_bg, SDL_Texture* p_helpText, SDL_Texture* p_name)
{
	this->ButtonText = p_button;
	this->BgText = p_bg;
	this->HelpText = p_helpText;
	this->NameText = p_name;
	for (int i = 0; i < TYPE_BUTTON; i++)
	{
		playButton[i] = { i * BUTTON_WIDTH, 0, BUTTON_WIDTH, BUTTON_HEIGHT };
		helpButton[i] = { i * BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
		exitButton[i] = { i * BUTTON_WIDTH, 2 * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
		muteButton[i] = { i * BUTTON_WIDTH, 3 * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
		soundButton[i] = { i * BUTTON_WIDTH, 4 * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
		homeButton[i] = { i * BUTTON_WIDTH, 5 * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
		retryButton[i] = { i * BUTTON_WIDTH, 6 * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
		pauseButton[i] = { i * BUTTON_WIDTH, 7 * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
		for (int j = 0; j < HUNTER_ANIMATION_FRAME; j++)
		{
			hunterButton[i][j] = { j * BUTTON_WIDTH * 2, (8 + 2 * i) * BUTTON_HEIGHT, BUTTON_WIDTH * 2, BUTTON_HEIGHT * 2 };
		}
		for (int j = 0; j < SECRET_ANIMATION_FRAME; j++)
		{
			YenButton[i][j] = { j * BUTTON_WIDTH * 2, (14 + 2 * i) * BUTTON_HEIGHT, BUTTON_WIDTH * 2, BUTTON_HEIGHT * 2};
		}
		//secretButton[i] = { i * BUTTON_WIDTH, 4 * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
	}
	for (int i = 0; i < MAX_HEALTH_BAR; i++)
	{
		HealthBar[i] = { i * HEALTH_BAR_WIDTH, 20 * BUTTON_HEIGHT, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT };
	}
}

void Menu::HandleInput(SDL_Event event, bool& gameRunning, Player& p_player, Secret& p_secret, Boss& p_boss, Mix_Chunk* p_menuSFX[])
{
	switch (event.type)
	{
	case SDL_TEXTINPUT:
		if (name_)
		{
			playerName += event.text.text;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			if (isMenu()) // ở menu
			{
				if (CheckMouse(button1.x, button1.y) && !help_)
				{
					pressed[0] = true;
					selected[0] = false;
					Mix_PlayChannel(-1, p_menuSFX[press], 0);
					//menu_ = false;
					//choose_ = true;
				}
				if (CheckMouse(button2.x, button2.y))
				{
					pressed[1] = true;
					selected[1] = false;
					Mix_PlayChannel(-1, p_menuSFX[press], 0);
					//help_ = true;
				}
				else help_ = false;
				if (CheckMouse(button3.x, button3.y) && !help_)
				{
					pressed[2] = true;
					selected[2] = false;
					Mix_PlayChannel(-1, p_menuSFX[press], 0);
					//gameRunning = false;
				}
			}

			if (p_player.getDead() || p_secret.getDead() || p_boss.GetDead())
			{
				if (CheckMouse(button4.x, button4.y))
				{
					pressed[3] = true;
					selected[3] = false;
					Mix_PlayChannel(-1, p_menuSFX[press], 0);
					//reset_ = true;
				}

				if (CheckMouse(button5.x, button5.y))
				{
					pressed[4] = true;
					selected[4] = false;
					Mix_PlayChannel(-1, p_menuSFX[press], 0);
					//menu_ = true;
					//reset_ = true;
				}
			}

			if (isChoose())
			{
				if (CheckMouse(button6.x, button6.y, BUTTON_WIDTH * 2, BUTTON_HEIGHT * 2))
				{
					pressed[5] = true;
					selected[5] = false;
					Mix_PlayChannel(-1, p_menuSFX[press], 0);
					//secret_ = false;
					//choose_ = false;
					//menu_ = false;
				}
				if (CheckMouse(button7.x, button7.y, BUTTON_WIDTH * 2, BUTTON_HEIGHT * 2) && unlock_)
				{
					pressed[6] = true;
					selected[6] = false;
					Mix_PlayChannel(-1, p_menuSFX[press], 0);
					//choose_ = false;
					//menu_ = false;
					//secret_ = true;
				}
			}

			if (paused_)
			{
				if (CheckMouse(button4.x, button4.y))
				{
					pressed[3] = true;
					selected[3] = false;
					Mix_PlayChannel(-1, p_menuSFX[press], 0);
					//paused_ = false;
				}

				if (CheckMouse(button5.x, button5.y))
				{
					pressed[4] = true;
					selected[4] = false;
					Mix_PlayChannel(-1, p_menuSFX[press], 0);
					//menu_ = true;
					//reset_ = true;
				}
			}

			
		}


		break;
	case SDL_MOUSEBUTTONUP:

		if (isMenu()) // ở menu
		{
			if (CheckMouse(button1.x, button1.y) && !help_ && pressed[0])
			{
				menu_ = false;
				choose_ = true;
			}
			if (CheckMouse(button2.x, button2.y) && pressed[1])
			{
				help_ = true;
			}
			else help_ = false;
			if (CheckMouse(button3.x, button3.y) && !help_ && pressed[2])
			{
				gameRunning = false;
			}
		}

		if (p_player.getDead() || p_secret.getDead() || p_boss.GetDead())
		{
			if (CheckMouse(button4.x, button4.y) && pressed[3])
			{
				reset_ = true;
			}

			if (CheckMouse(button5.x, button5.y) && pressed[4])
			{
				menu_ = true;
				reset_ = true;
			}
		}

		if (isChoose())
		{
			if (CheckMouse(button6.x, button6.y, BUTTON_WIDTH * 2, BUTTON_HEIGHT * 2) && pressed[5])
			{
				secret_ = false;
				choose_ = false;
				menu_ = false;
			}
			if (CheckMouse(button7.x, button7.y, BUTTON_WIDTH * 2, BUTTON_HEIGHT * 2) && unlock_ && pressed[6])
			{
				choose_ = false;
				menu_ = false;
				secret_ = true;
			}
		}

		if (paused_)
		{
			if (CheckMouse(button4.x, button4.y) && pressed[3])
			{
				paused_ = false;
			}

			if (CheckMouse(button5.x, button5.y) && pressed[4])
			{
				menu_ = true;
				paused_ = false;
				reset_ = true;
			}
		}
		for (int i = 0; i < 7; i++)
		{
			pressed[i] = false;
		}
		break;
	case SDL_MOUSEMOTION:
		if (isMenu()) // ở menu
		{
			if (CheckMouse(button1.x, button1.y) && !pressed[0])
			{
				selected[0] = true;
				if (!sfx_[0])
				{
					Mix_PlayChannel(-1, p_menuSFX[select], 0);
					sfx_[0] = true;
				}
			}
			else
			{
				selected[0] = false;
				sfx_[0] = false;
			}
			if (CheckMouse(button2.x, button2.y) && !pressed[1])
			{
				selected[1] = true;
				if (!sfx_[1])
				{
					Mix_PlayChannel(-1, p_menuSFX[select], 0);
					sfx_[1] = true;
				}
			}
			else
			{
				selected[1] = false;
				sfx_[1] = false;
			}
			if (CheckMouse(button3.x, button3.y) && !pressed[2])
			{
				selected[2] = true;
				if (!sfx_[2])
				{
					Mix_PlayChannel(-1, p_menuSFX[select], 0);
					sfx_[2] = true;
				}
			}
			else
			{
				selected[2] = false;
				sfx_[2] = false;
			}
		}

		if (p_player.getDead() || p_secret.getDead() || p_boss.GetDead())
		{
			if (CheckMouse(button4.x, button4.y) && !pressed[3])
			{
				selected[3] = true;
				if (!sfx_[3])
				{
					Mix_PlayChannel(-1, p_menuSFX[select], 0);
					sfx_[3] = true;
				}
			}
			else
			{
				selected[3] = false;
				sfx_[3] = false;
			}

			if (CheckMouse(button5.x, button5.y) && !pressed[4])
			{
				selected[4] = true;
				if (!sfx_[4])
				{
					Mix_PlayChannel(-1, p_menuSFX[select], 0);
					sfx_[4] = true;
				}
			}
			else
			{
				selected[4] = false;
				sfx_[4] = false;
			}
		}
		if (isChoose())
		{
			if (CheckMouse(button6.x, button6.y, BUTTON_WIDTH * 2, BUTTON_HEIGHT * 2))
			{
				selected[5] = true;
				if (!sfx_[5])
				{
					Mix_PlayChannel(-1, p_menuSFX[select], 0);
					sfx_[5] = true;
				}				//secret_ = true;
			}
			else
			{
				selected[5] = false;
				sfx_[5] = false;
			}
			if (CheckMouse(button7.x, button7.y, BUTTON_WIDTH * 2, BUTTON_HEIGHT * 2))
			{
				selected[6] = true;
				if (!sfx_[6])
				{
					Mix_PlayChannel(-1, p_menuSFX[select], 0);
					sfx_[6] = true;
				}				//secret_ = false;;
			}
			else
			{
				selected[6] = false;
				sfx_[6] = false;
			}
		}
		if (paused_)
		{
			if (CheckMouse(button4.x, button4.y))
			{
				selected[3] = true;
				if (!sfx_[3])
				{
					Mix_PlayChannel(-1, p_menuSFX[select], 0);
					sfx_[3] = true;
				}
			}
			else
			{
				selected[3] = false;
				sfx_[3] = false;
			}
			if (CheckMouse(button5.x, button5.y))
			{
				selected[4] = true;
				if (!sfx_[4])
				{
					Mix_PlayChannel(-1, p_menuSFX[select], 0);
					sfx_[4] = true;
				}
			}
			else
			{
				selected[4] = false;
				sfx_[4] = false;
			}
		}
		break;
	case SDL_KEYDOWN:
		if (event.key.repeat == 0) switch (event.key.keysym.sym) 
		{
		case SDLK_BACKSPACE:
			if (name_ && playerName.length() > 0)
			{
				playerName.pop_back();
			}
			break;
		case SDLK_RETURN:
			if (name_)
			{
				menu_ = true;
				name_ = false;
				if (playerName == "NGUYEN_THI_NGOC_YEN" || playerName == "NGUYEN THI NGOC YEN" || playerName == "YEN") unlock_ = true;
			}
			break;
		case SDLK_ESCAPE:
			if (!paused_) paused_ = true;
			else paused_ = false;
			break;
		case SDLK_f:
			if (full_screen_ == 1)
			{
				
				//SDL_SetWindowSize(g_window, 1440, 960);
				//SDL_RenderSetLogicalSize(g_screen, 720, 480);
				full_screen_ = 2;
				CommonFunc::fullScreen(full_screen_);

			}
			else if (full_screen_ == 2)
			{
				
				full_screen_ = 1;
				CommonFunc::fullScreen(full_screen_);
			}
			break;
		default:
			break;
		
		}

		if (isChoose())
		{
			if (event.key.repeat == 0)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_y:
					secretCode_[0] = true;
					//unlock_ = true;
					break;
				case SDLK_e:
					if (secretCode_[0])
					{
						secretCode_[1] = true;
						//unlock_ = true;
					}				
					else
					{
						for (int i = 0; i < 16; i++)
						{
							secretCode_[i] = false;
						}
					}
					break;
				case SDLK_n:
					if (secretCode_[1])
					{
						secretCode_[2] = true;
						//unlock_ = true;
					}
					else
					{
						for (int i = 0; i < 16; i++)
						{
							secretCode_[i] = false;
						}
					}
					break;
				case SDLK_2:
					if (secretCode_[2])
					{
						secretCode_[3] = true;
					}
					else
					{
						for (int i = 0; i < 16; i++)
						{
							secretCode_[i] = false;
						}
					}
					break;
				case SDLK_3:
					if (secretCode_[3])
					{
						secretCode_[4] = true;
					}
					else
					{
						for (int i = 0; i < 16; i++)
						{
							secretCode_[i] = false;
						}
					}
					break;
				case SDLK_1:
					if (secretCode_[4])
					{
						secretCode_[5] = true;
					}
					else
					{
						for (int i = 0; i < 16; i++)
						{
							secretCode_[i] = false;
						}
					}
					break;
				case SDLK_0:
					if (secretCode_[5])
					{
						secretCode_[6] = true;
						unlock_ = true;
					}
					else
					{
						for (int i = 0; i < 16; i++)
						{
							secretCode_[i] = false;
						}
					}
					break;
				
					
					break;
				default:
					break;
				}
			}
		}
		break;
	
	default:
		break;
	}
}

bool Menu::CheckMouse(const int& p_x, const int& p_y, int p_width, int p_height)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (p_width != 0 && p_height != 0)
	{
		if (x >= full_screen_ * p_x && y >= full_screen_ * p_y && x <= full_screen_ * (p_x + p_width) && y <= full_screen_ * (p_y + p_height)) return true;
	}
	if (x >= full_screen_ * p_x && y >= full_screen_ * p_y && x <= full_screen_ * (p_x + BUTTON_WIDTH) && y <= full_screen_ * (p_y + BUTTON_HEIGHT)) return true;
	return false;

}

void Menu::RenderMainMenu()
{
	if (isMenu())
	{
		CommonFunc::renderTexture(BgText, 0, 0, 720, 480);

		if (selected[0])
		{
			CommonFunc::renderTexture(ButtonText, button1.x, button1.y, 0, 0, &playButton[1]);
		}
		else if (pressed[0])
		{
			CommonFunc::renderTexture(ButtonText, button1.x, button1.y, 0, 0, &playButton[2]);
		}
		else
		{
			CommonFunc::renderTexture(ButtonText, button1.x, button1.y, 0, 0, &playButton[0]);
		}

		if (selected[1])
		{
			CommonFunc::renderTexture(ButtonText, button2.x, button2.y, 0, 0, &helpButton[1]);
		}
		else if (pressed[1])
		{
			CommonFunc::renderTexture(ButtonText, button2.x, button2.y, 0, 0, &helpButton[2]);
		}
		else
		{
			CommonFunc::renderTexture(ButtonText, button2.x, button2.y, 0, 0, &helpButton[0]);
		}

		if (selected[2])
		{
			CommonFunc::renderTexture(ButtonText, button3.x, button3.y, 0, 0, &exitButton[1]);
		}
		else if (pressed[2])
		{
			CommonFunc::renderTexture(ButtonText, button3.x, button3.y, 0, 0, &exitButton[2]);
		}
		else
		{
			CommonFunc::renderTexture(ButtonText, button3.x, button3.y, 0, 0, &exitButton[0]);
		}

		if (help_)
		{
			CommonFunc::renderTexture(HelpText, SCREEN_WIDTH / 2 - 432 / 2, SCREEN_HEIGHT / 2 - 272 / 2, 432, 272);
		}
	}
}

void Menu::RenderRetryMenu()
{
	if (selected[3])
	{
		CommonFunc::renderTexture(ButtonText, button4.x, button4.y, 0, 0, &retryButton[1]);
	}
	else if (pressed[3])
	{
		CommonFunc::renderTexture(ButtonText, button4.x, button4.y, 0, 0, &retryButton[2]);
	}
	else
	{
		CommonFunc::renderTexture(ButtonText, button4.x, button4.y, 0, 0, &retryButton[0]);
	}

	if (selected[4])
	{
		CommonFunc::renderTexture(ButtonText, button5.x, button5.y, 0, 0, &homeButton[1]);
	}
	else if (pressed[4])
	{
		CommonFunc::renderTexture(ButtonText, button5.x, button5.y, 0, 0, &homeButton[2]);
	}
	else
	{
		CommonFunc::renderTexture(ButtonText, button5.x, button5.y, 0, 0, &homeButton[0]);
	}
}

void Menu::RenderPauseMenu()
{
	if (paused_)
	{
		//CommonFunc::renderTexture(BgText, 0, 0, 720, 480);
		
		if (selected[3])
		{
			CommonFunc::renderTexture(ButtonText, button4.x, button4.y, 0, 0, &playButton[1]);
		}
		else if (pressed[3])
		{
			CommonFunc::renderTexture(ButtonText, button4.x, button4.y, 0, 0, &playButton[2]);
		}
		else
		{
			CommonFunc::renderTexture(ButtonText, button4.x, button4.y, 0, 0, &playButton[0]);
		}

		if (selected[4])
		{
			CommonFunc::renderTexture(ButtonText, button5.x, button5.y, 0, 0, &homeButton[1]);
		}
		else if (pressed[4])
		{
			CommonFunc::renderTexture(ButtonText, button5.x, button5.y, 0, 0, &homeButton[2]);
		}
		else
		{
			CommonFunc::renderTexture(ButtonText, button5.x, button5.y, 0, 0, &homeButton[0]);
		}
	}
	
}

void Menu::RenderChooseMenu()
{
	if (isChoose())
	{
		CommonFunc::renderTexture(BgText, 0, 0, 720, 480);

		if (selected[5])
		{
			CommonFunc::renderTexture(ButtonText, button6.x, button6.y, 0, 0, &hunterButton[1][hunter_frame / delay_]);
			hunter_frame++;
			if (hunter_frame / delay_ >= HUNTER_ANIMATION_FRAME)
			{
				hunter_frame = 0;
			}
		}
		else if (pressed[5])
		{
			CommonFunc::renderTexture(ButtonText, button6.x, button6.y, 0, 0, &hunterButton[2][hunter_frame / delay_]);
			hunter_frame++;
			if (hunter_frame / delay_ >= HUNTER_ANIMATION_FRAME)
			{
				hunter_frame = 0;
			}
		}
		else
		{
			CommonFunc::renderTexture(ButtonText, button6.x, button6.y, 0, 0, &hunterButton[0][hunter_frame / delay_]);
			hunter_frame++;
			if (hunter_frame / delay_ >= HUNTER_ANIMATION_FRAME)
			{
				hunter_frame = 0;
			}
		}
		if (unlock_)
		{
			if (selected[6])
			{
				CommonFunc::renderTexture(ButtonText, button7.x, button7.y, 0, 0, &YenButton[1][secret_frame / delay_]);
				secret_frame++;
				if (secret_frame / delay_ >= SECRET_ANIMATION_FRAME)
				{
					secret_frame = 0;
				}
			}
			else if (pressed[6])
			{
				//CommonFunc::renderTexture(ButtonText, button7.x, button7.y, 0, 0, &YenButton[2]);
				CommonFunc::renderTexture(ButtonText, button7.x, button7.y, 0, 0, &YenButton[2][secret_frame / delay_]);
				secret_frame++;
				if (secret_frame / delay_ >= SECRET_ANIMATION_FRAME)
				{
					secret_frame = 0;
				}
			}
			else
			{
				CommonFunc::renderTexture(ButtonText, button7.x, button7.y, 0, 0, &YenButton[0][secret_frame / delay_]);
				secret_frame++;
				if (secret_frame / delay_ >= SECRET_ANIMATION_FRAME)
				{
					secret_frame = 0;
				}
			}
		}
		else
		{
			
		}
		
	}
}

void Menu::RenderPlayerNameMenu()
{
	CommonFunc::renderTexture(NameText, 0, 0, 720, 480);
	SDL_Color redColor = { 255,0,0,255 };
	SDL_Texture* textTex = CommonFunc::createText(playerName, redColor);
	Entity text(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2, textTex);
	CommonFunc::renderTexture(text);
	SDL_DestroyTexture(textTex);
	textTex = NULL;
}
/*
void Menu::RenderHelpMenu()
{

}
*/

void Menu::RenderMainGame(Secret& p_secret)
{
	if (!isMenu() && !isName() && !isPasued() && !isChoose())
	{
		health_ = p_secret.getHealth();
	}
	CommonFunc::renderTexture(ButtonText, 0, 0, 0, 0, &HealthBar[health_]);
}