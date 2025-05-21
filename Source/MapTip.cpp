#include "MapTip.h"
#include "Screen.h"

MapTip::MapTip() :
	Frame{ Screen::WIDTH - TILE_WIDTH * SHOW_TILE_COLUMN_COUNT, 0, TILE_WIDTH * SHOW_TILE_COLUMN_COUNT, Screen::HEIGHT },
	pHTileHandles_{},
	grid_{ SHOW_TILE_COLUMN_COUNT, SHOW_TILE_COLUMN_COUNT, 32, 32 }
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
	if (IsOnCursor() == false)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(0xff * 0.5f));
	}

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

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0x00);

	if (IsOnCursor())
	{
		int localX{}, localY{};
		GetMosuePointLocal(&localX, &localY);

		int touchTileX{}, touchTileY{};
		grid_.ToTile(localX, localY, &touchTileX, &touchTileY);

		printfDx("(%d, %d)", localX, localY);
		printfDx("TILE(%d, %d)", touchTileX, touchTileY);
		
		DrawBox(offsetX_, offsetY_, offsetX_ + width_, offsetY_ + height_, 0xff0000, FALSE);

		DrawBox(
			touchTileX * TILE_WIDTH + offsetX_, touchTileY * TILE_HEIGHT + offsetY_,
			(touchTileX + 1) * TILE_WIDTH + offsetX_, (touchTileY + 1) * TILE_HEIGHT + offsetY_,
			0x00ffff, FALSE, 4);
	}

}

const int MapTip::TILE_COLUMN_COUNT{ 16 };
const int MapTip::TILE_ROW_COUNT{ 12 };

const int MapTip::TILE_WIDTH{ 32 };
const int MapTip::TILE_HEIGHT{ 32 };

const int MapTip::SHOW_TILE_COLUMN_COUNT{ 8 };
const int MapTip::SHOW_TILE_ROW_COUNT{ 0 };
