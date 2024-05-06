#include "RenderWindow.h"


bool CommonFunc::renderWindow(const char* pTitle, int nWidth, int nHeight)
{
	bool success = true;
	g_window = SDL_CreateWindow(pTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nWidth, nHeight, SDL_WINDOW_SHOWN);
	if (g_window == NULL)
	{
		success = false;
		std::cout << "CREATE WINDOW ERROR" << SDL_GetError() << std::endl;
	}
	else
	{
		
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		
		
		if (g_screen == NULL)
		{
			success = false;
			std::cout << "CREATE RENDERER ERROR" << SDL_GetError() << std::endl;
		}
		else
		{
			// mới
			SDL_RenderSetLogicalSize(g_screen, 720, 480);
			SDL_SetRenderDrawColor(g_screen, DRAW_COLOR_KEY, DRAW_COLOR_KEY, DRAW_COLOR_KEY, DRAW_COLOR_KEY);
			int imgFlag = IMG_INIT_PNG;
			if (!IMG_Init(imgFlag) & imgFlag)
			{
				success = false;
				std::cout << "IMG ERROR" << IMG_GetError() << std::endl; // mixer ttf
			}
		}
	}
	return success;
}

void CommonFunc::fullScreen(int nFullScreen)
{
	if (nFullScreen == 2)
	{
		SDL_SetWindowSize(g_window, 1440, 960);
		SDL_SetWindowPosition(g_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	else if (nFullScreen == 1)
	{
		SDL_SetWindowSize(g_window, 720, 480);
		SDL_SetWindowPosition(g_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	
}

SDL_Texture* CommonFunc::loadTexture(std::string path)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(g_screen, path.c_str());
	if (texture == NULL)
	{
		std::cout << "TEXTURE LOAD ERROR" << IMG_GetError() << std::endl;
	}
	return texture;

	//SDL_Surface* load_img = IMG_Load
}

bool CommonFunc::loadFont(std::string p_path)
{
	bool success = true;
	TTF_CloseFont(g_font);
	g_font = TTF_OpenFont(p_path.c_str(), 28);
	if (g_font == NULL)
	{
		std::cout << "LOAD FONT ERROR" << TTF_GetError() << std::endl;
		success = false;
	}
	return success;
}

SDL_Texture* CommonFunc::createText(std::string p_text, SDL_Color p_textColor)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(g_font, p_text.c_str(), p_textColor);
	SDL_Texture* texture = NULL;
	if (textSurface == NULL) {
		//std::cout << "TEXT SURFACE ERROR" << TTF_GetError() << std::endl;
	}
	else {
		//Tạo texture text từ surface
		texture = SDL_CreateTextureFromSurface(g_screen, textSurface);
		if (texture == NULL) {
			//std::cout << "TEXT FROM SURFACE ERROR" << TTF_GetError() << std::endl;
		}
		SDL_FreeSurface(textSurface);
		return texture;
	}
	return NULL;
}

void CommonFunc::renderTexture(Entity& entity, SDL_Rect* rec, SDL_Rect* camera, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect dst = { entity.getX(), entity.getY(), entity.getRect().w, entity.getRect().h };


	if (rec != NULL) {
		dst.w = rec->w;
		dst.h = rec->h;
	}
	if (camera != NULL) {
		dst.x = dst.x - camera->x;
		dst.y = dst.y - camera->y;
	}
	SDL_RenderCopyEx(g_screen, entity.getText(), rec, &dst, angle, center, flip);
}

void CommonFunc::renderTexture(SDL_Texture* p_text, float p_x, float p_y, int p_w, int p_h, SDL_Rect* rec, SDL_Rect* camera, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect dst = { p_x, p_y, p_w, p_h };


	if (rec != NULL) {
		dst.w = rec->w;
		dst.h = rec->h;
	}
	if (camera != NULL) {
		dst.x = dst.x - camera->x;
		dst.y = dst.y - camera->y;
	}
	SDL_RenderCopyEx(g_screen, p_text, rec, &dst, angle, center, flip);
}

void CommonFunc::renderAnimation(SDL_Texture* p_text, float p_x, float p_y, SDL_Rect* clip, SDL_Rect* camera, double angle, SDL_Point* center, SDL_RendererFlip flip) 
{
	//SDL_Rect src = { clip->x, clip->y, clip->w, clip->h };

	// lẽ ra còn rect!=NULL nhưng mà lười hehe :)))
	SDL_Rect dst = { p_x , p_y, clip->w, clip->h };
	if (camera != NULL)
	{
		dst.x = p_x - camera->x;
		dst.y = p_y - camera->y;
	}
	//if ()
	
	SDL_RenderCopyEx(g_screen, p_text, clip, &dst, angle, center, flip);
}

void CommonFunc::renderTile(Entity& entity, SDL_Rect* rec, SDL_Rect* camera) {
	SDL_Rect dst = { entity.getX() - camera->x, entity.getY() - camera->y, rec->w, rec->h };
	SDL_RenderCopy(g_screen, entity.getText(), rec, &dst);
}

void CommonFunc::clearRenderer()
{
	SDL_SetRenderDrawColor(g_screen, DRAW_COLOR_KEY, DRAW_COLOR_KEY, DRAW_COLOR_KEY, DRAW_COLOR_KEY);
	SDL_RenderClear(g_screen);
}

void CommonFunc::renderPresent()
{
	SDL_RenderPresent(g_screen);
}

void CommonFunc::cleanUp()
{
	SDL_DestroyWindow(g_window);
	SDL_DestroyRenderer(g_screen);
	g_window = NULL;
	g_screen = NULL;
	SDL_Quit();
	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
}

bool CommonFunc::checkCollision(SDL_Rect a, SDL_Rect b)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	if (bottomA <= topB) {
		return false;
	}

	if (topA >= bottomB) {
		return false;
	}

	if (rightA <= leftB) {
		return false;
	}

	if (leftA >= rightB) {
		return false;
	}

	return true;
}

bool CommonFunc::checkToMap(SDL_Rect& box, std::vector<Game_Map>& levelList, int& levelSTT)
{
	bool check = false;
	
	if (box.h >= 40) // boss
	{
			if (box.x >= levelList.at(levelSTT).getX() && box.x + box.w < levelList.at(levelSTT).getX() + LEVEL_WIDTH && box.y >= 0 && box.y < LEVEL_HEIGHT - TILE_HEIGHT)
			{

				int cot_left = (box.x - levelList.at(levelSTT).getX()) / TILE_WIDTH;
				int cot_right = cot_left + 4;
				int dong_up = box.y / TILE_HEIGHT;
				int dong_mid = dong_up + 2;
				int dong_down = dong_up + 5;

				int stt1 = dong_up * 565 + cot_left;
				int stt2 = dong_up * 565 + cot_right;
				int stt3 = dong_down * 565 + cot_left;
				int stt4 = dong_down * 565 + cot_right;
				int stt5 = dong_mid * 565 + cot_left;
				int stt6 = dong_mid * 565 + cot_right;

				if ((levelList.at(levelSTT).getTilesList().at(stt1)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt1)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt1)->getCollision()))
					{
						check = true;
					}
				}

				if ((levelList.at(levelSTT).getTilesList().at(stt3)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt3)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt3)->getCollision()))
					{
						check = true;
					}
				}
				if ((levelList.at(levelSTT).getTilesList().at(stt2)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt2)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt2)->getCollision()))
					{
						check = true;
					}
				}
				if ((levelList.at(levelSTT).getTilesList().at(stt4)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt4)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt4)->getCollision()))
					{
						check = true;
					}
				}
				if ((levelList.at(levelSTT).getTilesList().at(stt5)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt5)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt5)->getCollision()))
					{
						check = true;
					}
				}
				if ((levelList.at(levelSTT).getTilesList().at(stt6)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt6)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt6)->getCollision()))
					{
						check = true;
					}
				}
			}

	}
	else if (box.h >= 20) // player
	{
			if (box.x >= levelList.at(levelSTT).getX() && box.x + box.w < levelList.at(levelSTT).getX() + LEVEL_WIDTH && box.y >= 0 && box.y < LEVEL_HEIGHT - TILE_HEIGHT)
			{

				int cot_left = (box.x - levelList.at(levelSTT).getX()) / TILE_WIDTH;
				int cot_right = cot_left + 1;
				int dong_up = box.y / TILE_HEIGHT;
				int dong_mid = dong_up + 2;
				int dong_down = dong_up + 1;

				int stt1 = dong_up * 565 + cot_left;
				int stt2 = dong_up * 565 + cot_right;
				int stt3 = dong_down * 565 + cot_left;
				int stt4 = dong_down * 565 + cot_right;
				int stt5 = dong_mid * 565 + cot_left;
				int stt6 = dong_mid * 565 + cot_right;

				if ((levelList.at(levelSTT).getTilesList().at(stt1)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt1)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt1)->getCollision()))
					{
						check = true;
					}
				}

				if ((levelList.at(levelSTT).getTilesList().at(stt3)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt3)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt3)->getCollision()))
					{
						check = true;
					}
				}
				if ((levelList.at(levelSTT).getTilesList().at(stt2)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt2)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt2)->getCollision()))
					{
						check = true;
					}
				}
				if ((levelList.at(levelSTT).getTilesList().at(stt4)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt4)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt4)->getCollision()))
					{
						check = true;
					}
				}
				if ((levelList.at(levelSTT).getTilesList().at(stt5)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt5)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt5)->getCollision()))
					{
						check = true;
					}
				}
				if ((levelList.at(levelSTT).getTilesList().at(stt6)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt6)->getType() <= 59))
				{
					if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt6)->getCollision()))
					{
						check = true;
					}
				}
			}
	}
	return check;
}

