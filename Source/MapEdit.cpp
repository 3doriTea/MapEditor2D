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
	myMap_(config_.MAPEDIT_VIEW_Y * config_.MAPEDIT_VIEW_X, -1),
	grid_{ config_.TILE_PIX_SIZE * config_.MAPEDIT_VIEW_X, config_.TILE_PIX_SIZE * config_.MAPEDIT_VIEW_Y, 32, 32 },
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
			.Load("MapChip", "MAPEDIT_VIEW_X")
			.Load("MapChip", "MAPEDIT_VIEW_Y")
			.Build()
	}
{

	SetOffset(50, 50);
	SetSize(config_.TILE_PIX_SIZE * config_.MAPEDIT_VIEW_X, config_.TILE_PIX_SIZE * config_.MAPEDIT_VIEW_Y);

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

	SetTitle("マップ塗るキャンバス");
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
	// コントロールキーが押されている
	const bool ON_CONTROL_KEY{ Input::IsKey(KEY_INPUT_LCONTROL) };

	if (ON_CONTROL_KEY && Input::IsKeyDown(KEY_INPUT_S))
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

	if (ON_CONTROL_KEY && Input::IsKeyDown(KEY_INPUT_R))
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
	for (int y = 0; y < config_.MAPEDIT_VIEW_Y; y++)
	{
		for (int x = 0; x < config_.MAPEDIT_VIEW_X; x++)
		{
			const int INDEX = y * config_.MAPEDIT_VIEW_X + x;
			DrawGraph(offsetX_ + x * config_.TILE_PIX_SIZE, offsetY_ + y * config_.TILE_PIX_SIZE,
				myMap_[INDEX], TRUE);
		}
	}

#if USE_BOX_GRID
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(0xff * 0.5f));
	for (int y = 0; y < config_.MAPEDIT_VIEW_Y; y++)
	{
		DrawLine(
			offsetX_, y * config_.TILE_PIX_SIZE + offsetY_,
			config_.MAPEDIT_VIEW_X * config_.TILE_PIX_SIZE + offsetX_,
			y * config_.TILE_PIX_SIZE + offsetY_,
			0xffffff, 1);
	}

	for (int x = 0; x < config_.MAPEDIT_VIEW_X; x++)
	{
		DrawLine(
			x * config_.TILE_PIX_SIZE + offsetX_, offsetY_,
			x * config_.TILE_PIX_SIZE + offsetX_, config_.MAPEDIT_VIEW_Y * config_.TILE_PIX_SIZE + offsetY_,
			0xffffff, 1);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0x00);
