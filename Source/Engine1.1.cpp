#include "Engine.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>


using namespace std;

Eng::Engine::Engine()
{
	scale = 2.0;
	makeSprites();
	setScale();
	makeTexts();
}

Eng::Engine::Engine(float _scale)
{
	scale = _scale;
	makeSprites();
	setScale();
	makeTexts();
}

Eng::Engine::Engine(sf::RenderWindow* win)
{
	window = win;
	scale = 2.0;
	makeSprites();
	setScale();
	makeTexts();
}

void Eng::Engine::setWindow(sf::RenderWindow* win)
{
	window = win;
}

void Eng::Engine::fillBlocks(int ID)
{
	for (int x = 0; x < mapSizeX; x++)
	for (int y = 0; y < mapSizeY; y++)
		Map[x][y].setID(ID);
}

void Eng::Engine::setBlock(int x, int y, int ID)
{
	bool inAcceptValues = x >= 0 and x < mapSizeX and y >= 0 and y < mapSizeY;
	if (inAcceptValues and not heroIntersection(x, y))
		if (abs(CursorCoordinateInBlocksX - HeroCoordX / blockSize) < 2 &&
			abs(CursorCoordinateInBlocksY - HeroCoordY / blockSize) < 2)
		if (Map[x][y].getID() == BlockID::Nothing)
				Map[x][y].setID(ID);
}

void Eng::Engine::addStone(int cent)
{
	srand(std::time(NULL));
	for (int x = 0; x < mapSizeX; x++)
	for (int y = 0; y < mapSizeY; y++)
	{
		if ((rand() % 100) < cent)
			Map[x][y].setID(BlockID::Stone);
	}
	
}

void Eng::Engine::moveTime()
{
	time = clock.getElapsedTime().asMicroseconds();
	clock.restart();
	time = time / 1000000;
}

void Eng::Engine::generateStone(float cent)
{
	srand(std::time(NULL));
	for (int x = 0; x < mapSizeX; x++)
	for (int y = 0; y < mapSizeY; y++)
	{
		if (rand() % 100 < cent)
		{
			switch (rand() % 3)
			{
			case 0:
				addStructure(x, y, StructSize0, *Struct0, BlockID::Stone);
				break;

			case 1:
				addStructure(x, y, StructSize1, *Struct1, BlockID::Stone);
				break;

			case 2:
				addStructure(x, y, StructSize2, *Struct2, BlockID::Stone);
				break;
			}
		}
	}
}

void Eng::Engine::generateCaves(float cent)
{
	srand(std::time(NULL));
	for (int x = 0; x < mapSizeX; x++)
		for (int y = 0; y < mapSizeY; y++)
		{
			if (rand() % 100 < cent)
			{
				switch (rand() % 3)
				{
				case 0:
					addStructure(x, y, StructSize0, *Struct0, BlockID::Nothing);
					break;

				case 1:
					addStructure(x, y, StructSize1, *Struct1, BlockID::Nothing);
					break;

				case 2:
					addStructure(x, y, StructSize2, *Struct2, BlockID::Nothing);
					break;
				}
			}
		}
}

void Eng::Engine::addStructure(int xCoord, int yCoord, int StructSize, const int* Struct, int ID)
{
	for (int x = 0; x < StructSize; x++)
	for (int y = 0; y < StructSize; y++)
	{
		if ((xCoord + x >= mapSizeX) or (xCoord + x < 0)) break;
		if ((yCoord + y >= mapSizeY) or (yCoord + y < 0)) continue;
		if (Struct[x * StructSize + y])
			Map[xCoord + x][yCoord + y].setID(ID);
	}
}

void Eng::Engine::setHeroCoordinate(int x, int y)
{
	HeroCoordX = x * blockSize;
	HeroCoordY = y * blockSize;
}

