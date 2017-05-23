#include "aistate.h"

//Stupidly massive constructor alert
AIState::AIState(int pIndex, AIState* _parent, vector<int> _board, vector<int> _phand, vector<int> _aihand, int _numbPiecesPlayed, int _tileHScore, int _width) :
playerIndex(pIndex), parent(_parent), board(_board), phand(_phand), 
aihand(_aihand), numbPiecesPlayed(_numbPiecesPlayed), tileHScore(_tileHScore), 
width(_width), wins(0), totGames(0){}

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
				else currenthandcpy.erase(currenthandcpy.begin() + i);
				AIState* newChild;
				if(playerIndex == 0) newChild = new AIState(newPIndx, this, newboard, currenthandcpy, aihand, numbPiecesPlayed+1, tileHScore, width);
				else newChild = new AIState(newPIndx, this, newboard, phand, currenthandcpy, numbPiecesPlayed+1, tileHScore, width);
				children.push_back(newChild);
			}
		}
	}
    //Shuffled to make AI turns where not a lot of experimentation happens (likely at the start of the game) appear more random
    //The alternative is to have the AI play moves close together, not as satifiying. 
    random_shuffle(children.begin(), children.end());
	return children;
}

int AIState::getWinner ()
{
	if(numbPiecesPlayed < board.size()) return -1;

    calculateHueristicScore();
	if(playerScore > aiScore) return 0;
	else if (aiScore > playerScore) return 1;
	return 2;
}

float AIState::getHueristicScore()
{
	int result = getWinner();
	if(result >= 0 && result < 2) return result;
    if(result == 2) return 0.5;

	float scoreDiff;

	calculateHueristicScore();

	if(playerIndex == 0) scoreDiff = playerScore-aiScore;
	else scoreDiff = aiScore - playerScore;
	float sigscore = 1.0/(1.0+exp(-scoreDiff));
	stateHScore = sigscore;
	return sigscore;
}

void AIState::calculateHueristicScore()
{
    int latestPlayerScore = 0;
    int latestAIScore = 0;
    vector<int> scoreBoard = board;

    //First replace all unknowns with random values
    for(int i = 0; i < scoreBoard.size(); i++)
    {
    	if(scoreBoard[i] == UNKNOWN) scoreBoard[i] = tileHScore;
    }

    for(int i = 0; i < scoreBoard.size(); i++)
    {
        if(scoreBoard[i] == YOURWIZ)
        {
            int rowStart = i-(i%width);
            int colStart = i%width;

            for(int j = 0; j < width; j++)
            {
                int current = i-j;
                if(current < rowStart) break;
                if(board[current] == ROCK) break;
                if(board[current] == TREE) latestPlayerScore+=getTreeValueAt(current);
                if(board[current] < NONNUMERICTILES) latestPlayerScore+=board[current];
            }

            for(int j = 0; j < width; j++)
            {
                int current = i+j;
                if(current >= rowStart+width) break;
                if(board[current] == ROCK) break;
                if(board[current] == TREE) latestPlayerScore+=getTreeValueAt(current);
                if(board[current] < NONNUMERICTILES) latestPlayerScore+=board[current];
            }

            for(int j = 0; j < width; j++)
            {
                int current = i-(j*width);
                if(current < colStart) break;
                if(board[current] == ROCK) break;
                if(board[current] == TREE) latestPlayerScore+=getTreeValueAt(current);
                if(board[current] < NONNUMERICTILES) latestPlayerScore+=board[current];
            }

            for(int j = 0; j < width; j++)
            {
                int current = i+(j*width);
                if(current > colStart+(width*width)-width) break;
                if(board[current] == ROCK) break;
                if(board[current] == TREE) latestPlayerScore+=getTreeValueAt(current);
                if(board[current] < NONNUMERICTILES) latestPlayerScore+=board[current];  
            }
        } else if (scoreBoard[i] == AIWIZ)
        {
            int rowStart = i-(i%width);
            int colStart = i%width;

            for(int j = 0; j < width; j++)
            {
                int current = i-j;
                if(current < rowStart) break;
                if(board[current] == ROCK) break;
                if(board[current] == TREE) latestAIScore+=getTreeValueAt(current);
                if(board[current] < NONNUMERICTILES) latestAIScore+=board[current];
            }

            for(int j = 0; j < width; j++)
            {
                int current = i+j;
                if(current >= rowStart+width) break;
                if(board[current] == ROCK) break;
                if(board[current] == TREE) latestAIScore+=getTreeValueAt(current);
                if(board[current] < NONNUMERICTILES) latestAIScore+=board[current];
            }

            for(int j = 0; j < width; j++)
            {
                int current = i-(j*width);
                if(current < colStart) break;
                if(board[current] == ROCK) break;
                if(board[current] == TREE) latestAIScore+=getTreeValueAt(current);
                if(board[current] < NONNUMERICTILES) latestAIScore+=board[current];
            }

            for(int j = 0; j < width; j++)
            {
                int current = i+(j*width);
                if(current > colStart+(width*width)-width) break;
                if(board[current] == ROCK) break;
                if(board[current] == TREE) latestAIScore+=getTreeValueAt(current);
                if(board[current] < NONNUMERICTILES) latestAIScore+=board[current];  
            }
        }
    }
    playerScore = latestPlayerScore;
    aiScore = latestAIScore;
}

int AIState::getTreeValueAt(int postion)
{
    int total = 0;

    int rowStart = postion-(postion%width);
    int colStart = postion%width;

    for(int j = 0; j < width; j++)
    {
        int current = postion-j;
        if(current < rowStart) break;
        if(board[current] == YOURWIZ || board[current] == AIWIZ) total++;
    }

    for(int j = 0; j < width; j++)
    {
        int current = postion+j;
        if(current >= rowStart+width) break;
        if(board[current] == YOURWIZ || board[current] == AIWIZ) total++;
    }

    for(int j = 0; j < width; j++)
    {
        int current = postion-(j*width);
        if(current < colStart) break;
        if(board[current] == YOURWIZ || board[current] == AIWIZ) total++;
    }

    for(int j = 0; j < width; j++)
    {
        int current = postion+(j*width);
        if(current > colStart+(width*width)-width) break;
        if(board[current] == YOURWIZ || board[current] == AIWIZ) total++; 
    }
    return total;
}

void AIState::removeWorstNChildren(int numbToRemove){ 
    sort(children.begin(), children.end(), [](AIState* a, AIState* b){return a->stateHScore < b->stateHScore;});
    children.erase(children.begin()+numbToRemove, children.end()); 
};

void AIState::addScore(double value)
{
    wins += value;
    totGames++;
    if(parent) parent->addScore(1-value);
}
void AIState::removeChildren()
{
    //Reset the children as these are not 'real' children but just ones for the roll out.
    for(int i = 0; i < children.size(); i++)
    {
        delete children[i]; 
    }
    children.clear();
}
