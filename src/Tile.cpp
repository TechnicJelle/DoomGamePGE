#include "Tile.h"
#include "olcPixelGameEngine.h"


Tile::Tile(TileType type, int col, int row, const std::string &filename)
{
	this->type = type;
	this->col = col;
	this->row = row;

	for (int i = 0; i < 4; i++)
	{
		sides[i] = new WallSide(filename, i, (float)col + 0.5f, (float)row + 0.5f);
	}
}

Tile::TileType Tile::GetType()
{
	return type;
}
