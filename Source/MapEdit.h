#pragma once
#include "Frame.h"
#include <vector>
#include "Grid.h"
#include <map>
#include "MapChipConfig.h"
#include "EditorCommon.h"


class MapEdit : public Frame
{
public:
	MapEdit();
	~MapEdit();

	void UpdateFrame() override;
	void DrawFrame() override;

	//void SetSelectedIndex(const int _index) { selectedIndex_ = _index; }
	void SetSelectedIndexes(const SelectMapChips& _selectedIndexes) { selectedIndexes_ = _selectedIndexes; }


private:
	/// <summary>
	/// 同じタイルを埋めるように塗る
	/// </summary>
	/// <param name="_fillHImage"></param>
	/// <param name="_checkHImage"></param>
	/// <param name="_pickIndex"></param>
	void Fill(const int _fillHImage, const int _checkHImage, int _pickIndex);

	/// <summary>
	/// 安全な4近傍移動に変換する
	/// </summary>
	/// <param name="_from">移動元</param>
	/// <param name="_to">!!移動量!! !!相対的な移動量!!</param>
	/// <returns>移動先 / 安全ではない場合、-1を返す</returns>
	int ToSafeNeighbor(const int _from, const int _to) const;
	/// <summary>
	/// 安全な移動か
	/// </summary>
	/// <param name="_from">移動元</param>
	/// <param name="_moved">!!移動先!!</param>
	/// <returns>安全である true / false</returns>
	bool IsSafeMove(const int _from, const int _moved) const;

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

	void ToPoint(const int _index, int* _x, int* _y) const;

	int ToOffsetIndex(
		const int _pivotX, const int _pivotY,
		const int _addX, const int _addY) const;

	void SaveMapData(const std::string& _filePath);
	void LoadMapData(const std::string& _filePath);
private:
	MapChipConfig config_;

	Grid grid_;
	std::vector<int> pHTileHandles_;
	std::vector<int> myMap_;

	std::map<int, int> indexMap_;

	//int selectedIndex_;
	SelectMapChips selectedIndexes_;

	static const int TILE_COLUMN_COUNT;

	//static const int EDIT_TILE_COLUMN_COUNT;
	//static const int EDIT_TILE_ROW_COUNT;
};

