#include "MapTip.h"
#include "Screen.h"
#include "Input.h"

MapTip::MapTip() :
	Frame{ Screen::WIDTH - TILE_WIDTH * SHOW_TILE_COLUMN_COUNT, 0, TILE_WIDTH * SHOW_TILE_COLUMN_COUNT, Screen::HEIGHT },
	pHTileHandles_{},
	grid_{ SHOW_TILE_COLUMN_COUNT, SHOW_TILE_COLUMN_COUNT, 32, 32 },
	selectedIndex_{ -1 }
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

bool MapTip::TryGetSelectedTile(int* _pIndex, int* _pHandle)
{
	if (selectedIndex_ == -1)
	{
		return false;
	}

	*_pIndex = selectedIndex_;
	*_pHandle = pHTileHandles_[selectedIndex_];

	return true;
}

void MapTip::UpdateFrame()
{
	if (IsOnCursor())
	{
		tileOffset_ += -GetMouseWheelRotVol() * SHOW_TILE_COLUMN_COUNT;
		if (tileOffset_ < 0)
		{
			tileOffset_ = 0;
		}
	}

	// 右クリックキャンセル
	if (Input::IsMouseDown(MOUSE_INPUT_RIGHT))
	{
		selectedIndex_ = -1;
	}
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

		if ((i + tileOffset_) >= pHTileHandles_.size())
		{
			break;
		}

		DrawGraph(
			pickX * TILE_WIDTH + offsetX_,
			pickY * TILE_HEIGHT + offsetY_,
			pHTileHandles_[i + tileOffset_],// pickY * TILE_ROW_COUNT + pickX
			TRUE);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0x00);

	if (selectedIndex_ != -1)
	{
		int selectedX_ = selectedIndex_ % SHOW_TILE_COLUMN_COUNT;
		int selectedY_ = selectedIndex_ / SHOW_TILE_COLUMN_COUNT;
		DrawBox(
			selectedX_ * TILE_WIDTH + offsetX_ - SELECTED_FRAME_PADDING, selectedY_ * TILE_HEIGHT + offsetY_ - SELECTED_FRAME_PADDING,
			(selectedX_ + 1) * TILE_WIDTH + offsetX_ + SELECTED_FRAME_PADDING, (selectedY_ + 1) * TILE_HEIGHT + offsetY_ + SELECTED_FRAME_PADDING,
			0xff00ff, TRUE);
		DrawGraph(
			selectedX_ * TILE_WIDTH + offsetX_,
			selectedY_ * TILE_HEIGHT + offsetY_,
			pHTileHandles_[selectedIndex_],// pickY * TILE_ROW_COUNT + pickX
			TRUE);
	}

	if (IsOnCursor())
	{
		int localX{}, localY{};
		GetMosuePointLocal(&localX, &localY);

		int touchTileX{}, touchTileY{};
		grid_.ToTile(localX, localY, &touchTileX, &touchTileY);
		
		DrawBox(offsetX_, offsetY_, offsetX_ + width_, offsetY_ + height_, 0xff0000, FALSE);

		DrawBox(
			touchTileX * TILE_WIDTH + offsetX_, touchTileY * TILE_HEIGHT + offsetY_,
			(touchTileX + 1) * TILE_WIDTH + offsetX_, (touchTileY + 1) * TILE_HEIGHT + offsetY_,
			0x00ffff, FALSE, 4);

		if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
		{
			selectedIndex_ = touchTileY * SHOW_TILE_COLUMN_COUNT + touchTileX + tileOffset_;
		}
	}

}
int MapTip::GetChipIndex(int _hImage)
{
	for (int i = 0; i < pHTileHandles_.size(); i++)
	{
		if (pHTileHandles_[i] == _hImage)
		{
			return i;
		}
	}
	return -1;
}
const int MapTip::SELECTED_FRAME_PADDING{ 5 };

const int MapTip::TILE_COLUMN_COUNT{ 16 };
const int MapTip::TILE_ROW_COUNT{ 12 };

const int MapTip::TILE_WIDTH{ 32 };
const int MapTip::TILE_HEIGHT{ 32 };

const int MapTip::SHOW_TILE_COLUMN_COUNT{ 8 };
const int MapTip::SHOW_TILE_ROW_COUNT{ 0 };
