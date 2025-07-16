#pragma once
#include "Frame.h"
#include <vector>
#include "Grid.h"
#include <map>
#include "MapChipConfig.h"

class MapEdit : public Frame
{
public:
	MapEdit();
	~MapEdit();

	void UpdateFrame() override;
	void DrawFrame() override;

	void SetSelectedIndex(const int _index) { selectedIndex_ = _index; }

private:
	/// <summary>
	/// 同じタイルを埋めるように塗る
	/// </summary>
	/// <param name="_fillHImage"></param>
	/// <param name="_checkHImage"></param>
	/// <param name="_pickIndex"></param>
	void Fill(const int _fillHImage, const int _checkHImage, int _pickIndex);

	int ToSafeNeighbor(const int _from, const int _to);

	/// <summary>
	/// ハンドル to インデックス
	/// </summary>
	/// <param name="_hImage"></param>
	/// <returns></returns>
	int GetChipIndex(int _hImage);
	/// <summary>
	/// インデックス to ハンドル
	/// </summary>
	/// <param name="_index"></param>
	/// <returns></returns>
	int GetChipHandle(int _index);

	void SaveMapData(const std::string& _filePath);
	void LoadMapData(const std::string& _filePath);
private:
	MapChipConfig config_;

	Grid grid_;
	std::vector<int> pHTileHandles_;
	std::vector<int> myMap_;

	std::map<int, int> indexMap_;

	int selectedIndex_;

	static const int TILE_COLUMN_COUNT;

	//static const int EDIT_TILE_COLUMN_COUNT;
	//static const int EDIT_TILE_ROW_COUNT;
};

