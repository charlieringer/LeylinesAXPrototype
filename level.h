#include <Axilya/AXMain.h>
#include <algorithm>
#include "tile.h"
#include "aistate.h"
#include "ai.h"
#include "main.h"
#include "text.h"

class Level
{
	private:
		int width;
		vector<int> deckDistribution;

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

	public:	
		Level();
		void setup();
		void update();
		void draw();
};