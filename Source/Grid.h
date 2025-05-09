#pragma once

class Grid
{
public:
	Grid(
		const int _widthPix, const int _heightPix,
		const int _tileSizeX, const int _tileSizeY);
	~Grid();

	/// <summary>
	/// グリッド上座標をタイル座標に変換する 
	/// </summary>
	/// <param name="_srcPositionX">グリッド上座標 X</param>
	/// <param name="_srcPositionY">グリッド上座標 Y</param>
	/// <param name="_resultTileX">タイル座標が格納される X</param>
	/// <param name="_resultTileY">タイル座標が格納される Y</param>
	void ToTile(
		const int _srcPositionX, const int _srcPositionY,
		int* _resultTileX, int* _resultTileY) const;

	/// <summary>
	/// タイル座標をグリッド上座標に変換する
	/// </summary>
	/// <param name="_srcTileX">タイル座標 X</param>
	/// <param name="_srcTileY">タイル座標 Y</param>
	/// <param name="_resultPositionX">グリッド上座標が格納される X</param>
	/// <param name="_resultPositionY">グリッド上座標が格納される Y</param>
	void ToPosition(
		const int _srcTileX, const int _srcTileY,
		int* _resultPositionX, int* _resultPositionY) const;

private:
	int widthPix_;  // グリッド上座標の最大値 X
	int heightPix_;  // グリッド上座標の最大値 Y
	int tileSizeX_;  // 1つのタイルのサイズ X
	int tileSizeY_;  // 1つのタイルのサイズ Y
};
