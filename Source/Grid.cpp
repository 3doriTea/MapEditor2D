#include "Grid.h"

Grid::Grid(
	const int _widthPix, const int _heightPix,
	const int _tileSizeX, const int _tileSizeY) :
	widthPix_{ _widthPix },
	heightPix_{ _heightPix },
	tileSizeX_{ _tileSizeX },
	tileSizeY_{ _tileSizeY }
{
}

Grid::~Grid()
{
}

void Grid::ToTile(const int _srcPositionX, const int _srcPositionY, int* _resultTileX, int* _resultTileY) const
{
	*_resultTileX = _srcPositionX / tileSizeX_;
	*_resultTileY = _srcPositionY / tileSizeY_;
}

void Grid::ToPosition(const int _srcTileX, const int _srcTileY, int* _resultPositionX, int* _resultPositionY) const
{
	*_resultPositionX = _srcTileX * tileSizeX_;
	*_resultPositionY = _srcTileX * tileSizeY_;
}
