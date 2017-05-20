#pragma once
#include <Axilya/AXMain.h>
#include "main.h"
#include "tile.h"
using namespace std;

class AIState
{
	private:
		double wins;
		int totGames;
		int playerIndex;
		AIState* parent;
		vector<AIState*> children;
		int aiScore = 0;
		int playerScore = 0;
		int numbPiecesPlayed = 0;
		float hscore;
		bool pruned = false;
		vector<int> board; 
		vector<int> phand;
		vector<int> aihand;

		void calculateGameScore();
		float getScore();
		vector<int> calculateHueristicScores();

	public:
	    ~AIState();
	    AIState(int pIndex, AIState* _parent, vector<int> _board, vector<int> _phand, vector<int> _aihand, int _numbPiecesPlayed);

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
		void removeWorstNChildren(int numbToRemove){ 
			sort(children.begin(), children.end(), [](AIState* a, AIState* b){return a->hscore < b->hscore;});
			children.erase(children.begin()+numbToRemove, children.end()); 
		};
		vector<AIState*>& getChildren(){ return children; };
	    //For adding score
		void addScore(double value)
		{
		    wins += value;
			totGames++;
			if(parent) parent->addScore(1-value);
		}
		void removeChildren()
		{
		    //Reset the children as these are not 'real' children but just ones for the roll out.
		    for(int i = 0; i < children.size(); i++)
		    {
		        delete children[i]; 
		    }
		    children.clear();
		}

};