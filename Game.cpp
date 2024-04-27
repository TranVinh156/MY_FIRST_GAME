#include "Game.h"

bool Game::Init()
{
	bool success = true;
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "INIT ERROR" << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		if (!CommonFunc::renderWindow("GATE", SCREEN_WIDTH, SCREEN_HEIGHT))
		{
			success = false;
		}
	}
	if (TTF_Init() < 0)
	{
		std::cout << "TTF INIT ERROR" << std::endl;
		success = false;

	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		success = false;
	}
	return success;
}

void Game::SetRect()
{
	int m = 0;
	int n = 0;
	for (int i = 0; i < TOTAL_TILE_SPRITE; i++)
	{
		//TileClip[i] = { n, m, TILE_WIDTH, TILE_HEIGHT };

		TileClip[i].x = n;
		TileClip[i].y = m;
		TileClip[i].w = TILE_WIDTH;
		TileClip[i].h = TILE_HEIGHT;
		
		n += TILE_WIDTH;
		if (n >= TILE_WIDTH * 15)
		{
			n = 0;
			m += TILE_HEIGHT;
		}
	}
}

bool Game::LoadMedia()
{
	bool success = true;
	
	if (!CommonFunc::loadFont("ttf/Pixel-UniCode.ttf"))
	{
		success = false;
	}
	
	HunterText = CommonFunc::loadTexture("img/Knight.png");
	if (HunterText == NULL) success = false;
	SecretText = CommonFunc::loadTexture("img/Yen.png");
	if (SecretText == NULL) success = false;

	UndeadText = CommonFunc::loadTexture("img/Skeleton.png");
	if (UndeadText == NULL) success = false;
	/* map cu
	TileSetText = CommonFunc::loadTexture("img/map/TileSet.png");
	if (TileSetText == NULL) success = false;
	*/ 
	TileSetText = CommonFunc::loadTexture("img/map/newTile.png");
	if (TileSetText == NULL) success = false;
	//return success;
	BgText = CommonFunc::loadTexture("img/map/GATE.png");
	if (BgText == NULL) success = false;
	ButtonText = CommonFunc::loadTexture("img/map/newButton.png");
	if (ButtonText == NULL) success = false;
	HelpText = CommonFunc::loadTexture("img/map/helpMenu.png");
	if (HelpText == NULL) success = false;
	NameText = CommonFunc::loadTexture("img/map/NameMenu.png");
	if (NameText == NULL) success = false;
	DemonText = CommonFunc::loadTexture("img/DemonKing.png");
	if (DemonText == NULL) success = false;
	BulletText = CommonFunc::loadTexture("img/Bullet.png");
	if (BulletText == NULL) success = false;
	
	
	bgMusic = Mix_LoadMUS("sfx/xDeviruchi - Prepare for Battle! .wav");
	if (bgMusic == NULL)
	{
		std::cout << Mix_GetError() << std::endl;
		success = false;
	}
	
	playerSFX[0] = Mix_LoadWAV("sfx/Player/step1.wav");
	playerSFX[1] = Mix_LoadWAV("sfx/Player/Jump-1.wav");
	playerSFX[2] = Mix_LoadWAV("sfx/Player/land.wav");
	playerSFX[3] = Mix_LoadWAV("sfx/Player/Battle_Grunt16.wav");
	playerSFX[4] = Mix_LoadWAV("sfx/Player/Death-Scream-1.wav");
	for (int i = 0; i < 4; i++)
	{
		if (playerSFX[i] == NULL) success = false;
	}

	secretSFX[0] = Mix_LoadWAV("sfx/Secret/step1.wav");
	secretSFX[1] = Mix_LoadWAV("sfx/Secret/jump1.wav");
	secretSFX[2] = Mix_LoadWAV("sfx/Secret/land.wav");
	secretSFX[3] = Mix_LoadWAV("sfx/Secret/attack3.wav");
	secretSFX[4] = Mix_LoadWAV("sfx/Secret/death_10_meghan.wav");
	secretSFX[5] = Mix_LoadWAV("sfx/Secret/39_Absorb_04.wav");
	for (int i = 0; i < 5; i++)
	{
		if (secretSFX[i] == NULL) success = false;
	}

	skeletonSFX[0] = Mix_LoadWAV("sfx/Enemy/56_Attack_03.wav");
	skeletonSFX[1] = Mix_LoadWAV("sfx/Enemy/39_Block_03.wav");
	skeletonSFX[2] = Mix_LoadWAV("sfx/Enemy/die_monster.wav");
	skeletonSFX[3] = Mix_LoadWAV("sfx/Enemy/61_Hit_03.wav");
	for (int i = 0; i < 4; i++)
	{
		if (skeletonSFX[i] == NULL) success = false;
	}

	bossSFX[0] = Mix_LoadWAV("sfx/Boss/axe_boss.wav");
	bossSFX[1] = Mix_LoadWAV("sfx/Boss/39_Block_03.wav");
	bossSFX[2] = Mix_LoadWAV("sfx/Boss/04_Fire_explosion_04_medium.wav");
	bossSFX[3] = Mix_LoadWAV("sfx/Boss/hurt_monster.wav");
	bossSFX[4] = Mix_LoadWAV("sfx/Boss/die_boss.wav");

	for (int i = 0; i < 5; i++)
	{
		if (bossSFX[i] == NULL) success = false;
	}

	menuSFX[0] = Mix_LoadWAV("sfx/menu/select.wav");
	menuSFX[1] = Mix_LoadWAV("sfx/menu/press.wav");

	for (int i = 0; i < 2; i++)
	{
		if (menuSFX[i] == NULL) success = false;
	}

	return success;
	// còn load âm thanh menu Texture;
}

