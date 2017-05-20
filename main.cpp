//
//  Leylines protoype - Charlie Ringer - Copyright © 2017 cring002.github.io. All rights reserved.
//  
//  Axilya used with an MIT License - github.com/vexparadox/axilya
//
#include <Axilya/AXMain.h>
#include "level.h"

using namespace std;

Level* testLevel;

void setup()
{
    srand (time(NULL));
    Tile::loadTextures();
    testLevel = new Level();
    testLevel->setup();
}

void update(){
    testLevel->update();
}

void draw(){
    setBackground(100,150,75,0);
    testLevel->draw();
}

int main(int argc, char *argv[])
{
    //You can use axilya like a framework by passing the window update and draw methods
    if(!AXWindow::init(860, 640, "Leyline Prototype", AX_DEFAULT, update, draw)){
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
    if(value < 0.15) return -2;
    if(value < 0.3) return 4;
    if(value < 0.5) return 3;
    if(value < 0.75) return 2;
    return 1;
};

int getTileHScore()
{
    return 1;
};
