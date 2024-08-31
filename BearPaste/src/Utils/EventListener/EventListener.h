#pragma once

#include "../../SDK/SDK.h"

class CEventListener : public CGameEventListener
{
public:
	void Setup(const std::deque<const char*>& arrEvents);
	void Destroy();

	virtual void FireGameEvent(IGameEvent* pEvent) override;
	//virtual int GetEventDebugID() override { return 42; }
};

ADD_FEATURE(CEventListener, EventListener)