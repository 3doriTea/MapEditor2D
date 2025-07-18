#pragma once

/// <summary>
/// �I�����Ă���^�C���̏��
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

	int index;  // �^�C���̃C���f�b�N�X
	int handle;  // �^�C���摜�̃n���h��
	int offsetX;  // ���S�^�C�������X
	int offsetY;  // ���S�^�C�������Y
};

using SelectMapChips = std::vector<SelectMapChip>;
