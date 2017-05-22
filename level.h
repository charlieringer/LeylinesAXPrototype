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

class Level
{
	private:
		//Data about the level itself
		int width;
		vector<int> playerDeckDistribution;
		vector<int> aiDeckDistribution;
		map<int, int> specialBoardPositions;
		int numbPlayerWiz;
		int numbAIWiz;

		//Data about the playing pieces
		vector<Tile*> board;
		vector<Tile*> hand;
		vector<Tile*> aiHand;
		vector<int> playerDeck;
		vector<int> aiDeck;

		//Board offsets
		int xOffset;
    	int yOffset;

    	//Data for tracking the players move
    	bool tileSelected = false;
		Tile* selectedTile = nullptr;
		int selectedTileSX;
		int selectedTileSY;

		//Scores
		int playerScore = 0;
		int aiScore = 0;

		//Game tracking
		bool playersTurn = true;
		int numbPiecesPlayed = 0;
		int maxNumberOfPieces = 0;
		int tileHScore;

		//The AI
		AI ai;

		//Text stuff
		TextObject playerScoreText;
		TextObject aiScoreText;
		TextObject aiThinking;
		TextObject gameOver;
		AXFont* smallFont;
		AXFont* bigFont;

		//Makes the decks for each player
		void makeAIDeck();
		void makePlayerDeck();
		//Makes the starting hands
		void makeAIHand();
		void makePlayerHand();
		//Makes the board
		void makeBoard();

		void checkUserInput();
		void handleDroppedTile();
		void calculateGameScore();
		void runAI();
		void unpackState(AIState& newState);
		void waitForReset();
		int getNextPlayerTile();
		int getNextAITile();
		int getTileFromDistribution();
		int getTileHScore();

	public:	
		Level(string levelfile);
		void update();
		void draw();
};

#endif