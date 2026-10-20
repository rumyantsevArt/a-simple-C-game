#include "Block.h"

Eng::Block::Block()
{
	ID = BlockID::Nothing;
	heath = 0;
}

Eng::Block::Block(int id)
{
	switch (id){
		case BlockID::Nothing: heath = 0;   break;
		case BlockID::Dirt:    heath = 100; break;
		case BlockID::Stone:   heath = 300; break;
	}
	ID = id;
}

void Eng::Block::setID(int id)
{
	switch (ID) {
		case BlockID::Nothing: heath = 0;   break;
		case BlockID::Dirt:    heath = 100; break;
		case BlockID::Stone:   heath = 300; break;
	}
	ID = id;
}

int Eng::Block::getID() {return ID;}
