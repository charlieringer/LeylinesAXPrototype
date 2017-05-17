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

AIState::~AIState()
{
	for(int i = children.size()-1; i >= 0 ; i--)
	{
		if(children[i]) delete children[i];
	}
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
				if(currenthandcpy[i] < YOURWIZ) currenthandcpy[i] = UNKNOWN;
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
	calculateGameScore();
	if(numbPiecesPlayed < 25) return -1;
	if(playerScore > aiScore) return 0;
	else if (aiScore > playerScore) return 1;
	return 2;

}

float AIState::getScore()
{
	int result = getWinner();
	if(result >= 0 && result < 2) return (result+1)%2;

	float scoreDiff;

	if(playerIndex == 0) scoreDiff = playerScore-aiScore;
	else scoreDiff = aiScore-playerScore;
	float sigscore = 1.0/(1.0+exp(-scoreDiff));
	return sigscore;
}

float AIState::getHueristicScore()
{
	int result = getWinner();
	if(result >= 0 && result < 2) return (result+1)%2;

	float scoreDiff;

	vector<int> hscores = calculateHueristicScores();

	if(playerIndex == 0) scoreDiff = hscores[0]-hscores[1];
	else scoreDiff = hscores[1] - hscores[0];
	float sigscore = 1.0/(1.0+exp(-scoreDiff));
	hscore = sigscore;
	return sigscore;
}

void AIState::calculateGameScore()
{
    int latestPlayerScore = 0;
    int latestAIScore = 0;
    vector<int> scoreBoard = board;

    //First replace all unknowns with random values
    for(int i = 0; i < scoreBoard.size(); i++)
    {
    	if(scoreBoard[i] == UNKNOWN) scoreBoard[i] = getNextTileValue();
    }

    for(int i = 0; i < scoreBoard.size(); i++)
    {
        if(scoreBoard[i] == YOURWIZ)
        {
            int rowStart = i-(i%5);
            int colStart = i%5;
            for(int i = 0; i < 5; i++)
            {
                int currentHozIndx = rowStart+i;
                int currentVertIndx = colStart+(i*5);

                if(scoreBoard[currentHozIndx]  < YOURWIZ) latestPlayerScore+=scoreBoard[currentHozIndx];
                if(scoreBoard[currentVertIndx] < YOURWIZ) latestPlayerScore+=scoreBoard[currentVertIndx];
            }

        } else if (scoreBoard[i] == AIWIZ)
        {
            int rowStart = i-(i%5);
            int colStart = i%5;
            for(int i = 0; i < 5; i++)
            {
                int currentHozIndx = rowStart+i;
                int currentVertIndx = colStart+(i*5);

                if(scoreBoard[currentHozIndx] < YOURWIZ) latestAIScore+=scoreBoard[currentHozIndx];
                if(scoreBoard[currentVertIndx] < YOURWIZ) latestAIScore+=scoreBoard[currentVertIndx];
            }

        }
    }
    playerScore = latestPlayerScore;
    aiScore = latestAIScore;
}

vector<int> AIState::calculateHueristicScores()
{
    int latestPlayerScore = 0;
    int latestAIScore = 0;
    vector<int> scoreBoard = board;

    //First replace all unknowns with random values
    for(int i = 0; i < scoreBoard.size(); i++)
    {
    	if(scoreBoard[i] == UNKNOWN || scoreBoard[i] == EMPTY) scoreBoard[i] = getNextTileValue();
    }

    for(int i = 0; i < scoreBoard.size(); i++)
    {
        if(scoreBoard[i] == YOURWIZ)
        {
            int rowStart = i-(i%5);
            int colStart = i%5;
            for(int i = 0; i < 5; i++)
            {
                int currentHozIndx = rowStart+i;
                int currentVertIndx = colStart+(i*5);

                if(scoreBoard[currentHozIndx]  < YOURWIZ) latestPlayerScore+=scoreBoard[currentHozIndx];
                if(scoreBoard[currentVertIndx] < YOURWIZ) latestPlayerScore+=scoreBoard[currentVertIndx];
            }

        } else if (scoreBoard[i] == AIWIZ)
        {
            int rowStart = i-(i%5);
            int colStart = i%5;
            for(int i = 0; i < 5; i++)
            {
                int currentHozIndx = rowStart+i;
                int currentVertIndx = colStart+(i*5);

                if(scoreBoard[currentHozIndx] < YOURWIZ) latestAIScore+=scoreBoard[currentHozIndx];
                if(scoreBoard[currentVertIndx] < YOURWIZ) latestAIScore+=scoreBoard[currentVertIndx];
            }

        }
    }
    vector<int> scores;
    scores.push_back(latestPlayerScore);
    scores.push_back(latestAIScore);
    return scores;
}
