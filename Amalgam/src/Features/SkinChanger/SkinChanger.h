#pragma once
#include "../../SDK/SDK.h"
#include "../../Utils/Hash/FNV1A.h"

namespace attributes
{
	constexpr uint16_t paintkit_proto_def_index = 834;
	constexpr uint16_t custom_paintkit_seed_lo = 866;
	constexpr uint16_t custom_paintkit_seed_hi = 867;
	constexpr uint16_t has_team_color_paintkit = 745;
	constexpr uint16_t set_item_texture_wear = 725;
	constexpr uint16_t weapon_allow_inspect = 731;
	constexpr uint16_t set_attached_particle_static = 370;
	constexpr uint16_t set_attached_particle = 134;
	constexpr uint16_t is_festivized = 2053;
	constexpr uint16_t is_australium_item = 2027;
	constexpr uint16_t loot_rarity = 2022;
	constexpr uint16_t item_style_override = 542;
	constexpr uint16_t set_turn_to_gold = 150;
	constexpr uint16_t killstreak_tier = 2025;
	constexpr uint16_t killstreak_effect = 2013;
	constexpr uint16_t killstreak_idleeffect = 2014;
	constexpr uint16_t halloween_pumpkin_explosions = 1007;
	constexpr uint16_t halloween_green_flames = 1008;
	constexpr uint16_t halloween_voice_modulation = 1006;
	constexpr uint16_t add_jingle_to_footsteps = 364;
	constexpr uint16_t set_custom_buildmenu = 295;

#define HashCase(str) case FNV1A::HashConst(#str): return str

	inline uint16_t StringToAttribute(std::string str)
	{
		FNV1A_t hash = FNV1A::Hash(str.c_str());

		switch (hash)
		{
			HashCase(paintkit_proto_def_index);
			HashCase(custom_paintkit_seed_lo);
			HashCase(custom_paintkit_seed_hi);
			HashCase(has_team_color_paintkit);
			HashCase(set_item_texture_wear);
			HashCase(weapon_allow_inspect);
			HashCase(set_attached_particle_static);
			HashCase(set_attached_particle);
			HashCase(is_festivized);
			HashCase(is_australium_item);
			HashCase(loot_rarity);
			HashCase(item_style_override);
			HashCase(set_turn_to_gold);
			HashCase(killstreak_tier);
			HashCase(killstreak_effect);
			HashCase(killstreak_idleeffect);
			HashCase(halloween_pumpkin_explosions);
			HashCase(halloween_green_flames);
			HashCase(halloween_voice_modulation);
			HashCase(add_jingle_to_footsteps);
			HashCase(set_custom_buildmenu);
			default:
				return 0;
		}
	}
}

__forceinline float IntToStupidFloat(int desiredValue)
{
	return *reinterpret_cast<float*>(&desiredValue);
}

__forceinline int StupidFloatToInt(float desiredValue)
{
	return *reinterpret_cast<int*>(&desiredValue);
}

struct Attribute
{
	uint16_t attributeIndex;
	float attributeValue;
};

struct SkinInfo
{
	std::vector<Attribute> m_Attributes;
};

class Weapon;

#define MAX_WEAPONS 48

using MyWeapons = std::array<CHandle<Weapon>, MAX_WEAPONS>;

class Player
{
public:
	NETVAR(m_hActiveWeapon, CHandle<Weapon>, "CBaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(m_hMyWeapons, MyWeapons, "CBaseCombatCharacter", "m_hMyWeapons");
};

class Weapon
{
public:
	OFFSET(m_iItemDefinitionIndex, int, 3344);
};

class CSkinChanger
{
	std::unordered_map<int, SkinInfo> m_Skins = {};
	bool m_bForceFullUpdate = false;
	int m_nCurrentWeaponIndex = -1;
	bool m_bInitialSkinLoad = false;

public:
	void RedirectIndex(int& weaponIndex);
	void ApplySkin(Weapon* pWeapon);
	void ApplySkins();

	int GetWeaponIndex() const { return m_nCurrentWeaponIndex; }

	void SetAttribute(int index, std::string attributeStr, float value);
	void RemoveAttribute(int index, std::string attributeStr);
	inline const SkinInfo& GetSkinInfo(int index)
	{
		if (m_Skins.find(index) == m_Skins.end())
		{
			static SkinInfo empty = {};
			return empty;
		}

		return m_Skins[index];
	}

	void Save();
	void Load();
};

ADD_FEATURE(CSkinChanger, SkinChanger);