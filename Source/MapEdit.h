#pragma once
#include "Frame.h"
#include <vector>

class MapEdit : public Frame
{
public:
	MapEdit();
	~MapEdit();

	void UpdateFrame() override;
	void DrawFrame() override;

private:
	std::vector<int> pHTileHandles_;

	static const int TILE_ROW_COUNT;
	static const int TILE_COLUMN_COUNT;
	static const int TILE_WIDTH;
	static const int TILE_HEIGHT;

	static const int EDIT_TILE_COLUMN_COUNT;
	static const int EDIT_TILE_ROW_COUNT;
};

