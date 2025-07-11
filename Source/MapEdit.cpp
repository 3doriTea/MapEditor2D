#include "MapEdit.h"
#include "Input.h"
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>
#include "MapTip.h"
#include <Windows.h>


#define USE_BOX_GRID 1

namespace
{
	static const int INIT_POSITION_X{ 50 };
	static const int INIT_POSITION_Y{ 50 };

	enum COLOR
	{
		CYAN = 0x00ffff,
	};
}

MapEdit::MapEdit() :
	Frame{ true },
	myMap_(EDIT_TILE_ROW_COUNT * EDIT_TILE_COLUMN_COUNT, -1),
	grid_{ config_.TILE_PIX_SIZE * EDIT_TILE_COLUMN_COUNT, config_.TILE_PIX_SIZE * EDIT_TILE_ROW_COUNT, 32, 32 },
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
	SetOffset(50, 50);
	SetSize(config_.TILE_PIX_SIZE * EDIT_TILE_COLUMN_COUNT, config_.TILE_PIX_SIZE * EDIT_TILE_ROW_COUNT);

	pHTileHandles_ = std::vector<int>(config_.TILES_Y * config_.TILES_X);
	LoadDivGraph(
		"bg.png", config_.TILES_Y * TILE_COLUMN_COUNT,
		TILE_COLUMN_COUNT, config_.TILES_Y,
		config_.TILE_PIX_SIZE, config_.TILE_PIX_SIZE,
		pHTileHandles_.data());

	indexMap_.clear();
	for (auto itr = pHTileHandles_.begin(); itr != pHTileHandles_.end(); itr++)
	{
		indexMap_.insert({ *itr, itr - pHTileHandles_.begin() });
	}

	SetTitle("マップを塗りつぶしていくタイルキャンバス");
}

MapEdit::~MapEdit()
{
	/*for (int i = 0; i < config_.TILES_Y * TILE_COLUMN_COUNT; i++)
	{
		DeleteGraph(pHTileHandles_[i]);
	}*/
}

void MapEdit::UpdateFrame()
{
	if (Input::IsKey(KEY_INPUT_LCONTROL) && Input::IsKeyDown(KEY_INPUT_S))
	{
		OPENFILENAME of{};

		char filePath[MAX_PATH]{ "map.txt" };

		of.lStructSize = sizeof(OPENFILENAME);
		of.hwndOwner = NULL;
		of.hInstance = NULL;
		of.Flags = NULL;
		of.lpstrFilter = "全てのファイル (*.*)\0*.*\0";
		of.nFilterIndex = 0;
		of.lpstrFile = filePath;
		of.nMaxFile = MAX_PATH;
		of.Flags = OFN_OVERWRITEPROMPT;


		if (GetSaveFileName(&of))
		{
			printfDx("PATH:%s\n", filePath);
			SaveMapData(filePath);
		}
		else
		{
			printfDx("セーブキャンセル\n");
			// キャンセルされた
		}
	}
	if (Input::IsKey(KEY_INPUT_LCONTROL) && Input::IsKeyDown(KEY_INPUT_R))
	{
		OPENFILENAME of{};

		char filePath[MAX_PATH]{ "map.txt" };

		of.lStructSize = sizeof(OPENFILENAME);
		of.hwndOwner = NULL;
		of.hInstance = NULL;
		of.Flags = NULL;
		of.lpstrFilter = "全てのファイル (*.*)\0*.*\0";
		of.nFilterIndex = 0;
		of.lpstrFile = filePath;
		of.nMaxFile = MAX_PATH;
		of.Flags = OFN_FILEMUSTEXIST;


		if (GetOpenFileName(&of))
		{
			printfDx("PATH:%s\n", filePath);
			LoadMapData(filePath);
		}
		else
		{
			printfDx("ロードキャンセル\n");
			// キャンセルされた
		}
	}
}