void Eng::Engine::drawGrid()
{
	sf::RectangleShape lineX(sf::Vector2f(1, ScreenSizeY));
	lineX.setFillColor(sf::Color::White);
	sf::RectangleShape lineY(sf::Vector2f(ScreenSizeX, 1));
	lineY.setFillColor(sf::Color::White);
	for (int x = 1; x <= ScreenSizeX / blockPixelSize / scale + 1; x++)
	{
		lineX.setPosition((int(ScreenCoordX / blockSize + x) - (ScreenCoordX / blockSize)) * blockPixelSize * scale, 0);
		window->draw(lineX);
	}

	for (int y = 1; y <= ScreenSizeY / blockPixelSize / scale + 1; y++)
	{
		lineY.setPosition(0, (int(ScreenCoordY / blockSize + y) - (ScreenCoordY / blockSize)) * blockPixelSize * scale);
		window->draw(lineY);
	}
}

void Eng::Engine::drawHero()
{
	if (!window) return;
	if (reflection == Reflection::Left)
		HeroSprite.setPosition(
			((HeroCoordX - ScreenCoordX) / blockSize + 1) * blockPixelSize * scale, (HeroCoordY - ScreenCoordY) / blockSize * blockPixelSize * scale);
	else 
		HeroSprite.setPosition((HeroCoordX - ScreenCoordX) / blockSize * blockPixelSize * scale, (HeroCoordY - ScreenCoordY) / blockSize * blockPixelSize * scale);
	window->draw(HeroSprite);
	//cout << (HeroCoordX - ScreenCoordX) / blockSize * blockPixelSize * scale;
}

void Eng::Engine::walkingAnimation()
{
	currentWalkingFrame += time * animationSpeed;
	if (currentWalkingFrame >= 5) currentWalkingFrame = 0;
	HeroSprite.setTextureRect(sf::IntRect(blockPixelSize*int(currentWalkingFrame), 0, blockPixelSize, blockPixelSize));
}

void Eng::Engine::dig()
{
	if (CursorCoordinateInBlocksX >= 0 && CursorCoordinateInBlocksX < mapSizeX &&
		CursorCoordinateInBlocksY >= 0 && CursorCoordinateInBlocksY < mapSizeY)
	if (abs(CursorCoordinateInBlocksX - HeroCoordX / blockSize) < 2 &&
		abs(CursorCoordinateInBlocksY - HeroCoordY / blockSize) < 2)
				Map[CursorCoordinateInBlocksX][CursorCoordinateInBlocksY].setID(BlockID::Nothing);
}

void Eng::Engine::setWalkAnimationNumber(int frame)
{
	currentWalkingFrame = frame;
	HeroSprite.setTextureRect(sf::IntRect(blockPixelSize * int(currentWalkingFrame), 0, blockPixelSize, blockPixelSize));
}

void Eng::Engine::jumpAnimation()
{
	int UpperBlockY = int(HeroCoordY / blockSize) - 1;
	bool UpperisNotNothing = Map[int(HeroCoordX / blockSize)][int(UpperBlockY)].getID() != BlockID::Nothing ||
							 Map[int((HeroCoordX + blockSize - 1) / blockSize)][int(UpperBlockY)].getID() != BlockID::Nothing;
	if (UpperisNotNothing)
		jumpAnimationDo = false;
	else {
		currenJumpFrame += time * animationSpeed;
		if (currenJumpFrame >= 3) {
			currenJumpFrame = 2;
			jumpAnimationDo = false;
			jumpDo = true;
		}
		HeroSprite.setTextureRect(sf::IntRect(blockPixelSize * int(currenJumpFrame), blockPixelSize * 2, blockPixelSize, blockPixelSize));
	}
}

void Eng::Engine::setCursorCoordinate()
{
	CursorCoordinate = sf::Mouse::getPosition(*window);
	if (CursorCoordinate.x >= 0 && CursorCoordinate.x < ScreenSizeX && CursorCoordinate.y >= 0 && CursorCoordinate.y < ScreenSizeY) {
		CursorCoordinateInBlocksX = CursorCoordinate.x / scale / blockPixelSize + ScreenCoordX / blockSize;
		CursorCoordinateInBlocksY = CursorCoordinate.y / scale / blockPixelSize + ScreenCoordY / blockSize;
	}
}

