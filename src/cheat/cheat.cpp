#include "cheat.h"
#include "../IniReader.h"
#include <Trigger.h>
#include <GameMenuStatus.h>
#include <Pl0000.h>
#include <EntitySystem.h>
#include <Entity.h>
#include <cGameUIManager.h>
#include <injector/injector.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <random>


/* TODO #
*  Make functions for each cheat instead (since return will cancel the handle cheats instead of if condition  
*/
std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<int, std::string>>> Styles;

bool ChangedStyle = false;
bool BossChangedStyle = false;
bool oldChangedStyle = true;
bool changeOnce = true;

int AttackCancel = 0;

int OriginalAnimations[1000];
int OriginalBossAnimations[1000];


std::random_device rd;
std::mt19937 gen(rd());

bool PlayerIsOnce = true;

int MGR_PlaySound(const char* se, int a2)
{
	return ((int (__cdecl*)(const char* se, int a2))(shared::base + 0xA5E050))(se, a2);
}


int cheat::hex_to_int(const std::string& str)
{
	unsigned int hex_num = 0;
	for (size_t i = 0; i < str.length(); ++i) {
		hex_num = (hex_num << 8) | static_cast<unsigned int>(str[i]);
	}

	return ((hex_num & 0xFF000000) >> 24) | ((hex_num & 0x00FF0000) >> 8)
		| ((hex_num & 0x0000FF00) << 8) | ((hex_num & 0x000000FF) << 24);;
}

unsigned int cheat::ReadSinglePointer(unsigned int baseAddress, unsigned int offset) noexcept
{
	return injector::ReadMemory<unsigned int>(baseAddress + offset, true);
}


void cheat::SetFunctionToAddress(unsigned int addr1, unsigned int addr2)
{
	if (injector::ReadMemory<unsigned int>(addr1, true) != addr2) injector::WriteMemory<unsigned int>(addr1, addr2, true);
}


void cheat::AnimationChangerX(unsigned int pAnimUnit, std::string Type, bool IsPlayer)
{


	unsigned int animationName = injector::ReadMemory<unsigned int>(pAnimUnit, true);
	int AnimationsCount = 0;
	if (Type == "RaidenStyles") AnimationsCount = injector::ReadMemory<unsigned int>(pAnimUnit + 0x4C, true);
	if (Type == "SamStyles") AnimationsCount = 352;
	if (Type == "BossSamStyles") AnimationsCount = 328;
	animationName = injector::ReadMemory<unsigned int>(animationName + 0x4, true);
	unsigned int targetAnim = 0x0;
	unsigned int animAddr = 0x0;
	int j = 0x0;

	if (AnimationsCount)
		for (int i = 0; i < AnimationsCount; i++) {
			targetAnim = injector::ReadMemory<unsigned int>(animationName + 0x8 + j, true);


			for (int k = 0; k < Styles[Type][0].size(); k++)
				if ((IsPlayer && targetAnim == cheat::hex_to_int(ChangedStyle ? Styles[Type][0][k] : Styles[Type][1][k]))
					|| (!IsPlayer && targetAnim == cheat::hex_to_int(BossChangedStyle ? Styles[Type][0][k] : Styles[Type][1][k]))
					) {

					if (ChangedStyle && IsPlayer)
						injector::WriteMemory<unsigned int>(animationName + 0x8 + j, cheat::hex_to_int(Styles[Type][1][k]), true);

					if (!ChangedStyle && IsPlayer)
						injector::WriteMemory<unsigned int>(animationName + 0x8 + j, OriginalAnimations[i], true);

					if (BossChangedStyle && !IsPlayer)
						injector::WriteMemory<unsigned int>(animationName + 0x8 + j, cheat::hex_to_int(Styles[Type][1][k]), true);

					if (!BossChangedStyle && !IsPlayer)
						injector::WriteMemory<unsigned int>(animationName + 0x8 + j, OriginalBossAnimations[i], true);

					break;
				}
			targetAnim = 0x0;
			j += 0x3C;
		}
}

