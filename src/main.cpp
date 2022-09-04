#include "olcPixelGameEngine.h"
#include "Level.h"
//#include "olcSoundWaveEngine.h"
//#include "AssetManager.h"

//using am = AssetManager;


#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantParameter"

[[maybe_unused]] static float Map(float value, float start1, float stop1, float start2, float stop2)
{
	return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

#pragma clang diagnostic pop


class DoomGame : public olc::PixelGameEngine
{
public:
	DoomGame()
	{
		sAppName = "DoomGame";
	}

private:
	int nMapWidth = 16;
	int nMapHeight = 16;

	Level level;

	float fFOV = 3.14159f / 4.0f;
	float fDepth = 16.0f;

	uint32_t nLayerBackground = 0;

public:
	bool OnUserCreate() override
	{
#pragma region Level Init
		std::string map;
		map += "################";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "#..............#";
		map += "################";
		level = Level(this, nMapWidth, nMapHeight, map);

		level.Print();
#pragma endregion Level Init

#pragma region Background
		nLayerBackground = CreateLayer();
		SetDrawTarget(nLayerBackground);
		Clear(olc::BLACK);
		for (int y = 0; y < ScreenHeight(); y++)
		{
			if (y < ScreenHeight() / 2)
			{
				float fac = std::clamp(Map((float)y, 0.0f, (float)ScreenHeight(), 1.0f, -3.0f), 0.0f, 1.0f);
				DrawLine(0, y, ScreenWidth(), y, olc::PixelF(0, 0.6f * fac, fac));
			}
			else
			{
				int brightness = (int)std::clamp(Map((float)y, 0.0f, (float)ScreenHeight(), -300.0f, 64.0f), 0.0f, 255.0f);
				DrawLine(0, y, ScreenWidth(), y, olc::Pixel(brightness, brightness, brightness));
			}
		}

		EnableLayer(nLayerBackground, true);
		SetDrawTarget(nullptr);
#pragma endregion Background
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::Key::ESCAPE).bPressed)
			return false;

#pragma region Rendering
		Clear(olc::BLANK);

		for (int x = 0; x < ScreenWidth(); x++)
		{
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)ScreenWidth() * fFOV);

			float fDistanceToWall = 0.0f;
			bool bHitWall = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);


			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += 0.01f;

				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else
				{
					if (map[nTestY * nMapWidth + nTestX] == '#')
					{
						bHitWall = true;
					}
				}
			}

			int nCeiling = (int)(((float)ScreenHeight() / 2.0f) - (float)ScreenHeight() / ((float)fDistanceToWall));
			int nFloor = ScreenHeight() - nCeiling;

			for (int y = 0; y < ScreenHeight(); y++)
			{
				float fBrightness = 1.0f - (fDistanceToWall / fDepth);
				if (y > nCeiling && y <= nFloor)
					Draw(x, y, olc::PixelF(fBrightness, fBrightness, fBrightness));
			}
		}
#pragma endregion // Rendering

		return true;
	}
};


int main()
{
	DoomGame game;
	if (game.Construct(1280, 720, 1, 1))
		game.Start();

	return 0;
}
