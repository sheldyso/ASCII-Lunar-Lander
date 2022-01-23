#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include "GameObjects.h"
#include "Constants.h"

class Game
{
public:
	Game();
	void Initialise();
	void Update(float deltaTime);
	void Draw();
	static bool exitGame;
	static float elapsedTime;
	static int playedTime;

private:

	enum GAME_STATE {
		SPLASH,
		MENU,
		PLAY,
		GAME_OVER,
		SAVE_SCREEN,
		SCOREBOARD,
		OPTIONS,
		HELP
	};

	// Console Variables
	SMALL_RECT windowSize = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD characterBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD characterPosition{ 0, 0 };
	SMALL_RECT consoleWriteArea = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	CHAR_INFO consoleBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);

	// Game Variables
	Splash splash;
	Player player;
	Background background;
	Explosion explosion;
	GAME_STATE currentGameState = SPLASH;
	Score score;
};
#endif //!GAME_H