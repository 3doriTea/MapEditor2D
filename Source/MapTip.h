#pragma once
#include "Frame.h"
#include <vector>
#include <set>
#include "Grid.h"
#include "MapChipConfig.h"
#include "EditorCommon.h"


class MapChip : public Frame
{
public:
	MapChip();
	~MapChip();

	bool TryGetSelectedMapChip(SelectMapChips& _pickedMapChip);

	void UpdateFrame() override;
	void DrawFrame() override;

	int GetChipIndex(int _hImage) const;

	/// <summary>
	/// タッチされているタイルの座標を取得する
	/// </summary>
	/// <param name="_x">タイル座標 x ポインタ渡し</param>
	/// <param name="_y">タイル座標 y ポインタ渡し</param>
	void GetTouchTilePos(int* _x, int* _y) const;
	/// <summary>
	/// 相対的なタイル座標に変換する
	/// </summary>
	/// <param name="_index">タイルのインデックス</param>
	/// <param name="_x">タイル座標 x</param>
	/// <param name="_y">タイル座標 y</param>
	void ToLocalTilePos(const int _index, int* _x, int* _y) const;

	int GetTileIndex(const int _localTileX, const int _localTileY) const;

	int GetTileHandle(const int _tileX, const int _tileY) const;

	/// <summary>
	/// タイル座標が表示範囲内かチェックする
	/// </summary>
	/// <param name="_tileX">絶対タイル座標</param>
	/// <param name="_tileY">絶対タイル座標</param>
	/// <returns>タイル座標が表示範囲内である true / false</returns>
	bool CheckIsInView(const int _tileX, const int _tileY) const;

private:
	const MapChipConfig config_;

	Grid grid_;
	std::vector<int> pHTileHandles_;

	int tileOffset_;
	//int selectedIndex_;  // 選択中の画像インデックス
	std::vector<int> selectedIndexes_;
	int showOffsetX_;  // 表示する基準点
	int showOffsetY_;  // 表示する基準点

	//std::string mapChipTitle_;  // チップのタイトル
	//int mapChipWidth_;          // チップの横カウント
	//int mapChipHeight_;          // チップの縦カウント

	static const int SELECTED_FRAME_PADDING;
	//static const int TILE_ROW_COUNT;
	//static const int TILE_COLUMN_COUNT;
};

