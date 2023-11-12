#pragma once
#include <Windows.h>
#include <Psapi.h>
#include "shared.h"
#include <Pl0000.h>
#include <BehaviorEmBase.h>
#include <string>
namespace cheat
{
	inline bool bInitialized = false;
	inline bool infiniteFc = false;
    inline bool infiniteHealth = false;
	inline bool oneHitKill = false;
	inline bool infiniteSubWeapon = false;
	inline bool noDamageStat = false;
	inline bool stealth = false;
	inline bool groundCheat = false;
	inline bool groundEnabled = false;
	inline bool visorSwitch = false;
	inline bool autoHpUp = false;
	inline bool timeStop = false;
	inline bool timeStopSwitch = false;
	inline bool dealZeroDamage = false;
	inline bool &OnFocus = *(bool*)(shared::base + 0x19D509C);
	inline bool infTimer = false;
	inline bool noSlowMotion = false;
	inline float playerSlowRate = 1.0f;
	inline bool damageMultiplierEnabled = false;
	inline float damageMultiplier = 1.0f; // by default
	inline bool playerEffect = false;
	inline int playerEffectNum = 199;
	inline float effectInterval = 0.5f;


	inline bool PlayerIsArmstrong = true;
	inline int TestMesh = 0;
	inline int BossHPShow = 0;
	inline int BossHPModify = 0;
	inline bool EnableInstantCharges = false;

	inline unsigned int test = 0x0;

	inline bool fixFace = true;
	inline int SelectedWeaponEffect = 0;

	void Initialize();
	void Update();
	void UpdateStyleSwitcher() noexcept;
	void HandleCheats() noexcept;
	void LoadConfig() noexcept;
	void SaveConfig() noexcept;
	void Reset() noexcept;
	void SetFunctionToAddress(unsigned int addr1, unsigned int addr2);

	void AnimationsToArrays(std::string AnimationNames, std::string type);

	int hex_to_int(const std::string& str);
	unsigned int ReadDoublePointer(unsigned int baseAddress, unsigned int offset) noexcept;
	unsigned int ReadSinglePointer(unsigned int baseAddress, unsigned int offset) noexcept;
	void WriteDoublePointer(unsigned int baseAddress, unsigned int offset, unsigned int value) noexcept;
	const char* GetAnimationNameById(void* pAnimUnit, int animId);
	void AnimationChanger(unsigned int pAnimUnit, std::string* Style1, std::string* Style2, std::string Type, bool IsPlayer);
	void AnimationChangerX(unsigned int pAnimUnit, std::string Type, bool IsPlayer);
}