bool CommonFunc::checkToMap(SDL_Rect& box, std::vector<Game_Map>& levelList, bool& on_ground, int& groundSTT, int& levelSTT)
{
	bool check = false;
	if (box.h >= 60) // boss
	{
			if (box.x >= levelList.at(levelSTT).getX() && box.x-12 <= levelList.at(levelSTT).getX() + LEVEL_WIDTH && box.y >= 0 && box.y + 16 < LEVEL_HEIGHT) 
			{

				// phải tính lại
				int cot_left = (box.x - levelList.at(levelSTT).getX()) / TILE_WIDTH;
				int cot_right = cot_left + 4;
				int dong_up = (box.y) / TILE_HEIGHT;
				int dong_down = dong_up + 5;


				int stt1 = dong_up * 565 + cot_left;
				int stt3 = dong_up * 565 + cot_right;
				int stt4 = dong_down * 565 + cot_left;
				int stt2 = dong_down * 565 + cot_right;

			
				if ((box.x < levelList.at(levelSTT).getX()/* && box.x + box.w > levelList.at(levelSTT).getX()*/) || (/*box.x < levelList.at(levelSTT).getX() + LEVEL_WIDTH &&*/ box.x > levelList.at(levelSTT).getX() + LEVEL_WIDTH)) {
					on_ground = false;
				}
				else {
					if ((levelList.at(levelSTT).getTilesList().at(stt1)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt1)->getType() <= 59))
						if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt1)->getCollision())) check = true;
					if ((levelList.at(levelSTT).getTilesList().at(stt2)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt2)->getType() <= 90))
						if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt2)->getCollision())) check = true;
					if ((levelList.at(levelSTT).getTilesList().at(stt3)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt3)->getType() <= 59))
						if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt3)->getCollision())) check = true;
					if ((levelList.at(levelSTT).getTilesList().at(stt4)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt4)->getType() <= 90))
						if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt4)->getCollision())) check = true;
					if ((levelList.at(levelSTT).getTilesList().at(stt2)->getType() > 90) && (levelList.at(levelSTT).getTilesList().at(stt4)->getType() > 90)) on_ground = false;
					if ((levelList.at(levelSTT).getTilesList().at(stt4)->getType() > 90) && (levelList.at(levelSTT).getTilesList().at(stt2)->getType() <= 90) && box.x + box.w <= levelList.at(levelSTT).getTilesList().at(stt2)->getX()) on_ground = false;
				}

				groundSTT = stt4;
				//levelSTT = i;
			}
		//}
	}
	else if (box.h >= 20) // player
	{
			if (box.x >= levelList.at(levelSTT).getX() && box.x < levelList.at(levelSTT).getX() + LEVEL_WIDTH && box.y >= 0 && box.y + 32 < LEVEL_HEIGHT) {

				// phải tính lại
				int cot_left = (box.x - levelList.at(levelSTT).getX()) / TILE_WIDTH;
				int cot_right = cot_left + 1;
				int dong_up = (box.y) / TILE_HEIGHT;
				int dong_mid = dong_up + 1;
				int dong_down = dong_up + 2;


				int stt1 = dong_up * 565 + cot_left;
				int stt3 = dong_up * 565 + cot_right;
				int stt4 = dong_down * 565 + cot_left;
				int stt2 = dong_down * 565 + cot_right;
				int stt5 = dong_mid * 565 + cot_left;
				int stt6 = dong_mid * 565 + cot_right;



				if ((box.x < levelList.at(levelSTT).getX() && box.x + box.w - 12 > levelList.at(levelSTT).getX()) || (box.x < levelList.at(levelSTT).getX() + LEVEL_WIDTH && box.x + box.w - 12 > levelList.at(levelSTT).getX() + LEVEL_WIDTH)) {
					on_ground = false;
				}
				else {
					if ((levelList.at(levelSTT).getTilesList().at(stt1)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt1)->getType() <= 59))
						if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt1)->getCollision())) check = true;
					if ((levelList.at(levelSTT).getTilesList().at(stt2)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt2)->getType() <= 59))
						if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt2)->getCollision())) check = true;
					if ((levelList.at(levelSTT).getTilesList().at(stt3)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt3)->getType() <= 59))
						if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt3)->getCollision())) check = true;
					if ((levelList.at(levelSTT).getTilesList().at(stt6)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt5)->getType() <= 59))
						if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt5)->getCollision())) check = true;
					if ((levelList.at(levelSTT).getTilesList().at(stt6)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt6)->getType() <= 59))
						if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt6)->getCollision())) check = true;
					if ((levelList.at(levelSTT).getTilesList().at(stt4)->getType() >= 0) && (levelList.at(levelSTT).getTilesList().at(stt4)->getType() <= 59))
						if (checkCollision(box, levelList.at(levelSTT).getTilesList().at(stt4)->getCollision())) check = true;
					if ((levelList.at(levelSTT).getTilesList().at(stt2)->getType() > 90) && (levelList.at(levelSTT).getTilesList().at(stt4)->getType() > 90)) on_ground = false;
					if ((levelList.at(levelSTT).getTilesList().at(stt4)->getType() > 90) && (levelList.at(levelSTT).getTilesList().at(stt2)->getType() <= 90) && box.x + box.w <= levelList.at(levelSTT).getTilesList().at(stt2)->getX()) on_ground = false;
				}

				groundSTT = stt4;
				//levelSTT = i;
			}
		//}
	}
	return check;
}



/*bool CommonFunc::checkTouchWall(SDL_Rect& box, std::vector<Game_Map> levelList)
{

}*/

