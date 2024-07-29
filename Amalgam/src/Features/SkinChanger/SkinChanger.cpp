#include "SkinChanger.h"

#include <array>
#include <format>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "../Configs/Configs.h"

MAKE_SIGNATURE(GetItemSchema, "client.dll", "48 83 EC ? E8 ? ? ? ? 48 83 C0 ? 48 83 C4 ? C3 CC CC CC", 0x0);
MAKE_SIGNATURE(CEconItemSchema_GetAttributeDefinition, "client.dll", "89 54 24 ? 53 48 83 EC ? 48 8B D9 48 8D 54 24 ? 48 81 C1 ? ? ? ? E8 ? ? ? ? 8B D0 3B 83 ? ? ? ? 73 ? 8B 83 ? ? ? ? 83 F8 ? 74 ? 3B D0 7F ? 48 81 C3 ? ? ? ? 44 8B C2 83 FA ? 74 ? 48 8B 03 8B CA", 0x0);
MAKE_SIGNATURE(CAttributeList_SetRuntimeAttributeValue, "client.dll", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 48 8B EC 48 83 EC ? 44 8B 49", 0x0);

class CEconItemAttribute
{
public:
	void* pad = 0;
	unsigned int m_iAttributeDefinitionIndex;
	union
	{
		int m_iRawValue32;
		float m_flValue;
	};
	int m_nRefundableCurrency = 0;

	inline CEconItemAttribute(uint16_t iAttributeDefinitionIndex, float flValue)
	{
		m_iAttributeDefinitionIndex = iAttributeDefinitionIndex;
		m_flValue = flValue;
	}
};

class CAttributeList
{
public:
	void* pad;
	CUtlVector<CEconItemAttribute, CUtlMemory<CEconItemAttribute>> m_Attributes;
	void* m_pManager;

	inline void AddAttribute(int iIndex, float flValue)
	{
		if (m_Attributes.Count() > 14)
			return;

		CEconItemAttribute attr(iIndex, flValue);

		m_Attributes.AddToTail(attr);
	}

	using GetItemSchemaFN = void* (__fastcall*)();
	using GetAttributeDefinitionFN = void* (__fastcall*)(void*, int);
	using SetRuntimeAttributeValueFN = void(__fastcall*)(CAttributeList*, void*, float);

	void SetAttribute(int index, float value)
	{
		auto schema = reinterpret_cast<GetItemSchemaFN>(S::GetItemSchema())();

		auto attributeDefinition = reinterpret_cast<GetAttributeDefinitionFN>(S::CEconItemSchema_GetAttributeDefinition())(schema, index);
		if (!attributeDefinition)
			return;

		reinterpret_cast<SetRuntimeAttributeValueFN>(S::CAttributeList_SetRuntimeAttributeValue())(this, attributeDefinition, value);
	}
};

#define Redirect(from, to) case from: { nWeaponIndex = to; break; }

void CSkinChanger::RedirectIndex(int& nWeaponIndex)
{
	switch (nWeaponIndex)
	{
		//Redirect(Misc_t_FryingPan, Misc_t_GoldFryingPan);
		Redirect(Soldier_m_RocketLauncher, Soldier_m_RocketLauncherR);
		Redirect(Scout_m_Scattergun, Scout_m_ScattergunR);
		Redirect(Pyro_m_FlameThrower, Pyro_m_FlameThrowerR);
		Redirect(Demoman_m_GrenadeLauncher, Demoman_m_GrenadeLauncherR);
		Redirect(Demoman_s_StickybombLauncher, Demoman_s_StickybombLauncherR);
		Redirect(Heavy_m_Minigun, Heavy_m_MinigunR);
		Redirect(Engi_t_Wrench, Engi_t_WrenchR);
		Redirect(Medic_s_MediGun, Medic_s_MediGunR);
		Redirect(Sniper_m_SniperRifle, Sniper_m_SniperRifleR);
		Redirect(Sniper_s_SMG, Sniper_s_SMGR);
		Redirect(Spy_t_Knife, Spy_t_KnifeR);
		Redirect(Spy_m_Revolver, Spy_m_RevolverR);
		Redirect(Engi_s_EngineersPistol, Engi_s_PistolR);
		Redirect(Soldier_s_SoldiersShotgun, Soldier_s_ShotgunR);
		Redirect(Pyro_s_PyrosShotgun, Pyro_s_ShotgunR);
		Redirect(Heavy_s_HeavysShotgun, Heavy_s_ShotgunR);
		Redirect(Engi_m_EngineersShotgun, Engi_m_ShotgunR);
		Redirect(Scout_t_Bat, Scout_t_BatR);
		Redirect(Soldier_t_Shovel, Soldier_t_ShovelR);
		Redirect(Pyro_t_FireAxe, Pyro_t_FireAxeR);
		Redirect(Demoman_t_Bottle, Demoman_t_BottleR);
		Redirect(Medic_t_Bonesaw, Medic_t_BonesawR);
		Redirect(Sniper_t_Kukri, Sniper_t_KukriR);
		default: break;
	}
}

void CSkinChanger::ApplySkin(Weapon* pWeapon)
{
	if (!pWeapon)
		return;

	int& nWeaponIndex = pWeapon->m_iItemDefinitionIndex();
	RedirectIndex(nWeaponIndex);

	auto attributeList = reinterpret_cast<CAttributeList*>(reinterpret_cast<std::uintptr_t>(pWeapon) + 3512);
	if (!attributeList)
		return;

//#ifdef _DEBUG
	if (attributeList->m_Attributes.Count() > 0 && m_Skins.find(nWeaponIndex) == m_Skins.end())
	{
		// This weapon seems to already have a skin applied, but we don't have it in our map
		// Let's print out what attributes it has
		SDK::Output("SkinChanger-DEBUG", std::format("Weapon that needs to have different pre-filled attributes: {}", nWeaponIndex).c_str(), { 255, 0, 0, 255 });

		for (const auto& attribute : attributeList->m_Attributes)
		{
			SDK::Output("SkinChanger-DEBUG", std::format("Attribute: {} Value: {}", attribute.m_iAttributeDefinitionIndex, attribute.m_flValue).c_str(), {255, 0, 0, 255});
		}
	}
//#endif

	auto PreFilledAttributeCount = [&](int index) -> int
		{
			// Most weapons have no attributes, some have more than one.
			// Seems all snipers have this "no_jump" attribute
			switch (index)
			{
				case Sniper_m_TheBazaarBargain:
				case Sniper_m_SniperRifle:
				case Sniper_m_SniperRifleR:
					return 1;

				default: return 0;
			}
		};

	// If we have attributes, we've already applied all the attributes we want
	if (attributeList->m_Attributes.Count() > PreFilledAttributeCount(m_nCurrentWeaponIndex))
		return;

	// Not a weapon we plan to add attributes to
	if (m_Skins.find(nWeaponIndex) == m_Skins.end())
		return;

	// Apply the attributes if we have requested attributes for it
	const auto& vecAttributes = m_Skins[nWeaponIndex].m_Attributes;
	if (vecAttributes.empty())
		return;

	for (const auto& attribute : vecAttributes)
		attributeList->SetAttribute(attribute.attributeIndex, attribute.attributeValue);
}

void CSkinChanger::ApplySkins()
{
	if (!m_bInitialSkinLoad)
	{
		Load();
		m_bInitialSkinLoad = true;
	}

	auto pLocal = (Player*)I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer());
	if (!pLocal)
		return;

	auto pWeapon = pLocal->m_hActiveWeapon().Get();
	if (m_bForceFullUpdate)
	{
		I::ClientState->ForceFullUpdate();
		m_bForceFullUpdate = false;
	}

	if (!pWeapon)
		return;

	int& nWeaponIndex = pWeapon->m_iItemDefinitionIndex();
	RedirectIndex(nWeaponIndex);

	m_nCurrentWeaponIndex = nWeaponIndex;

	const auto& m_hMyWeapons = pLocal->m_hMyWeapons();
	for (int i = 0; m_hMyWeapons[i].IsValid(); i++)
	{
		auto pWeapon = m_hMyWeapons[i].Get();
		if (!pWeapon)
			continue;

		ApplySkin(pWeapon);
	}
}