void ReadAnimationsOnce(unsigned int pAnimUnit, std::string Type)
{
	unsigned int animationName = injector::ReadMemory<unsigned int>(pAnimUnit, true);
	int AnimationsCount = 0;
	if (Type == "Raiden") AnimationsCount = injector::ReadMemory<unsigned int>(pAnimUnit + 0x4C, true);
	if (Type == "Sam") AnimationsCount = 352;
	if (Type == "BossSam") AnimationsCount = 328;
	animationName = injector::ReadMemory<unsigned int>(animationName + 0x4, true);
	unsigned int targetAnim = 0x0;
	unsigned int animAddr = 0x0;
	int j = 0x0;

	if (AnimationsCount)
		for (int i = 0; i < AnimationsCount; i++) {
			if (Type!="BossSam")
				OriginalAnimations[i] = injector::ReadMemory<unsigned int>(animationName + 0x8 + j, true);
			if (Type=="BossSam") 
				OriginalBossAnimations[i] = injector::ReadMemory<unsigned int>(animationName + 0x8 + j, true);
			j += 0x3C;
		}
}

void cheat::UpdateStyleSwitcher() noexcept
{
	Pl0000* player = cGameUIManager::Instance.m_pPlayer;

	if (!player && !PlayerIsOnce)
		PlayerIsOnce = true;


	if (player) {

		if (PlayerIsOnce && !Trigger::GameFlags.GAME_KOGEKKO_PLAY) {
			if (player->m_nModelIndex == 0x10010) ReadAnimationsOnce((unsigned int)player->field_75C, "Raiden");
			if (player->m_nModelIndex == 0x11400) ReadAnimationsOnce((unsigned int)player->field_75C, "Sam");

			for (auto i = (Entity**)EntitySystem::Instance.m_pStartEntityArray; i != (Entity**)EntitySystem::Instance.m_pEndEntityArray; i = (Entity**)i[2])
			{
				auto entity = *i;
				if (!entity) continue;
				auto Enemy = (BehaviorEmBase*)entity->GetItemInstance();
				if (!Enemy) continue;

				if (entity->m_nEntityIndex != 0x20020) //Boss Sam
					continue;
				else {
					ReadAnimationsOnce((unsigned int)Enemy->field_75C, "BossSam");
				}
			}

			ChangedStyle = false;
			BossChangedStyle = false;
			PlayerIsOnce = false;
		}


		if ((GetAsyncKeyState('4') & 0x8000) && ChangedStyle && !Trigger::GameFlags.GAME_KOGEKKO_PLAY && !Trigger::StaFlags.STA_PAUSE) {

			ChangedStyle = false;
			LoadConfig();
			
			if (player->m_nModelIndex == 0x10010)
			{
				AnimationChangerX((unsigned int)player->field_75C, "RaidenStyles", true);
				if (player->m_nCurrentAction == 0x0) player->SetState(0, 0, 0, 0);
			}

			if (player->m_nModelIndex == 0x11400) 
			{
				AnimationChangerX((unsigned int)player->field_75C, "SamStyles", true);
				if (player->m_nCurrentAction == 0x100000) player->SetState(0x100000, 0, 0, 0);
			}

			if (AttackCancel) {
				player->field_2620 = 0;
				player->field_263C = 0;
				player->field_2628 = 0;
			}
			MGR_PlaySound("core_se_sys_decide_l", 0);
		}

		if ((GetAsyncKeyState('5') & 0x8000) && !ChangedStyle && !Trigger::GameFlags.GAME_KOGEKKO_PLAY && !Trigger::StaFlags.STA_PAUSE) {

			ChangedStyle = true;
			LoadConfig();
			
			if (player->m_nModelIndex == 0x10010) 
			{
				AnimationChangerX((unsigned int)player->field_75C, "RaidenStyles", true);
				if (player->m_nCurrentAction == 0x0) player->SetState(0, 0, 0, 0);
			}

			if (player->m_nModelIndex == 0x11400) 
			{
				AnimationChangerX((unsigned int)player->field_75C, "SamStyles", true);
				if (player->m_nCurrentAction == 0x100000) player->SetState(0x100000, 0, 0, 0);
			}
			
			
			if (AttackCancel) {
				player->field_2620 = 0;
				player->field_263C = 0;
				player->field_2628 = 0;
			}

			MGR_PlaySound("core_se_sys_decide_l", 0);
		}


		for (auto i = (Entity**)EntitySystem::Instance.m_pStartEntityArray; i != (Entity**)EntitySystem::Instance.m_pEndEntityArray; i = (Entity**)i[2])
		{
			auto entity = *i;

			if (!entity)
				continue;

			auto Enemy = (BehaviorEmBase*)entity->GetItemInstance();

			if (!Enemy)
				continue;

			if (entity->m_nEntityIndex != 0x20020) //Boss Sam
				continue;
			else if (!Trigger::StaFlags.STA_PAUSE) {
				//00030003 action 2000 anim
				 //00030006 action 2060 anim 
				 //00030005 action 2040 anim 



				// Генерация случайного значения от 0 до 1
				std::uniform_real_distribution<double> dis(0.0, 1.0);
				double randomValue = dis(gen);

				// Генерация случайного значения от 0 до 1
				//double randomValue = static_cast<double>(std::rand()) / RAND_MAX;
				int Action = Enemy->m_nCurrentAction;
				if (randomValue <= 0.5 && !BossChangedStyle) {
				
					if (Enemy->m_nCurrentActionId==0 && !Trigger::StaFlags.STA_QTE && !Trigger::StaFlags.STA_EVENT) {
						BossChangedStyle = true;
						AnimationChangerX((unsigned int)player->field_75C, "BossSamStyles", false);
						Enemy->SetState(Action, 0, 0, 0);
					}
				
				}
				if (randomValue > 0.5 && BossChangedStyle) {
					if (Enemy->m_nCurrentActionId == 0 && !Trigger::StaFlags.STA_QTE && !Trigger::StaFlags.STA_EVENT) {
						BossChangedStyle = false;
						AnimationChangerX((unsigned int)player->field_75C, "BossSamStyles", false);
						Enemy->SetState(Action, 0, 0, 0);
					}
				}


				
			}
		}



	}
}



