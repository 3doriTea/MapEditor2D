#include "Frame.h"
#include "Input.h"
#include "Screen.h"


namespace
{
	int TITLE_BAR_HEIGHT{ 30 };
	int TITLE_TEXT_OFFSET_X{ 4 };
	int TITLE_FONT_SIZE{ 16 };
	int DESC_FONT_SIZE{ 10 };
}

Frame::Frame(bool _showTitleBar) :
	offsetX_{ 0 },
	offsetY_{ 0 },
	width_{ 0 },
	height_{ 0 },
	showTitleBar_{ _showTitleBar },
	isDragging_{ false },
	frameTitle_{ "" },
	frameDescription_{ "" }
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

			// âÊñ äOÇ…çsÇ¡ÇƒÇ¢ÇÈÇ»ÇÁÉMÉäÉMÉäÇ…ñﬂÇ∑
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
		
		// É^ÉCÉgÉãÇÃï`âÊ
		SetFontSize(TITLE_FONT_SIZE);
		int textOffsetY = -TITLE_BAR_HEIGHT + (TITLE_BAR_HEIGHT - GetFontSize()) / 2;
		DrawFormatString(TITLE_TEXT_OFFSET_X + offsetX_, textOffsetY + offsetY_, 0x000000, "%s", frameTitle_.c_str());
		
		// ê‡ñæï∂ÇÃï`âÊ
		SetFontSize(DESC_FONT_SIZE);
		int drawDescWidth{ GetDrawFormatStringWidth("%s", frameDescription_.c_str()) };
		int descTextOffsetY{ -TITLE_BAR_HEIGHT + (TITLE_BAR_HEIGHT - GetFontSize()) / 2 };
		DrawFormatString(width_ - TITLE_TEXT_OFFSET_X - drawDescWidth + offsetX_, descTextOffsetY + offsetY_, 0x000000, "%s", frameDescription_.c_str());

		SetFontSize(DEFAULT_FONT_SIZE);
	}

	DrawFrame();
}

Frame* Frame::druggingFrame_{ nullptr };
