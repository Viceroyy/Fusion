#include "Core.h"

#include "../SDK/SDK.h"
#include "../Features/Visuals/Materials/Materials.h"
#include "../Features/Configs/Configs.h"
#include "../Features/Commands/Commands.h"
#include "../Features/ImGui/Menu/Menu.h"
#include "../Utils/EventListener/EventListener.h"
#include "../Features/Visuals/Visuals.h"

//#include <iostream>
//#include <fstream>
//
//std::ofstream File;
//const char *szClassName;
//
//void DumpTable(RecvTable *pTable, int nDepth)
//{
//	if (!pTable)
//		return;
//
//	const char *Types[7] = { "int", "float", "Vec3", "Vec2", "const char *", "Array", "void *" };
//
//	if (nDepth == 0)
//		File << "class " << szClassName << "\n{\npublic:\n";
//
//	for (int n = 0; n < pTable->m_nProps; n++)
//	{
//		RecvProp *pProp = pTable->GetProp(n);
//
//		if (!pProp)
//			continue;
//
//		std::string_view sVarName(pProp->m_pVarName);
//
//		if (!sVarName.find("baseclass") || !sVarName.find("0") || !sVarName.find("1") || !sVarName.find("2"))
//			continue;
//
//		const char *szType = Types[pProp->GetType()];
//
//		if (sVarName.find("m_b") == 0 && pProp->GetType() == 0)
//			szType = "bool";
//
//		if (sVarName.find("m_vec") == 0)
//			szType = "Vec3";
//
//		if (sVarName.find("m_h") == 0)
//			szType = "EHANDLE";
//
//		if (pProp->GetOffset())
//			File << "\tNETVAR(" << sVarName << ", " << szType << ", \"" << szClassName << "\", \"" << sVarName << "\");\n";
//
//		if (auto DataTable = pProp->GetDataTable())
//			DumpTable(DataTable, nDepth + 1);
//	}
//
//	if (nDepth == 0)
//		File << "};\n";
//}
//
//void DumpTables()
//{
//	File.open("NETVAR_DUMP_NEW.h");
//
//	for (ClientClass *pClass = I::BaseClientDLL->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
//		szClassName = pClass->m_pNetworkName;
//		DumpTable(pClass->m_pRecvTable, 0);
//	}
//
//	File.close();
//}

__forceinline bool CheckRenderIsCompatible()
{
	// Check for if we are running in DXVK mode.
	if (GetModuleHandleA("dxvk_d3d9.dll"))
	{
		MessageBoxA(nullptr, "You are running with graphics options that Fusion does not support.\n"
			"Please remove -vulkan from your launch options and reinject.", "Error", MB_ICONERROR);
		U::Core.bUnload = true;
		return false;
	}

	// Check for if we are running in DirectX 8.
	auto cvDXLevel = I::CVar->FindVar("mat_dxlevel");
	auto iLevel = cvDXLevel->GetInt();
	if (iLevel < 90)
	{
		std::string fmt = std::format("You are running with graphics options that Fusion does not support.\nPlease remove -dxlevel {} from your launch options and reinject.\0", iLevel);
		MessageBox(nullptr, fmt.c_str(), "Error", MB_ICONERROR);
		U::Core.bUnload = true;
		return false;
	}

	return true;
}

void CCore::Load()
{
	U::Signatures.Initialize();
	U::Interfaces.Initialize();
	//DumpTables();

	U::ConVars.Initialize();

	if (!CheckRenderIsCompatible())
		return;

	U::Hooks.Initialize();
	F::Materials.LoadMaterials();
	F::Commands.Initialize();

	F::EventListener.Setup({ "item_pickup" });

	F::Configs.LoadConfig(F::Configs.sCurrentConfig, false);
	H::Fonts.Reload();
	F::Menu.ConfigLoaded = true;

	I::MatSystemSurface->PlaySound("hl1/fvox/activated.wav");
	SDK::Output("Fusion", "Loaded", { 175, 150, 255, 255 });
}

void CCore::Unload()
{
	if (!bUnload)
	{
		G::Unload = true;

		F::EventListener.Destroy();
		U::Hooks.Unload();
		U::ConVars.Unload();
		F::Materials.UnloadMaterials();

		F::Visuals.RestoreWorldModulation();
		Vars::Visuals::World::SkyboxChanger.Value = "Off"; // hooks won't run, remove here
		if (I::Input->CAM_IsThirdPerson())
		{
			auto pLocal = H::Entities.GetLocal();
			if (pLocal)
			{
				I::Input->CAM_ToFirstPerson();
				pLocal->ThirdPersonSwitch();
			}
		}
		if (auto cl_wpn_sway_interp = U::ConVars.FindVar("cl_wpn_sway_interp"))
			cl_wpn_sway_interp->SetValue(0.f);
		if (auto cl_wpn_sway_scale = U::ConVars.FindVar("cl_wpn_sway_scale"))
			cl_wpn_sway_scale->SetValue(0.f);

		Sleep(250);
	}

	I::MatSystemSurface->PlaySound("hl1/fvox/deactivated.wav");
	SDK::Output("Fusion", "Unloaded", { 175, 150, 255, 255 });
}

bool CCore::ShouldUnload()
{
	return SDK::IsGameWindowInFocus() && GetAsyncKeyState(VK_F11) & 0x8000 || bUnload;
}

MAKE_HOOK(IMaterialSystem_FindTexture, U::Memory.GetVFunc(I::MaterialSystem, 81), ITexture*, __fastcall,
	void* ecx, char const* pTextureName, const char* pTextureGroupName, bool complain, int nAdditionalCreationFlags)
{
	ITexture* const result{ CALL_ORIGINAL(ecx, pTextureName, pTextureGroupName, complain, nAdditionalCreationFlags) };

	if (Vars::Visuals::Other::ThePS2Inator.Value)
	{
		auto replaceWorld = [&]() -> ITexture*
			{
				if (!result
					|| result->IsTranslucent()
					|| !pTextureName
					|| !pTextureGroupName
					|| !std::string_view{ pTextureGroupName }.starts_with(TEXTURE_GROUP_WORLD)
					|| std::string_view{ pTextureName }.find("sky") != std::string_view::npos
					|| std::string_view{ pTextureName }.find("water") != std::string_view::npos)
				{
					return nullptr;
				}

				Vector clr{};

				result->GetLowResColorSample(0.5f, 0.5f, &clr.x);

				unsigned char bits[4]
				{
					static_cast<unsigned char>(clr.x * 255.0f),
					static_cast<unsigned char>(clr.y * 255.0f),
					static_cast<unsigned char>(clr.z * 255.0f),
					255
				};

				return I::MaterialSystem->CreateTextureFromBits(1, 1, 1, IMAGE_FORMAT_RGBA8888, 4, bits);
			};

		if (ITexture* const replacement{ replaceWorld() })
		{
			return replacement;
		}
	}

	return result;
}