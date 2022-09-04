#include "Player.h"
#include "olcPixelGameEngine.h"
#include "Tile.h"


static void Limit(olc::vf2d* vec, float max)
{
	if (vec->mag2() <= max * max) return;
	olc::vf2d ret = vec->norm();
	ret *= max;
	vec = &ret;
}

Player::Player(olc::PixelGameEngine* pge, Level* level, float startX, float startY, float startAngle)
{
	this->pge = pge;
	this->level = level;
	position = { startX, startY };
	angle = 0.0f;
	Rotate(startAngle);
}

void Player::Rotate(float amt)
{
	angle += amt;
}

void Player::Move(float fElapsedTime)
{
	if (pge->GetKey(olc::Key::A).bHeld)
		Rotate(-ROTATION_SPEED * fElapsedTime);
	if (pge->GetKey(olc::Key::D).bHeld)
		Rotate(ROTATION_SPEED * fElapsedTime);

	olc::vf2d moveDir;
	if (pge->GetKey(olc::Key::W).bHeld)
	{
		moveDir = GetHeading();
	}
	else if (pge->GetKey(olc::Key::S).bHeld)
	{
		moveDir = -GetHeading();
	}

	if (moveDir != olc::vf2d{ 0.0f, 0.0f })
	{
		std::shared_ptr<Tile> tile;
		float* dist;
		level->DDA(position, moveDir, 1.0f, tile, nullptr, dist);

		olc::vf2d add = (moveDir * (*dist - WALL_PADDING));
		Limit(&add, MOVEMENT_SPEED * fElapsedTime);
		position += add;
	}
}

olc::vf2d Player::GetHeading()
{
	return { cosf(angle), sinf(angle) };
}

olc::vf2d Player::GetPosition()
{
	return position;
}
