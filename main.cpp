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
int numbPiecesPlayed = 0;
AI ai;
TextObject playerScoreText;
TextObject aiScoreText;



void setup()
{
    srand (time(NULL));
    AXFont font("Arial.ttf", 20);
    Tile::loadTextures();

    playerScoreText.setText("Player Score: " + to_string(playerScore), font);
    aiScoreText.setText("AI Score: " + to_string(aiScore), font);

    int xOffset = 350;
    int yOffset = 100;
    for(int i = 0; i < 5; i ++)
    {
        for(int j = 0; j < 5; j ++)
        {
            Tile* tile = new Tile(i*81+xOffset, j*81+yOffset, 80, 80);
            tile->setTileType(0);
            tile->setDraggable(false);
            board.push_back(tile);
        }
    }

    for(int i = 0; i < 4; i++)
    {            
        Tile* tile = new Tile(i*81+10, 500, 80, 80);
        tile->setTileType(getNextTileValue());
        tile->setDraggable(true);
        hand.push_back(tile);
    }

    for(int i = 0; i < 3; i++)
    {            
        Tile* tile = new Tile(i*81+10, 400, 80, 80);
        tile->setTileType(YOURWIZ);
        tile->setDraggable(true);
        hand.push_back(tile);
    }

    for(int i = 0; i < 4; i++)
    {            
        Tile* tile = new Tile();
        tile->setTileType(getNextTileValue());
        tile->setDraggable(true);
        aiHand.push_back(tile);
    }

    for(int i = 0; i < 3; i++)
    {            
        Tile* tile = new Tile();
        tile->setTileType(AIWIZ);
        tile->setDraggable(true);
        aiHand.push_back(tile);
    }
}

void update(){
    if(playersTurn) checkUserInput();
    else runAI();
}

void draw(){
    setBackground(100,150,75,0);
    playerScoreText.display(10,10);
    aiScoreText.display(10,50);
    for(int i = 0; i < board.size(); i++)board[i]->display();
    for(int i = 0; i < hand.size(); i++) hand[i]->display();
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
    if(value < 0.6) return 2;
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
            board[i]->setTileType(selectedTile->getType());
            hand.erase(
                remove_if( hand.begin(), hand.end(),
                    [](const Tile* tile){ return tile == selectedTile;}
                ),hand.end());
            
            if(selectedTile->getType() < YOURWIZ)
            {
                Tile* tile = new Tile(selectedTileSX, selectedTileSY, 80, 80);
                tile->setTileType(getNextTileValue());
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
    AXLog::debug("Running AI");
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
    unpackState(newState);
}

void unpackState(AIState* newState)
{
    for(int i = 0; i < newState->aihand.size(); i++) 
    {
        Tile* tile = new Tile();
        tile->setTileType(newState->aihand[i]);
        tile->setDraggable(false);
        aiHand.push_back(tile);
    }

    int latestWizX = 10;
    int latestTileX = 10;

    for(int i = 0; i < newState->phand.size(); i++) 
    {
        Tile* tile = new Tile();
        tile->setTileType(newState->phand[i]);
        tile->setDraggable(true);

        if(newState->phand[i] == YOURWIZ)
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
            tile->setTileType(newState->board[i*5+j]);
            tile->setDraggable(false);
            board.push_back(tile);
        }

    numbPiecesPlayed++;
    calculateGameScore();
    playersTurn = true;
    // delete currentState;
    // delete newState;

}
