#include "../SDK/SDK.h"

#include "../Features/Commands/Commands.h"
#include <boost/algorithm/string/split.hpp>
#include <regex>

const std::vector<std::pair<std::string, std::string>> patterns
{
	{ "r", "w" },
	{ "l", "w" },
	{ "R", "W" },
	{ "L", "W" },
	{ "n([aeiou])", "ny$1" },
	{ "N([aeiou])", "Ny$1" },
	{ "N([AEIOU])", "NY$1" }
};

const std::vector<std::string> suffixes
{
	" UwU", " OwO", " owo", " uwu", " nwn", " :3", " >w<", " ^w^", " <3"
};

std::string owoify(std::string text)
{
	for (auto& pattern : patterns)
	{
		std::regex reg(pattern.first);

		text = std::regex_replace(text, reg, pattern.second);
	}

	if (text.size() < 124)
	{
		int suffix{ I::UniformRandomStream->RandomInt(0, static_cast<int>(suffixes.size() - 1)) };

		text += suffixes[suffix];
	}

	return text;
}

class split_q
{
public:
	split_q() : in_q(false) {}

	bool operator()(char ch) const
	{
		if (ch == '\"')
		{
			in_q = !in_q;
		}
		return !in_q && ch == ' ';
	}

private:
	mutable bool in_q;
};

MAKE_HOOK(EngineClient_ClientCmd_Unrestricted, U::Memory.GetVFunc(I::EngineClient, 106), void, __fastcall,
	void* rcx, const char* szCmdString)
{
	if (Vars::Misc::Chat::OwOify.Value)
	{
		if (strstr(szCmdString, "say"))
		{
			std::string cmdString(szCmdString);

			if (cmdString.rfind("say", 0) != 0)
			{
				return CALL_ORIGINAL(rcx, szCmdString);
			}

			std::smatch match{};
			std::regex_search(cmdString, match, std::regex("(say.* )\"(.*)\""));

			cmdString = match[1].str() + "\"" + owoify(match[2].str()) + "\"";

			return CALL_ORIGINAL(rcx, cmdString.c_str());
		}
	}

	std::string cmdString = szCmdString;
	std::deque<std::string> cmdArgs;
	boost::split(cmdArgs, cmdString, split_q());
	if (!cmdArgs.empty())
	{
		const std::string cmdName = cmdArgs.front();
		cmdArgs.pop_front();

		if (F::Commands.Run(cmdName, cmdArgs))
			return;
	}

	CALL_ORIGINAL(rcx, cmdString.c_str());
}
