#ifndef DOOMGAME_LEVEL_H
#define DOOMGAME_LEVEL_H

#include "Player.h"

class Level
{
private:
	olc::PixelGameEngine* pge;

	int tilesColumns;
	int tilesRows;
	std::vector <std::shared_ptr<Tile>> tiles;
	Player player;

public:
	Level();

	Level(olc::PixelGameEngine pge, int w, int h, std::string mapContent);

	[[maybe_unused]] void Print();

	void DDA(const olc::vf2d &rayStart, const olc::vf2d &rayDir, float maxDistance,
			 std::shared_ptr <Tile> oTile, olc::vf2d* oIntersection, float* oDist);
};


#endif //DOOMGAME_LEVEL_H