void Eng::Engine::pickAxeAnimation()
{
	currentPickAxeFrame += time * animationSpeed;
	if (currentPickAxeFrame >= 4)
	{
		currentPickAxeFrame = 0;
		PickAxeStrke = false;
	}
	HeroSprite.setTextureRect(sf::IntRect(blockPixelSize * int(currentPickAxeFrame), blockPixelSize, blockPixelSize, blockPixelSize));
}

void Eng::Engine::setScreen()
{
	ScreenCoordX = HeroCoordX - ScreenSizeX / blockPixelSize / scale / 2 * blockSize + blockSize / 2;
	ScreenCoordY = HeroCoordY - ScreenSizeY / blockPixelSize / scale / 2 * blockSize + blockSize / 2;
	if (ScreenCoordX < 0)
		ScreenCoordX = 0;
	else
		if (ScreenCoordX / blockSize + ScreenSizeX / blockPixelSize / scale > mapSizeX)
			ScreenCoordX = (mapSizeX - ScreenSizeX / blockPixelSize / scale) * blockSize;

	if (ScreenCoordY < 0)
		ScreenCoordY = 0;
	else
		if (ScreenCoordY / blockSize + ScreenSizeY / blockPixelSize / scale > mapSizeY)
			ScreenCoordY = (mapSizeX - ScreenSizeY / blockPixelSize / scale) * blockSize;
}

void Eng::Engine::changeReflection(int ref)
{
	if ((ref != Reflection::Right) and (ref != Reflection::Left)) return;
	reflection = ref;
	HeroSprite.setScale(ref * scale, scale);
}

void Eng::Engine::moveScreenUp()
{
	ScreenCoordY -= time * Velocity;
}

void Eng::Engine::moveScreenRight()
{
	ScreenCoordX += time * Velocity;
}

void Eng::Engine::moveScreenLeft()
{
	ScreenCoordX -= time * Velocity;
}

void Eng::Engine::moveScreenDown()
{
	ScreenCoordY += time * Velocity;
}

void Eng::Engine::setScale(float sc)
{
	scale = sc;
	sf::Vector2f vScale(sc, sc);

	HeroSprite.setScale(vScale);


	DirtSprite.setScale(vScale);


	StoneSprite.setScale(vScale);


	UnknownSprite.setScale(vScale);
}

void Eng::Engine::moveUpHero()
{
	int NextBlockY = int(HeroCoordY / blockSize) - 1;
	HeroCoordY -= time * Velocity;
	bool NextisNotNothing = Map[int(HeroCoordX / blockSize)][int(NextBlockY)].getID() != BlockID::Nothing ||
						    Map[int((HeroCoordX + blockSize - 1) / blockSize)][int(NextBlockY)].getID() != BlockID::Nothing;
	if (time * Velocity < blockSize) {
		if (NextisNotNothing && HeroCoordY < (NextBlockY + 1) * blockSize)
			HeroCoordY = (NextBlockY + 1) * blockSize;
	}
	else 
		for (int i = 0; i <= int(time * Velocity / blockSize); i++)
		{
			NextisNotNothing = Map[int(HeroCoordX / blockSize)][int(NextBlockY - i)].getID() != BlockID::Nothing ||
							   Map[int((HeroCoordX + blockSize - 1) / blockSize)][int(NextBlockY - i)].getID() != BlockID::Nothing;
			if (NextisNotNothing)
			{
				HeroCoordY = (NextBlockY - i + 1) * blockSize;
				break;
			}
		}
	if (HeroCoordY < 0)
		HeroCoordY = 0;
}

