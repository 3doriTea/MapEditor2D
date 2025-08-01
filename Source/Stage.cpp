#include "Stage.h"
#include <cassert>
#include "MapTip.h"
#include "MapEdit.h"
#include "Input.h"
#include "EditorCommon.h"

namespace
{
	const int MAP_WIDTH{ 32 };
	const int MAP_HEIGHT{ 22 };

	static inline int ToImageId(int _x, int _y)
	{
		return Stage::TILE_COLUMN_COUNT * _y + _x;
	}

	static inline int Ti(int _x, int _y)
	{
		return ToImageId(_x, _y);
	}

	int map[MAP_HEIGHT][MAP_WIDTH]
	/*{
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
		{ Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0), Ti(0, 0) },
	};*/
	{
		/*#include "../map.txt"*/
		/*{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,137 },
		{ 0,114,99,115,0,0,0,6,6,6,0,22,7,7,7,7,23,0,0,0,6,6,6,0,0,0,0,0,0,0,0,137 },
		{ 0,99,99,99,0,0,0,0,0,22,7,7,7,7,7,7,7,23,0,0,0,0,0,0,0,0,0,6,0,0,0,137 },
		{ 0,130,99,131,0,6,0,0,0,7,7,7,7,7,7,7,7,39,0,0,0,0,0,0,0,0,0,6,0,0,0,137 },
		{ 0,0,0,0,6,6,0,0,0,38,7,7,7,7,7,7,39,0,0,0,0,0,0,0,0,0,0,6,0,0,0,137 },
		{ 0,0,0,6,6,6,0,0,0,0,0,102,102,102,102,102,102,102,0,0,0,24,9,8,0,0,0,6,6,0,0,137 },
		{ 0,0,6,6,0,0,0,0,0,0,102,102,107,107,107,107,107,102,0,0,24,9,9,0,0,0,0,6,6,0,0,137 },
		{ 0,0,0,0,0,4,0,0,0,102,102,107,107,107,107,107,107,102,102,0,9,9,9,25,0,0,0,6,6,0,0,137 },
		{ 0,0,0,0,0,5,21,20,5,102,107,107,107,107,107,107,107,107,102,0,40,9,9,41,0,0,0,0,0,0,0,137 },
		{ 0,55,55,55,20,37,36,37,36,102,107,107,107,107,107,107,107,107,102,0,0,0,0,0,0,0,0,0,0,0,0,137 },
		{ 55,55,55,55,0,0,0,0,0,102,102,107,107,107,107,107,107,107,102,0,0,0,0,0,0,0,0,0,0,0,0,137 },
		{ 55,55,55,55,55,0,0,0,0,0,102,107,107,107,107,107,107,107,102,0,0,8,0,0,0,8,0,0,0,0,0,137 },
		{ 55,55,55,55,55,0,0,0,0,0,102,102,102,102,107,107,102,102,102,0,0,0,0,0,0,0,0,0,6,0,0,137 },
		{ 55,55,55,55,55,0,116,101,117,0,0,0,0,102,102,102,102,0,0,0,0,0,0,8,0,0,0,0,6,0,0,137 },
		{ 55,55,55,0,0,116,101,101,101,0,0,64,65,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,137 },
		{ 0,0,0,0,0,101,101,101,101,0,0,49,49,0,0,0,0,0,0,0,0,144,144,0,0,0,0,6,0,0,0,137 },
		{ 0,6,0,0,0,132,101,101,133,0,0,80,81,0,0,0,0,0,0,0,160,145,145,161,0,0,0,6,0,0,0,137 },
		{ 0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,160,161,0,0,144,145,145,145,145,144,0,6,0,0,0,0,137 },
		{ 0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,176,177,0,0,144,145,145,145,145,144,0,0,0,0,0,0,137 },
		{ 0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,0,176,145,145,177,0,0,0,0,0,0,0,137 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,144,144,0,0,0,0,0,0,0,0,137 },
		{ 137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137,137 },*/
	};
}