void CSkinChanger::SetAttribute(int index, std::string attributeStr, float value)
{
	if (index == -1)
		return;

	uint16_t attributeIndex = attributes::StringToAttribute(attributeStr);

	if (attributeIndex == attributes::paintkit_proto_def_index)
		value = IntToStupidFloat(static_cast<int>(value));

	if (m_Skins.find(index) == m_Skins.end())
		m_Skins[index] = SkinInfo();

	// Check if attribute already exists, if so, update it
	bool bFound = false;

	for (auto& attribute : m_Skins[index].m_Attributes)
	{
		if (attribute.attributeIndex == attributeIndex)
		{
			attribute.attributeValue = value;

			bFound = true;
			break;
		}
	}

	if (!bFound)// Attribute doesn't exist, add it
		m_Skins[index].m_Attributes.push_back({ attributeIndex, value });

	m_bForceFullUpdate = true;
}

void CSkinChanger::RemoveAttribute(int index, std::string attributeStr)
{
	if (m_Skins.find(index) == m_Skins.end())
		return;

	auto& attributes = m_Skins[index].m_Attributes;

	uint16_t attributeIndex = attributes::StringToAttribute(attributeStr);

	// Find attribute
	for (auto it = attributes.begin(); it != attributes.end(); ++it)
	{
		if (it->attributeIndex == attributeIndex)
		{
			attributes.erase(it);
			m_bForceFullUpdate = true;

			return;
		}
	}
}

