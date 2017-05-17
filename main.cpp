//
//  main.cpp
//  Axilya
//
//  Created by William Meaton on 31/01/2017.
//  Copyright © 2017 willmeaton.com. All rights reserved.
//
#include <Axilya/AXMain.h>
#include <algorithm>
#include "tile.h"
#include "aistate.h"
#include "ai.h"
#include "main.h"
#include "text.h"

using namespace std;

vector<Tile*> board;
vector<Tile*> hand;
vector<Tile*> aiHand;
bool tileSelected = false;
Tile* selectedTile = nullptr;
int selectedTileSX;
int selectedTileSY;
bool playersTurn = true;
int playerScore = 0;
int aiScore = 0;
int numbPiecesPlayed = 1;
AI ai;
TextObject playerScoreText;
TextObject aiScoreText;
TextObject aiThinking;
TextObject gameOver;



void setup()
{
    srand (time(NULL));
    AXFont font("Arial.ttf", 20);
    Tile::loadTextures();

    playerScoreText.setText("Player Score: " + to_string(playerScore), font);
    aiScoreText.setText("AI Score: " + to_string(aiScore), font);
    aiThinking.setText("AI THINKING...", font);

    makeBoard();
    makeAIHand();
    makePlayerHand();
}

void makeAIHand()
{
    for(int i = 0; i < 4; i++)
    {            
        Tile* tile = new Tile();
        tile->setType(getNextTileValue());
        tile->setDraggable(true);
        aiHand.push_back(tile);
    }

    for(int i = 0; i < 3; i++)
    {            
        Tile* tile = new Tile();
        tile->setType(AIWIZ);
        tile->setDraggable(true);
        aiHand.push_back(tile);
    }

}

void makePlayerHand()
{
    for(int i = 0; i < 4; i++)
    {            
        Tile* tile = new Tile(i*81+10, 500, 80, 80);
        tile->setType(getNextTileValue());
        tile->setDraggable(true);
        hand.push_back(tile);
    }

    for(int i = 0; i < 3; i++)
    {            
        Tile* tile = new Tile(i*81+10, 400, 80, 80);
        tile->setType(YOURWIZ);
        tile->setDraggable(true);
        hand.push_back(tile);
    }
}

void makeBoard()
{
    int xOffset = 350;
    int yOffset = 100;
    for(int i = 0; i < 5; i ++)
    {
        for(int j = 0; j < 5; j ++)
        {
            Tile* tile = new Tile(i*81+xOffset, j*81+yOffset, 80, 80);
            tile->setType(0);
            tile->setDraggable(false);
            board.push_back(tile);
        }
    }
    board[12]->setType(getNextTileValue());
}

void update(){
    if(numbPiecesPlayed < 25)
    {
        if(playersTurn) checkUserInput();
        else runAI();
    } else {
        AXFont font("Arial.ttf", 40);
        if(playerScore > aiScore) gameOver.setText("You Won! Click to replay.", font);
        else  if (aiScore > playerScore) gameOver.setText("You Lost! Click to replay.", font);
        else gameOver.setText("You Drew! Click to replay.", font);
        waitForReset(); 
    }

}

void draw(){
    setBackground(100,150,75,0);
    playerScoreText.display(10,10);
    aiScoreText.display(10,50);

    for(int i = 0; i < board.size(); i++)board[i]->display();
    for(int i = 0; i < hand.size(); i++) hand[i]->display();

    if(!playersTurn)
    {
        fill(AXColour(255,255,255,128));
        drawRectCenter(400, 300, 300, 100);
        aiThinking.displayCentered(400, 300);
    }
    if(numbPiecesPlayed == 25)
    {
        fill(AXColour(255,255,255,128));
        drawRectCenter(400, 300, 500, 200);
        gameOver.displayCentered(400, 300);
    }

}

int main(int argc, char *argv[])
{
    //You can use axilya like a framework by passing the window update and draw methods
    if(!AXWindow::init(800, 600, "Leyline Prototype", AX_DEFAULT, update, draw)){
        AXLog::log("Window failed to initialise", "Quitting", AX_LOG_ERROR);
        return -1;
    }
    setup();
    //return the window run, this is where the update and draw will be called
    return AXWindow::run();
}
int getNextTileValue()
{
    float value = (float)rand() / RAND_MAX;
    if(value < 0.1) return -2;
    if(value < 0.2) return 4;
    if(value < 0.4) return 3;
    if(value < 0.7) return 2;
    return 1;
}

void checkUserInput()
{
    int mX = AXInput::mouseX;
    int mY = AXInput::mouseY;

    if(!AXInput::getValue("MB1") && tileSelected)
    {
        handleDroppedTile();
    } else if(tileSelected)
    {
        selectedTile->setXCenteredOn(mX);
        selectedTile->setYCenteredOn(mY);
        return;
    } else if(AXInput::getValue("MB1"))
    {
        for(int i = 0; i < hand.size(); i++)
        {
            if(hand[i]->containsPoint(mX,mY))
            {
                selectedTile = hand[i];
                tileSelected = true;
                selectedTileSX = selectedTile->getX();
                selectedTileSY = selectedTile->getY();
            }
        }
    }
}

