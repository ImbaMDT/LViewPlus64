#pragma once
#include "ConfigSet.h"

/// Defines offsets for reading structs from league of legends memory
class Offsets {
	
public:
	Offsets();

	static int GameTime;

	static int ObjIndex;
	static int ObjTeam;
	static int ObjMissileName;
	static int ObjNetworkID;
	static int ObjPos;
	static int ObjMissileSpellCast;
	static int ObjVisibility;
	static int ObjSpawnCount;
	static int ObjHealth;
	static int ObjMaxHealth;
	static int ObjMana;
	static int ObjMaxMana;
	static int ObjRecallState;
	static int ObjRecallStateSize;
	static int ObjAbilityHaste;
	static int ObjLethality;
	static int ObjArmor;
	static int ObjBonusArmor;
	static int ObjMagicRes;
	static int ObjBonusMagicRes;
	static int ObjBaseAtk;
	static int ObjBonusAtk;
	static int ObjMoveSpeed;
	static int ObjSpellBook;
	static int ObjTransformation;
	static int ObjName;
	static int PlayerName;
	static int ObjLvl;
	static int ObjExpiry;
	static int ObjCrit;
	static int ObjCritMulti;
	static int ObjAbilityPower;
	static int ObjAtkSpeedMulti;
	static int ObjItemList;
	static int ObjSrcIndex;
	static int ObjAtkRange;
	static int ObjTargetable;
	static int ObjInvulnerable;
	static int ObjIsMoving;
	static int ObjDirection;
	static int ObjExpierience;
	static int ObjMagicPen;
	static int ObjMagicPenMulti;
	static int ObjAdditionalApMulti;
	static int ObjManaRegen;
	static int ObjHealthRegen;

	static int Ping;
	static int OffPing;
	static int ShowPing;

	static int Chat;
	static int ChatIsOpen;

	static int SpellBookActiveSpellCast;
	static int SpellBookSpellSlots;

	static int ObjBuffManager;
	static int BuffManagerEntriesArray;
	static int BuffEntryBuff;
	static int BuffType;
	static int BuffEntryBuffCount;
	static int BuffEntryBuffCountAlt;
	static int BuffEntryBuffCountAlt2;
	static int BuffEntryBuffStartTime;
	static int BuffEntryBuffEndTime;
	static int BuffName;
	static int BuffEntryBuffNodeStart;
	static int BuffEntryBuffNodeCurrent;

	static int ItemListItem;
	static int ItemInfo;
	static int ItemInfoId;

	static int CurrentDashSpeed;
	static int IsDashing;
	static int DashPos;
	static int IsMoving;
	static int NavBegin;
	static int NavEnd;

	static int ViewProjMatrices;
	static int Renderer;
	static int RendererWidth;
	static int RendererHeight;

	static int SpellSlotLevel;
	static int SpellSlotTime;
	static int SpellSlotAmmo;
	static int SpellSlotCharges;
	static int SpellSlotTimeCharge;
	static int SpellSlotDamage;
	static int SpellSlotSpellInfo;
	static int SpellInfoSpellData;
	static int SpellDataSpellName;
	static int SpellDataMissileName;
	static int SpellSlotSmiteTimer;
	static int SpellSlotSmiteCharges;

	static int ObjectManager;
	static int LocalPlayer;

	static int UnderMouseObject;

	static int HeroList;
	static int MinionList;
	static int MissileList;
	static int TurretList;

	static int ObjectMapCount;
	static int ObjectMapRoot;
	static int ObjectMapNodeNetId;
	static int ObjectMapNodeObject;

	static int MissileSpellInfo;
	static int MissileSrcIdx;
	static int MissileDestIdx;
	static int MissileStartPos;
	static int MissileEndPos;

	static int SpellCastSpellInfo;
	static int SpellCastStartTime;
	static int SpellCastStartTimeAlt;
	static int SpellCastCastTime;
	static int SpellCastStart;
	static int SpellCastEnd;
	static int SpellCastSrcIdx;
	static int SpellCastDestIdx;

	static int MinimapObject;
	static int MinimapObjectHud;
	static int MinimapHudPos;
	static int MinimapHudSize;

	static int AiManager;
	static int AiManagerStartPath;
	static int AiManagerEndPath;
	static int AiManagerTargetPosition;
	static int AiManagerIsMoving;
	static int AiManagerIsDashing;
	static int AiManagerCurrentSegment;
	static int AiManagerDashSpeed;
	static int ServerPos;
	static int Velocity;

	static int ObjNavPath1;
	static int ObjNavPath10;
	static int ObjLifeRegen;
	static int objBonusAtkSpeed;
	static int ObjMagicPenetration;
	static int ObjGreyHealth;
	static int ObjTime;
	static int ObjZombie;
	static int HudInstance; // maybe for going back to mouse pos
};