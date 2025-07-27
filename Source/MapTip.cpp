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
	Frame{ true },
	pHTileHandles_{},
	grid_{ config_.MAPCHIP_VIEW_X, config_.MAPCHIP_VIEW_X, config_.TILE_PIX_SIZE, config_.TILE_PIX_SIZE },
	//selectedIndex_{ -1 },
	selectedIndexes_{},
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
	SetTitle("マップチップ");
	SetOffset(Screen::WIDTH - config_.TILE_PIX_SIZE * config_.MAPCHIP_VIEW_X, 30);
	SetSize(config_.TILE_PIX_SIZE * config_.MAPCHIP_VIEW_X, config_.TILE_PIX_SIZE * config_.MAPCHIP_VIEW_Y);

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

bool MapChip::TryGetSelectedMapChip(SelectMapChips& _selectedMapChip)
{
	if (selectedIndexes_.size() <= 0)
	{
		return false;
	}

	_selectedMapChip.clear();

	int pivotChipIndex{ *selectedIndexes_.begin() };
	int pivotX{};
	int pivotY{};

	ToLocalTilePos(pivotChipIndex, &pivotX, &pivotY);

	pivotX += showOffsetX_;
	pivotY += showOffsetY_;

	for (auto&& index : selectedIndexes_)
	{
		int selectedOffsetX{};
		int selectedOffsetY{};

		ToLocalTilePos(index, &selectedOffsetX, &selectedOffsetY);

		selectedOffsetX += showOffsetX_;
		selectedOffsetY += showOffsetY_;

		_selectedMapChip.push_back(
			{
				index,
				pHTileHandles_[index],
				selectedOffsetX - pivotX,
				selectedOffsetY - pivotY,
			});
	}

	return true;
}

//bool MapChip::TryGetSelectedTile(int* _pIndex, int* _pHandle)
//{
//	//if (selectedIndex_ == -1)
//	if (selectedIndexes_.size() <= 0)
//	{
//		return false;
//	}
//
//	//*_pIndex = selectedIndex_;
//
//	*_pIndex = *selectedIndexes_.begin();
//	*_pHandle = pHTileHandles_[*selectedIndexes_.begin()];
//
//	return true;
//}

void MapChip::UpdateFrame()
{
	// 右クリックキャンセル
	if (Input::IsMouseDown(MOUSE_INPUT_RIGHT))
	{
		//selectedIndex_ = -1;
		selectedIndexes_.clear();
	}

	if (IsOnCursor())
	{
		if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
		{
			int touchTileX{}, touchTileY{};
			GetTouchTilePos(&touchTileX, &touchTileY);
			//selectedIndex_ = GetTileIndex(touchTileX, touchTileY);//touchTileY * config_.MAPCHIP_VIEW_X + touchTileX + tileOffset_;

			if (Input::IsKey(KEY_INPUT_LSHIFT) == false)
			{
				selectedIndexes_.clear();
			}

			// 8近傍であるか
			bool isNear8{ true };
			// 基準として選ばれているタイルがある
			if (selectedIndexes_.size() > 0)
			{
				int x{}, y{};
				ToLocalTilePos(selectedIndexes_[0], &x, &y);
				x -= touchTileX;
				y -= touchTileY;
				if (x * x > 1 || y * y > 1)
				{
					isNear8 = false;  // 8近傍ではない！
				}
			}

			if (isNear8)  // 8近傍のときだけ追加
			{
				selectedIndexes_.push_back(GetTileIndex(touchTileX, touchTileY));
			}
		}

		int scroll = GetMouseWheelRotVol();

		int maxViewX{ config_.TILES_X - config_.MAPCHIP_VIEW_X };
		if (maxViewX < 0)
		{
			maxViewX = 0;
		}
		int maxViewY{ config_.TILES_Y - config_.MAPCHIP_VIEW_Y };
		if (maxViewY < 0)
		{
			maxViewY = 0;
		}

		if (Input::IsKey(KEY_INPUT_LSHIFT))
		{
			showOffsetX_ -= scroll;
			if (showOffsetX_ < 0)
			{
				showOffsetX_ = 0;
			}
			else if (showOffsetX_ > maxViewX)
			{
				showOffsetX_ = maxViewX;
			}
		}
		else
		{
			showOffsetY_ -= scroll;
			if (showOffsetY_ < 0)
			{
				showOffsetY_ = 0;
			}
			else if (showOffsetY_ > maxViewY)
			{
				showOffsetY_ = maxViewY;
			}
		}
	}
}

