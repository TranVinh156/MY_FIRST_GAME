#ifndef RENDER_WINDOW_H_
#define RENDER_WINDOW_H_

#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>
#include <string>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <cmath>

#include "Entity.h"
#include "Gamemap.h"
#include "Tile.h"
#include "Skeleton.h"
#include "BOSS.h"
#include "Player.h"
#include "Bullet.h"
#include "Secret.h"

class Game_Map;

const int FRAME_PER_SECOND = 35;

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 480;

const int LEVEL_WIDTH = 9040;
const int LEVEL_HEIGHT = 1792;

static TTF_Font* g_font = NULL;
static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 170;
const int COLOR_KEY_B = 180;
const int DRAW_COLOR_KEY = 0xff;


const float GRAVITY = 0.3;
const float MAX_GRAVITY = 10;

const int TILE_WIDTH = 16;
const int TILE_HEIGHT = 16;
const int TOTAL_TILE = 63280;
const int TOTAL_TILE_SPRITE = 270;
const int TOTAL_MAP = 2;
const int TOTAL_LEVEL = 2;




namespace CommonFunc
{
	bool renderWindow(const char* pTitle, int nWidth, int nHeight);
	void fullScreen(int p_full_screen);
	//texture
	SDL_Texture* loadTexture(std::string path);
	void renderTexture(Entity& entity, SDL_Rect* rec = NULL, SDL_Rect* camera = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderTexture(SDL_Texture* p_text, float p_x, float p_y, int p_w = 0, int p_h = 0, SDL_Rect* rec = NULL, SDL_Rect* camera = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	void renderAnimation(SDL_Texture* p_text, float p_x, float p_y, SDL_Rect* clip, SDL_Rect* camera, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderTile(Entity& entity, SDL_Rect* rec, SDL_Rect* camera);

	bool checkCollision(SDL_Rect a, SDL_Rect b);

	bool checkToMap(SDL_Rect& box, std::vector<Game_Map>& levelList, int& levelSTT);
	bool checkToMap(SDL_Rect& box, std::vector<Game_Map>& levelList, bool& on_ground, int& groundSTT, int& levelSTT);

	bool loadFont(std::string p_path);
	SDL_Texture* createText(std::string p_text, SDL_Color p_textColor);
	void clearRenderer();
	void renderPresent();
	void cleanUp();
};


#endif // !RENDER_WINDOW_H_

