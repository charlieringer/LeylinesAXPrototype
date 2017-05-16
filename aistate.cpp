#include "aistate.h"

AIState::AIState(int pIndex, AIState* _parent, vector<int> _board, vector<int> _phand, vector<int> _aihand, int _numbPiecesPlayed)
{
	playerIndex = pIndex;
	parent = _parent;
	wins = totGames = 0;
	board = _board; 
    phand = _phand;
	aihand = _aihand;
	numbPiecesPlayed = _numbPiecesPlayed;
}

vector<AIState*> AIState::generateChildren ()
{
	int newPIndx = (playerIndex == 0) ? 1 : 0;
	vector<int> currenthand = (playerIndex == 0) ? phand : aihand;
	for(int j = 0; j < board.size(); j++)
	{
		if(board[j] == EMPTY)
		{
			for(int i = 0; i < currenthand.size(); i++)
			{
				vector<int> currenthandcpy = currenthand;
				vector<int> newboard = board;
				newboard[j] = currenthandcpy[i];
				if(currenthandcpy[i] < YOURWIZ) currenthandcpy[i] = getNextTileValue();
				else currenthandcpy.erase(currenthandcpy.begin() +i);
				AIState* newChild;
				if(playerIndex == 0) newChild = new AIState(newPIndx, this, newboard, currenthandcpy, aihand, numbPiecesPlayed+1);
				else newChild = new AIState(newPIndx, this, newboard, phand, currenthandcpy, numbPiecesPlayed+1);
				children.push_back(newChild);
			}
		}
	}
	return children;
}

int AIState::getWinner ()
{
	if(numbPiecesPlayed < 25) return -1;
	calculateGameScore();
	if(playerScore > aiScore) return 0;
	else if (aiScore > playerScore) return 1;
	return 2;

}

float AIState::getScore()
{
	int result = getWinner();
	if(result == playerIndex) return 1;
	if(result == (playerIndex+1)%2) return 0;

	float scoreDiff;

	if(playerIndex == 0) scoreDiff = playerScore-aiScore;
	else scoreDiff = aiScore - playerScore;
	float sigscore = 1.0/1.0+exp(-scoreDiff);

	return sigscore;
}

void AIState::calculateGameScore()
{
    int latestPlayerScore = 0;
    int latestAIScore = 0;

    for(int i = 0; i < board.size(); i++)
    {
        if(board[i] == YOURWIZ)
        {
            int rowStart = i-(i%5);
            int colStart = i%5;
            for(int i = 0; i < 5; i++)
            {
                int currentHozIndx = rowStart+i;
                int currentVertIndx = colStart+(i*5);

                if(board[currentHozIndx]  < YOURWIZ) latestPlayerScore+=board[currentHozIndx];
                if(board[currentVertIndx] < YOURWIZ) latestPlayerScore+=board[currentVertIndx];
            }

        } else if (board[i] == AIWIZ)
        {
            int rowStart = i-(i%5);
            int colStart = i%5;
            for(int i = 0; i < 5; i++)
            {
                int currentHozIndx = rowStart+i;
                int currentVertIndx = colStart+(i*5);

                if(board[currentHozIndx] < YOURWIZ) latestAIScore+=board[currentHozIndx];
                if(board[currentVertIndx] < YOURWIZ) latestAIScore+=board[currentVertIndx];
            }

        }
    }
    playerScore = latestPlayerScore;
    aiScore = latestAIScore;
}
