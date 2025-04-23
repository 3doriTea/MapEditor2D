#pragma once
#include "../Library/GameObject.h"

class Stage : public GameObject
{
public:
	Stage();
	~Stage();

	void Update() override;
	void Draw() override;

private:
	int hImageBackground_;
	int tileWidth_;
	int tileHeight_;

public:
	static const int TILE_ROW_COUNT;
	static const int TILE_COLUMN_COUNT;
};