Stage::Stage(MapChip* _pMapTip, MapEdit* _pMapEdit) :
	hImageBackground_{ -1 },
	//pHTileHandles_{ nullptr }
	//pHTileHandles_{},
	pMapTip_{ _pMapTip },
	pMapEdit_{ _pMapEdit }
{
	this->SetDrawOrder(10);

	//hImageBackground_ = LoadGraph("bg.png");

	//pHTileHandles_ = new int[TILE_ROW_COUNT * TILE_COLUMN_COUNT];
	//pHTileHandles_ = std::vector<int>(TILE_ROW_COUNT * TILE_COLUMN_COUNT);
	//LoadDivGraph(
	//	"bg.png", TILE_ROW_COUNT * TILE_COLUMN_COUNT,
	//	TILE_COLUMN_COUNT, TILE_ROW_COUNT,
	//	TILE_WIDTH, TILE_HEIGHT,
	//	//pHTileHandles_);
	//	pHTileHandles_.data());
	//assert(hImageBackground_ > 0);

	#pragma region 地形生成
	const int CACHE_ARRAY_SIZE{ 256 };
	const int CACHE_VALUE_MAX{ 256 };

	auto generateRandomCache = [&CACHE_ARRAY_SIZE](float* cacheArray) -> void
	{
		for (int i = 0; i < CACHE_ARRAY_SIZE; i++)
		{
			cacheArray[i] = static_cast<float>(rand() % CACHE_VALUE_MAX) / CACHE_VALUE_MAX;
		}
	};

	auto generateRandom = [&generateRandomCache](const int& x, const int& y, const int& solt) -> float
	{
		static float cacheArray[CACHE_ARRAY_SIZE]{};
		generateRandomCache(cacheArray);

		return cacheArray[(x * y + solt) % CACHE_ARRAY_SIZE];
	};
	#pragma endregion

	//int width, height;
	//GetGraphSize(hImageBackground_, &width, &height);
	//tileWidth_ = width / TILE_COLUMN_COUNT;
	//tileHeight_ = height / TILE_ROW_COUNT;
	tileWidth_ = __TILE_PIX_SIZE;
	tileHeight_ = TILE_PIX_SIZE;
}

Stage::~Stage()
{
	//DeleteGraph(hImageBackground_);
	/*for (int i = 0; i < TILE_ROW_COUNT * TILE_COLUMN_COUNT; i++)
	{
		DeleteGraph(pHTileHandles_[i]);
	}*/
	//delete[] pHTileHandles_;
}

void Stage::Update()
{
	/*if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
	{
		printfDx("左押された:");
	}
	if (Input::IsMouse(MOUSE_INPUT_LEFT))
	{
		printfDx("左押されている:");
	}
	if (Input::IsMouseUp(MOUSE_INPUT_LEFT))
	{
		printfDx("左離されてた:");
	}

	if (Input::IsMouseDown(MOUSE_INPUT_RIGHT))
	{
		printfDx("右押された:");
	}
	if (Input::IsMouse(MOUSE_INPUT_RIGHT))
	{
		printfDx("右押されている:");
	}
	if (Input::IsMouseUp(MOUSE_INPUT_RIGHT))
	{
		printfDx("右離されてた:");
	}
	printfDx("\n");*/

	int hImage_{};
	int selectedIndex_{};
	/*if (pMapTip_->TryGetSelectedTile(&selectedIndex_, &hImage_) == false)
	{
		pMapEdit_->SetSelectedIndex(-1);
		return;
	}*/
	SelectMapChips selectedMapChips{};
	//if (pMapTip_->TryGetSelectedMapChip(pickedMapChips) == false)
	//{
	//	//pMapEdit_->SetSelectedIndex(-1);
	//	pMapEdit_->SetSelectedIndex(-1);
	//	return;
	//}
	bool hasSelectedMapChip{ pMapTip_->TryGetSelectedMapChip(selectedMapChips) };
	pMapEdit_->SetSelectedIndexes(selectedMapChips);

	if (hasSelectedMapChip)
	{
		int mx{}, my{};
		GetMousePoint(&mx, &my);

		int sizeX{}, sizeY{};
		GetGraphSize(selectedMapChips.begin()->handle, &sizeX, &sizeY);
		for (auto&& selected : selectedMapChips)
		{
			DrawGraph(
				mx - sizeX / 2 + sizeX * selected.offsetX,
				my - sizeY / 2 + sizeY * selected.offsetY,
				selected.handle,
				TRUE);
		}
	}
}

void Stage::Draw()
{
	//for (int pickX = 0; pickX < MAP_WIDTH; pickX++)
	//{
	//	for (int pickY = 0; pickY < MAP_HEIGHT; pickY++)
	//	{
	//		int data{ map[pickY][pickX] };
	//		/*
	//		int sourceTileX{ data % TILE_COLUMN_COUNT };
	//		int sourceTileY{ data / TILE_COLUMN_COUNT };*/
	//		/*DrawRectGraph(
	//			pickX * tileWidth_, pickY * tileHeight_,
	//			sourceTileX * tileWidth_, sourceTileY * tileHeight_,
	//			tileWidth_, tileHeight_,
	//			hImageBackground_, TRUE);*/
	//		/*DrawRectGraph(
	//			pickX * tileWidth_, pickY * tileHeight_,
	//			sourceTileX * tileWidth_, sourceTileY * tileHeight_,
	//			tileWidth_, tileHeight_,
	//			hImageBackground_, TRUE);*/
	//		assert(pHTileHandles_[data] > -1);
	//		DrawGraph(
	//			pickX * tileWidth_,
	//			pickY * tileHeight_,
	//			pHTileHandles_[data],// pickY * TILE_ROW_COUNT + pickX
	//			TRUE);
	//	}
	//}
}

const int Stage::TILE_ROW_COUNT{ 12 };
const int Stage::TILE_COLUMN_COUNT{ 16 };

const int Stage::__TILE_PIX_SIZE{ 32 };
const int Stage::TILE_PIX_SIZE{ 32 };