void MapEdit::DrawFrame()
{
	for (int y = 0; y < EDIT_TILE_ROW_COUNT; y++)
	{
		for (int x = 0; x < EDIT_TILE_COLUMN_COUNT; x++)
		{
			const int INDEX = y * EDIT_TILE_COLUMN_COUNT + x;
			DrawGraph(offsetX_ + x * config_.TILE_PIX_SIZE, offsetY_ + y * config_.TILE_PIX_SIZE,
				myMap_[INDEX], TRUE);
		}
	}

#if USE_BOX_GRID
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(0xff * 0.5f));
	for (int y = 0; y < EDIT_TILE_ROW_COUNT; y++)
	{
		DrawLine(
			offsetX_, y * config_.TILE_PIX_SIZE + offsetY_,
			EDIT_TILE_COLUMN_COUNT * config_.TILE_PIX_SIZE + offsetX_,
			y * config_.TILE_PIX_SIZE + offsetY_,
			0xffffff, 1);
	}

	for (int x = 0; x < EDIT_TILE_COLUMN_COUNT; x++)
	{
		DrawLine(
			x * config_.TILE_PIX_SIZE + offsetX_, offsetY_,
			x * config_.TILE_PIX_SIZE + offsetX_, EDIT_TILE_ROW_COUNT * config_.TILE_PIX_SIZE + offsetY_,
			0xffffff, 1);
		/*DrawBox(
			x * TILE_WIDTH + offsetX_, y * TILE_HEIGHT + offsetY_,
			(x + 1) * TILE_WIDTH + offsetX_, (y + 1) * TILE_HEIGHT + offsetY_,
			0xffffff, FALSE);*/
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0x00);
#endif

	DrawBox(  // 周りの枠線を描画する
		offsetX_, offsetY_,
		EDIT_TILE_COLUMN_COUNT * config_.TILE_PIX_SIZE + offsetX_, EDIT_TILE_ROW_COUNT * config_.TILE_PIX_SIZE + offsetY_,
		0xff0000, FALSE, 3
	);

#if USE_BOX_GRID
#else
	for (int y = 0; y < EDIT_TILE_ROW_COUNT; y++)
	{
		for (int x = 0; x < EDIT_TILE_COLUMN_COUNT; x++)
		{
			/*DrawBox(
				x * TILE_WIDTH + offsetX_, y * TILE_HEIGHT + offsetY_,
				(x + 1) * TILE_WIDTH + offsetX_, (y + 1) * TILE_HEIGHT + offsetY_,
				0xffffff, FALSE);*/
		}
	}
#endif

	int localX{}, localY{};
	GetMousePointLocal(&localX, &localY);

	int touchTileX{}, touchTileY{};
	grid_.ToTile(localX, localY, &touchTileX, &touchTileY);

	if (localX < 0 || localY < 0)
	{
		return;  // 範囲外なら選択枠表示しない
	}

	if (false == (
		0 <= touchTileX && touchTileX < EDIT_TILE_COLUMN_COUNT &&
		0 <= touchTileY && touchTileY < EDIT_TILE_ROW_COUNT))
	{
		return;  // 範囲外なら選択枠を表示しない
	}

	DrawBox(
		touchTileX * config_.TILE_PIX_SIZE + offsetX_, touchTileY * config_.TILE_PIX_SIZE + offsetY_,
		(touchTileX + 1) * config_.TILE_PIX_SIZE + offsetX_, (touchTileY + 1) * config_.TILE_PIX_SIZE + offsetY_,
		COLOR::CYAN, FALSE, 8);

	if (selectedIndex_ == -1)
	{
		return;
	}

	// 左クリック中
	if (Input::IsMouse(MOUSE_INPUT_LEFT))
	{
		int tile{ pHTileHandles_[selectedIndex_] };

		// 削除モード
		if (Input::IsKey(KEY_INPUT_LSHIFT))
		{
			tile = -1;
		}

		// 塗りつぶし
		if (Input::IsKey(KEY_INPUT_F))
		{
			const int INDEX{ touchTileY * EDIT_TILE_COLUMN_COUNT + touchTileX };
			Fill(tile, myMap_[INDEX], INDEX);
		}
		// 1タイルだけ変更
		else
		{
			myMap_[touchTileY * EDIT_TILE_COLUMN_COUNT + touchTileX] = tile;
		}
	}
}

void MapEdit::Fill(const int _fillHImage, const int _checkHImage, int _pickIndex)
{
	if (_pickIndex < 0 || myMap_.size() <= _pickIndex)
	{
		return;
	}

	if (myMap_[_pickIndex] != _checkHImage
		|| myMap_[_pickIndex] == _fillHImage)
	{
		return;
	}

	myMap_[_pickIndex] = _fillHImage;

	// 4方向に再帰的
	Fill(_fillHImage, _checkHImage, ToSafeNeighbor(_pickIndex, +1));
	Fill(_fillHImage, _checkHImage, ToSafeNeighbor(_pickIndex, -1));
	Fill(_fillHImage, _checkHImage, ToSafeNeighbor(_pickIndex, +EDIT_TILE_COLUMN_COUNT));
	Fill(_fillHImage, _checkHImage, ToSafeNeighbor(_pickIndex, -EDIT_TILE_COLUMN_COUNT));
}

