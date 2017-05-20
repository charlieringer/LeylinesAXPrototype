#include "level.h"

Level::Level()
{
    //LEVEL PARSEING GOES HERE!!!!!
    deckDistribution.push_back(1);
    deckDistribution.push_back(1);
    deckDistribution.push_back(1);
    deckDistribution.push_back(2);
    deckDistribution.push_back(2);
    deckDistribution.push_back(2);
    deckDistribution.push_back(3);
    deckDistribution.push_back(3);
    deckDistribution.push_back(3);
    deckDistribution.push_back(4);
    deckDistribution.push_back(4);
    deckDistribution.push_back(-2);
    deckDistribution.push_back(-2);

    width = 5;
}

void Level::setup()
{
    smallFont = new AXFont("data/Arial.ttf", 20);
    bigFont = new AXFont("data/Arial.ttf", 40);
    
    playerScoreText.setText("Player Score: " + to_string(playerScore), smallFont);
    aiScoreText.setText("AI Score: " + to_string(aiScore), smallFont);
    aiThinking.setText("AI THINKING...", smallFont);

    xOffset = 520-(81*(width/2));
    yOffset = 280-(81*(width/2));
    makeBoard();
    makeAIDeck();
    makePlayerDeck();
    makeAIHand();
    makePlayerHand();
}