void MapChip::DrawFrame()
{
	DrawBox(offsetX_, offsetY_, offsetX_ + width_, offsetY_ + height_, 0xffffff, TRUE);

	if (IsOnCursor())  // カーソルが入っているとき、枠を赤く表示する
	{
		DrawBox(
			offsetX_ - 1, offsetY_ - 1,
			offsetX_ + width_ + 1, offsetY_ + height_ + 1,
			0xff0000, FALSE);
	}
	else  // カーソルが入っていないなら全体を半透明に描画する
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(0xff * 0.5f));
	}

	/*for (int y = 0; y < config_.MAPCHIP_VIEW_Y; y++)
	{
		for (int x = 0; x < config_.MAPCHIP_VIEW_X; x++)
		{
			
			GetTileHandle(GetTileIndex(x, y))
		}
	}*/
	
	// 各タイルの描画
	for (int y = 0; y < config_.MAPCHIP_VIEW_Y; y++)
	{
		int tileY = y + showOffsetY_;
		if (tileY < 0 || config_.TILES_Y <= tileY)
		{
			continue;
		}

		for (int x = 0; x < config_.MAPCHIP_VIEW_X; x++)
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

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0x00);

	// 選択タイルの表示
	// 選択されているタイルがある
	//if (selectedIndex_ != -1)
	if (selectedIndexes_.size() > 0)
	{
		for (auto&& index : selectedIndexes_)
		{
			int selectedTileX{}, selectedTileY{};
			//ToLocalTilePos(selectedIndex_, &selectedTileX, &selectedTileY);
			ToLocalTilePos(index, &selectedTileX, &selectedTileY);

			// 選択されているタイルが表示範囲内
			if (CheckIsInView(selectedTileX, selectedTileY))
			{
				DrawBox(
					selectedTileX * config_.TILE_PIX_SIZE + offsetX_ - SELECTED_FRAME_PADDING, selectedTileY * config_.TILE_PIX_SIZE + offsetY_ - SELECTED_FRAME_PADDING,
					(selectedTileX + 1) * config_.TILE_PIX_SIZE + offsetX_ + SELECTED_FRAME_PADDING, (selectedTileY + 1) * config_.TILE_PIX_SIZE + offsetY_ + SELECTED_FRAME_PADDING,
					0xff00ff, TRUE);
				DrawGraph(
					selectedTileX * config_.TILE_PIX_SIZE + offsetX_,
					selectedTileY * config_.TILE_PIX_SIZE + offsetY_,
					pHTileHandles_[index],
					TRUE);
			}
		}
	}

	// カーソルが選択しているタイルの枠を表示
	if (IsOnCursor())
	{
		int touchTileX{}, touchTileY{};
		GetTouchTilePos(&touchTileX, &touchTileY);

		// カーソルが選択しているタイルが表示範囲内
		if (CheckIsInView(touchTileX, touchTileY))
		{
			DrawBox(
				touchTileX * config_.TILE_PIX_SIZE + offsetX_, touchTileY * config_.TILE_PIX_SIZE + offsetY_,
				(touchTileX + 1) * config_.TILE_PIX_SIZE + offsetX_, (touchTileY + 1) * config_.TILE_PIX_SIZE + offsetY_,
				0x00ffff, FALSE, 4);
		}
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

void MapChip::ToLocalTilePos(const int _index, int* _x, int* _y) const
{
	*_x = _index % config_.TILES_X - showOffsetX_;
	*_y = _index / config_.TILES_X - showOffsetY_;
}

int MapChip::GetTileIndex(const int _localTileX, const int _localTileY) const
{
	int tileY = _localTileY + showOffsetY_;
	if (tileY < 0 || config_.TILES_Y <= tileY)
	{
		return -1;
	}

	int tileX = _localTileX + showOffsetX_;
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

bool MapChip::CheckIsInView(const int _tileX, const int _tileY) const
{
	return
	{
		0 <= _tileX && _tileX < config_.MAPCHIP_VIEW_X
			&& 0 <= _tileY && _tileY < config_.MAPCHIP_VIEW_Y
	};
}

const int MapChip::SELECTED_FRAME_PADDING{ 5 };
