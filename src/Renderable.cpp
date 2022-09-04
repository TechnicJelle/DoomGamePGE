//
// Created by TechnicJelle on 03/09/2022.
//

#include "Renderable.h"
#include "Player.h"

Renderable::Renderable(const std::string &filename)
{
	decal = new olc::Decal(new olc::Sprite(filename));

	position = { 1, 1 };
	normal = { 1, 0 };

	CalculatePointsInWorldSpace();
}

void Renderable::CalculatePointsInWorldSpace()
{
	p1 = { position.x + normal.y / 2.0f, position.y - normal.x / 2.0f };
	p2 = { position.x - normal.y / 2.0f, position.y + normal.x / 2.0f };
}

bool Renderable::AlignsWithPlayer(Player* player, float threshold)
{
	return normal.dot(player->GetHeading()) < threshold;
}

void Renderable::Render()
{

}

float distCalc(const olc::vf2d &v1, const olc::vf2d &v2)
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	return sqrtf(dx * dx + dy * dy);
}

float angleBetween2(const olc::vf2d &v1, const olc::vf2d &v2)
{
	//Thanks to https://github.com/EV4gamer for making this one!
	float a = atan2f(v2.y, v2.x) - atan2f(v1.y, v1.x);
	return fmodf(a + 6.2831853072f, 6.2831853072f); //TWO_PI
}

std::pair<int, float> WorldToScreen(Player* player, olc::vf2d p)
{
	olc::vf2d pp = p - player->GetPosition();
	float dist = distCalc(player->GetPosition(), p);
	float angle = angleBetween2(player->GetHeading(), pp);
	if (angle > 3.1415926536f) //PI
		angle -= 6.2831853072f; //Thanks https://github.com/EV4gamer //TWO_PI

	int ix = roundf((staticWidth / 2.0f) + angle * (staticWidth / fieldOfView)); //Thanks https://github.com/StevenClifford!
	return (ix, dist);
}