bool Game::CreateMap()
{
	path_pos level1("img/map/maplevel1.tmx", {16433, 16442, 16449, 9107, 12509, 12520, 8561, 8575, 16467, 16479, 16490, 11435, 
		18227, 22191, 29530, 29546, 26165, 22783, 22799, 28445, 28454, 22811, 30160, 
		30172, 30183, 22836, 26238, 22853, 30202, 23273, 27238, 30627, 34576, 34588, 
		34602, 23259, 23245, 26626, 28905, 28916, 34561, 34553, 38511, 46415, 43581, 
		43571, 43561, 39609, 36224, 50896, 50882, 50866, 43530, 58770, 12497, 34088, 
		34099, 26224, 25581, 46942, 50363, 50376, 50386, 50396, 46433, 46441, 42475, 
		42492, 54327, 61667, 57717});
	mapList.push_back(level1);
	
	path_pos level2("img/map/newMap.tmx", { 28860, 28871, 23777, 35121, 38506, 42460, 42472, 46430, 46406, 53754, 53766, 53778
		, 49819, 30076, 30086, 18773, 18788, 38030, 41994, 49338, 45384, 53295, 53271
		, 60620, 60630, 60642, 56684, 19987, 19996, 23953, 27903, 21168, 23439, 23449
		, 17222, 16676, 27443, 30829, 34792, 38733, 42697, 50038, 46087, 20693, 16738
		, 12783, 15628, 20148, 20735, 24694, 20746, 16783, 19635, 15686, 14569, 11727
		, 19088, 25345, 25322, 32687, 28734, 36636, 40022, 43978 });
	mapList.push_back(level2);
	
	if (mapList.size() < 2)
	{
		return false;
	}
	return true;
}


void Game::PlayMusic()
{
	if (Mix_PlayingMusic() == 0) {
		Mix_FadeInMusic(bgMusic, -1, 1000);
		Mix_VolumeMusic(20);
	}
	else if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
	else if (playerList.at(0).getDead()) Mix_HaltMusic();
}

bool Game::CreateMenu()
{
	Menu MenuGame(ButtonText, BgText, HelpText, NameText);
	menuList.push_back(MenuGame);
	if (menuList.size() < 0)
	{
		return false;
	}
	return true;
}

bool Game::CreateLevel()
{

	/*
	for (int i = 0; i < TOTAL_MAP; i++)
	{
		Game_Map level(i * LEVEL_WIDTH, 0, mapList.at(i).path, TileSetText);
		level.SetSkeletonPos(mapList.at(i).skeleton_pos);
		levelList.push_back(level);
	}
	if (levelList.size() < TOTAL_LEVEL)
	{
		return false;
	}
	*/
	for (int i = 0; i < TOTAL_MAP; i++)
	{
		Game_Map level(0, 0, mapList.at(i).path, TileSetText);
		level.SetSkeletonPos(mapList.at(i).skeleton_pos);
		levelList.push_back(level);
	}
	
	if (levelList.size() < TOTAL_MAP)
	{
		return false;
	}
	return true;
}

