#pragma once

/// <summary>
/// 選択しているタイルの情報
/// </summary>
struct SelectMapChip
{
	SelectMapChip(
		const int _index,
		const int _handle,
		const int _offsetX,
		const int _offsetY) :
		index{ _index },
		handle{ _handle },
		offsetX{ _offsetX },
		offsetY{ _offsetY }
	{
	}

	int index;  // タイルのインデックス
	int handle;  // タイル画像のハンドル
	int offsetX;  // 中心タイルからのX
	int offsetY;  // 中心タイルからのY
};

using SelectMapChips = std::vector<SelectMapChip>;
