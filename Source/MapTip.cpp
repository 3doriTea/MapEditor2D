#define NOMINMAX
#include "MapTip.h"
#include "Screen.h"
#include "Input.h"
#include <Windows.h>
#include <cassert>
#include <algorithm>
#include "MapChipConfig.h"

namespace
{
	//static const size_t PROFILE_BUFFER_SIZE{ 128 };
}

MapChip::MapChip() :
	Frame{ false },
	pHTileHandles_{},
	grid_{ config_.MAPCHIP_VIEW_X, config_.MAPCHIP_VIEW_X, config_.TILE_PIX_SIZE, config_.TILE_PIX_SIZE },
	selectedIndex_{ -1 },
	showOffsetX_{ 0 },
	showOffsetY_{ 0 },
	config_
	{
		MapChipConfigBuilder{}
			.Load("MapChip", "TILE_PIX_SIZE")
			.Load("MapChip", "TILES_X")
			.Load("MapChip", "TILES_Y")
			.Load("MapChip", "MAPCHIP_VIEW_X")
			.Load("MapChip", "MAPCHIP_VIEW_Y")
			.Load("MapChip", "MAPCHIP_FRAME_WIDTH")
			.Load("MapChip", "MAPCHIP_FRAME_HEIGHT")
			.Build()
	}
{
	SetOffset(Screen::WIDTH - config_.TILE_PIX_SIZE * config_.MAPCHIP_VIEW_X, 0);
	SetSize(config_.TILE_PIX_SIZE * config_.MAPCHIP_VIEW_X, Screen::HEIGHT);

	pHTileHandles_ = std::vector<int>(config_.TILES_Y * config_.TILES_X);
	LoadDivGraph(
		"bg.png", config_.TILES_Y * config_.TILES_X,
		config_.TILES_X, config_.TILES_Y,
		config_.TILE_PIX_SIZE, config_.TILE_PIX_SIZE,
		pHTileHandles_.data());
}

MapChip::~MapChip()
{
	for (int i = 0; i < config_.TILES_Y * config_.TILES_X; i++)
	{
		DeleteGraph(pHTileHandles_[i]);
	}
}

bool MapChip::TryGetSelectedTile(int* _pIndex, int* _pHandle)
{
	if (selectedIndex_ == -1)
	{
		return false;
	}

	*_pIndex = selectedIndex_;
	*_pHandle = pHTileHandles_[selectedIndex_];

	return true;
}

void MapChip::UpdateFrame()
{
	// 右クリックキャンセル
	if (Input::IsMouseDown(MOUSE_INPUT_RIGHT))
	{
		selectedIndex_ = -1;
	}

	if (IsOnCursor())
	{
		if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
		{
			int touchTileX{}, touchTileY{};
			GetTouchTilePos(&touchTileX, &touchTileY);
			selectedIndex_ = GetTileIndex(touchTileX, touchTileY);//touchTileY * config_.MAPCHIP_VIEW_X + touchTileX + tileOffset_;
		}

		int scroll = GetMouseWheelRotVol();

		if (Input::IsKey(KEY_INPUT_LSHIFT))
		{
			showOffsetX_ -= scroll;
			if (showOffsetX_ < 0)
			{
				showOffsetX_ = 0;
			}
			else if (showOffsetX_ >= config_.TILES_X)
			{
				showOffsetX_ = config_.TILES_X - 1;
			}
		}
		else
		{
			showOffsetY_ -= scroll;
			if (showOffsetY_ < 0)
			{
				showOffsetY_ = 0;
			}
			else if (showOffsetY_ >= config_.TILES_Y)
			{
				showOffsetY_ = config_.TILES_Y - 1;
			}
		}
	}
}

