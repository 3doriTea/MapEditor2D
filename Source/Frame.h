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
	int offsetX_;  // フレームの基点x座標
	int offsetY_;  // フレームの基点y座標
	int width_;  // フレームの横幅
	int height_;  // フレームの縦幅

private:
	bool showTitleBar_;  // フレームの上の方にタイトルバーを表示するか
	bool isOnCursor_;  // カーソルがフレームの中にあるかどうか
	bool isDragging_;  // フレームをドラッグ中か
};

