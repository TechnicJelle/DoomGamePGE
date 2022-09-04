#include "Level.h"
#include "Tile.h"
#include "olcPixelGameEngine.h"

#define index2dArray(x, y, w) (y * w + x)

Level::Level(olc::PixelGameEngine pge, int w, int h, std::string mapContent)
{
	this->pge = pge;
	tilesColumns = w;
	tilesRows = h;
	if (mapContent.size() != tilesColumns * tilesRows) throw std::invalid_argument("MapContent size does not match map dimensions");
	tiles = std::vector<std::shared_ptr<Tile >>(tilesColumns * tilesRows);

	std::vector<olc::vf2d> starts = std::vector<olc::vf2d>();

	for (int row = 0; row < tilesRows; row++)
		for (int col = 0; col < tilesColumns; col++)
		{
			switch (mapContent[index2dArray(row, col, tilesColumns)])
			{
				case '.': // Empty
					tiles[index2dArray(col, row, tilesColumns)] = std::make_shared<Tile>(Tile::TileType::EMPTY, col, row, "");
					break;
				case '#': // Wall
					tiles[index2dArray(col, row, tilesColumns)] = std::make_shared<Tile>(Tile::TileType::WALL, col, row, "checkers.png");
					break;
				case 'P': // Player Start Level
					tiles[index2dArray(col, row, tilesColumns)] = std::make_shared<Tile>(Tile::TileType::START, col, row, "liftStart.png");
					starts.emplace_back(col, row);
					break;
				case 'E': // End Next Level
					tiles[index2dArray(col, row, tilesColumns)] = std::make_shared<Tile>(Tile::TileType::NEXT, col, row, "liftNext.png");
					break;
				default:
					throw std::invalid_argument("Invalid map character");
			}
		}

	if (starts.size() != 1)
	{
		std::string msg = "Invalid number of player starts: " + std::to_string(starts.size());
		if (starts.size() > 1)
		{
			msg += " at: ";
			for (olc::vf2d &start : starts)
				msg += "(" + std::to_string(start.x) + ", " + std::to_string(start.y) + "), ";
		}

		throw std::invalid_argument(msg);
	}

	player = Player(pge, this, starts[0].x, starts[0].y, 1.5707963268f); //HALF_PI


}

[[maybe_unused]] void Level::Print()
{
	for (int row = 0; row < tilesRows; row++)
	{
		for (int col = 0; col < tilesColumns; col++)
		{
			std::cout << tiles[index2dArray(col, row, tilesColumns)];
		}
		std::cout << std::endl;
	}
}

void Level::DDA(const olc::vf2d &rayStart, const olc::vf2d &rayDir, float maxDistance = 100.0f,
				std::shared_ptr<Tile> oTile = nullptr, olc::vf2d* oIntersection = nullptr, float* oDist = nullptr)
{
	//From https://www.youtube.com/watch?v=NbSee-XM7WA

	olc::vf2d rayUnitStepSize = { abs(1.0f / rayDir.x), abs(1.0f / rayDir.y) };

	olc::vi2d mapCheck = { (int)rayStart.x, (int)rayStart.y }; //round down to xy index
	olc::vf2d rayLength1D;

	olc::vi2d step;

	// Establish Starting Conditions
	if (rayDir.x < 0)
	{
		step.x = -1;
		rayLength1D.x = (rayStart.x - (float)mapCheck.x) * rayUnitStepSize.x;
	}
	else
	{
		step.x = 1;
		rayLength1D.x = ((float)mapCheck.x + 1 - rayStart.x) * rayUnitStepSize.x;
	}

	if (rayDir.y < 0)
	{
		step.y = -1;
		rayLength1D.y = (rayStart.y - (float)mapCheck.y) * rayUnitStepSize.y;
	}
	else
	{
		step.y = 1;
		rayLength1D.y = ((float)mapCheck.y + 1 - rayStart.y) * rayUnitStepSize.y;
	}

	bool tileFound = false;
	float distance = 0.0f;
	while (!tileFound && distance < maxDistance)
	{
		// Walk along shortest path
		if (rayLength1D.x < rayLength1D.y)
		{
			mapCheck.x += step.x;
			distance = rayLength1D.x;
			rayLength1D.x += rayUnitStepSize.x;
		}
		else
		{
			mapCheck.y += step.y;
			distance = rayLength1D.y;
			rayLength1D.y += rayUnitStepSize.y;
		}

		//Test tile at new test point
		if (mapCheck.x < 0 || mapCheck.x >= tilesColumns ||
			mapCheck.y < 0 || mapCheck.y >= tilesColumns)
			continue;

		std::shared_ptr<Tile> target = tiles[index2dArray(mapCheck.x, mapCheck.y, tilesColumns)];
		if (target->GetType() == Tile::TileType::EMPTY) continue;
		oTile = target;
		tileFound = true;
	}

	oDist = &distance;

	// Calculate intersection location
	if (tileFound)
	{
		olc::vf2d intersection = (rayStart + (rayDir * distance));
		oIntersection = &intersection;
	}
}

Level::Level() = default;

