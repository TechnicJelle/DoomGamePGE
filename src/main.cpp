#include "olcPixelGameEngine.h"
//#include "olcSoundWaveEngine.h"
//#include "AssetManager.h"

//using am = AssetManager;


class DoomGame : public olc::PixelGameEngine
{
public:
	DoomGame()
	{
		sAppName = "DoomGame";
	}

private:
	float fPlayerX = 8.0f;
	float fPlayerY = 8.0f;
	float fPlayerA = 0.0f;

	int nMapWidth = 16;
	int nMapHeight = 16;

	std::wstring map;

	float fFOV = 3.14159f / 4.0f;
	float fDepth = 16.0f;

	uint32_t nLayerBackground;

public:
	bool OnUserCreate() override
	{
		map += L"################";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"#..............#";
		map += L"################";

		nLayerBackground = CreateLayer();
		SetDrawTarget(nLayerBackground);
		Clear(olc::BLACK);
		for (int y = ScreenHeight() / 2; y < ScreenHeight(); y++)
		{
			float fCol = (float)y / ((float)ScreenHeight() / 2.0f);
			DrawLine(0, y, ScreenWidth(), y, olc::PixelF(0, 0, fCol));
		}

		EnableLayer(nLayerBackground, true);
		SetDrawTarget(nullptr);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::Key::ESCAPE).bPressed)
			return false;
		if (GetKey(olc::Key::A).bHeld)
			fPlayerA -= 1.0f * fElapsedTime;
		if (GetKey(olc::Key::D).bHeld)
			fPlayerA += 1.0f * fElapsedTime;
		if (GetKey(olc::Key::W).bHeld)
		{
			fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}
		if (GetKey(olc::Key::S).bHeld)
		{
			fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}

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
				fDistanceToWall += 0.1f;

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

			int nCeiling = (float)(ScreenHeight() / 2.0f) - ScreenHeight() / ((float)fDistanceToWall);
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
	if (game.Construct(320, 180, 4, 4))
		game.Start();

	return 0;
}

