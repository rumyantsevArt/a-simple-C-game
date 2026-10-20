#pragma once
#include <SFML/Graphics.hpp>

namespace Eng 
{
	enum BlockID
	{
		Nothing,
		Dirt,
		Stone,

		//========

		Unknown
	};

	class Block //Еденица карты
	{
		int ID = BlockID::Nothing;
		int heath = 0;

	public:
		Block();

		Block(int ID);

		void setID(int id);

		int  getID();
	};

	const Block NothingBlock(Nothing);

	const Block DirtBlock(Dirt);

	const Block StoneBlock(Stone);
}