bool Game::CreateBoss()
{
	/*
	Boss DemonKing(levelList.at(9).getX() + 16 * 20, 256, DemonText);
	//Boss DemonKing(16 * 20, 256, DemonText);
	BossList.push_back(DemonKing);

	Goblin GoblinKing(16 * 20, 304, GoblinText);
	GoblinList.push_back(GoblinKing);

	if (BossList.size() < 0 || GoblinList.size() < 0)
	{
		return false;
	}
	*/
	Boss DemonKing(TILE_WIDTH * (20331 % 565) - 112, TILE_WIDTH * (20331 / 565) - 80 - 80, DemonText);
	BossList.push_back(DemonKing);

	if (BossList.size() < 0)
	{
		return false;
	}
	return true;
}

bool Game::CreatePlayer()
{
	Player Hunter(16 * 16, 21 * 16, HunterText);
	//Player Hunter(LEVEL_WIDTH - SCREEN_WIDTH, 96, HunterText);
	playerList.push_back(Hunter);
	if (playerList.size() < 0)
	{
		return false;
	}
	return true;
}

bool Game::CreateSecret()
{
	Secret Yen(16 * 16, 20 * 16, SecretText);
	//Secret Yen(LEVEL_WIDTH - SCREEN_WIDTH, 96, SecretText);
	secretList.push_back(Yen);
	if (secretList.size() < 0)
	{
		return false;
	}
	return true;
}

bool Game::CreateUnDead()
{	
	
		if (levelList.at(levelSTT).GetSkeletonPos().size() > 0)
		{
			for (int j = 0; j < levelList.at(levelSTT).GetSkeletonPos().size(); j++)
			{
				Skeleton* undead = new Skeleton(levelList.at(levelSTT).getX() + (levelList.at(levelSTT).GetSkeletonPos().at(j) % 565) * TILE_WIDTH - 32, levelList.at(levelSTT).getY() + (levelList.at(levelSTT).GetSkeletonPos().at(j) / 565) * TILE_HEIGHT - 48, UndeadText);
				undead->SetLevelSTT(levelSTT);
				skeletonList.push_back(undead);
			}
		}
	
	
	return true;
}

void Game::HandleGameInput(SDL_Event& event)
{
	if (event.type == SDL_QUIT) gameRunning = false;
	menuList.at(0).HandleInput(event, gameRunning, playerList.at(0), secretList.at(0), BossList.at(0), menuSFX);
	if (!menuList.at(0).isMenu() && !menuList.at(0).isPasued() && !menuList.at(0).isChoose() && !menuList.at(0).isName())
	{
		playerName = menuList.at(0).GetPlayerName();
		secret_ = menuList.at(0).isSecret();
		if (!secret_)
		{
			playerList.at(0).HandleInput(event, playerSFX);
		}
		else
		{
			secretList.at(0).HandleInput(event, secretSFX);
		}
		
	}
	/*
	if (event.type == SDL_TEXTINPUT) {
		// Nếu có sự kiện nhập liệu
		playerName += event.text.text;
	}
	else if (event.type == SDL_KEYDOWN) {
		// Xoá ký tự nếu nhấn phím Backspace
		if (event.key.keysym.sym == SDLK_BACKSPACE && playerName.length() > 0) {
			playerName.pop_back();
		}
		// Kết thúc nhập khi nhấn phím Enter
		else if (event.key.keysym.sym == SDLK_RETURN) {
			//quit = true;
		}
	}
	*/
}
void Game::FPSCounter()
{
	avgFPS = countedFrames / (fps.getTicks() / 1000.f);
	/*
	if (avgFPS > 45) {
		avgFPS = 45;
	}
	*/
	timeText.str("");
	timeText << "FPS: " << avgFPS;

	SDL_Color whiteColor = { 255,255,255,255 };
	SDL_Texture* textTex = CommonFunc::createText(timeText.str().c_str(), whiteColor);
	Entity text(64, 0, textTex);
	CommonFunc::renderTexture(text);
	++countedFrames;
	SDL_DestroyTexture(textTex);
	textTex = NULL;
}

