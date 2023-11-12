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
#include <cWeaponSelectMenu.h>
#include <PlayerManagerImplement.h>
#include <XInput.h>
#ifdef _MSC_VER < 1700 //pre 2012
#pragma comment(lib,"Xinput.lib")
#else
#pragma comment(lib,"Xinput9_1_0.lib")
#endif
#include <Windows.h>



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

unsigned int oldWeapon = 0x0;
bool WeaponChanged = false;
int NewCustomWeaponId = 0;

std::string GamepadKeyForModStyle = "XINPUT_GAMEPAD_RIGHT_THUMB_UP";
std::string GamepadKeyForNormalStyle = "XINPUT_GAMEPAD_RIGHT_THUMB_DOWN";
std::string GamepadKeyForWeaponSwitchBack = "XINPUT_GAMEPAD_RIGHT_THUMB_LEFT";
std::string GamepadKeyForWeaponSwitchForward = "XINPUT_GAMEPAD_RIGHT_THUMB_RIGHT";


std::string KeyForModStyle = "34";
std::string KeyForNormalStyle = "35";
std::string KeyForWeaponSwitchBack = "36";
std::string KeyForWeaponSwitchForward = "37";


DWORD dwResult;
XINPUT_STATE state;
//ZeroMemory(&state, sizeof(XINPUT_STATE));

/*bool IsButtonPressed(std::string Button)
{


	std::stringstream ss(Button);
	WORD wButton;
	ss >> wButton;
	wButton = static_cast<WORD>(std::stoi(Button));

	dwResult = XInputGetState(0, &state); // Получаем состояние геймпада
	if (dwResult == ERROR_SUCCESS)
	{
		if ((state.Gamepad.wButtons & wButton) != 0) // Проверяем нажатие кнопки
		{
			return true;
		}
	}

	return false;
}*/

