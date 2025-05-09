#pragma once

class Grid
{
public:
	Grid(
		const int _widthPix, const int _heightPix,
		const int _tileSizeX, const int _tileSizeY);
	~Grid();

	/// <summary>
	/// �O���b�h����W���^�C�����W�ɕϊ����� 
	/// </summary>
	/// <param name="_srcPositionX">�O���b�h����W X</param>
	/// <param name="_srcPositionY">�O���b�h����W Y</param>
	/// <param name="_resultTileX">�^�C�����W���i�[����� X</param>
	/// <param name="_resultTileY">�^�C�����W���i�[����� Y</param>
	void ToTile(
		const int _srcPositionX, const int _srcPositionY,
		int* _resultTileX, int* _resultTileY) const;

	/// <summary>
	/// �^�C�����W���O���b�h����W�ɕϊ�����
	/// </summary>
	/// <param name="_srcTileX">�^�C�����W X</param>
	/// <param name="_srcTileY">�^�C�����W Y</param>
	/// <param name="_resultPositionX">�O���b�h����W���i�[����� X</param>
	/// <param name="_resultPositionY">�O���b�h����W���i�[����� Y</param>
	void ToPosition(
		const int _srcTileX, const int _srcTileY,
		int* _resultPositionX, int* _resultPositionY) const;

private:
	int widthPix_;  // �O���b�h����W�̍ő�l X
	int heightPix_;  // �O���b�h����W�̍ő�l Y
	int tileSizeX_;  // 1�̃^�C���̃T�C�Y X
	int tileSizeY_;  // 1�̃^�C���̃T�C�Y Y
};
