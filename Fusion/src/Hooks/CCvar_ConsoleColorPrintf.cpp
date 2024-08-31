#include "../SDK/SDK.h"

MAKE_HOOK(CCvar_ConsoleColorPrintf, U::Memory.GetVFunc(I::CVar, 23), void, __fastcall,
	void* rcx, const Color_t& clr, const char* pFormat, ...)
{
	va_list marker;
	char buffer[4096];
	va_start(marker, pFormat);
	vsnprintf_s(buffer, sizeof(buffer), pFormat, marker);
	va_end(marker);

	char* msg = buffer;
	if (!*msg) { return; }

	std::string Message = msg;

	return CALL_ORIGINAL(rcx, clr, "%s", Message.c_str());
}