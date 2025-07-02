#include "Frame.h"
#include "Input.h"
#include "Screen.h"


namespace
{
	int TITLE_BAR_HEIGHT{ 30 };
	int TITLE_TEXT_OFFSET_X{ 4 };
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
	isDragging_{ false },
	frameTitle_{ "" }
{
}

Frame::~Frame()
{
}

void Frame::Update()
{
	if (druggingFrame_ != nullptr && druggingFrame_ != this)
	{
		return;
	}

	int mouseX{}, mouseY{};
	GetMousePoint(&mouseX, &mouseY);

	cursorLocalPositionX = mouseX - offsetX_;
	cursorLocalPositionY = mouseY - offsetY_;

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
	if (isOnTitleBar && Input::IsMouseDown(MOUSE_INPUT_LEFT)
		&& isDragging_ == false)
	{
		isDragging_ = true;
		mouseDiffX = mouseX - offsetX_;
		mouseDiffY = mouseY - offsetY_;
		druggingFrame_ = this;
	}

	if (isDragging_)
	{
		offsetX_ = mouseX - mouseDiffX;
		offsetY_ = mouseY - mouseDiffY;
		if (!(GetMouseInput() & MOUSE_INPUT_LEFT))
		{
			isDragging_ = false;
			druggingFrame_ = nullptr;

			// ‰æ–ÊŠO‚És‚Á‚Ä‚¢‚é‚È‚çƒMƒŠƒMƒŠ‚É–ß‚·
			if (offsetX_ < 0)
			{
				offsetX_ = 0;
			}
			else if (offsetX_ > Screen::WIDTH - width_)
			{
				offsetX_ = Screen::WIDTH - width_;
			}
			if (offsetY_ - TITLE_BAR_HEIGHT < 0)
			{
				offsetY_ = TITLE_BAR_HEIGHT;
			}
			else if (offsetY_ > Screen::HEIGHT)
			{
				offsetY_ = Screen::HEIGHT;
			}
		}
	}

	UpdateFrame();
}

void Frame::Draw()
{
	if (showTitleBar_)
	{
		DrawBox(offsetX_, offsetY_, offsetX_ + width_, offsetY_ - TITLE_BAR_HEIGHT, 0xffffff, TRUE);
		int textOffsetY = -TITLE_BAR_HEIGHT + (TITLE_BAR_HEIGHT - GetFontSize()) / 2;
		DrawFormatString(TITLE_TEXT_OFFSET_X + offsetX_, textOffsetY + offsetY_, 0x000000, "%s", frameTitle_.c_str());
	}

	DrawFrame();
}

Frame* Frame::druggingFrame_{ nullptr };