#endif

	DrawBox(  // 周りの枠線を描画する
		offsetX_, offsetY_,
		config_.MAPEDIT_VIEW_X * config_.TILE_PIX_SIZE + offsetX_, config_.MAPEDIT_VIEW_Y * config_.TILE_PIX_SIZE + offsetY_,
		0xff0000, FALSE, 3
	);

	int localX{}, localY{};
	GetMousePointLocal(&localX, &localY);

	int touchTileX{}, touchTileY{};
	grid_.ToTile(localX, localY, &touchTileX, &touchTileY);

	if (localX < 0 || localY < 0)
	{
		SetDescription("");
		return;  // 範囲外なら選択枠表示しない
	}

	if (false == (
		0 <= touchTileX && touchTileX < config_.MAPEDIT_VIEW_X &&
		0 <= touchTileY && touchTileY < config_.MAPEDIT_VIEW_Y))
	{
		SetDescription("");
		return;  // 範囲外なら選択枠を表示しない
	}
	
	// 選択中のタイルを持っている
	const bool HAS_SELECTED{ selectedIndexes_.size() > 0 };
	const bool ON_LSHIFT{ Input::IsKey(KEY_INPUT_LSHIFT) };
	const bool ON_F_KEY{ Input::IsKey(KEY_INPUT_F) };

	if (HAS_SELECTED)
	{
		// 選択タイルを持っているなら
		SetDescription("マウス左");
	
		if (ON_LSHIFT && ON_F_KEY)
		{
			AddDescription("クリックで隣接する同じタイルを全部消せるよ！");
		}
		else if (ON_LSHIFT)
		{
			AddDescription("ドラッグで消せるよ！(F)+で塗りつぶし");
		}
		else if (ON_F_KEY)
		{
			AddDescription("クリックで塗りつぶせるよ！(LShift)+でタイル消し");
		}
		else  // なにもおされていない
		{
			AddDescription("ドラッグで塗れるよ！(F)+で塗りつぶし (LShift)+でタイル消し");
		}
	}
	else  // 選択タイルを持っていない
	{
		SetDescription("マウスドラッグで範囲選択できるよ！");
	}

	// 左クリック中
	if (Input::IsMouse(MOUSE_INPUT_LEFT))
	{
		if (HAS_SELECTED)  // 選択中のタイルがあるなら
		{
			// 選択中の全タイルを処理していく
			for (auto itr = selectedIndexes_.begin(); itr != selectedIndexes_.end(); itr++)
			{
				int tile{ pHTileHandles_[(*itr).index]};

				// 削除モード
				if (ON_LSHIFT)
				{
					tile = -1;
				}

				// 基準となるインデックス
				const int INDEX_PIVOT{ touchTileY * config_.MAPEDIT_VIEW_X + touchTileX };

				// 基準タイルしか選択されていない かつ 塗りつぶしが押されているなら
				if (ON_F_KEY && selectedIndexes_.size() == 1)
				{
					Fill(tile, myMap_[INDEX_PIVOT], INDEX_PIVOT);
				}
				// 1タイルだけ変更
				else
				{
					const int INDEX{ (touchTileY + (*itr).offsetY) * config_.MAPEDIT_VIEW_X + (touchTileX + (*itr).offsetX) };

					if (IsSafeMove(INDEX_PIVOT, INDEX))
					{
						myMap_[INDEX] = tile;

					}
				}
			}
		}
		else  // 選択中のタイルがない
		{
			
		}
	}

	// 水色枠を描画するよ
	DrawBox(
		touchTileX * config_.TILE_PIX_SIZE + offsetX_, touchTileY * config_.TILE_PIX_SIZE + offsetY_,
		(touchTileX + 1) * config_.TILE_PIX_SIZE + offsetX_, (touchTileY + 1) * config_.TILE_PIX_SIZE + offsetY_,
		COLOR::CYAN, FALSE, 8);
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
	Fill(_fillHImage, _checkHImage, ToSafeNeighbor(_pickIndex, +config_.MAPEDIT_VIEW_X));
	Fill(_fillHImage, _checkHImage, ToSafeNeighbor(_pickIndex, -config_.MAPEDIT_VIEW_X));
}

int MapEdit::ToSafeNeighbor(const int _from, const int _to) const
{
	int moved = _from + _to;

	// 安全ではない移動に関しては配置しない
	if (IsSafeMove(_from, moved) == false)
	{
		return -1;
	}

	return moved;
}

bool MapEdit::IsSafeMove(const int _from, const int _moved) const
{
	int fromX{}, fromY{};
	ToPoint(_from, &fromX, &fromY);

	int movedX{}, movedY{};
	ToPoint(_moved, &movedX, &movedY);

	// 2次元配列的に飛んでいる場所に関しては配置できない
	if (std::abs(fromX - movedX) > 1 || std::abs(fromY - movedY) > 1)
	{
		return false;
	}

	// そもそも一次元で範囲外ならだめ
	if (_moved < 0 || myMap_.size() <= _moved)
	{
		return false;
	}

	return true;
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

void MapEdit::ToPoint(const int _index, int* _x, int* _y) const
{
	*_x = _index % config_.MAPEDIT_VIEW_X;
	*_y = _index / config_.MAPEDIT_VIEW_X;
}

int MapEdit::ToOffsetIndex(
	const int _pivotX, const int _pivotY,
	const int _addX, const int _addY) const
{
	//_addX

	return 0;
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

	ofs << config_.MAPEDIT_VIEW_X << " " << config_.MAPEDIT_VIEW_Y << std::endl;

	ofs << "#DATA" << std::endl;
	for (int y = 0; y < config_.MAPEDIT_VIEW_Y; y++)
	{
		for (int x = 0; x < config_.MAPEDIT_VIEW_X; x++)
		{
			int handle{ myMap_[x + y * config_.MAPEDIT_VIEW_X] };
			if (handle == -1)
			{
				ofs << -1;
			}
			else
			{
				ofs << GetChipIndex(handle);
			}

			if (x != config_.MAPEDIT_VIEW_X - 1)
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