void Level::makeAIHand()
{
    for(int i = 0; i < 4; i++)
    {            
        Tile* tile = new Tile();
        tile->setType(getNextAITile());
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

void Level::makePlayerHand()
{
    for(int i = 0; i < 4; i++)
    {            
        Tile* tile = new Tile(i*81+10, 500, 80, 80);
        tile->setType(getNextPlayerTile());
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

void Level::makeAIDeck()
{
    aiDeck = deckDistribution;
    random_shuffle(aiDeck.begin(), aiDeck.end());
}

void Level::makePlayerDeck()
{
    playerDeck = deckDistribution;
    random_shuffle(playerDeck.begin(), playerDeck.end());
}

int Level::getNextPlayerTile()
{
    if(playerDeck.size() == 0) makePlayerDeck();
    int returnValue = playerDeck.back();
    playerDeck.pop_back();
    return returnValue;
}

int Level::getNextAITile()
{
    if(aiDeck.size() == 0) makeAIDeck();
    int returnValue = aiDeck.back();
    aiDeck.pop_back();
    return returnValue;
}

void Level::makeBoard()
{
    for(int i = 0; i < width; i ++)
    {
        for(int j = 0; j < width; j ++)
        {
            Tile* tile = new Tile(i*81+xOffset, j*81+yOffset, 80, 80);
            tile->setType(0);
            tile->setDraggable(false);
            board.push_back(tile);
        }
    }
    board[(width*width)/2]->setType(getTileFromDistribution());

    maxNumberOfPieces = width*width;
    numbPiecesPlayed = 1;
}

int Level::getTileFromDistribution()
{
    int randIndex = rand()%deckDistribution.size();
    return deckDistribution[randIndex];
}

void Level::update()
{
    if(numbPiecesPlayed < 25)
    {
        if(playersTurn) checkUserInput();
        else runAI();
    } else {
        if(playerScore > aiScore) gameOver.setText("You Won! Click to replay.", bigFont);
        else  if (aiScore > playerScore) gameOver.setText("You Lost! Click to replay.", bigFont);
        else gameOver.setText("You Drew! Click to replay.", bigFont);
        waitForReset(); 
    }
}

void Level::draw(){
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

void Level::checkUserInput()
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

void Level::handleDroppedTile()
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
                    [this](const Tile* tile){ return tile == selectedTile;}
                ),hand.end());
            
            if(selectedTile->getType() < YOURWIZ)
            {
                Tile* tile = new Tile(selectedTileSX, selectedTileSY, 80, 80);
                tile->setType(getNextPlayerTile());
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

void Level::calculateGameScore()
{
    int latestPlayerScore = 0;
    int latestAIScore = 0;

    for(int i = 0; i < board.size(); i++)
    {
        if(board[i]->getType() == YOURWIZ)
        {
            int rowStart = i-(i%width);
            int colStart = i%width;
            for(int i = 0; i < width; i++)
            {
                int currentHozIndx = rowStart+i;
                int currentVertIndx = colStart+(i*width);

                if(board[currentHozIndx]->getType() < YOURWIZ) latestPlayerScore+=board[currentHozIndx]->getType();
                if(board[currentVertIndx]->getType() < YOURWIZ) latestPlayerScore+=board[currentVertIndx]->getType();
            }

        } else if (board[i]->getType() == AIWIZ)
        {
            int rowStart = i-(i%width);
            int colStart = i%width;
            for(int i = 0; i < width; i++)
            {
                int currentHozIndx = rowStart+i;
                int currentVertIndx = colStart+(i*width);

                if(board[currentHozIndx]->getType() < YOURWIZ) latestAIScore+=board[currentHozIndx]->getType();
                if(board[currentVertIndx]->getType() < YOURWIZ) latestAIScore+=board[currentVertIndx]->getType();
            }

        }
    }
    playerScore = latestPlayerScore;
    aiScore = latestAIScore;

    playerScoreText.setText("Player Score: " + to_string(playerScore), smallFont);
    aiScoreText.setText("AI Score: " + to_string(aiScore), smallFont);
}

void Level::runAI()
{
    if(numbPiecesPlayed == 25) return;
    vector<int> aiHandForState;
    vector<int> playerHandForState;
    vector<int> boardForState;

    for(int i = 0; i < aiHand.size(); i++) aiHandForState.push_back(aiHand[i]->getType());
    for(int i = 0; i < hand.size();   i++) playerHandForState.push_back(UNKNOWN);
    for(int i = 0; i < board.size();  i++) boardForState.push_back(board[i]->getType());

    AIState* currentState = new AIState(1, NULL, boardForState, playerHandForState, aiHandForState, numbPiecesPlayed);
    AIState* newState = ai.run(currentState);

    for(int i = 0; i < aiHand.size(); i++) delete aiHand[i];
    for(int i = 0; i < board.size();  i++) delete board[i];
    aiHand.clear();
    board.clear();
    unpackState(*newState);
    delete currentState;
}

void Level::unpackState(AIState& newState)
{
    for(int i = 0; i < newState.getAIHand().size(); i++) 
    {
        Tile* tile = new Tile();
        if(newState.getAIHand()[i] != UNKNOWN) tile->setType(newState.getAIHand()[i]);
        else tile->setType(getNextAITile());
        tile->setDraggable(false);
        aiHand.push_back(tile);
    }

    int latestWizX = 10;
    int latestTileX = 10;

    // for(int i = 0; i < newState.getPlayerHand().size(); i++) 
    // {
    //     Tile* tile = new Tile();
    //     if(newState.getPlayerHand()[i] != UNKNOWN) tile->setType(newState.getPlayerHand()[i]);
    //     else tile->setType(ge());

    //     tile->setDraggable(true);

    //     if(newState.getPlayerHand()[i] == YOURWIZ)
    //     {
    //         tile->setX(latestWizX);
    //         tile->setY(400);
    //         latestWizX+=81;
    //     } else {
    //         tile->setX(latestTileX);
    //         tile->setY(500);
    //         latestTileX+=81;
    //     }
    //     tile->setWH(80);
    //     hand.push_back(tile);
    // }

    for(int i = 0; i < width; i ++)
        for(int j = 0; j < width; j ++)
        {
            Tile* tile = new Tile(i*81+xOffset, j*81+yOffset, 80, 80);
            tile->setType(newState.getBoard()[i*width+j]);
            tile->setDraggable(false);
            board.push_back(tile);
        }

    numbPiecesPlayed++;
    calculateGameScore();
    playersTurn = true;
}

void Level::waitForReset()
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

        playersTurn = true;
        playerScore = 0;
        aiScore = 0;

        playerScoreText.setText("Player Score: " + to_string(playerScore), smallFont);
        aiScoreText.setText("AI Score: " + to_string(aiScore), smallFont);

        makeBoard();
        makeAIDeck();
        makePlayerDeck();
        makeAIHand();
        makePlayerHand();
    }
}