#include "WallSide.h"
#include "olcPixelGameEngine.h"

WallSide::WallSide(const std::string &filename, int i, float tileCenterX, float tileCenterY) : Renderable(filename)
{
	float normalDir = 1.5707963268f * (float)i; //HALF_PI
	int x = (int)(cosf(normalDir));
	int y = (int)(sinf(normalDir));

	normal = { (float)x, (float)y };
	position = { tileCenterX + (float)x / 2.0f, tileCenterY + (float)y / 2.0f };
	CalculatePointsInWorldSpace();
}
