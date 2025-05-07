#pragma once
#include "../Library/GameObject.h"

class Frame : public GameObject
{
public:
	Frame(
		int _offsetX,
		int _offsetY,
		int _width,
		int _height,
		bool _showTitleBar = false);
	virtual ~Frame();

	void Update() override;
	void Draw() override;
	virtual void UpdateFrame() = 0;
	virtual void DrawFrame() = 0;

	virtual void IsOnClick() {}

	inline bool IsOnCursor() const { return isOnCursor_; }

protected:
	int offsetX_;  // �t���[���̊�_x���W
	int offsetY_;  // �t���[���̊�_y���W
	int width_;  // �t���[���̉���
	int height_;  // �t���[���̏c��

private:
	bool showTitleBar_;  // �t���[���̏�̕��Ƀ^�C�g���o�[��\�����邩
	bool isOnCursor_;  // �J�[�\�����t���[���̒��ɂ��邩�ǂ���
	bool isDragging_;  // �t���[�����h���b�O����
};

