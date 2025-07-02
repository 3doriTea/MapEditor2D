#pragma once
#include "Frame.h"
#include <vector>
#include "Grid.h"


class MapTip : public Frame
{
public:
	MapTip();
	~MapTip();

	bool TryGetSelectedTile(int* _pIndex, int* _pHandle);

	void UpdateFrame() override;
	void DrawFrame() override;

	int GetChipIndex(int _hImage);

private:
	Grid grid_;
	std::vector<int> pHTileHandles_;

	int tileOffset_;
	int selectedIndex_;  // 選択中の画像インデックス

	static const int SELECTED_FRAME_PADDING;
	static const int TILE_ROW_COUNT;
	static const int TILE_COLUMN_COUNT;
	static const int TILE_WIDTH;
	static const int TILE_HEIGHT;
	static const int SHOW_TILE_ROW_COUNT;
	static const int SHOW_TILE_COLUMN_COUNT;
};