void Eng::Engine::moveDownHero()
{
	float NextBlockY = int((HeroCoordY + blockSize - 1) / blockSize) + 1;
	HeroCoordY += time * Velocity;
	bool NextisNotNothing = Map[int(HeroCoordX / blockSize)][int(NextBlockY)].getID() != BlockID::Nothing ||
					 	    Map[int((HeroCoordX + blockSize - 1) / blockSize)][int(NextBlockY)].getID() != BlockID::Nothing;
	if (time * Velocity < blockSize) {
		if (NextisNotNothing && HeroCoordY > (NextBlockY - 1) * blockSize)
			HeroCoordY = (NextBlockY - 1) * blockSize;
	}
	else
		for (int i = 0; i <= int(time * Velocity / blockSize); i++)
		{
			NextisNotNothing = Map[int(HeroCoordX / blockSize)][int(NextBlockY + i)].getID() != BlockID::Nothing ||
							   Map[int((HeroCoordX + blockSize - 1) / blockSize)][int(NextBlockY + i)].getID() != BlockID::Nothing;
			if (NextisNotNothing)
			{
				HeroCoordY = (NextBlockY + i - 1) * blockSize;
				break;
			}
		}
	if (HeroCoordY > (mapSizeY - 1) * blockSize)
		HeroCoordY = (mapSizeY - 1) * blockSize;
}

void Eng::Engine::moveRightHero()
{
	float NextBlockX  = int((HeroCoordX + blockSize - 1) / blockSize) + 1;
	HeroCoordX += time * Velocity;
	bool NextisNotNothing = Map[int(NextBlockX)][int(HeroCoordY / blockSize)].getID()  != BlockID::Nothing ||
						    Map[int(NextBlockX)][int((HeroCoordY + blockSize - 1) / blockSize)].getID() != BlockID::Nothing;
	if (time * Velocity < blockSize) {
		if (NextisNotNothing && HeroCoordX > (NextBlockX - 1) * blockSize)
			HeroCoordX = (NextBlockX - 1) * blockSize;
	}
	else
		for (int i = 0; i <= int(time * Velocity / blockSize); i++)
		{
			NextisNotNothing = Map[int(NextBlockX + i)][int(HeroCoordY / blockSize)].getID() != BlockID::Nothing ||
							   Map[int(NextBlockX + i)][int((HeroCoordY + blockSize - 1) / blockSize)].getID() != BlockID::Nothing;
			if (NextisNotNothing)
			{
				HeroCoordX = (NextBlockX + i - 1) * blockSize;
				break;
			}
		}
	if (HeroCoordX > (mapSizeX - 1) * blockSize)
		HeroCoordX = (mapSizeX - 1) * blockSize;
}

void Eng::Engine::moveLeftHero()
{
	float NextBlockX = int(HeroCoordX / blockSize) - 1;
	HeroCoordX -= time * Velocity;
	bool NextisNotNothing = Map[int(NextBlockX)][int(HeroCoordY / blockSize)].getID() != BlockID::Nothing ||
						    Map[int(NextBlockX)][int((HeroCoordY + blockSize - 1) / blockSize)].getID() != BlockID::Nothing;

	if (time * Velocity < blockSize){
		if (NextisNotNothing && HeroCoordX < (NextBlockX + 1) * blockSize)
			HeroCoordX = NextBlockX * blockSize + blockSize;
	}
	else 
		for (int i = 0; i <= int(time * Velocity / blockSize); i++)
		{
			NextisNotNothing = Map[int(NextBlockX - i)][int(HeroCoordY / blockSize)].getID() != BlockID::Nothing ||
							   Map[int(NextBlockX - i)][int((HeroCoordY + blockSize - 1) / blockSize)].getID() != BlockID::Nothing;
			if (NextisNotNothing)
			{
				HeroCoordX = (NextBlockX - i + 1) * blockSize;
				break;
			}
		}
	if (HeroCoordX < 0)
		HeroCoordX = 0;
}

