#ifndef DOOMGAME_RENDERABLE_H
#define DOOMGAME_RENDERABLE_H


#include "olcPixelGameEngine.h"
#include "Player.h"

class Renderable
{
#pragma region Fields
private:
	olc::Decal* decal;
	float distToPlayer{ };

protected:
	olc::vf2d normal;
	olc::vf2d position;
	olc::vf2d p1;
	olc::vf2d p2;

#pragma endregion Fields

#pragma region Methods
protected:
	void CalculatePointsInWorldSpace();

public:
	explicit Renderable(const std::string &filename);

	bool AlignsWithPlayer(Player* player, float threshold = 0.5f);

	void SetVisibility(bool visibility);

	void Render();

#pragma endregion Methods

};


#endif //DOOMGAME_RENDERABLE_H
