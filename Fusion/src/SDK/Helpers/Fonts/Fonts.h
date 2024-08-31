#pragma once
#include "../../../Utils/Feature/Feature.h"
#include "../../Definitions/Interfaces.h"
#include <unordered_map>
#include "../../Vars.h"

enum EFonts
{
	FONT_NAME,
	FONT_CONDS,
	FONT_ESP,
	FONT_INDICATORS
};

struct Font_t
{
	const char* m_szName;
	int m_nTall, m_nFlags, m_nWeight;
	unsigned long m_dwFont;
};

class CFonts
{
private:
	std::unordered_map<EFonts, Font_t> m_mapFonts = {};

public:
	void Reload(float flDPI = 1.f);
	const Font_t& GetFont(EFonts eFont);
};

ADD_FEATURE_CUSTOM(CFonts, Fonts, H);