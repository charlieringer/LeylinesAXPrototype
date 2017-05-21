#pragma once
#include <Axilya/AXMain.h>
#include "tile.h"
using namespace std;

#ifndef AISTATE_H
#define AISTATE_H

class AIState
{
	private:
		double wins = 0.0;
		int totGames = 0;
		int playerIndex = 0;
		AIState* parent = nullptr;
		vector<AIState*> children;
		int aiScore = 0;
		int playerScore = 0;
		int numbPiecesPlayed = 0;
		int tileHScore = 0;
		float stateHScore = 0.0;
		bool pruned = false;
		vector<int> board; 
		vector<int> phand;
		vector<int> aihand;

		void calculateGameScore();
		float getScore();
		vector<int> calculateHueristicScores();

	public:
	    ~AIState();
	    AIState(int pIndex, AIState* _parent, vector<int> _board, vector<int> _phand, vector<int> _aihand, int _numbPiecesPlayed, int _tileHScore);

	    float getHueristicScore();
	    vector<AIState*> generateChildren ();
		int getWinner ();

		double getWins(){ return wins; };
		double getTotalGames(){ return totGames; };
		bool isPruned(){ return pruned; };
		vector<int> getAIHand(){ return aihand; };
		vector<int> getPlayerHand(){ return phand; };
		vector<int> getBoard(){ return board; };
		int getNumbChildren(){ return children.size(); };
		AIState* getNthChild(int n){ return children[n]; };
		void setPruned(bool _pruned){ pruned = _pruned; };
		vector<AIState*>& getChildren(){ return children; };
		void removeWorstNChildren(int numbToRemove);
		void addScore(double value);
		void removeChildren();
};

#endif