#pragma once
#include <Axilya/AXMain.h>
#include "main.h"
#include "tile.h"
using namespace std;

class AIState
{
	public:
		//Tracks Wins
		double wins;
		//Tracks total games played
		int totGames;
		//Which player caused that state
		int playerIndex;
		//It's parent
		AIState* parent;
		//List of child nodes
		vector<AIState*> children;

		int aiScore = 0;
		int playerScore = 0;
		int numbPiecesPlayed = 0;

		vector<int> board; 
		vector<int> phand;
		vector<int> aihand;

	
	    //All of the constructors
	    AIState();
	    AIState(int pIndex, AIState* _parent, vector<int> _board, vector<int> _phand, vector<int> _aihand, int _numbPiecesPlayed);

		//For adding score
		void addScore(double value)
		{
		    wins += value;
			totGames++;
			if(parent) parent->addScore(1-value);
		}

		vector<AIState*> generateChildren ();
		int getWinner ();
		void calculateGameScore();
};