void Game::skeCounter()
{
	skeText.str("");
	skeText << "SKE: " << skeletonCount;
	SDL_Color whiteColor = { 255, 255, 255, 255 };
	SDL_Color redColor = { 255, 0, 0, 255 };
	SDL_Texture* textTex = CommonFunc::createText(skeText.str().c_str(), whiteColor);
	if (skeletonCount >= 50)
	{
		textTex = CommonFunc::createText(skeText.str().c_str(), redColor);
	}
	Entity text(64 * 4, 0, textTex);
	CommonFunc::renderTexture(text);
	SDL_DestroyTexture(textTex);
	textTex = NULL;
}

void Game::RenderName()
{
	SDL_Color whiteColor = { 255,255,255,255 };
	SDL_Texture* textTex = CommonFunc::createText(playerName, whiteColor);
	
	Entity text(SCREEN_WIDTH - 256, 0, textTex);
	CommonFunc::renderTexture(text);
	
	
	SDL_DestroyTexture(textTex);
	textTex = NULL;
}

void Game::RenderPlayer()
{
	playerList.at(0).Update(skeletonList, levelList, BossList.at(0), playerSFX);
	playerList.at(0).HandleCamera(camera, levelSTT, nextlevel_, skeletonCount);
	playerList.at(0).Render(camera, playerSFX);
}

void Game::RenderSecret()
{
	secretList.at(0).Update(skeletonList, levelList, BossList.at(0), secretSFX);
	secretList.at(0).HandleCamera(camera, levelSTT, nextlevel_, skeletonCount);
	secretList.at(0).Render(camera, secretSFX);
}

void Game::RenderSkeleton()
{
	for (int i = 0; i < skeletonList.size(); i++)
	{
		if (skeletonList.at(i) != NULL)
		{
			if (!skeletonList.at(i)->isDead())
			{
				if (skeletonList.at(i)->SetLoaded(playerList.at(0), secretList.at(0), secret_))
				{
					skeletonList.at(i)->Update(playerList.at(0), secretList.at(0), levelList, secret_, skeletonSFX);
					skeletonList.at(i)->render(camera, skeletonSFX);
				}
				
			}
			else
			{
				skeletonCount++;
				delete skeletonList.at(i);
				skeletonList.at(i) = NULL;
				//skeletonList.erase(skeletonList.begin() + i);
			}
			
		}
		
	}
	
}

void Game::RenderBullet()
{
	std::vector<Bullet*> bulletList = BossList.at(0).GetBulletList();
	for (int i = 0; i < BossList.at(0).GetBulletList().size(); i++)
	{
		if (bulletList.at(i) != NULL)
		{
			if (bulletList.at(i)->isMoving())
			{
				bulletList.at(i)->Render(camera, BulletText);
				bulletList.at(i)->Update(levelList);
			}
			else
			{
				delete bulletList.at(i);
				bulletList.at(i) = NULL;
				bulletList.erase(bulletList.begin() + i);
				BossList.at(0).SetBulletList(bulletList);
			}
		}
		
	}
}

void Game::RenderMap()
{
	levelList.at(levelSTT).render(TileClip, &camera);
}

void Game::RenderBoss()
{
	BossList.at(0).Update(playerList.at(0), secretList.at(0), levelList, secret_, bossSFX);
	BossList.at(0).Render(camera, bossSFX);
}

void Game::Render_MainMenu()
{
	CommonFunc::clearRenderer();
	menuList.at(0).RenderMainMenu();
	CommonFunc::renderPresent();
}

void Game::Render_ChooseMenu()
{
	CommonFunc::clearRenderer();
	menuList.at(0).RenderChooseMenu();
	//secret_ = menuList.at(0).isSecret();
	CommonFunc::renderPresent();
}

void Game::Render_PauseMenu()
{
	//CommonFunc::clearRenderer();
	menuList.at(0).RenderPauseMenu();
	CommonFunc::renderPresent();
}

void Game::Render_PlayerNameMenu()
{
	CommonFunc::clearRenderer();
	menuList.at(0).RenderPlayerNameMenu();
	CommonFunc::renderPresent();
}