void handleDroppedTile()
{
    int mX = AXInput::mouseX;
    int mY = AXInput::mouseY;
    tileSelected = false;
    bool tilePlaced = false;
    for(int i = 0; i < board.size(); i++)
    {
        if(board[i]->containsPoint(mX,mY) && board[i]->getType() == EMPTY)
        {
            board[i]->setType(selectedTile->getType());
            hand.erase(
                remove_if( hand.begin(), hand.end(),
                    [](const Tile* tile){ return tile == selectedTile;}
                ),hand.end());
            
            if(selectedTile->getType() < YOURWIZ)
            {
                Tile* tile = new Tile(selectedTileSX, selectedTileSY, 80, 80);
                tile->setType(getNextTileValue());
                tile->setDraggable(true);
                hand.push_back(tile);
            }
            tilePlaced = true;
            playersTurn = false;
            calculateGameScore();
            numbPiecesPlayed++;
            break;
        }
    }
    if(!tilePlaced)
    {
        selectedTile->setX(selectedTileSX);
        selectedTile->setY(selectedTileSY);
    }
    selectedTile = nullptr;
}

void calculateGameScore()
{
    int latestPlayerScore = 0;
    int latestAIScore = 0;

    for(int i = 0; i < board.size(); i++)
    {
        if(board[i]->getType() == YOURWIZ)
        {
            int rowStart = i-(i%5);
            int colStart = i%5;
            for(int i = 0; i < 5; i++)
            {
                int currentHozIndx = rowStart+i;
                int currentVertIndx = colStart+(i*5);

                if(board[currentHozIndx]->getType() < YOURWIZ) latestPlayerScore+=board[currentHozIndx]->getType();
                if(board[currentVertIndx]->getType() < YOURWIZ) latestPlayerScore+=board[currentVertIndx]->getType();
            }

        } else if (board[i]->getType() == AIWIZ)
        {
            int rowStart = i-(i%5);
            int colStart = i%5;
            for(int i = 0; i < 5; i++)
            {
                int currentHozIndx = rowStart+i;
                int currentVertIndx = colStart+(i*5);

                if(board[currentHozIndx]->getType() < YOURWIZ) latestAIScore+=board[currentHozIndx]->getType();
                if(board[currentVertIndx]->getType() < YOURWIZ) latestAIScore+=board[currentVertIndx]->getType();
            }

        }
    }
    playerScore = latestPlayerScore;
    aiScore = latestAIScore;

    AXFont font("Arial.ttf", 20);
    playerScoreText.setText("Player Score: " + to_string(playerScore), font);
    aiScoreText.setText("AI Score: " + to_string(aiScore), font);
    //AXLog::debug("Player Score: " + to_string(playerScore) + " AI Score: " + to_string(aiScore));
}

void runAI()
{
    if(numbPiecesPlayed == 25) return;
    vector<int> aiHandForState;
    vector<int> playerHandForState;
    vector<int> boardForState;

    for(int i = 0; i < aiHand.size(); i++) aiHandForState.push_back(aiHand[i]->getType());
    for(int i = 0; i < hand.size();   i++) playerHandForState.push_back(hand[i]->getType());
    for(int i = 0; i < board.size();  i++) boardForState.push_back(board[i]->getType());

    AIState* currentState = new AIState(1, NULL, boardForState, playerHandForState, aiHandForState, numbPiecesPlayed);
    AIState* newState = ai.run(currentState);

    for(int i = 0; i < aiHand.size(); i++) delete aiHand[i];
    for(int i = 0; i < hand.size(); i++) delete hand[i];
    for(int i = 0; i < board.size();  i++) delete board[i];
    aiHand.clear();
    hand.clear();
    board.clear();
    unpackState(*newState);
    delete currentState;
}

void unpackState(AIState& newState)
{
    for(int i = 0; i < newState.aihand.size(); i++) 
    {
        Tile* tile = new Tile();
        tile->setType(newState.aihand[i]);
        tile->setDraggable(false);
        aiHand.push_back(tile);
    }

    int latestWizX = 10;
    int latestTileX = 10;

    for(int i = 0; i < newState.phand.size(); i++) 
    {
        Tile* tile = new Tile();
        tile->setType(newState.phand[i]);
        tile->setDraggable(true);

        if(newState.phand[i] == YOURWIZ)
        {
            tile->setX(latestWizX);
            tile->setY(400);
            latestWizX+=81;
        } else {
            tile->setX(latestTileX);
            tile->setY(500);
            latestTileX+=81;
        }
        tile->setWH(80);
        hand.push_back(tile);
    }

    int xOffset = 350;
    int yOffset = 100;
    for(int i = 0; i < 5; i ++)
        for(int j = 0; j < 5; j ++)
        {
            Tile* tile = new Tile(i*81+xOffset, j*81+yOffset, 80, 80);
            tile->setType(newState.board[i*5+j]);
            tile->setDraggable(false);
            board.push_back(tile);
        }

    numbPiecesPlayed++;
    calculateGameScore();
    playersTurn = true;
}

void waitForReset()
{
    if(AXInput::getValue("MB1"))
    {
        for(int i = 0; i < aiHand.size(); i++) delete aiHand[i];
        for(int i = 0; i < hand.size(); i++) delete hand[i];
        for(int i = 0; i < board.size();  i++) delete board[i];
        numbPiecesPlayed = 1;
        aiHand.clear();
        hand.clear();
        board.clear();

        AXFont font("Arial.ttf", 20);
        playersTurn = true;
        playerScore = 0;
        aiScore = 0;

        playerScoreText.setText("Player Score: " + to_string(playerScore), font);
        aiScoreText.setText("AI Score: " + to_string(aiScore), font);

        makeBoard();
        makeAIHand();
        makePlayerHand();
    }
}
