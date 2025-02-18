#include "pch.h"

#include "discord_register.h"
#include "discord_rpc.h"


time_t startTime = 0;
uintptr_t g_moduleBase = NULL;

void Init()
{
	startTime = time(0) - 10;

	DiscordEventHandlers Handler;
	memset(&Handler, 0, sizeof(Handler));
	//Discord_Initialize("440617643032969216", &Handler, true, nullptr);
	Discord_Initialize("1341151076430250144", &Handler, true, nullptr);
}

void Update(const char* state, const char* details)
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = state;
	discordPresence.details = details;
	discordPresence.startTimestamp = startTime;
	Discord_UpdatePresence(&discordPresence);
}

void Shutdown()
{
	Discord_Shutdown();
}


enum DifficultyId
{
	Acolyte = 0,
	Warrior = 1,
	Mentor = 2,
	MasterNinja = 3
};

enum ChapterId
{
	SkyCityTokyo = 62,
	CastleOfTheDragon = 66,
	Thunderclap = 70,
	CaptiveGoddess = 74,
	AquaCapital = 78,
	LycanCastle = 82,
	FlyingFortress = 86,
	SubmitOrDie = 90,
	HeartOfDarkness = 94,
	TempleOfSacrifice = 98,
	TemperedGravestone = 102,
	QuickeningDevastation = 106,
	UnderworldAbyss = 110,
	PoisenedBlood = 114,
	FallenGoddess = 118,
	CityOfWater = 122,
	ShrineMaiden = 126
};


typedef uint64_t(__fastcall* LOADTDP4STY_t)(
	uint64_t param_1,
	uint64_t param_2,
	uint64_t param_3,
	uint64_t fileId
);

static LOADTDP4STY_t LoadTDP4STY = nullptr;
static std::string difficultyName = "";
static std::string chapterName = "";
static uint64_t Hook_LoadTDP4STY(uint64_t param_1, uint64_t param_2, uint64_t param_3, uint64_t fileId)
{
	DifficultyId difficultyId = (DifficultyId)((fileId - 2) % 4);
	ChapterId chapterId = (ChapterId)(fileId - difficultyId);


	difficultyName = "Path of the ";
	switch (difficultyId)
	{
	case DifficultyId::Acolyte: difficultyName += "Acolyte"; break;
	case DifficultyId::Warrior: difficultyName += "Warrior"; break;
	case DifficultyId::Mentor: difficultyName += "Mentor"; break;
	case DifficultyId::MasterNinja: difficultyName += "Master Ninja"; break;
	}

	int chapterNumber = 0;
	switch (chapterId)
	{
	case ChapterId::SkyCityTokyo: chapterName = "Sky City Tokyo"; chapterNumber = 1; break;
	case ChapterId::CastleOfTheDragon: chapterName = "The Castle of the Dragon"; chapterNumber = 2; break;
	case ChapterId::Thunderclap: chapterName = "Thunderclap of Catastrophe"; chapterNumber = 3; break;
	case ChapterId::CaptiveGoddess: chapterName = "A Captive Goddess"; chapterNumber = 4; break;
	case ChapterId::AquaCapital: chapterName = "The Aqua Capital"; chapterNumber = 6; break;
	case ChapterId::LycanCastle: chapterName = "The Lycanthropes' Castle"; chapterNumber = 7; break;
	case ChapterId::FlyingFortress: chapterName = "The Flying Fortress Daedalus"; chapterNumber = 10; break;
	case ChapterId::SubmitOrDie: chapterName = "Submit, or Die!"; chapterNumber = 11; break;
	case ChapterId::HeartOfDarkness: chapterName = "Heart of Darkness"; chapterNumber = 12; break;
	case ChapterId::TempleOfSacrifice: chapterName = "The Temple of Sacrifice"; chapterNumber = 13; break;
	case ChapterId::TemperedGravestone: chapterName = "A Tempered Gravestone"; chapterNumber = 14; break;
	case ChapterId::QuickeningDevastation: chapterName = "Quickening Devastation"; chapterNumber = 15; break;
	case ChapterId::UnderworldAbyss: chapterName = "The Underworld Abyss"; chapterNumber = 16; break;
	case ChapterId::PoisenedBlood: chapterName = "Poisoned Blood"; chapterNumber = 17; break;
	case ChapterId::FallenGoddess: chapterName = "City of the Fallen Goddess"; chapterNumber = 8; break;
	case ChapterId::CityOfWater: chapterName = "Night in the City of Water"; chapterNumber = 11; break;
	case ChapterId::ShrineMaiden: chapterName = "The Shrine-Maiden's Battle"; chapterNumber = 5; break;
	}

	chapterName = std::format("Chapter {}: {}", chapterNumber, chapterName);

	Update(chapterName.c_str(), difficultyName.c_str());


	return LoadTDP4STY(param_1, param_2, param_3, fileId);
}


void HackThread(HMODULE hModule)
{
	Sleep(10000);
	Init();
	Update("Main menu", "");


	g_moduleBase = (uintptr_t)GetModuleHandle(L"gamemodule.dll");

	LoadTDP4STY = (LOADTDP4STY_t)(g_moduleBase + 0x1763700);

	//	Detour LoadTDP4STY
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)LoadTDP4STY, (PBYTE)Hook_LoadTDP4STY);
	DetourTransactionCommit();
}


BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

