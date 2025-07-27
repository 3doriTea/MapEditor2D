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
	/// �^�b�`����Ă���^�C���̍��W���擾����
	/// </summary>
	/// <param name="_x">�^�C�����W x �|�C���^�n��</param>
	/// <param name="_y">�^�C�����W y �|�C���^�n��</param>
	void GetTouchTilePos(int* _x, int* _y) const;
	/// <summary>
	/// ���ΓI�ȃ^�C�����W�ɕϊ�����
	/// </summary>
	/// <param name="_index">�^�C���̃C���f�b�N�X</param>
	/// <param name="_x">�^�C�����W x</param>
	/// <param name="_y">�^�C�����W y</param>
	void ToLocalTilePos(const int _index, int* _x, int* _y) const;

	int GetTileIndex(const int _localTileX, const int _localTileY) const;

	int GetTileHandle(const int _tileX, const int _tileY) const;

	/// <summary>
	/// �^�C�����W���\���͈͓����`�F�b�N����
	/// </summary>
	/// <param name="_tileX">��΃^�C�����W</param>
	/// <param name="_tileY">��΃^�C�����W</param>
	/// <returns>�^�C�����W���\���͈͓��ł��� true / false</returns>
	bool CheckIsInView(const int _tileX, const int _tileY) const;

private:
	const MapChipConfig config_;

	Grid grid_;
	std::vector<int> pHTileHandles_;

	int tileOffset_;
	//int selectedIndex_;  // �I�𒆂̉摜�C���f�b�N�X
	std::vector<int> selectedIndexes_;
	int showOffsetX_;  // �\�������_
	int showOffsetY_;  // �\�������_

	//std::string mapChipTitle_;  // �`�b�v�̃^�C�g��
	//int mapChipWidth_;          // �`�b�v�̉��J�E���g
	//int mapChipHeight_;          // �`�b�v�̏c�J�E���g

	static const int SELECTED_FRAME_PADDING;
	//static const int TILE_ROW_COUNT;
	//static const int TILE_COLUMN_COUNT;
};

