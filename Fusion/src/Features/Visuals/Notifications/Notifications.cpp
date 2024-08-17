#include "Notifications.h"

void CNotifications::Add(const std::string& sText, Color_t cColor, float flLifeTime)
{
    vNotifications.push_back({ sText, cColor, float(SDK::PlatFloatTime()) + flLifeTime });
}

void CNotifications::Draw()
{
    if (vNotifications.size() > unsigned(iMaxNotifySize + 1))
        vNotifications.erase(vNotifications.begin());

    for (auto it = vNotifications.begin(); it != vNotifications.end();)
    {
        if (it->m_flTime <= SDK::PlatFloatTime())
            it = vNotifications.erase(it);
        else
            ++it;
    }

    if (vNotifications.empty())
        return;

    int y = 2;
    const auto& fFont = H::Fonts.GetFont(FONT_INDICATORS);

    for (auto& tNotification : vNotifications)
    {
        int x = 2;
        int w, h;
        I::MatSystemSurface->GetTextSize(fFont.m_dwFont, SDK::ConvertUtf8ToWide(tNotification.m_sText).c_str(), w, h);
        w += 4; 
        h += 4; 

        const float flLife = std::min(tNotification.m_flTime - SDK::PlatFloatTime(), Vars::Logging::Lifetime.Value - (tNotification.m_flTime - SDK::PlatFloatTime()));
        if (flLife < 0.1f)
            x -= Math::RemapValClamped(flLife, 0.1f, 0.f, 0.f, w);

        auto& cAccent = Vars::Menu::Theme::Accent.Value;
        auto& cBackground = Vars::Menu::Theme::Background.Value;
        auto& cActive = Vars::Menu::Theme::Active.Value;

        H::Draw.FillRect(x, y, w, h, { cBackground.r, cBackground.g, cBackground.b, 255 }); // Background

        H::Draw.LineRect(x, y, w, h, { cAccent.r, cAccent.g, cAccent.b, 255 }); // Outline

        H::Draw.String(fFont, x + 2, y + 2, { cActive.r, cActive.g, cActive.b, 255 }, ALIGN_TOPLEFT, tNotification.m_sText.c_str()); // Text

        y += h + 2;
    }
}
