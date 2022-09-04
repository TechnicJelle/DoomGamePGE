#ifndef DOOMGAME_TILE_H
#define DOOMGAME_TILE_H


#include "WallSide.h"

class Tile
{
public:
	enum TileType
	{
		EMPTY,
		WALL,
		START,
		NEXT,
	};

private:
	TileType type;
	int col;
	int row;

	WallSide* sides[4]{ };

public:
	Tile(TileType type, int col, int row, const std::string &filename);

	TileType GetType();
};


#endif //DOOMGAME_TILE_H
