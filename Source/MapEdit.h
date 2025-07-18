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
	/// �����^�C���𖄂߂�悤�ɓh��
	/// </summary>
	/// <param name="_fillHImage"></param>
	/// <param name="_checkHImage"></param>
	/// <param name="_pickIndex"></param>
	void Fill(const int _fillHImage, const int _checkHImage, int _pickIndex);

	int ToSafeNeighbor(const int _from, const int _to);

	/// <summary>
	/// �n���h�� to �C���f�b�N�X
	/// </summary>
	/// <param name="_hImage"></param>
	/// <returns></returns>
	int GetChipIndex(int _hImage);
	/// <summary>
	/// �C���f�b�N�X to �n���h��
	/// </summary>
	/// <param name="_index"></param>
	/// <returns></returns>
	int GetChipHandle(int _index);

	int ToOffsetIndex(const int _addX, const int _addY);

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

