// Includes
#include <Windows.h>
#include <chrono>
#include <string>
#include "Game.h"


#pragma region TYPEDEFS
// TYPEDEFS
typedef std::chrono::steady_clock::time_point Time;
typedef std::chrono::high_resolution_clock HiResClock;
typedef std::chrono::duration<float> TimeDiff;

#pragma endregion

int main(void)
{
	Game gameInstance;
	float deltaTime = 0.0f;
	Time currentFrameTime = HiResClock::now();
	Time previousFrameTime = HiResClock::now();


	// Main Game Loop
	while (!gameInstance.exitGame)
	{
		gameInstance.Initialise();
		//Calculate the deltaTime
		Time currentFrameTime = HiResClock::now();
		TimeDiff diff = currentFrameTime - previousFrameTime;
		deltaTime = diff.count();

		if (deltaTime >= (1.0f / FRAME_RATE))
		{
			// Update the Application
			gameInstance.Update(deltaTime);

			// Cache the timestamp of the frame
			previousFrameTime = currentFrameTime;
		}
		//Draw the Screen
		gameInstance.Draw();
	}
	return 0;
}