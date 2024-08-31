#include "EventListener.h"
#include "../../Features/Visuals/Visuals.h"

// TODO: Add listener to all events
void CEventListener::Setup(const std::deque<const char*>& deqEvents)
{
	if (deqEvents.empty())
		return;

	for (auto szEvent : deqEvents)
	{
		I::GameEventManager->AddListener(this, szEvent, false);

		if (!I::GameEventManager->FindListener(this, szEvent))
			throw std::runtime_error(std::format("failed to add listener: {}", szEvent));
	}
}

void CEventListener::Destroy()
{
	I::GameEventManager->RemoveListener(this);
}

void CEventListener::FireGameEvent(IGameEvent* pEvent)
{
	if (pEvent == nullptr) { return; }
	if (I::EngineClient->IsPlayingTimeDemo()) { return; }

	const FNV1A_t uNameHash = FNV1A::Hash(pEvent->GetName());

	// Pickup Timers
	if (Vars::Visuals::UI::PickupTimers.Value && uNameHash == FNV1A::HashConst("item_pickup"))
	{
		if (auto pEntity = I::ClientEntityList->GetClientEntity(I::EngineClient->GetPlayerForUserID(pEvent->GetInt("userid"))))
		{
			auto itemName = pEvent->GetString("item");
			if (std::strstr(itemName, "medkit"))
				F::Visuals.PickupDatas.push_back({ 1, I::EngineClient->Time(), pEntity->GetAbsOrigin() });
			else if (std::strstr(itemName, "ammopack"))
				F::Visuals.PickupDatas.push_back({ 0, I::EngineClient->Time(), pEntity->GetAbsOrigin() });
		}
	}
}