void Eng::Engine::fall()
{
	int LowerBlockY = int((HeroCoordY + blockSize - 1) / blockSize) + 1;
	int UpperBlockY = int(HeroCoordY / blockSize) - 1;
	inFall = true;

	HeroCoordY += time * verticalVelocety;
	bool LowerisNotNothing = Map[int(HeroCoordX / blockSize)][int(LowerBlockY)].getID() != BlockID::Nothing ||
					 		 Map[int((HeroCoordX + blockSize - 1) / blockSize)][int(LowerBlockY)].getID() != BlockID::Nothing;

	bool UpperisNotNothing = Map[int(HeroCoordX / blockSize)][int(UpperBlockY)].getID() != BlockID::Nothing ||
							 Map[int((HeroCoordX + blockSize - 1) / blockSize)][int(UpperBlockY)].getID() != BlockID::Nothing;

	if (abs(verticalVelocety * time) < blockSize)
	{
		if (UpperisNotNothing && HeroCoordY < (UpperBlockY + 1) * blockSize)
		{
			HeroCoordY = (UpperBlockY + 1) * blockSize;
			verticalVelocety = 0;
		}

		if ((LowerisNotNothing && (HeroCoordY > ((LowerBlockY - 1) * blockSize))))
		{
			HeroCoordY = (LowerBlockY - 1) * blockSize;
			verticalVelocety = 0;
			inFall = false;
		}
	}
	else{
		if (verticalVelocety > 0)
			for (int i = 0; i <= abs(time * verticalVelocety / blockSize); i++)
			{
				LowerisNotNothing = Map[int(HeroCoordX / blockSize)][int(LowerBlockY + i)].getID() != BlockID::Nothing ||
									Map[int((HeroCoordX + blockSize - 1) / blockSize)][int(LowerBlockY + i)].getID() != BlockID::Nothing;
				if (LowerisNotNothing)
				{
					HeroCoordY = (LowerBlockY + i - 1) * blockSize;
					verticalVelocety = 0;
					inFall = false;
					break;
				}
			}

		else {
			for (int i = 0; i <= abs(time * verticalVelocety / blockSize); i++)
			{
				UpperisNotNothing = Map[int(HeroCoordX / blockSize)][int(UpperBlockY - i)].getID() != BlockID::Nothing ||
									Map[int((HeroCoordX + blockSize - 1) / blockSize)][int(UpperBlockY - i)].getID() != BlockID::Nothing;
				if (UpperisNotNothing)
				{
					HeroCoordY = (UpperBlockY - i + 1) * blockSize;
					verticalVelocety = 0;
					break;
				}
			}
		}
	}


	if (verticalVelocety < 2000)
		verticalVelocety += verticalAcelleration * time * blockSize;
	else verticalVelocety = 2000;

	if (HeroCoordY < 0) {
		HeroCoordY = 0;
		verticalVelocety = 0;
	}

	if (HeroCoordY > (mapSizeY - 1) * blockSize) {
		HeroCoordY = (mapSizeY - 1) * blockSize;
		verticalVelocety = 0;
		inFall = false;
	}
}

void Eng::Engine::jump()
{
	verticalVelocety = -jumpForce*blockSize;
	jumpDo = false;
}

bool Eng::Engine::heroIntersection(int x, int y)
{
	bool inMinCoordX = (x - 1) * blockSize < HeroCoordX;
	bool inMinCoordY = (y - 1) * blockSize < HeroCoordY;
	bool inMaxCoordX = (x + 1) * blockSize > HeroCoordX;
	bool inMaxCoordY = (y + 1) * blockSize > HeroCoordY;

	if (inMinCoordX && inMinCoordY && inMaxCoordX && inMaxCoordY) 
	{
		//debugText.setString("true");
		//window->draw(debugText);
		return true;
	}
	else
	{
		//debugText.setString("false");
		//window->draw(debugText);
		return false; 
	}
}

