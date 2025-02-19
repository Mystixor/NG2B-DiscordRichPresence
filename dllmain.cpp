#include "pch.h"

#include "discord_register.h"
#include "discord_rpc.h"


#define v1070


time_t startTime = 0;
uintptr_t g_moduleBase = NULL;


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

enum MissionId
{
	Mission_01 = 181,
	Mission_02,
	Mission_03,
	Mission_04,
	Mission_05,
	Mission_06,
	Mission_07,
	Mission_08,
	Mission_09,
	Mission_10,
	Mission_11,
	Mission_12,
	Mission_13,
	Mission_14,
	Mission_15,
	Mission_16,
	Mission_17,
	Mission_18,
	Mission_19,
	Mission_20,
	Mission_21,
	Mission_22,
	Mission_23,
	Mission_24,
	Mission_25,
	Mission_31,
	Mission_32,
	Mission_33,
	Mission_34,
	Mission_35,
	Mission_41,
	Mission_42,
	Mission_43,
	Mission_44,
	Mission_45
};


void Init()
{
	startTime = time(0) - 10;

	DiscordEventHandlers Handler;
	memset(&Handler, 0, sizeof(Handler));
	Discord_Initialize("1341151076430250144", &Handler, true, nullptr);
}

void Update(const char* state, const char* details, const char* largeImageKey, const char* largeImageText)
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = state;
	discordPresence.details = details;
	discordPresence.startTimestamp = startTime;
	discordPresence.largeImageKey = largeImageKey;
	discordPresence.largeImageText = largeImageText;
	Discord_UpdatePresence(&discordPresence);
}

void Shutdown()
{
	Discord_Shutdown();
}


typedef uint64_t(__fastcall* LOADTDP4STY_t)(
	uint64_t param_1,
	uint64_t param_2,
	uint64_t param_3,
	uint64_t fileId
);

