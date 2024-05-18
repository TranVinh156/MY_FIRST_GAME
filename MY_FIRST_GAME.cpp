// MY_FIRST_GAME.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "RenderWindow.h"
#include "Game.h"
Game MyGame;

int main(int argc, char* argv[])
{
    std::srand(std::time(NULL));
    bool fullscreen = false;
    if (!MyGame.Init()) return 0;
    else
    {
        if (!MyGame.LoadMedia()) return -1;
        else if (!MyGame.CreateMap() || !MyGame.CreateLevel() || !MyGame.CreatePlayer() || !MyGame.CreateSecret() || !MyGame.CreateMenu() || !MyGame.CreateUnDead() || !MyGame.CreateBoss())
        {
            return -1;
        }
        else
        {
            
            while (MyGame.isRunning())
            {
                //fps.start();
                while (SDL_PollEvent(&g_event) != 0)
                {
                    
                    MyGame.HandleGameInput(g_event);
                    

                }
                //CommonFunc::fullScreen(true);
                MyGame.PlayMusic();
                if (MyGame.GetMenuList().at(0).isName())
                {
                    //
                    MyGame.Render_PlayerNameMenu();
                }
                else if (MyGame.GetMenuList().at(0).isMenu())
                {
                    MyGame.Render_MainMenu();
                }
                else if (MyGame.GetMenuList().at(0).isChoose())
                {
                    MyGame.Render_ChooseMenu();
                }
                else if (MyGame.GetMenuList().at(0).isPasued())
                {
                    MyGame.Render_PauseMenu();
                }
                else
                {
                    MyGame.RenderGame();
                    //MyGame.PlayMusic();
                }
                
            }
        }
    }
    MyGame.CleanGame();
    CommonFunc::cleanUp();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
