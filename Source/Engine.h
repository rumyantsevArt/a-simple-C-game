#pragma once
#include "Block.h"
#include <SFML/Graphics.hpp>


namespace Eng 
{
    enum und //Задний фон
    {
        red   = 40,
        green = 40,
        blue  = 40
    };

    enum Reflection
    {
        Right = 1,
        Left = -1
    };

    const int mapSizeX  = 40;  //Ширина карты
    const int mapSizeY  = 40;  //Высота карты

    const int blockPixelSize = 16;  //Размер блока (пиксели)
    const float blockSize = 100;

    const int StructSize0 = 5;
    const int StructSize1 = 5;
    const int StructSize2 = 5;

    class Engine
    {
    public:
        int ScreenSizeX = 800;
        int ScreenSizeY = 600;

        bool PickAxeStrke = false;

        int CursorCoordinateInBlocksX;
        int CursorCoordinateInBlocksY;

        float HeroCoordX = 0;
        float HeroCoordY = 0;
        float scale;   //Масштаб

        float time = 0;
        float fpsTime = 0;

        bool inFall = false;
        bool jumpAnimationDo = false;
        bool jumpDo = false;

    private:
        sf::Clock clock;
        
        unsigned long int YCoordMax = HeroCoordY;

        int fps = 0;
        int Velocity = 5*blockSize;
        float verticalAcelleration = 80;
        float verticalVelocety = 0;
        float jumpForce = 20;

        int ScreenCoordX = 0;
        int ScreenCoordY = 0;

        sf::Vector2i CursorCoordinate;

        int reflection = Reflection::Right;

        float currentWalkingFrame = 0; //Номер кадра
        float currentPickAxeFrame = 0;
        float currenJumpFrame = 0;
        float animationSpeed = 15;

        sf::Font font;
        sf::Text coordinate;
        sf::Text fpsText;
        sf::Text debugText;
        sf::Text BlockIDtext;

        sf::RenderWindow* window;

        Block Map[mapSizeX][mapSizeY]; //Временная карта

    public:
        Engine();

        Engine(float _scale);

        Engine(sf::RenderWindow* win);

        void setWindow(sf::RenderWindow* win);

        void fillBlocks(int ID);    //Заполнить карту блоками

        void setBlock(int x, int y, int ID); //Установить блок

        void addStone(int cent);

        void moveTime();

        void generateStone(float cent);

        void generateCaves(float cent);

        void addStructure(int x, int y, int StructSize, const int* Struct, int ID);

        void setHeroCoordinate(int x, int y);

        void drawHero();

        void drawGrid();

        void walkingAnimation();

        void jumpAnimation();

        void setWalkAnimationNumber(int frame);

        void pickAxeAnimation();

        void setCursorCoordinate();

        void dig();

        void drawCoordinate();

        void moveUpHero();

        void moveDownHero();

        void moveRightHero();

        void moveLeftHero();

        void fall();

        void jump();

        bool heroIntersection(int x, int y);

        void makeTexts();

        void changeReflection(int ref);

        void setScreen();

        void moveEntites();

        void drawBlocks();  //Отрисовать блоки

        void makeSprites(); //Загрузить текстуры и создать спрайты

        void setScale();  //Установить мастшаб

        void setScale(float sc);

        void moveScreenUp();

        void moveScreenRight();

        void moveScreenLeft();

        void moveScreenDown();

    private:
        sf::Texture HeroTexture;
        sf::Sprite  HeroSprite;

        sf::Texture DirtTexture;
        sf::Sprite  DirtSprite;

        sf::Texture StoneTexture;
        sf::Sprite  StoneSprite;

        sf::Texture UnknownTexture;
        sf::Sprite  UnknownSprite;
    };

    const int Struct0[StructSize0][StructSize0] =
    {
        0,1,1,1,0,
        1,1,1,1,1,
        1,1,1,1,1,
        1,1,1,1,1,
        0,1,1,1,0
    };

    const int Struct1[StructSize1][StructSize1] =
    {
        0,0,0,1,0,
        0,0,1,1,1,
        0,1,1,1,1,
        0,1,1,1,1,
        0,0,1,1,0
    };

    const int Struct2[StructSize2][StructSize2] =
    {
        0,1,1,0,0,
        1,1,1,1,0,
        1,1,1,1,0,
        0,1,1,0,0,
        0,0,0,0,0
    };
}

