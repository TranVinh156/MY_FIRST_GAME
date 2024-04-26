#ifndef MENU_H_
#define MENU_H_

#pragma once

#include "RenderWindow.h"


class Menu
{
public:
	enum SFX
	{
		select = 0,
		press = 1,
	};
	Menu(SDL_Texture* p_button, SDL_Texture* p_bg, SDL_Texture* p_helpText, SDL_Texture* p_name);

	~Menu() {};

	void HandleInput(SDL_Event event, bool& gameRunning, Player& p_player, Secret& p_secret, Boss& p_boss, Mix_Chunk* p_menuSFX[]);
	bool CheckMouse(const int& p_x, const int& p_y, int p_width = 0, int p_height = 0);
	
	void RenderMainMenu();
	void RenderRetryMenu();
	void RenderChooseMenu();
	void RenderHelpMenu();
	void RenderPauseMenu();
	void RenderPlayerNameMenu();
	bool isName() const { return name_; };
	bool isMenu() const { return menu_; };
	bool isChoose() const { return choose_; };
	bool isPasued() const { return paused_; };
	bool isReset() const { return reset_; };
	bool isSecret() const { return secret_; };
	void setReset(bool p_reset) { reset_ = p_reset; };
	std::string GetPlayerName() const { return playerName; };

private:
	const int BUTTON_WIDTH = 128;
	const int BUTTON_HEIGHT = 128;

	
	static const int TYPE_BUTTON = 3;
	static const int HUNTER_ANIMATION_FRAME = 10;
	static const int SECRET_ANIMATION_FRAME = 6;
	int hunter_frame = 0;
	int secret_frame = 0;
	int delay_ = 150;
	SDL_Rect playButton[TYPE_BUTTON];
	SDL_Rect exitButton[TYPE_BUTTON];
	SDL_Rect helpButton[TYPE_BUTTON];
	SDL_Rect retryButton[TYPE_BUTTON];
	SDL_Rect homeButton[TYPE_BUTTON];
	SDL_Rect soundButton[TYPE_BUTTON];
	SDL_Rect muteButton[TYPE_BUTTON];
	SDL_Rect pauseButton[TYPE_BUTTON];
	SDL_Rect hunterButton[TYPE_BUTTON][HUNTER_ANIMATION_FRAME]; ///
	SDL_Rect YenButton[TYPE_BUTTON][SECRET_ANIMATION_FRAME];
	//SDL_Rect YenButton[TYPE_BUTTON];
	
	// nut main menu
	SDL_Point button1 = { SCREEN_WIDTH / 6 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2 };
	SDL_Point button2 = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2 };
	SDL_Point button3 = { 5 * SCREEN_WIDTH / 6 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2 };

	// nút menu retry

	SDL_Point button4 = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2 };
	SDL_Point button5 = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2 + BUTTON_HEIGHT + 8 };

	// nút menu chọn nhân vật
	SDL_Point button6 = { SCREEN_WIDTH / 4 - BUTTON_WIDTH, SCREEN_HEIGHT / 2 - BUTTON_HEIGHT + 20 };
	SDL_Point button7 = { 3 * SCREEN_WIDTH / 4 - BUTTON_WIDTH, SCREEN_HEIGHT / 2 - BUTTON_HEIGHT + 20 };

	bool menu_ = false;
	bool choose_ = false;
	bool retry_ = false;
	bool paused_ = false;
	bool help_ = false;
	bool reset_ = false;
	bool name_ = true;
	bool unlock_ = false;
	int full_screen_ = 1;
	std::string playerName = "";
	bool secret_ = false;
	SDL_Texture* ButtonText = NULL;
	SDL_Texture* BgText = NULL;
	SDL_Texture* HelpText = NULL;
	SDL_Texture* NameText = NULL;
	bool selected[7] = { false }; // được chọn
	bool pressed[7] = { false }; 
	bool secretCode_[16] = { false };
	bool sfx_[7] = { false };
};



#endif