bool IsButtonPressed(const std::string& button)
{
	XINPUT_STATE state;
	DWORD dwResult = XInputGetState(0, &state); // Получаем состояние геймпада

	if (dwResult == ERROR_SUCCESS)
	{
		if (button == "XINPUT_GAMEPAD_A" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_B" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_X" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_Y" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_LEFT_SHOULDER" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_RIGHT_SHOULDER" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_BACK" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_START" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_LEFT_THUMB" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_RIGHT_THUMB" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_DPAD_UP" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_DPAD_DOWN" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_DPAD_LEFT" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_DPAD_RIGHT" && (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0)
			return true;
		else if (button == "XINPUT_GAMEPAD_LEFT_THUMB_UP" && state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return true;
		else if (button == "XINPUT_GAMEPAD_LEFT_THUMB_DOWN" && state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return true;
		else if (button == "XINPUT_GAMEPAD_LEFT_THUMB_LEFT" && state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return true;
		else if (button == "XINPUT_GAMEPAD_LEFT_THUMB_RIGHT" && state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return true;
		else if (button == "XINPUT_GAMEPAD_RIGHT_THUMB_UP" && state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return true;
		else if (button == "XINPUT_GAMEPAD_RIGHT_THUMB_DOWN" && state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return true;
		else if (button == "XINPUT_GAMEPAD_RIGHT_THUMB_LEFT" && state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return true;
		else if (button == "XINPUT_GAMEPAD_RIGHT_THUMB_RIGHT" && state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return true;
	}

	return false;
}

void DestroyCustomWeapon(Pl0000* player)
{
	((void(__thiscall*)(Pl0000 * player))(shared::base + 0x77D0E0))(player);
}

void PlayerChangeCustomWeapon(Pl0000* player, int WeaponId)
{
	((void(__thiscall*)(Pl0000 * player, int WeaponId))(shared::base + 0x77F5E0))(player, WeaponId);
}

void PlayerChangeCustomWeapon2(void* player)
{
	((void(__thiscall*)(void* player))(shared::base + 0x7948D0))(player);
}


void CheckChangedWeaponMaybe(int a1)
{
	((void(__cdecl*)(int a1))(shared::base + 0x8245E0))(a1);
}

void Check1()
{
	((void)(shared::base + 0x824200));
}

void Check2()
{
	((void)(shared::base + 0x8244D0));
}


int CheckChangedWeaponMaybeAll()
{
	return ((int(__cdecl*)())(shared::base + 0x824970))();
}


void WeaponSelectSwitch(Pl0000* menu)
{
	((void(__thiscall*)(Pl0000 * menu))(shared::base + 0x592D40))(menu);
}


int WeaponSelecMenu_Open(cWeaponSelectMenu* menu)
{
	return ((int(__thiscall*)(cWeaponSelectMenu * menu))(shared::base + 0x5B3AD0))(menu);
}

void WeaponSelectMenuOpen()
{
	((void)(shared::base + 0x829370));
	
}


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

int requestAnimationByMap2(Behavior* behavior, int a2, int a3, float a4, float a5, float a6, float a7, float a8)
{
	return ((int(__thiscall*)(Behavior * behavior, int a2, int a3, int a4, float a5, float a6, float a7, float a8))(shared::base + 0x6A4080))(behavior, a2, a3, a4, a5, a6, a7, a8);
}

void AnimationPlayer(Behavior* behavior, float a2, float a3)
{
	((void(__thiscall*)(Behavior * behavior, float a2, float a3))(shared::base + 0x794790))(behavior, a2, a3);
}

void AnimationStop(Behavior* behavior, int a2, float a3)
{
	((void(__thiscall*)(Behavior * behavior, int a2, float a3))(shared::base + 0x694BC0))(behavior, a2, a3);
}

unsigned int cheat::ReadDoublePointer(unsigned int baseAddress, unsigned int offset) noexcept
{
	unsigned int address = 0;
	address = injector::ReadMemory<unsigned int>(baseAddress, true);
	if (address) {
		return injector::ReadMemory<unsigned int>(address + offset, true);
	}
	else return 0;
}



Entity* GetHandleInstance(int* instance)
{
	return ((Entity * (__thiscall*)(int*))(shared::base + 0x681330))(instance);
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

void CustomWeaponChange(Pl0000* player,int NewCustomWeaponId)
{
	auto pl = PlayerManagerImplement::pInstance;
	int currentCustomWeapon = pl->GetCustomWeaponEquipped();

	if (!WeaponChanged
		&& player->m_nCurrentAction != 0x9D
		&& player->m_nCurrentAction != 0x9E
		&& player->m_nCurrentAction != 0xA1
		&& player->m_nCurrentAction != 0xA2
		&& player->m_nCurrentAction != 0xA3
		&& player->m_nCurrentAction != 0xA4
		&& player->m_nCurrentAction != 0xA5
		&& player->m_nCurrentAction != 0x45
		&& player->m_nCurrentAction != 0x46
		)

	{

		if (player->field_FF8
			//&& currentCustomWeapon !=5
			)
		{
			//Entity* PlayerCustomWeaponEntity = GetHandleInstance(&player->field_FF8);
			//auto PlayerCustomWeapon = (Behavior*)PlayerCustomWeaponEntity->GetEntityInstance();
			//PlayerCustomWeapon->DisableRender();
			requestAnimationByMap2(player, 4, 0, 0.0, 1.0, 0x8100000, -1.0, 1.0);
			//AnimationPlayer(player, 1.0, 1.0);

		}

		//if (NewCustomWeaponId == 5)
			//player->SetState(0, 0, 0, 0);

		if (cheat::ReadDoublePointer((unsigned int)&player->m_pAnimationSlots, 0x14) == 4 || currentCustomWeapon == 5) {



			WeaponChanged = true;

			if (NewCustomWeaponId != 5) player->m_nSwordState = 0;
			else player->m_nSwordState = 1;

			DestroyCustomWeapon(player);

			cheat::SetFunctionToAddress(shared::base + 0x17E9FD8, NewCustomWeaponId);

			PlayerChangeCustomWeapon(player, NewCustomWeaponId);
			cheat::SetFunctionToAddress(shared::base + 0x19C074C, 1);
		}
	}
	
}

void cheat::UpdateStyleSwitcher() noexcept
{
	Pl0000* player = cGameUIManager::Instance.m_pPlayer;

	if (!player && !PlayerIsOnce)
		PlayerIsOnce = true;

	if (player) {

		if (PlayerIsOnce && !Trigger::GameFlags.GAME_KOGEKKO_PLAY) {

			//auto pl = PlayerManagerImplement::pInstance;
			//int CurrentCustomWeapon = pl->GetCustomWeaponEquipped();

			if (player->m_nModelIndex == 0x10010) ReadAnimationsOnce((unsigned int)player->field_75C, "Raiden");
			if (player->m_nModelIndex == 0x11400) ReadAnimationsOnce((unsigned int)player->field_75C, "Sam");

			for (auto i = (Entity**)EntitySystem::Instance.m_pStartEntityArray; i != (Entity**)EntitySystem::Instance.m_pEndEntityArray; i = (Entity**)i[2])
			{
				auto entity = *i;
				if (!entity) continue;
				auto Enemy = (BehaviorEmBase*)entity->GetEntityInstance();
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

		auto pl = PlayerManagerImplement::pInstance;
		int currentCustomWeapon = pl->GetCustomWeaponEquipped();

		
		if ((GetKeyState(std::stoi(KeyForNormalStyle, nullptr, 16)) & 0x8000) && ChangedStyle && !Trigger::GameFlags.GAME_KOGEKKO_PLAY && !Trigger::StaFlags.STA_PAUSE) {

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

		

		if ((GetKeyState(std::stoi(KeyForModStyle, nullptr, 16)) & 0x8000) && !ChangedStyle && !Trigger::GameFlags.GAME_KOGEKKO_PLAY && !Trigger::StaFlags.STA_PAUSE) {

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

		
		if (( (GetKeyState(std::stoi(KeyForWeaponSwitchForward, nullptr, 16)) & 0x8000) || IsButtonPressed(GamepadKeyForWeaponSwitchForward))
			&& !WeaponChanged && !Trigger::GameFlags.GAME_KOGEKKO_PLAY && !Trigger::StaFlags.STA_PAUSE && !Trigger::StaFlags.STA_QTE) {


			if (currentCustomWeapon == 0) NewCustomWeaponId = 2;
				if (currentCustomWeapon == 2) NewCustomWeaponId = 3;
				if (currentCustomWeapon == 3) NewCustomWeaponId = 4;
				if (currentCustomWeapon == 4) NewCustomWeaponId = 5;
				if (currentCustomWeapon == 5) NewCustomWeaponId = 0;
				CustomWeaponChange(player, NewCustomWeaponId);
		}
		
			if (((GetKeyState(std::stoi(KeyForWeaponSwitchBack, nullptr, 16)) & 0x8000) || IsButtonPressed(GamepadKeyForWeaponSwitchBack))
				&& !WeaponChanged && !Trigger::GameFlags.GAME_KOGEKKO_PLAY && !Trigger::StaFlags.STA_PAUSE && !Trigger::StaFlags.STA_QTE) {

			if (currentCustomWeapon == 0) NewCustomWeaponId = 5;
			if (currentCustomWeapon == 2) NewCustomWeaponId = 0;
			if (currentCustomWeapon == 3) NewCustomWeaponId = 2;
			if (currentCustomWeapon == 4) NewCustomWeaponId = 3;
			if (currentCustomWeapon == 5) NewCustomWeaponId = 4;
			CustomWeaponChange(player, NewCustomWeaponId);
		}

			if (WeaponChanged
				&& injector::ReadMemory<unsigned int>(shared::base + 0x17E9FDC, true) == 0
				&& injector::ReadMemory<unsigned int>(shared::base + 0x17E9FD4, true) == NewCustomWeaponId
				)
			{

				WeaponChanged = false;
				//if (NewCustomWeaponId!=5) 
				AnimationStop(player, 0, 12.0);
				//player->SetState(0, 0, 0, 0);
				PlayerChangeCustomWeapon2(player);
				MGR_PlaySound("core_se_sys_decide_l", 0);
			}



		for (auto i = (Entity**)EntitySystem::Instance.m_pStartEntityArray; i != (Entity**)EntitySystem::Instance.m_pEndEntityArray; i = (Entity**)i[2])
		{
			auto entity = *i;

			if (!entity)
				continue;

			auto Enemy = (BehaviorEmBase*)entity->GetEntityInstance();

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
	
	KeyForModStyle = iniReader.ReadString("StyleSwitcherSettings", "KeyForModStyle", "35");
	KeyForNormalStyle = iniReader.ReadString("StyleSwitcherSettings", "KeyForNormalStyle", "34");
	
	KeyForWeaponSwitchBack = iniReader.ReadString("StyleSwitcherSettings", "KeyForWeaponSwitchBack", "36");
	KeyForWeaponSwitchForward = iniReader.ReadString("StyleSwitcherSettings", "KeyForWeaponSwitchForward", "37");


	GamepadKeyForModStyle = iniReader.ReadString("StyleSwitcherSettings", "GamepadKeyForModStyle", "XINPUT_GAMEPAD_RIGHT_THUMB_UP");
	GamepadKeyForNormalStyle = iniReader.ReadString("StyleSwitcherSettings", "GamepadKeyForNormalStyle", "XINPUT_GAMEPAD_RIGHT_THUMB_DOWN");

	GamepadKeyForWeaponSwitchBack = iniReader.ReadString("StyleSwitcherSettings", "GamepadKeyForWeaponSwitchBack", "XINPUT_GAMEPAD_RIGHT_THUMB_LEFT");
	GamepadKeyForWeaponSwitchForward = iniReader.ReadString("StyleSwitcherSettings", "GamepadKeyForWeaponSwitchForward", "XINPUT_GAMEPAD_RIGHT_THUMB_RIGHT");

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