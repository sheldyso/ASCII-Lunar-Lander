// Includes
#include "Constants.h"
#include "Game.h"
#include "Utility.h"

// Game specific variables for timing and exiting
bool Game::exitGame = false;
float Game::elapsedTime = 0.0f;
int Game::playedTime = 0;

Game::Game()
{
	//Constructor
};

void Game::Initialise()
{
	//Set window title
	SetConsoleTitle(L"Dylan Sheldon - ASCII Game Engine");

	//Set the screen's buffer size
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	//Set the window size
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

}

void Game::Update(float deltaTime)
{

	switch (currentGameState)
	{
	case SPLASH:
	{
		splash.duration += deltaTime;
		if (splash.duration < 3)
		{
			WriteImageToBuffer(consoleBuffer, splash.CHARACTERS, nullptr, splash.HEIGHT, splash.WIDTH, ((SCREEN_WIDTH / 2) - (splash.WIDTH / 2)), ((SCREEN_HEIGHT / 2) - (splash.HEIGHT / 2)));
		}
		else
		{
			currentGameState = MENU;
		}
		break;
	}
	case MENU:
	{
		//"Magic Number" - 6 shifts list to the left to be in the centre of the screen

		ClearScreen(consoleBuffer);
		WriteImageToBuffer(consoleBuffer, splash.CHARACTERS, nullptr, splash.HEIGHT, splash.WIDTH, ((SCREEN_WIDTH / 2) - (splash.WIDTH / 2)), ((SCREEN_HEIGHT / 2) - ((splash.HEIGHT / 2) + 9)));
		WriteTextToBuffer(consoleBuffer, "1. PLAY", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) - 1);
		WriteTextToBuffer(consoleBuffer, "2. OPTIONS", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 0);
		WriteTextToBuffer(consoleBuffer, "3. SCOREBOARD", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 1);
		WriteTextToBuffer(consoleBuffer, "4. HELP", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 2);
		WriteTextToBuffer(consoleBuffer, "5. QUIT", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 3);
		WriteTextToBuffer(consoleBuffer, "Difficulty: " + player.difficulty, 0, 0);

		if (GetAsyncKeyState(Key1))
		{
			currentGameState = PLAY;
		}
		else if (GetAsyncKeyState(Key2))
		{
			currentGameState = OPTIONS;
		}
		else if (GetAsyncKeyState(Key3))
		{
			currentGameState = SCOREBOARD;
		}
		else if (GetAsyncKeyState(Key4))
		{
			currentGameState = HELP;
		}
		else if (GetAsyncKeyState(Key5))
		{
			exitGame = true;
		}
		break;
	}
	case PLAY:
	{
		Game::elapsedTime = Game::elapsedTime + deltaTime;
		Game::playedTime = round(Game::elapsedTime);
		if (!player.hasCrashed && !player.hasLanded)
		{
			//Take the input
			if (GetAsyncKeyState(KeyW) && player.landerFuel > 0.0f)
			{
				player.isAccelerating = true;
			}
			if (GetAsyncKeyState(KeyA))
			{
				--player.xPos;
			}
			if (GetAsyncKeyState(KeyD))
			{
				++player.xPos;
			}

			// Acceleration
			if (player.isAccelerating)
			{
				player.landerFuel -= player.fuelConsumtionRate;
				player.landerAcceleration += (player.accelerationRate * deltaTime);
			}
			else
			{
				player.landerAcceleration -= (player.decelerationRate * deltaTime);
			}

			// Reset acceleration bool
			player.isAccelerating = false;

			// Camp the acceleration
			player.landerAcceleration = ClampFloat(player.landerAcceleration, -1.0f, 1.0f);

			if (player.landerAcceleration >= 0.2f) //REMOVE MAGIC NUMBER
			{
				player.yPos--;
			}
			else if (player.landerAcceleration < 0.0f)
			{
				player.yPos++;
			}

			// Clamp the input
			player.xPos = ClampInt(player.xPos, 0, SCREEN_WIDTH - player.WIDTH);
			player.yPos = ClampInt(player.yPos, 0, SCREEN_HEIGHT - player.HEIGHT);

			// Get 2 characters below landing gear
			char bottomLeftChar = background.Levels()[player.xPos + SCREEN_WIDTH * (player.yPos + (player.HEIGHT - 1))];
			char bottomRightChar = background.Levels()[(player.xPos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.yPos + (player.HEIGHT - 1))];
			char scoreLeftChar = background.Levels()[player.xPos + SCREEN_WIDTH * (player.yPos + (player.HEIGHT))];
			char scoreRightChar = background.Levels()[(player.xPos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.yPos + (player.HEIGHT))];
			// Land or crash condition
				// Landed
			if (bottomLeftChar == '_' && bottomRightChar == '_' && player.landerAcceleration > -0.4f)
			{
				if (scoreLeftChar == '8' && scoreRightChar == 'X')
				{
					player.score += (8 * 50);
				}
				else if (scoreLeftChar == '7' && scoreRightChar == 'X')
				{
					player.score += (7 * 50);
				}
				else if (scoreLeftChar == '6' && scoreRightChar == 'X')
				{
					player.score += (6 * 50);
				}
				else if (scoreLeftChar == '5' && scoreRightChar == 'X')
				{
					player.score += (5 * 50);
				}
				else if (scoreLeftChar == '4' && scoreRightChar == 'X')
				{
					player.score += (4 * 50);
				}
				else if (scoreLeftChar == '3' && scoreRightChar == 'X')
				{
					player.score += (3 * 50);
				}
				else if (scoreLeftChar == '2' && scoreRightChar == 'X')
				{
					player.score += (2 * 50);
				}
				else if (scoreLeftChar == '1' && scoreRightChar == 'X')
				{
					player.score += (50);
				}
				player.hasLanded = true;
				currentGameState = GAME_OVER;
			}
			else if (bottomLeftChar == '_' && bottomRightChar == '_' && player.landerAcceleration < -0.4f)
			{
				player.hasCrashed = true;
				currentGameState = GAME_OVER;
			}
			else if (bottomLeftChar != ' ' || bottomRightChar != ' ')
			{
				player.hasCrashed = true;
				currentGameState = GAME_OVER;
			}

			// Clear the screen
			ClearScreen(consoleBuffer);

			// Draw Background image
			WriteImageToBuffer(consoleBuffer, background.Levels(), nullptr, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);

			// Draw lander image
			WriteImageToBuffer(consoleBuffer, player.CHARACTERS, player.COLOURS, player.HEIGHT, player.WIDTH, player.xPos, player.yPos);


			// Draw UI Text
			WriteTextToBuffer(consoleBuffer, "SCORE: " + std::to_string(player.score), 1, 0);
			WriteTextToBuffer(consoleBuffer, "TIME: " + std::to_string(playedTime) + "s", 1, 1);
			WriteTextToBuffer(consoleBuffer, "FUEL: " + std::to_string(player.landerFuel), 1, 2);
			WriteTextToBuffer(consoleBuffer, "SPEED: " + std::to_string(player.ConvertSpeed(player.landerAcceleration)) + " M/s", 1, 3);

		}
		break;
	}
	case GAME_OVER:
	{
		// Draw Win Lose conditions
		if (player.hasLanded)
		{
			WriteTextToBuffer(consoleBuffer, "THE EAGLE HAS LANDED!", (SCREEN_WIDTH / 2) - 6, SCREEN_HEIGHT / 2);
			player.landTimer += deltaTime;
			if (player.landTimer >= 4)
			{
				currentGameState = PLAY;
				player.Reset();
			}
		}
		else if (player.hasCrashed)
		{
			WriteTextToBuffer(consoleBuffer, "HOUSTON, WE HAVE A PROBLEM!", (SCREEN_WIDTH / 2) - 6, SCREEN_HEIGHT / 2);

			// Explosion effect
			static float explotionDurationCounter = 0.0f;
			explotionDurationCounter += deltaTime;
			if (explotionDurationCounter >= 0.5f)
			{
				WriteImageToBuffer(consoleBuffer, explosion.CHARACTERS, explosion.COLOURS, explosion.HEIGHT, explosion.WIDTH, player.xPos, player.yPos);
				explotionDurationCounter = 0.0f;
			}
			else
			{
				WriteImageToBuffer(consoleBuffer, explosion.EMPTY_CHARACTERS, nullptr, explosion.HEIGHT, explosion.WIDTH, player.xPos, player.yPos);
			}
			// Returns player to Menu after 4s
			player.crashTimer += deltaTime;
			if (player.crashTimer >= 4)
			{
				currentGameState = SAVE_SCREEN;
				// Use Difficulties fuel rate to reset the fuel amount
				if (player.fuelConsumtionRate == 0.25f)
				{
					player.Easy();
				}
				else if (player.fuelConsumtionRate == 0.5f)
				{
					player.Medium();
				}
				else
				{
					player.Hard();
				}
				player.Reset();
			}
		}
		break;
	}
	case SAVE_SCREEN:
	{
		ClearScreen(consoleBuffer);
		WriteTextToBuffer(consoleBuffer, "Your Score Was: " + std::to_string(player.score), (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2));
		player.winTimer += deltaTime;
		if (player.winTimer > 6)
		{
			score.SaveScore(player.score, playedTime);
			currentGameState = MENU;
			player.score = 0;
			Game::elapsedTime = 0.0f;
			Game::playedTime = 0;
		}
		break;
	}
	case SCOREBOARD:
	{
		ClearScreen(consoleBuffer);
		WriteTextToBuffer(consoleBuffer, "Your Scores:", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) - 1);
		WriteTextToBuffer(consoleBuffer, "Score 1: " + std::to_string(score.Numbers[0]) + " Time: " + std::to_string(score.Times[0]) + "s", (SCREEN_WIDTH / 2) - 12, (SCREEN_HEIGHT / 2) + 1);
		WriteTextToBuffer(consoleBuffer, "Score 2: " + std::to_string(score.Numbers[1]) + " Time: " + std::to_string(score.Times[1]) + "s", (SCREEN_WIDTH / 2) - 12, (SCREEN_HEIGHT / 2) + 3);
		WriteTextToBuffer(consoleBuffer, "Score 3: " + std::to_string(score.Numbers[2]) + " Time: " + std::to_string(score.Times[2]) + "s", (SCREEN_WIDTH / 2) - 12, (SCREEN_HEIGHT / 2) + 5);
		WriteTextToBuffer(consoleBuffer, "Score 4: " + std::to_string(score.Numbers[3]) + " Time: " + std::to_string(score.Times[3]) + "s", (SCREEN_WIDTH / 2) - 12, (SCREEN_HEIGHT / 2) + 7);
		WriteTextToBuffer(consoleBuffer, "Press 4 to Return", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 9);

		if (GetAsyncKeyState(Key4))
		{
			currentGameState = MENU;
		}
		break;
	}
	case OPTIONS:
	{
		ClearScreen(consoleBuffer);
		WriteTextToBuffer(consoleBuffer, "Difficulty: ", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) - 1);
		WriteTextToBuffer(consoleBuffer, "1. Easy", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 0);
		WriteTextToBuffer(consoleBuffer, "2. Medium", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 1);
		WriteTextToBuffer(consoleBuffer, "3. Hard", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 2);

		if (GetAsyncKeyState(Key1))
		{
			player.Easy();
			currentGameState = MENU;
		}
		else if (GetAsyncKeyState(Key2))
		{
			player.Medium();
			currentGameState = MENU;
		}
		else if (GetAsyncKeyState(Key3))
		{
			player.Hard();
			currentGameState = MENU;
		}
		break;
	}
	case HELP:
	{
		ClearScreen(consoleBuffer);
		WriteTextToBuffer(consoleBuffer, "--Controls--", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) - 2);
		WriteTextToBuffer(consoleBuffer, "W - Accelerate", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) - 0);
		WriteTextToBuffer(consoleBuffer, "A - Thrust Left", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 1);
		WriteTextToBuffer(consoleBuffer, "D - Thrust Right", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 2);
		WriteTextToBuffer(consoleBuffer, "--How To Play--", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 4);
		WriteTextToBuffer(consoleBuffer, "Land on the Zones to gain Points.", (SCREEN_WIDTH / 2) - 14, (SCREEN_HEIGHT / 2) + 6);
		WriteTextToBuffer(consoleBuffer, "If you land too hard, the lander will Crash.", (SCREEN_WIDTH / 2) - 20, (SCREEN_HEIGHT / 2) + 7);
		WriteTextToBuffer(consoleBuffer, "The more Fuel you have when you land, The higher the score!", (SCREEN_WIDTH / 2) - 26, (SCREEN_HEIGHT / 2) + 8);
		WriteTextToBuffer(consoleBuffer, "Press 4 to Return", (SCREEN_WIDTH / 2) - 6, (SCREEN_HEIGHT / 2) + 10);

		if (GetAsyncKeyState(Key4))
		{
			currentGameState = MENU;
		}
		break;
	}
	default:
		break;
	}
}

void Game::Draw()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}