#pragma once
#include <Axilya/AXMain.h>
#include <algorithm>
#include <map>
#include "tile.h"
#include "aistate.h"
#include "ai.h"
#include "text.h"

#ifndef LEVEL_H
#define LEVEL_H

using namespace std;

class Spell 
{
public:
	int manaCost;
	Spell();

	//Takes in a board, edits it based on what the spell does, and returns true is the board got changed.
	virtual bool humanPlaySpell(vector<Tile*>& board, int playLocation) = 0;
	virtual vector<int> aiPlaySpell(vector<int> board, int playLocation) = 0;
}

class BlockPathSpell 
{
public:
	int manaCost;
	Spell();

	//Takes in a board, edits it based on what the spell does, and returns true is the board got changed.
	virtual bool humanPlaySpell(vector<Tile*>& board, int playLocation) = 0;
	virtual vector<int> aiPlaySpell(vector<int> board, int playLocation) = 0;
}

#endif