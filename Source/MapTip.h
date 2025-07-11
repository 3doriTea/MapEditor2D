#pragma once
#include "Frame.h"
#include <vector>
#include "Grid.h"
#include "MapChipConfig.h"


class MapChip : public Frame
{
public:
	MapChip();
	~MapChip();

	bool TryGetSelectedTile(int* _pIndex, int* _pHandle);

	void UpdateFrame() override;
	void DrawFrame() override;

	int GetChipIndex(int _hImage) const;

	/// <summary>
	/// タッチされているタイルの座標を取得する
	/// </summary>
	/// <param name="_x">タイル座標 x ポインタ渡し</param>
	/// <param name="_y">タイル座標 y ポインタ渡し</param>
	void GetTouchTilePos(int* _x, int* _y) const;

	int GetTileIndex(const int _localTileX, const int _localTileY) const;

	int GetTileHandle(const int _tileX, const int _tileY) const;

private:
	const MapChipConfig config_;

	Grid grid_;
	std::vector<int> pHTileHandles_;

	int tileOffset_;
	int selectedIndex_;  // 選択中の画像インデックス
	int showOffsetX_;  // 表示する基準点
	int showOffsetY_;  // 表示する基準点

	//std::string mapChipTitle_;  // チップのタイトル
	//int mapChipWidth_;          // チップの横カウント
	//int mapChipHeight_;          // チップの縦カウント

	static const int SELECTED_FRAME_PADDING;
	//static const int TILE_ROW_COUNT;
	//static const int TILE_COLUMN_COUNT;
};

