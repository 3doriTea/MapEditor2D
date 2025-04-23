#include "Stage.h"
#include <cassert>

namespace
{
	const int MAP_WIDTH{ 10 };
	const int MAP_HEIGHT{ 10 };

	static inline int ToImageId(int _x, int _y)
	{
		return Stage::TILE_COLUMN_COUNT * _y + _x;
	}

	const int MAP[MAP_HEIGHT][MAP_WIDTH]
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{ 64, 64, 64, 64, 0, 0, 0, 0, 0, 0, },
		{ 64, 64, 64, 7, 7, 7, 7, 7, 0, 0, },
		{ 64, 64, 64, 7, 7, ToImageId(6, 6), 7, 7, 0, 0,},
		{ 64, 64, 64, 7, 7, 7, 7, 7, 0, 0, },
		{ 64, 64, 64, 7, 7, 7, 7, 7, 0, 0, },
		{ 64, 64, 64, 7, 7, 7, 7, 7, 0, 0, },
		{ 0, 0, 0, 7, 7, 7, 7, 7, 0, 0, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
	};
}

Stage::Stage() :
	hImageBackground_{ -1 }
{
	hImageBackground_ = LoadGraph("bg.png");
	assert(hImageBackground_ > 0);

	int width, height;
	GetGraphSize(hImageBackground_, &width, &height);
	tileWidth_ = width / TILE_COLUMN_COUNT;
	tileHeight_ = height / TILE_ROW_COUNT;
}

Stage::~Stage()
{
	DeleteGraph(hImageBackground_);
}

void Stage::Update()
{
	//printfDx("Update \n");
}

void Stage::Draw()
{
	//DrawGraph(0, 0, hImageBackground_, TRUE);
	for (int pickX = 0; pickX < MAP_WIDTH; pickX++)
	{
		for (int pickY = 0; pickY < MAP_WIDTH; pickY++)
		{
			int data{ MAP[pickY][pickX] };

			int sourceTileX{ data % TILE_COLUMN_COUNT };
			int sourceTileY{ data / TILE_COLUMN_COUNT };

			DrawRectGraph(
				pickX * tileWidth_, pickY * tileHeight_,
				sourceTileX * tileWidth_, sourceTileY * tileHeight_,
				tileWidth_, tileHeight_,
				hImageBackground_, TRUE);
		}
	}
}

const int Stage::TILE_ROW_COUNT{ 12 };
const int Stage::TILE_COLUMN_COUNT{ 16 };
