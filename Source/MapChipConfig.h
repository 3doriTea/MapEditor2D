#pragma once
#include <string>
#include <map>
#include <Windows.h>

namespace
{
	static const size_t PROFILE_BUFFER_SIZE{ 128 };
}

struct MapChipConfig
{
	int TILE_PIX_SIZE;         // 1タイルのサイズ(ピクセル)
	int TILES_X;               // 全体のタイル数 x
	int TILES_Y;               // 全体のタイル数 y
	int MAPCHIP_VIEW_X;        // 表示されるタイル数 x
	int MAPCHIP_VIEW_Y;        // 表示されるタイル数 y
	int MAPCHIP_FRAME_WIDTH;   // マップチップ画面フレームの横幅
	int MAPCHIP_FRAME_HEIGHT;  // マップチップ画面フレームの高さ
};

class MapChipConfigElement
{
public:
	MapChipConfigElement(const std::string& _section, const std::string _name) :
		name_{ _name },
		section_{ _section }
	{}

	virtual void Load(MapChipConfig& _config) = 0;

	inline int LoadInt()
	{
		return GetPrivateProfileInt(
			section_.c_str(),
			name_.c_str(),
			-1,
			"./setup.ini");
	}

	inline std::string LoadStr()
	{
		char buffer[PROFILE_BUFFER_SIZE]{};
		GetPrivateProfileString(
			section_.c_str(),
			name_.c_str(),
			"",
			buffer,
			PROFILE_BUFFER_SIZE,
			"./setup.ini");

		return buffer;
	}

private:
	std::string name_;
	std::string section_;
};
#pragma region element マクロ
#define MAPCHIP_CONFIG_ELEMENT(Section, KeyName) \
class KeyName : public MapChipConfigElement \
{ \
public: \
	inline KeyName() : \
		MapChipConfigElement{ Section, #KeyName } \
	{ \
	} \
	inline void Load(MapChipConfig& _config) override \
	{\
		_config.KeyName = LoadInt();\
	}\
};
#pragma endregion

#pragma region element追加マクロ

#pragma endregion

#pragma region element 実装

MAPCHIP_CONFIG_ELEMENT("MapChip", TILE_PIX_SIZE);
MAPCHIP_CONFIG_ELEMENT("MapChip", TILES_X);
MAPCHIP_CONFIG_ELEMENT("MapChip", TILES_Y);
MAPCHIP_CONFIG_ELEMENT("MapChip", MAPCHIP_VIEW_X);
MAPCHIP_CONFIG_ELEMENT("MapChip", MAPCHIP_VIEW_Y);
MAPCHIP_CONFIG_ELEMENT("MapChip", MAPCHIP_FRAME_WIDTH);
MAPCHIP_CONFIG_ELEMENT("MapChip", MAPCHIP_FRAME_HEIGHT);

#pragma endregion

#pragma region MapChip

#define MAPCHIP_INSERT(Section, Key)\
elements_.insert({ { Section, #Key }, new Key{} });

#pragma endregion

class MapChipConfigBuilder
{
public:
	inline MapChipConfigBuilder() :
		elements_{},
		config_{}
	{
		MAPCHIP_INSERT("MapChip", TILE_PIX_SIZE);
		MAPCHIP_INSERT("MapChip", TILES_X);
		MAPCHIP_INSERT("MapChip", TILES_Y);
		MAPCHIP_INSERT("MapChip", MAPCHIP_VIEW_X);
		MAPCHIP_INSERT("MapChip", MAPCHIP_VIEW_Y);
		MAPCHIP_INSERT("MapChip", MAPCHIP_FRAME_WIDTH);
		MAPCHIP_INSERT("MapChip", MAPCHIP_FRAME_HEIGHT);
	}

	inline int LoadInt(const std::string& _section, const std::string& _name)
	{
		return GetPrivateProfileInt(
			_section.c_str(),
			_name.c_str(),
			-1,
			"./setup.ini");
	}

	inline std::string LoadStr(const std::string& _section, const std::string& _name)
	{
		char buffer[PROFILE_BUFFER_SIZE]{};
		GetPrivateProfileString(
			_section.c_str(),
			_name.c_str(),
			"",
			buffer,
			PROFILE_BUFFER_SIZE,
			"./setup.ini");

		return buffer;
	}

	inline MapChipConfigBuilder& Load(
		const std::string& _section,
		const std::string& _name)
	{
		elements_[{ _section, _name }]->Load(config_);
		return *this;
	}

	inline const MapChipConfig Build() const
	{
		return config_;
	}

private:
	MapChipConfig config_;

	std::map<std::pair<std::string, std::string>, MapChipConfigElement*> elements_;
};