static LOADTDP4STY_t LoadTDP4STY = nullptr;
static std::string state = "";
static std::string details = "";
static std::string largeImageKey = "";
static std::string largeImageText = "";
static uint32_t karma = 0;
static std::mutex presenceMutex;
static uint64_t Hook_LoadTDP4STY(uint64_t param_1, uint64_t param_2, uint64_t param_3, uint64_t fileId)
{
	{
		std::lock_guard<std::mutex> guard(presenceMutex);

		if (fileId >= 62 && fileId <= 129)
		{
			//	Story-mode or Chapter Challenge
			DifficultyId difficultyId = (DifficultyId)((fileId - 2) % 4);
			ChapterId chapterId = (ChapterId)(fileId - difficultyId);

			std::string difficultyName;
			switch (difficultyId)
			{
			case DifficultyId::Acolyte: difficultyName = "Acolyte"; break;
			case DifficultyId::Warrior: difficultyName = "Warrior"; break;
			case DifficultyId::Mentor: difficultyName = "Mentor"; break;
			case DifficultyId::MasterNinja: difficultyName = "Master Ninja"; break;
			}

			std::string chapterName = "";
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

			details = std::format("Chapter {:d} ({:s})", chapterNumber, difficultyName);
			largeImageText = std::format("Chapter {:d}: {:s}", chapterNumber, chapterName);
			largeImageKey = std::format("tex_stl_chapter_{:02d}", chapterNumber);
		}
		else if (fileId >= 181)
		{
			//	Tag Missions
			MissionId missionId = (MissionId)fileId;

			std::string missionCollection;
			uint32_t missionNumber = 0;
			uint32_t imageKeyNumber = 0;
			switch (missionId)
			{
			case MissionId::Mission_01:
			case MissionId::Mission_02:
			case MissionId::Mission_03:
			case MissionId::Mission_04:
			case MissionId::Mission_05:
			case MissionId::Mission_06:
			case MissionId::Mission_07:
			case MissionId::Mission_08:
			case MissionId::Mission_09:
			case MissionId::Mission_10:
				missionCollection = "Acolyte";
				missionNumber = missionId - 180;
				imageKeyNumber = missionId - 180;
				break;
			case MissionId::Mission_11:
			case MissionId::Mission_12:
			case MissionId::Mission_13:
			case MissionId::Mission_14:
			case MissionId::Mission_15:
			case MissionId::Mission_16:
			case MissionId::Mission_17:
			case MissionId::Mission_18:
			case MissionId::Mission_19:
			case MissionId::Mission_20:
				missionCollection = "Warrior";
				missionNumber = missionId - MissionId::Mission_10;
				imageKeyNumber = missionId - 180;
				break;
			case MissionId::Mission_21:
			case MissionId::Mission_22:
			case MissionId::Mission_23:
			case MissionId::Mission_24:
			case MissionId::Mission_25:
				missionCollection = "Mentor";
				missionNumber = missionId - MissionId::Mission_20;
				imageKeyNumber = missionId - 180;
				break;
			case MissionId::Mission_31:
			case MissionId::Mission_32:
			case MissionId::Mission_33:
			case MissionId::Mission_34:
			case MissionId::Mission_35:
				missionCollection = "Master Ninja";
				missionNumber = missionId - MissionId::Mission_25;
				imageKeyNumber = missionId - 175;
				break;
			case MissionId::Mission_41:
			case MissionId::Mission_42:
			case MissionId::Mission_43:
			case MissionId::Mission_44:
			case MissionId::Mission_45:
				missionCollection = "Ultimate Ninja";
				missionNumber = missionId - MissionId::Mission_35;
				imageKeyNumber = missionId - 170;
				break;
			default:
				missionCollection = "[Error]";
				missionNumber = fileId;
				imageKeyNumber = 0;
				break;
			}

			details = std::format("Tag Missions: {:s} {:02d}", missionCollection, missionNumber);
			largeImageText = std::format("{:s}: Mission {:d}", missionCollection, missionNumber);

			if (imageKeyNumber)
				largeImageKey = std::format("stl_mission_{:02d}", imageKeyNumber);
			else
				largeImageKey = "";
		}
		else
		{
			//	Unknown missions (test levels, etc.)
			details = std::format("Unknown mission ({:05d})", fileId);
			largeImageText = "";
			largeImageKey = "";
		}
	}

	return LoadTDP4STY(param_1, param_2, param_3, fileId);
}


void HackThread(HMODULE hModule)
{
	Sleep(10000);	//	Wait for basic game libraries to be loaded

	Init();

	g_moduleBase = (uintptr_t)GetModuleHandle(L"gamemodule.dll");

#ifdef v1070
	LoadTDP4STY = (LOADTDP4STY_t)(g_moduleBase + 0x1763700);
	uint32_t* p_Karma = (uint32_t*)(g_moduleBase + 0x36257f0);
	uint32_t* p_KarmaPartner = (uint32_t*)(g_moduleBase + 0x3625b30);	//	In Tag Missions, your and your partner's karma are tracked separately
	bool* p_IsInGame = (bool*)(g_moduleBase + 0x222ee9c);				//	This is more of game state Id, but it seems to be zero only in the main menu so this works
#endif

	//	Detour LoadTDP4STY
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)LoadTDP4STY, (PBYTE)Hook_LoadTDP4STY);
	DetourTransactionCommit();

	while (true)
	{
		{
			std::lock_guard<std::mutex> guard(presenceMutex);

			if (*p_IsInGame)
			{
				karma = *p_Karma + *p_KarmaPartner;
				state = std::format("Karma: {:d}", karma);
			}
			else
			{
				state = "";
				details = "Main menu";
				largeImageKey = "";
				largeImageText = "";
			}

			Update(state.c_str(), details.c_str(), largeImageKey.c_str(), largeImageText.c_str());
		}

		Sleep(20000);
	}
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