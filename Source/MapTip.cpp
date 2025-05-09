#include "MapTip.h"
#include "Screen.h"

MapTip::MapTip() :
	Frame{ Screen::WIDTH - TILE_WIDTH * SHOW_TILE_COLUMN_COUNT, 0, TILE_WIDTH * SHOW_TILE_COLUMN_COUNT, Screen::HEIGHT },
	pHTileHandles_{}
{
	pHTileHandles_ = std::vector<int>(TILE_ROW_COUNT * TILE_COLUMN_COUNT);
	LoadDivGraph(
		"bg.png", TILE_ROW_COUNT * TILE_COLUMN_COUNT,
		TILE_COLUMN_COUNT, TILE_ROW_COUNT,
		TILE_WIDTH, TILE_HEIGHT,
		//pHTileHandles_);
		pHTileHandles_.data());
}

MapTip::~MapTip()
{
	for (int i = 0; i < TILE_ROW_COUNT * TILE_COLUMN_COUNT; i++)
	{
		DeleteGraph(pHTileHandles_[i]);
	}
}

void MapTip::UpdateFrame()
{
}

void MapTip::DrawFrame()
{
	DrawBox(offsetX_, offsetY_, offsetX_ + width_, offsetY_ + height_, 0xffffff, TRUE);
	for (int i = 0; i < TILE_ROW_COUNT * TILE_COLUMN_COUNT; i++)
	{
		int pickX{ i % 8 };
		int pickY{ i / 8 };

		DrawGraph(
			pickX * TILE_WIDTH + offsetX_,
			pickY * TILE_HEIGHT + offsetY_,
			pHTileHandles_[i],// pickY * TILE_ROW_COUNT + pickX
			TRUE);
	}
}

const int MapTip::TILE_COLUMN_COUNT{ 16 };
const int MapTip::TILE_ROW_COUNT{ 12 };

const int MapTip::TILE_WIDTH{ 32 };
const int MapTip::TILE_HEIGHT{ 32 };

const int MapTip::SHOW_TILE_COLUMN_COUNT{ 8 };
const int MapTip::SHOW_TILE_ROW_COUNT{ 0 };