void Eng::Engine::makeTexts()
{
	font.loadFromFile("Textures\\font.ttf");

	fpsText.setFont(font);
	fpsText.setCharacterSize(48);
	fpsText.setFillColor(sf::Color::White);

	coordinate.setFont(font);
	coordinate.setCharacterSize(48);
	coordinate.setFillColor(sf::Color::White);

	BlockIDtext.setFont(font);
	BlockIDtext.setCharacterSize(48);
	BlockIDtext.setFillColor(sf::Color::White);
	BlockIDtext.setPosition(sf::Vector2f(0,24));

	debugText.setFont(font);
	debugText.setCharacterSize(48);
	debugText.setFillColor(sf::Color::White);
	debugText.setPosition(sf::Vector2f(0, 48));
}

void Eng::Engine::drawCoordinate()
{
	if (!window) return;
	coordinate.setString(to_string(HeroCoordX) + " " + to_string(HeroCoordY));

	fps += 1;
	fpsTime += time;
	if (fpsTime >= 1) 
	{
		fpsText.setString(to_string(fps));
		fpsTime = 0;
		fps     = 0;
	};

	fpsText.setPosition(0, -25);

	switch (Map[CursorCoordinateInBlocksX][CursorCoordinateInBlocksY].getID())
	{
	case BlockID::Nothing:
		BlockIDtext.setString("Nothing");
		break;
	case BlockID::Dirt:
		BlockIDtext.setString("Dirt");
		break;
	case BlockID::Stone:
		BlockIDtext.setString("Stone");
		break;
	default:
		BlockIDtext.setString("Default");
		break;
	}
	window->draw(BlockIDtext);
	window->draw(fpsText);
	window->draw(coordinate);
}

void Eng::Engine::drawBlocks()
{
	if (!window) return;
	for (int x = ScreenCoordX / blockSize - 1;  (x < (ScreenCoordX / blockSize + ScreenSizeX / (blockPixelSize * scale))) and (x < mapSizeX); x++)
	for (int y = ScreenCoordY / blockSize - 1;  (y < (ScreenCoordY / blockSize + ScreenSizeY / (blockPixelSize * scale))) and (y < mapSizeY); y++)
		{
			if (x < 0) break;
			if (y < 0) continue;

			switch (Map[x][y].getID())
			{
			case BlockID::Nothing:
				break;

			case BlockID::Dirt:
				DirtSprite.setPosition((x - (ScreenCoordX / blockSize)) * blockPixelSize * scale, (y - (ScreenCoordY / blockSize)) * blockPixelSize * scale);
				window->draw(DirtSprite);
				break;

			case BlockID::Stone:
				StoneSprite.setPosition((x - (ScreenCoordX / blockSize)) * blockPixelSize * scale, (y - (ScreenCoordY / blockSize)) * blockPixelSize * scale);
				window->draw(StoneSprite);
				break;

			default:
				UnknownSprite.setPosition((x - (ScreenCoordX / blockSize)) * blockPixelSize * scale, (y - (ScreenCoordY / blockSize)) * blockPixelSize * scale);
				window->draw(UnknownSprite);
				break;
			}
		}
}

void Eng::Engine::makeSprites()
{
	HeroTexture.loadFromFile("Textures\\HeroTexture5.png");
	HeroSprite.setTexture(HeroTexture);
	HeroSprite.setTextureRect(sf::IntRect(0, 0, blockPixelSize, blockPixelSize));

	DirtTexture.loadFromFile("Textures\\Dirt.png");
	DirtSprite.setTexture(DirtTexture);

	StoneTexture.loadFromFile("Textures\\Stone.png");
	StoneSprite.setTexture(StoneTexture);

	UnknownTexture.loadFromFile("Textures\\Unknown.png");
	UnknownSprite.setTexture(UnknownTexture);
}

void Eng::Engine::setScale()
{
	sf::Vector2f vScale(scale, scale);

	HeroSprite.setScale(vScale);


	DirtSprite.setScale(vScale);


	StoneSprite.setScale(vScale);


	UnknownSprite.setScale(vScale);
}

