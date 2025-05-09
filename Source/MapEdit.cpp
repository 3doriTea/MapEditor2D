#include "MapEdit.h"

#define USE_BOX_GRID 1

MapEdit::MapEdit() :
	Frame{ 50, 50, TILE_WIDTH * EDIT_TILE_COLUMN_COUNT, TILE_HEIGHT * EDIT_TILE_ROW_COUNT, true },
	myMap_(EDIT_TILE_ROW_COUNT * EDIT_TILE_COLUMN_COUNT, -1),
	grid_{ TILE_WIDTH * EDIT_TILE_COLUMN_COUNT, TILE_HEIGHT * EDIT_TILE_ROW_COUNT, 32, 32 }
{
}

MapEdit::~MapEdit()
{
	/*for (int i = 0; i < TILE_ROW_COUNT * TILE_COLUMN_COUNT; i++)
	{
		DeleteGraph(pHTileHandles_[i]);
	}*/
}

void MapEdit::UpdateFrame()
{
	/*if (IsOnCursor())
	{
		printfDx("in");
	}*/
}

void MapEdit::DrawFrame()
{
#if USE_BOX_GRID
	for (int y = 0; y < EDIT_TILE_ROW_COUNT; y++)
	{
		DrawLine(
			offsetX_, y * TILE_HEIGHT + offsetY_,
			EDIT_TILE_COLUMN_COUNT * TILE_WIDTH + offsetX_,
			y * TILE_WIDTH + offsetY_,
			0xffffff, 1);
	}

	for (int x = 0; x < EDIT_TILE_COLUMN_COUNT; x++)
	{
		DrawLine(
			x * TILE_WIDTH + offsetX_, offsetY_,
			x * TILE_WIDTH + offsetX_, EDIT_TILE_ROW_COUNT * TILE_WIDTH + offsetY_,
			0xffffff, 1);
		/*DrawBox(
			x * TILE_WIDTH + offsetX_, y * TILE_HEIGHT + offsetY_,
			(x + 1) * TILE_WIDTH + offsetX_, (y + 1) * TILE_HEIGHT + offsetY_,
			0xffffff, FALSE);*/
	}
#endif

	DrawBox(  // Žü‚è‚Ì˜gü‚ð•`‰æ‚·‚é
		offsetX_, offsetY_,
		EDIT_TILE_COLUMN_COUNT * TILE_WIDTH + offsetX_, EDIT_TILE_ROW_COUNT * TILE_HEIGHT + offsetY_,
		0xff0000, FALSE, 3
	);

#if USE_BOX_GRID
#else
	for (int y = 0; y < EDIT_TILE_ROW_COUNT; y++)
	{
		for (int x = 0; x < EDIT_TILE_COLUMN_COUNT; x++)
		{
			/*DrawBox(
				x * TILE_WIDTH + offsetX_, y * TILE_HEIGHT + offsetY_,
				(x + 1) * TILE_WIDTH + offsetX_, (y + 1) * TILE_HEIGHT + offsetY_,
				0xffffff, FALSE);*/
		}
	}
#endif

	int localX{}, localY{};
	GetMosuePointLocal(&localX, &localY);

	int touchTileX{}, touchTileY{};
	grid_.ToTile(localX, localY, &touchTileX, &touchTileY);

	int touchTileLocalX{}, touchTileLocalY{};
	grid_.ToPosition(touchTileX, touchTileY, &touchTileLocalX, &touchTileLocalY);

	printfDx("(%d, %d)\n", touchTileX, touchTileY);
	DrawBox(
		touchTileX * TILE_WIDTH + offsetX_, touchTileY * TILE_HEIGHT + offsetY_,
		(touchTileX + 1) * TILE_WIDTH + offsetX_, (touchTileY + 1) * TILE_HEIGHT + offsetY_,
		0x00ffff, FALSE, 8);
}

const int MapEdit::TILE_ROW_COUNT{ 12 };
const int MapEdit::TILE_COLUMN_COUNT{ 16 };

const int MapEdit::TILE_WIDTH{ 32 };
const int MapEdit::TILE_HEIGHT{ 32 };

const int MapEdit::EDIT_TILE_COLUMN_COUNT{ 20 };
const int MapEdit::EDIT_TILE_ROW_COUNT{ 20 };