void MapChip::DrawFrame()
{
	// カーソルが入っていないなら半透明に描画する
	if (IsOnCursor() == false)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(0xff * 0.5f));
	}

	// 各タイルの描画
	DrawBox(offsetX_, offsetY_, offsetX_ + width_, offsetY_ + height_, 0xffffff, TRUE);
	
	for (int y = 0; y < config_.TILES_Y; y++)
	{
		int tileY = y + showOffsetY_;
		if (tileY < 0 || config_.TILES_Y <= tileY)
		{
			continue;
		}

		for (int x = 0; x < config_.TILES_X; x++)
		{
			int tileX = x + showOffsetX_;
			if (tileX < 0 || config_.TILES_X <= tileX)
			{
				continue;
			}

			DrawGraph(
				x * config_.TILE_PIX_SIZE + offsetX_,
				y * config_.TILE_PIX_SIZE + offsetY_,
				GetTileHandle(tileX, tileY),// pickY * config_.TILES_Y + pickX
				TRUE);
		}
	}
	
	//for (int i = 0; i < config_.TILES_Y * config_.TILES_X; i++)
	//{
	//	int pickX{ i % 8 };
	//	int pickY{ i / 8 };
	//	if ((i + tileOffset_) >= pHTileHandles_.size())
	//	{
	//		break;
	//	}
	//	DrawGraph(
	//		pickX * config_.TILE_PIX_SIZE + offsetX_,
	//		pickY * config_.TILE_PIX_SIZE + offsetY_,
	//		pHTileHandles_[i + tileOffset_],// pickY * config_.TILES_Y + pickX
	//		TRUE);
	//}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0x00);

	// 選択タイルの表示
	if (selectedIndex_ != -1)
	{
		int selectedX_ = selectedIndex_ % config_.MAPCHIP_VIEW_X;
		int selectedY_ = selectedIndex_ / config_.MAPCHIP_VIEW_X;
		DrawBox(
			selectedX_ * config_.TILE_PIX_SIZE + offsetX_ - SELECTED_FRAME_PADDING, selectedY_ * config_.TILE_PIX_SIZE + offsetY_ - SELECTED_FRAME_PADDING,
			(selectedX_ + 1) * config_.TILE_PIX_SIZE + offsetX_ + SELECTED_FRAME_PADDING, (selectedY_ + 1) * config_.TILE_PIX_SIZE + offsetY_ + SELECTED_FRAME_PADDING,
			0xff00ff, TRUE);
		DrawGraph(
			selectedX_ * config_.TILE_PIX_SIZE + offsetX_,
			selectedY_ * config_.TILE_PIX_SIZE + offsetY_,
			pHTileHandles_[selectedIndex_],// pickY * config_.TILES_Y + pickX
			TRUE);
	}

	// カーソルが選択しているタイルの枠を表示
	if (IsOnCursor())
	{
		int touchTileX{}, touchTileY{};
		GetTouchTilePos(&touchTileX, &touchTileY);

		DrawBox(offsetX_, offsetY_, offsetX_ + width_, offsetY_ + height_, 0xff0000, FALSE);

		DrawBox(
			touchTileX * config_.TILE_PIX_SIZE + offsetX_, touchTileY * config_.TILE_PIX_SIZE + offsetY_,
			(touchTileX + 1) * config_.TILE_PIX_SIZE + offsetX_, (touchTileY + 1) * config_.TILE_PIX_SIZE + offsetY_,
			0x00ffff, FALSE, 4);
	}

}
int MapChip::GetChipIndex(int _hImage) const
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
void MapChip::GetTouchTilePos(int* _x, int* _y) const
{
	int localX{}, localY{};
	GetMousePointLocal(&localX, &localY);
	grid_.ToTile(localX, localY, _x, _y);
}
int MapChip::GetTileIndex(const int _localTileX, const int _localTileY) const
{
	int tileY = _localTileX + showOffsetY_;
	if (tileY < 0 || config_.TILES_Y <= tileY)
	{
		return -1;
	}

	int tileX = _localTileY + showOffsetX_;
	if (tileX < 0 || config_.TILES_X <= tileX)
	{
		return -1;
	}

	return GetChipIndex(GetTileHandle(tileX, tileY));
}
int MapChip::GetTileHandle(const int _tileX, const int _tileY) const
{
	if (_tileX < 0 || config_.TILES_X <= _tileX)
	{
		return -1;
	}
	if (_tileY < 0 || config_.TILES_Y <= _tileY)
	{
		return -1;
	}
	return pHTileHandles_[config_.TILES_X * _tileY + _tileX];
}
const int MapChip::SELECTED_FRAME_PADDING{ 5 };
