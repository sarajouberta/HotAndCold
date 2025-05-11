#pragma once
#include "Actor.h"
#include "Player.h"

class JumpableMonster : public Actor
{
public:
	JumpableMonster(float x, float y, Game* game);

	bool jumpedOn;
	void jumpOnIt(); // marca monstruo como saltado sobre él
	bool isJumpedOn(); //

	bool isBeingJumpedOnByPlayer(Player* player);  //recibe pntero a player
};