unsigned char vrTimerOriginalBytes[6];

void cheat::Initialize()
{
	if (!bInitialized)
	{
		injector::ReadMemoryRaw(shared::base + 0x81B440, vrTimerOriginalBytes, 6, true);
		LoadConfig();

		//std::srand(std::time(nullptr));
		bInitialized = true;
	}
}

void cheat::Update()
{
	if (!bInitialized)
		return;

	HandleCheats();
}

// Handles all cheats at once
void cheat::HandleCheats() noexcept
{
	// Player
	UpdateStyleSwitcher();
}

// Loads config (ini file)
void cheat::LoadConfig() noexcept
{
	CIniReader iniReader("StyleSwitcherSettings.ini");
	AttackCancel = iniReader.ReadInteger("StyleSwitcherSettings", "CancelAttackIfStyleSwitch", 0) == 1;


	std::string_view szFileName = "Styles.ini";
	std::string m_szFileName = "Styles.ini";

	char buffer[MAX_PATH];
	HMODULE hm = NULL;
	GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&ends_with, &hm);
	GetModuleFileNameA(hm, buffer, sizeof(buffer));
	std::string modulePath = buffer;

	if (szFileName.find(':') != std::string_view::npos)
	{
		m_szFileName = szFileName;
	}
	else if (szFileName.length() == 0)
	{
		m_szFileName = modulePath.substr(0, modulePath.find_last_of('.')) + ".ini";
	}
	else
	{
		m_szFileName = modulePath.substr(0, modulePath.rfind('\\') + 1) + szFileName.data();
	}



	std::ifstream file(m_szFileName);
	if (!file.is_open()) {
		std::cerr << "Failed to open file." << std::endl;
	}

	std::string currentSection;
	std::string line;
	int index = 0;
	
	if (file.is_open())
	while (std::getline(file, line)) {
		// Удаление начальных и конечных пробелов
		size_t startPos = line.find_first_not_of(" \t");
		size_t endPos = line.find_last_not_of(" \t");
		if (startPos != std::string::npos) {
			line = line.substr(startPos, endPos - startPos + 1);
		}
		else {
			continue;  // Пропуск пустых строк
		}

		if (line.empty()) {
			continue;  // Пропуск пустых строк
		}

		if (line[0] == '[' && line.back() == ']') {
			currentSection = line.substr(1, line.size() - 2);
			Styles[currentSection] = {};
		}
		else if (line[0] == ';') {
			continue;  // Пропуск комментариев
		}
		else {
			if (line.size() >= 9) {
				std::string oldAnim = line.substr(0, 4);
				std::string newAnim = line.substr(5, 4);
				Styles[currentSection][0][index] = oldAnim;
				Styles[currentSection][1][index] = newAnim;
				index++;
			}
		}
	}
	
}

// Saves config (ini file)
void cheat::SaveConfig() noexcept
{

}

// Resets cheats
void cheat::Reset() noexcept
{

}