void CSkinChanger::Save()
{
	try
	{
		boost::property_tree::ptree writeTree;

		for (const auto& skin : m_Skins)
		{
			int index = skin.first;
			if (index == -1)
				continue;

			const auto& vecAttributes = skin.second.m_Attributes;
			std::string strIndex = std::to_string(index);

			boost::property_tree::ptree attributes_tree;
			for (const auto& attribute : vecAttributes)
			{
				attributes_tree.put(std::to_string(attribute.attributeIndex), attribute.attributeValue);
			}

			writeTree.add_child(strIndex, attributes_tree);
		}

		write_json(F::Configs.sConfigPath + "\\Core\\Skins.json", writeTree);
	}
	catch (...) {}
}

void CSkinChanger::Load()
{
	try
	{
		if (std::filesystem::exists(F::Configs.sConfigPath + "\\Core\\Skins.json"))
		{
			boost::property_tree::ptree readTree;
			read_json(F::Configs.sConfigPath + "\\Core\\Skins.json", readTree);
			m_Skins.clear();

			for (const auto& item : readTree)
			{
				int index = std::stoi(item.first);
				if (index == -1)
					continue;

				const auto& vecAttributes = item.second;

				for (const auto& attributeItem : vecAttributes)
				{
					int attributeIndex = std::stoi(attributeItem.first);
					float attributeValue = attributeItem.second.get_value<float>();

					if (attributeIndex == attributes::paintkit_proto_def_index)
						attributeValue = static_cast<float>(StupidFloatToInt(attributeValue));

					m_Skins[index].m_Attributes.push_back({ static_cast<uint16_t>(attributeIndex), attributeValue });

					switch (attributeIndex)
					{
						case attributes::paintkit_proto_def_index: { Vars::Misc::SkinChanger::SkinIndex.Value = attributeValue; break; }
						case attributes::set_attached_particle: { Vars::Misc::SkinChanger::UnusualIndex.Value = attributeValue; break; }
						case attributes::killstreak_idleeffect: { Vars::Misc::SkinChanger::SheenIndex.Value = attributeValue; break; }
						case attributes::killstreak_effect: { Vars::Misc::SkinChanger::EyeIndex.Value = attributeValue; break; }
					}
				}
			}
		}
	}
	catch (...) {}
}