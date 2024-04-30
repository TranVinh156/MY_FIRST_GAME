#ifndef GAME_H_
#define GAME_H_

#pragma once
#include "RenderWindow.h"
#include "SDL_ttf.h"
#include "Menu.h"
#include "Timer.h"

//class Skeleton;

struct path_pos
{
	const char* path;
	std::vector<int> undead_pos;
	path_pos(const char* p_path, std::vector<int> p_undead_pos)
	{
		path = p_path;
		undead_pos = p_undead_pos;
	}
};

class Game
{
public:
	Game() { SetRect(); };
	~Game() {};
	bool Init();

	void SetRect();

	bool LoadMedia();
	bool CreateMap();

	bool CreateLevel();
	bool CreatePlayer();
	bool CreateSecret();
	bool CreateUnDead();
	bool CreateBoss();
	bool CreateMenu();
	
	void HandleGameInput(SDL_Event& event);
	
	void FPSCounter();
	void killCounter();
	void RenderName();

	void RenderGame();

	void RenderBoss();
	void RenderPlayer();
	void RenderSecret();
	void RenderUndead();
	void RenderMap();
	void Render_MainMenu();
	void Render_ChooseMenu();
	void Render_PauseMenu();
	void Render_PlayerNameMenu();
	void RenderBullet();

	void PlayMusic();
	void CleanGame();
	void GoToNextLevel();
	void ResetGame();
	bool isRunning() { return gameRunning; };
	std::vector<Menu> GetMenuList() { return menuList; };
	
private:
	SDL_Texture* HunterText = NULL;
	SDL_Texture* SecretText = NULL;
	SDL_Texture* UndeadText = NULL;
	SDL_Texture* ArcherText = NULL;
	SDL_Texture* TileSetText = NULL;
	SDL_Texture* DemonText = NULL;
	SDL_Texture* BgText = NULL;
	SDL_Texture* HelpText = NULL;
	SDL_Texture* NameText = NULL;
	SDL_Texture* ButtonText = NULL;
	SDL_Texture* BulletText = NULL;
	SDL_Texture* ArrowText = NULL;
	
	Mix_Music* bgMusic = NULL;
	Mix_Chunk* bossMusic = NULL;
	Mix_Chunk* playerSFX[5] = { NULL };
	Mix_Chunk* secretSFX[6] = { NULL };
	Mix_Chunk* undeadSFX[4] = { NULL };
	Mix_Chunk* bossSFX[5] = { NULL };
	Mix_Chunk* menuSFX[2] = { NULL };
	ImpTimer fps;
	std::stringstream timeText;
	std::stringstream killText;
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect TileClip[TOTAL_TILE_SPRITE];

	std::vector <path_pos> mapList;
	std::vector <Player> playerList;
	std::vector <Secret> secretList;
	std::vector <Undead*> undeadList;
	std::vector <Archer*> archerList;
	std::vector <Game_Map> levelList;
	std::vector <Menu> menuList;
	std::vector <Boss> BossList;
	//std::vector <Bullet*> bulletList;
	
	
	int levelSTT = 0;
	bool gameRunning = true;
	bool nextlevel_ = false;
	bool secret_ = true;
	bool fullScreen_ = false;
	int undeadCount = 0;
	int countedFrames = 0;
	int avgFPS = 0;
	int frameStart;
	std::string playerName = "";
};



#endif // !GAME_H_

