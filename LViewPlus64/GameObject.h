#pragma once
#include <string>
#include <map>
#include <chrono>

#include "Vector.h"
#include "windows.h"
#include "MemoryLoadable.h"
#include "UnitInfo.h"
#include "GameData.h"
#include "Spell.h"
#include "SpellInterface.h"
#include "ItemSlot.h"
#include "BuffInstance.h"

#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python.hpp>

using namespace boost::python;
using namespace std::chrono;

class GameObject: MemoryLoadable, SpellInterface {

public:
	void                  LoadFromMem(DWORD64 base, HANDLE hProcess, bool deepLoad = true);

	uint64_t			  get_ai_address(DWORD64 base, HANDLE hProcess, bool deepLoad = true);
				          
	bool                  HasUnitTags(const UnitTag& type1) const;
				          
	float                 GetAcquisitionRadius() const;
	float                 GetSelectionRadius() const;
	float                 GetPathingRadius() const;
	float                 GetGameplayRadius() const;
				          
	float                 GetBasicAttackMissileSpeed()  const;
	float                 GetBasicAttackWindup() const;
				          
	float                 GetAttackSpeedRatio() const;
	float                 GetBaseMovementSpeed() const;
	float                 GetBaseAttackSpeed() const;
	float                 GetBaseAttackRange() const;
	float                 GetAttackRange() const;
	float                 GetHpBarHeight() const;
				          
	bool                  IsEnemyTo(const GameObject& other) const;
	bool                  IsAllyTo(const GameObject& other) const;
	bool                  IsEqualTo(const GameObject& other) const;
	bool                  IsNotEqualTo(const GameObject& other) const;
				          
public:			     	 
	float                 health;
	float                 maxHealth;
	float				  mana;
	float				  maxMana;
	float                 baseAttack;
	float                 bonusAttack;
	float                 armour;
	float                 magicResist;
	float                 crit;
	float                 critMulti;
	float                 abilityPower;
	float                 atkSpeedMulti;
	float                 movementSpeed;
	float                 duration;
	float 				  atkRange;
	float 				  manaRegen;
	float			  	  healthRegen;
	int					  isRecalling;
				          
	short                 objectIndex;
	short                 team;
				          
	float				  currentDashSpeed;
	bool				  isDashing;
	bool				  dIsMoving;
	Vector3				  dashPos;
	Vector3				  navBegin;
	Vector3				  navEnd;

	bool                  isVisible;
	bool                  isAlive;
	bool 				  isTargetable;
	bool 				  isInvulnerable;
	bool 				  isDirection;
	bool 				  isMoving;

	float                 lastVisibleAt;
				          
				          
	std::string           name;
	Vector3               position;

	Vector3               ai_navBegin;
	Vector3               ai_navEnd;
	DWORD64               ai_currentDashSpeed;
	DWORD64               currentTargetPosition;
	DWORD64               currentSegment;
	bool                  ai_isMoving;
	bool                  ai_isDashing;
	float                 velocity;
	Vector3               serverPos;

	high_resolution_clock::time_point timeSinceLastPreviousPosition;
	Vector3               previousPosition;
				          
	DWORD64               networkId;
	DWORD64               address;
				          
	UnitInfo*             unitInfo = GameData::UnknownUnit;
						 
protected:				 
	static const SIZE_T   sizeBuff     = 0x4100;
	static const SIZE_T   sizeBuffDeep = 0x1000;
						 
	static BYTE           buff[sizeBuff];
	static BYTE           buffDeep[sizeBuffDeep];

	// Champion related stuff
public:
	void                  LoadChampionFromMem(DWORD64 base, HANDLE hProcess, bool deepLoad = true);
	void				  LoadBuffFromMem(DWORD64 base, HANDLE hProcess, bool deepLoad);
	float                 GetBasicAttackDamage();
	Spell*                GetSummonerSpell(SummonerSpellType type);
			              
	bool                  IsRanged();
	boost::python::list                  ItemsToPyList();
	boost::python::list                  BuffsToPyList();
				          
	Spell                 Q = Spell(SpellSlot::Q);
	Spell                 W = Spell(SpellSlot::W);
	Spell                 E = Spell(SpellSlot::E);
	Spell                 R = Spell(SpellSlot::R);
	Spell                 D = Spell(SpellSlot::D);
	Spell                 F = Spell(SpellSlot::F);
				          
	DWORD64               level;
	ItemSlot              itemSlots[7];

	std::vector<BuffInstance> buffVector;

private:		         
	static DWORD64        spellSlotPointerBuffer[7];
	static BYTE           itemListBuffer[0x100];

	// Spell related stuff
public:
	void                  LoadMissileFromMem(DWORD64 base, HANDLE hProcess, bool deepLoad = true);
		                  
	bool                  HasSpellFlags(SpellFlags flags)   const override;
	bool                  EqualSpellFlags(SpellFlags flags) const override;
	float                 GetSpeed()                        const override;
	float                 GetCastRange()                    const override;
	float                 GetWidth()                        const override;
	float                 GetCastRadius()                   const override;
	float                 GetDelay()                        const override;
	float                 GetHeight()                       const override;
	float                 GetTravelTime()                   const override;
	std::string           GetIcon()                         const override;
						  
	short                 srcIndex;
	short                 destIndex;
	Vector3               startPos;
	Vector3               endPos;
				          
	SpellInfo*            spellInfo = GameData::UnknownSpell;
};