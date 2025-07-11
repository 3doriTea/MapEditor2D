#pragma once
#include "../Library/GameObject.h"
#include <vector>

class MapChip;
class MapEdit;

class Stage : public GameObject
{
public:
	Stage(MapChip* _pMapTip, MapEdit* _pMapEdit);
	~Stage();

	void Update() override;
	void Draw() override;

private:
	int selectedTileIndex_;
	int selectedTileHandle_;
	MapChip* pMapTip_;
	MapEdit* pMapEdit_;
	int hImageBackground_;
	int tileWidth_;
	int tileHeight_;
	//int* pHTileHandles_;
	//std::vector<int> pHTileHandles_;

public:
	static const int TILE_ROW_COUNT;
	static const int TILE_COLUMN_COUNT;
	static const int __TILE_PIX_SIZE;
	static const int TILE_PIX_SIZE;
};
