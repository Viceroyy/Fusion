#pragma once
#include "../Misc/IClientMode.h"
#include "../Misc/CGameEventListener.h"
#include "../../../Utils/Memory/Memory.h"

class CBaseEntity;
class CBasePlayer;
class CViewSetup;

class CBaseHudChat
{
public:
	void ChatPrintf(int pIndex, const char* fmt, ...)
	{
		typedef void(_cdecl* FN)(void*, int, int, const char*, ...);
		reinterpret_cast<FN>(U::Memory.GetVFunc(this, 19))(this, pIndex, 0, fmt);
	}

	void StartMessageMode(int iMessageModeType)
	{
		typedef void(__fastcall* FN)(void*, int);
		reinterpret_cast<FN>(U::Memory.GetVFunc(this, 20))(this, iMessageModeType);
	}
};

class CClientModeShared : public IClientMode, public CGameEventListener
{
public:
	virtual ~CClientModeShared() {};
	virtual void Init() = 0;
	virtual void InitViewport() = 0;
	virtual void VGui_Shutdown() = 0;
	virtual void Shutdown() = 0;
	virtual void LevelInit(const char* newmap) = 0;
	virtual void LevelShutdown(void) = 0;
	virtual void Enable() = 0;
	virtual void Disable() = 0;
	virtual void Layout() = 0;
	virtual void ReloadScheme(bool flushLowLevel) = 0;
	virtual void OverrideView(CViewSetup* pSetup) = 0;
	virtual bool ShouldDrawDetailObjects() = 0;
	virtual bool ShouldDrawEntity(CBaseEntity* pEnt) = 0;
	virtual bool ShouldDrawLocalPlayer(CBasePlayer* pPlayer) = 0;
	virtual bool ShouldDrawViewModel() = 0;
	virtual bool ShouldDrawParticles() = 0;
	virtual bool ShouldDrawCrosshair(void) = 0;
	virtual bool ShouldBlackoutAroundHUD() override = 0;
	virtual HeadtrackMovementMode_t ShouldOverrideHeadtrackControl() override = 0;
	virtual void AdjustEngineViewport(int& x, int& y, int& width, int& height) = 0;
	virtual void PreRender(CViewSetup* pSetup) = 0;
	virtual void PostRender() = 0;
	virtual void PostRenderVGui() = 0;
	virtual void ProcessInput(bool bActive) = 0;
	virtual bool CreateMove(float flInputSampleTime, CUserCmd* cmd) = 0;
	virtual void Update() = 0;
	virtual int KeyInput(int down, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual int HudElementKeyInput(int down, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual void OverrideMouseInput(float* x, float* y) = 0;
	virtual void StartMessageMode(int iMessageModeType) = 0;
	virtual Panel* GetMessagePanel() = 0;
	virtual void ActivateInGameVGuiContext(Panel* pPanel) = 0;
	virtual void DeactivateInGameVGuiContext() = 0;
	virtual bool ShouldDrawFog(void) = 0;
	virtual float GetViewModelFOV(void) = 0;
	virtual Panel* GetViewport() {}
	virtual AnimationController* GetViewportAnimationController() {}
	virtual void FireGameEvent(IGameEvent* event) = 0;
	virtual bool CanRecordDemo(char* errorMsg, int length) const { return true; }
	virtual int HandleSpectatorKeyInput(int down, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual void ComputeVguiResConditions(KeyValues* pkvConditions) override = 0;
	virtual wchar_t* GetServerName() { return nullptr; }
	virtual void SetServerName(wchar_t* name) {}
	virtual wchar_t* GetMapName() { return nullptr; }
	virtual void SetMapName(wchar_t* name) {}
	virtual bool DoPostScreenSpaceEffects(const CViewSetup* pSetup) = 0;
	virtual void DisplayReplayMessage(const char* pLocalizeName, float flDuration, bool bUrgent, const char* pSound, bool bDlg) = 0;
	virtual bool IsInfoPanelAllowed() override { return true; }
	virtual void InfoPanelDisplayed() override {}
	virtual bool IsHTMLInfoPanelAllowed() override { return true; }
	virtual void OnDemoRecordStart(char const* pDemoBaseName) override {}
	virtual void OnDemoRecordStop() override {}

	char szPad[24];
	CBaseHudChat* m_pChatElement;
};

MAKE_INTERFACE_SIGNATURE(CClientModeShared, ClientModeShared, "client.dll", "48 8B 0D ? ? ? ? 48 8B 10 48 8B 19 48 8B C8 FF 92", 0x0, 1);