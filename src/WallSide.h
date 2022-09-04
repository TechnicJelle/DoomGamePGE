#ifndef DOOMGAME_WALLSIDE_H
#define DOOMGAME_WALLSIDE_H


#include "olcPixelGameEngine.h"
#include "Renderable.h"

class WallSide : public Renderable
{
public:
	WallSide(const std::string &filename, int i, float tileCenterX, float tileCenterY);
};


#endif //DOOMGAME_WALLSIDE_H
