#ifndef DOOMGAME_PLAYER_H
#define DOOMGAME_PLAYER_H


#include "Level.h"

class Player
{
#pragma region Fields
private:
	const float ROTATION_SPEED = 0.1f;
	const float MOVEMENT_SPEED = 0.1f;
	const float WALL_PADDING = 0.3f;

	olc::PixelGameEngine* pge;
	Level* level;

	olc::vf2d position;
	float angle;

public:
	const float VIEW_DEPTH = 4.0f;

#pragma endregion Fields


#pragma region Methods
private:
	void Rotate(float amt);

	void Move(float fElapsedTime);

public:
	Player(olc::PixelGameEngine* pge, Level* level, float startX, float startY, float startAngle);

	olc::vf2d GetPosition();

	olc::vf2d GetHeading();

#pragma endregion Methods
};


#endif //DOOMGAME_PLAYER_H
