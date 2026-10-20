#include <SFML/Graphics.hpp>
#include "Engine.h"
#include <stdlib.h>
#include <iostream>


using namespace sf;
using namespace std;

int main(){
    Eng::Engine engine(3.0);
    engine.fillBlocks(Eng::BlockID::Dirt);
    engine.addStone(9);
    engine.generateStone(2.5);
    engine.generateCaves(1);
    engine.addStructure(
        Eng::mapSizeX / 2, 
        Eng::mapSizeY / 2, 
        Eng::StructSize0, 
        *Eng::Struct0, 
        Eng::BlockID::Nothing);
    engine.setHeroCoordinate(Eng::mapSizeX / 2 + 2, Eng::mapSizeY / 2 + 4);
    RenderWindow window(VideoMode(engine.ScreenSizeX, engine.ScreenSizeY), L"Калькулятор для Архипова");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    engine.setWindow(&window);

    float timeForF4 = 0;
    float timeForF5 = 0;

    bool F4WasPressed = false;
    bool F5WasPressed = false;

    bool drawGrid = false;
    bool drawCoordinate = false;

    while (window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
            }
        }
        engine.setCursorCoordinate();
        if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))){
            engine.changeReflection(Eng::Reflection::Left);
            engine.moveLeftHero();
            if (!engine.inFall)
                engine.walkingAnimation();
        }
        if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))){
            engine.changeReflection(Eng::Reflection::Right);
            engine.moveRightHero();
            if (!engine.inFall)
                engine.walkingAnimation();
        }
        if (Keyboard::isKeyPressed(Keyboard::Space) && !engine.inFall) {
           engine.jumpAnimationDo = true;
        }

        if (Keyboard::isKeyPressed(Keyboard::F4) && !F4WasPressed)
        {
            drawCoordinate = !drawCoordinate;
            F4WasPressed = true;
        }

        if (Keyboard::isKeyPressed(Keyboard::F5) && !F5WasPressed) {
            drawGrid = !drawGrid;
            F5WasPressed = true;
        }

        if (timeForF4 < 0.25 && F4WasPressed) {
            timeForF4 += engine.time;
        }
        else {
            timeForF4 = 0;
            F4WasPressed = false;
        }

        if (timeForF5 < 0.25 && F5WasPressed) {
            timeForF5 += engine.time;
        }
        else {
            timeForF5 = 0;
            F5WasPressed = false;
        }

        //if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
        //    engine.moveUpHero();
        //}

        //if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) {
        //    engine.moveDownHero();
        //}

        if (!Keyboard::isKeyPressed(Keyboard::Right) and 
            !Keyboard::isKeyPressed(Keyboard::Left ) and 
            !Keyboard::isKeyPressed(Keyboard::D    ) and 
            !Keyboard::isKeyPressed(Keyboard::A    ) or
            engine.inFall)
        {
            engine.setWalkAnimationNumber(0);
        }

        if (event.type == Event::EventType::Resized){
            window.setSize(sf::Vector2u(engine.ScreenSizeX, engine.ScreenSizeY));
        }

        if (Mouse::isButtonPressed(Mouse::Button::Left)) {
            sf::Vector2i CursorCoord = sf::Mouse::getPosition(window);
            if ((CursorCoord.x >= 0 and CursorCoord.x < engine.ScreenSizeX) && 
                (CursorCoord.y >= 0 and CursorCoord.y < engine.ScreenSizeY))
            {
                engine.PickAxeStrke = true;
                engine.dig();
            }
        }

        if (Mouse::isButtonPressed(Mouse::Button::Right))
            engine.setBlock(engine.CursorCoordinateInBlocksX, engine.CursorCoordinateInBlocksY, Eng::BlockID::Stone);

        if (engine.jumpAnimationDo)
            engine.jumpAnimation();

        if (engine.PickAxeStrke)
            engine.pickAxeAnimation();

        if (engine.jumpDo)
            engine.jump();

        engine.moveTime();
        engine.fall();

        engine.setScreen();
        window.clear(Color(Eng::und::red, Eng::und::green, Eng::und::blue, 0));
        engine.drawBlocks();
        engine.drawHero();

        if (drawGrid)
            engine.drawGrid();

        if (drawCoordinate)
            engine.drawCoordinate();

        window.display();
    }

    return 0;
}