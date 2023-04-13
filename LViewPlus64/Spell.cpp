#include "Spell.h"
#include "Utils.h"
#include "Offsets.h"
#include "GameData.h"

BYTE Spell::buffer[0x150];
const char* Spell::spellTypeName[6] = { "Q", "W", "E", "R", "D", "F"};
const HKey   Spell::spellSlotKey[6]  = { HKey::Q, HKey::W, HKey::E, HKey::R, HKey::D, HKey::F};

std::map<std::string, SummonerSpellType> Spell::summonerSpellTypeDict = {
	{std::string("summonerhaste"),                   SummonerSpellType::GHOST},
	{std::string("summonerheal"),                    SummonerSpellType::HEAL},
	{std::string("summonerbarrier"),                 SummonerSpellType::BARRIER},
	{std::string("summonerexhaust"),                 SummonerSpellType::EXHAUST},
	{std::string("summonermana"),                    SummonerSpellType::CLARITY},
	{std::string("summonermark"),					 SummonerSpellType::SNOWBALL},
	{std::string("summonerflash"),                   SummonerSpellType::FLASH},
	{std::string("summonerteleport"),                SummonerSpellType::TELEPORT},
	{std::string("summonerboost"),                   SummonerSpellType::CLEANSE},
	{std::string("summonerdot"),                     SummonerSpellType::IGNITE},
	{std::string("summonersmite"),                   SummonerSpellType::SMITE},
	{std::string("summonerrecall"),                   SummonerSpellType::RECALL},
	{std::string("s5_summonersmiteplayerganker"),    SummonerSpellType::SMITE},
	{std::string("s5_summonersmiteduel"),            SummonerSpellType::SMITE},

};

float Spell::GetRemainingCooldown(float gameTime) {
	return (readyAt > gameTime ? readyAt - gameTime : 0.f);
}

const char* Spell::GetTypeStr() {
	return spellTypeName[(int)slot];
}

void Spell::MoveAndTrigger(const Vector2& pos) {
	return Input::MoveAndPress(spellSlotKey[(int)slot], pos.x, pos.y);
}

void Spell::Trigger(bool charge) {
	if (charge == true) {
		return Input::KeyDown(spellSlotKey[(int)slot]);
	}
	else {
		return Input::PressKey(spellSlotKey[(int)slot]);
	}
}

void Spell::LoadFromMem(DWORD64 base, HANDLE hProcess, bool deepLoad) {

	addressSlot = base;
	Mem::Read(hProcess, base, buffer, 0x150);

	memcpy(&readyAt, buffer + Offsets::SpellSlotTime, sizeof(float));
	memcpy(&level, buffer + Offsets::SpellSlotLevel, sizeof(int));
	memcpy(&ammo, buffer + Offsets::SpellSlotAmmo, sizeof(int));
	memcpy(&value, buffer + Offsets::SpellSlotDamage, sizeof(float));
	memcpy(&timeCharge, buffer + Offsets::SpellSlotSmiteCharges, sizeof(int));

	DWORD64 spellInfoPtr;
	memcpy(&spellInfoPtr, buffer + Offsets::SpellSlotSpellInfo, sizeof(DWORD64));
	
	DWORD64 spellDataPtr = Mem::ReadDWORD(hProcess, spellInfoPtr + Offsets::SpellInfoSpellData);
	DWORD64 spellNamePtr = Mem::ReadDWORD(hProcess, spellDataPtr + Offsets::SpellDataSpellName);
	DWORD64 spellMissileNamePtr = Mem::ReadDWORD(hProcess, spellDataPtr + Offsets::SpellDataMissileName);

	char buff[50];
	Mem::Read(hProcess, spellNamePtr, buff, 50);
	name = Character::ToLower(std::string(buff));

	char buff2[50];
	Mem::Read(hProcess, spellMissileNamePtr, buff2, 50);
	missileName = Character::ToLower(std::string(buff2));

	auto it = summonerSpellTypeDict.find(name.c_str());
	if (it != summonerSpellTypeDict.end())
		summonerSpellType = it->second;

	info = GameData::GetSpellInfoByName(name);
}

bool Spell::EqualSpellFlags(SpellFlags flags) const
{
	return info->flags == flags;
}

bool Spell::HasSpellFlags(SpellFlags flags) const
{
	return (info->flags & flags) == flags;
}

float Spell::GetSpeed() const
{
	return info->speed;
}

float Spell::GetCastRange() const
{
	return info->castRange;
}

float Spell::GetWidth() const
{
	return info->width;
}

float Spell::GetCastRadius() const
{
	return info->castRadius;
}

float Spell::GetDelay() const
{
	return info->delay;
}

float Spell::GetHeight() const
{
	return info->height;
}

float Spell::GetTravelTime() const
{
	return info->travelTime;
}

std::string Spell::GetIcon() const
{
	return info->icon;
}
