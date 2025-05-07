#include "Frame.h"

namespace
{
	int TITLE_BAR_HEIGHT{ 30 };
}

Frame::Frame(
	int _offsetX,
	int _offsetY,
	int _width,
	int _height,
	bool _showTitleBar) :
	offsetX_{ _offsetX },
	offsetY_{ _offsetY },
	width_{ _width },
	height_{ _height },
	showTitleBar_{ _showTitleBar },
	isDragging_{ false }
{
}

Frame::~Frame()
{
}

void Frame::Update()
{
	int mouseX{}, mouseY{};
	GetMousePoint(&mouseX, &mouseY);

	isOnCursor_ =
	{
		offsetX_ <= mouseX && mouseX <= offsetX_ + width_ &&
		offsetY_ <= mouseY && mouseY <= offsetY_ + height_
	};

	bool isOnTitleBar = 
	{
		offsetX_ <= mouseX && mouseX <= offsetX_ + width_ &&
		offsetY_ - TITLE_BAR_HEIGHT <= mouseY && mouseY <= offsetY_
	};

	static int mouseDiffX{};
	static int mouseDiffY{};
	if (isOnTitleBar && GetMouseInput() & MOUSE_INPUT_LEFT)
	{
		isDragging_ = true;
		mouseDiffX = mouseX - offsetX_;
		mouseDiffY = mouseY - offsetY_;
	}

	if (isDragging_)
	{
		offsetX_ = mouseX - mouseDiffX;
		offsetY_ = mouseY - mouseDiffY;
		if (!(GetMouseInput() & MOUSE_INPUT_LEFT))
		{
			isDragging_ = false;
		}
	}

	UpdateFrame();
}

void Frame::Draw()
{
	if (showTitleBar_)
	{
		DrawBox(offsetX_, offsetY_, offsetX_ + width_, offsetY_ - TITLE_BAR_HEIGHT, 0xffffff, TRUE);
	}

	DrawFrame();
}
