#pragma once
#include "../Library/GameObject.h"
#include <vector>

class MapTip;
class MapEdit;

class Stage : public GameObject
{
public:
	Stage(MapTip* _pMapTip, MapEdit* _pMapEdit);
	~Stage();

	void Update() override;
	void Draw() override;

private:
	MapTip* pMapTip_;
	MapEdit* pMapEdit_;
	int hImageBackground_;
	int tileWidth_;
	int tileHeight_;
	//int* pHTileHandles_;
	std::vector<int> pHTileHandles_;

public:
	static const int TILE_ROW_COUNT;
	static const int TILE_COLUMN_COUNT;
	static const int TILE_WIDTH;
	static const int TILE_HEIGHT;
};
