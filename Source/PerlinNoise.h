#pragma once
#include <cstdint>

class PerlinNoise
{
public:
	static void SetRand(const int& _seed);
	static float Noize(const float& _x, const float& _y);
private:
	//uint8_t 
};