void Game::RenderGame()
{
	fps.start();
	frameStart = fps.getTicks();
	CommonFunc::clearRenderer();
	if (nextlevel_)
	{
		GoToNextLevel();
	}
	RenderMap();
	RenderSkeleton();
	RenderBullet();
	
	FPSCounter();
	skeCounter();
	if (levelSTT == 1) RenderBoss();
	if (!secret_)
	{
		RenderPlayer();
		RenderName();
	}
	else
	{
		RenderSecret();
		RenderName();
	}
	
	if (playerList.at(0).getDead() || secretList.at(0).getDead() || BossList.at(0).GetDead())
	{
		menuList.at(0).RenderRetryMenu();
	}
	if (menuList.at(0).isReset())
	{
		ResetGame();
	}
	
	fps.unpause();
	CommonFunc::renderPresent();
	
	int real_imp_time = fps.getTicks() - frameStart;
	//std::cout << real_imp_time << std::endl;
	int time_one_frame = 1000 / FRAME_PER_SECOND;
	if (real_imp_time < FRAME_PER_SECOND)
	{
		int delay_time = time_one_frame - real_imp_time;
		//avgFPS = 45;
		if (delay_time >= 0) SDL_Delay(delay_time);
	}
}



void Game::CleanGame()
{
	SDL_DestroyTexture(HunterText);
	SDL_DestroyTexture(UndeadText);
	SDL_DestroyTexture(TileSetText);
	SDL_DestroyTexture(BgText);
	SDL_DestroyTexture(DemonText);
	SDL_DestroyTexture(ButtonText);
	SDL_DestroyTexture(BulletText);
	SDL_DestroyTexture(SecretText);
	SDL_DestroyTexture(HelpText);
	SDL_DestroyTexture(NameText);
	
	Mix_FreeMusic(bgMusic);
	bgMusic = NULL;

	for (int i = 0; i < 5; i++)
	{
		Mix_FreeChunk(playerSFX[i]);
		playerSFX[i] = NULL;
	}
	for (int i = 0; i < 6; i++)
	{
		Mix_FreeChunk(secretSFX[i]);
		secretSFX[i] = NULL;
	}
	for (int i = 0; i < 4; i++)
	{
		Mix_FreeChunk(skeletonSFX[i]);
		skeletonSFX[i] = NULL;
	}
	for (int i = 0; i < 5; i++)
	{
		Mix_FreeChunk(bossSFX[i]);
		bossSFX[i] = NULL;
	}
	for (int i = 0; i < 2; i++)
	{
		Mix_FreeChunk(menuSFX[i]);
		menuSFX[i] = NULL;
	}
	if (!skeletonList.empty())
	{
		for (int i = skeletonList.size() - 1; i >= 0; i--) {
			delete skeletonList.at(i);
			skeletonList.at(i) = NULL;
			skeletonList.erase(skeletonList.begin() + i);
		}
	}

	

	SecretText = NULL;
	BulletText = NULL;
	BgText = NULL;
	DemonText = NULL;
	HelpText = NULL;
	NameText = NULL;
	ButtonText = NULL;
	HunterText = NULL;
	UndeadText = NULL;
	TileSetText = NULL;
}

void Game::ResetGame()
{
	playerList.at(0).ResetPlayer();
	secretList.at(0).ResetSecret();
	camera.x = 0;
	camera.y = 0;
	levelSTT = 0;
	skeletonCount = 0;


	if (!skeletonList.empty())
	{
		for (int i = skeletonList.size() - 1; i >= 0; i--) {
			delete skeletonList.at(i);
			skeletonList.at(i) = NULL;
			skeletonList.erase(skeletonList.begin() + i);
		}
	}

	BossList.at(0).ResetBoss();
	for (int i = levelList.size() - 1; i >=0; i--)
	{
		levelList.erase(levelList.begin() + i);
	}
	CreateLevel();
	CreateUnDead();
	//CreateBoss();
	fps.stop();
	fps.start();
	frameStart = fps.getTicks();
	countedFrames = 0;
	menuList.at(0).setReset(false);
}

void Game::GoToNextLevel()
{
	levelSTT = 1;
	if (!skeletonList.empty())
	{
		for (int i = skeletonList.size() - 1; i >= 0; i--) {
			delete skeletonList.at(i);
			skeletonList.at(i) = NULL;
			skeletonList.erase(skeletonList.begin() + i);
		}
	}
	nextlevel_ = false;
	CreateUnDead();
	//CreateBoss();
}