int MapEdit::ToSafeNeighbor(const int _from, const int _to)
{
	int moved = _from + _to;

	int fromX{ _from % EDIT_TILE_COLUMN_COUNT };
	int fromY{ _from / EDIT_TILE_COLUMN_COUNT };

	int movedX{ moved % EDIT_TILE_COLUMN_COUNT };
	int movedY{ moved / EDIT_TILE_COLUMN_COUNT };

	if (std::abs(fromX - movedX) > 1 || std::abs(fromY - movedY) > 1)
	{
		return -1;
	}

	return moved;
}

int MapEdit::GetChipIndex(int _hImage)
{
	if (indexMap_.count(_hImage) <= 0)
	{
		return -1;
	}
	return indexMap_[_hImage];
}

int MapEdit::GetChipHandle(int _index)
{
	if (_index < 0 || pHTileHandles_.size() <= _index)
	{
		return -1;
	}
	return pHTileHandles_[_index];
}

void MapEdit::SaveMapData(const std::string& _filePath)
{
	printfDx("File saved.\n");

	std::ofstream ofs{ _filePath };

	MapChip* mapTip = FindGameObject<MapChip>();

	//ofs << "data1" << " " << "data2" << std::endl;

	/*for (auto& map : myMap_)
	{
		ofs << map << ",";
	}*/

	ofs << "#TinyMapData" << std::endl;

	ofs << EDIT_TILE_COLUMN_COUNT << " " << EDIT_TILE_ROW_COUNT << std::endl;

	ofs << "#DATA" << std::endl;
	for (int y = 0; y < EDIT_TILE_ROW_COUNT; y++)
	{
		for (int x = 0; x < EDIT_TILE_COLUMN_COUNT; x++)
		{
			int handle{ myMap_[x + y * EDIT_TILE_COLUMN_COUNT] };
			if (handle == -1)
			{
				ofs << -1;
			}
			else
			{
				ofs << GetChipIndex(handle);
			}

			if (x != EDIT_TILE_COLUMN_COUNT - 1)
			{
				ofs << ",";
			}
		}
		ofs << std::endl;
	}


	ofs.close();
}

void MapEdit::LoadMapData(const std::string& _filePath)
{
	std::ifstream ifs{ _filePath };

	if (!ifs.is_open())
	{
		ifs.close();
		printfDx("読み込みエラー");
		return;
	}

	std::string pick{};
	while (pick != "#TinyMapData")
	{
		if (ifs.eof())
		{
			ifs.close();
			MessageBox(nullptr, "読み込みに失敗しました。ファイル形式が違います。",  "読み込みエラー", MB_OK | MB_ICONWARNING);
			//printfDx("読み込みエラー\n");
			return;
		}
		std::getline(ifs, pick);
	}

	/*while (pick != "#HEAD")
	{
		if (ifs.eof())
		{
			ifs.close();
			printfDx("読み込みエラー\n");
			return;
		}
		std::getline(ifs, pick);
	}*/

	std::getline(ifs, pick, ' ');
	int sizeX{ std::stoi(pick) };
	std::getline(ifs, pick);
	int sizeY{ std::stoi(pick) };

	while (pick != "#DATA")
	{
		if (ifs.eof())
		{
			ifs.close();
			printfDx("読み込みエラー\n");
			return;
		}
		std::getline(ifs, pick);
	}

	myMap_.clear();
	for (int i = 0; i < sizeX * sizeY; i++)
	{
		if ((i + 1) % sizeX == 0)
		{
			std::getline(ifs, pick);
		}
		else
		{
			std::getline(ifs, pick, ',');
		}

		myMap_.push_back(GetChipHandle(std::stoi(pick)));
	}
	printfDx("File loaded.\n");
}

const int MapEdit::TILE_COLUMN_COUNT{ 16 };

const int MapEdit::EDIT_TILE_COLUMN_COUNT{ 20 };
const int MapEdit::EDIT_TILE_ROW_COUNT{ 20 };
