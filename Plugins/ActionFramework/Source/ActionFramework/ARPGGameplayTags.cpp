// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

//FARPGGameplayTags FARPGGameplayTags::GameplayTags;
//
//FARPGGameplayTags::FARPGGameplayTags()
//{
//	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Target has the death status.");
//
//}

namespace ARPGGameplayTags
{
	//Ability
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Attack, "Abilities.Attack", "Attack Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_HitReact, "Abilities.HitReact", "HitReact Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Parry, "Abilities.HitReact", "Parry Ability Tag");

	//Effect
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEffects_HitReact, "GameplayEffects.HitReact", "HitReact GameplayEffect");

	//Status
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Target has the death status.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Parry, "Status.Parry", "Target has the block state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Block, "Status.Block", "Target has the parry state");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Attack_CanNextAttack, "Status.Attack.CanNextAttack", "Target can  combo attack");

	
	//Event
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Attack_Hit, "GameplayEvent.Attack.Hit", "Event on target hit");

	//ItemType
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_OpenInventory, "ItemType.Equipment.Weapon", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_OpenEquipment, "ItemType.Equipment.Weapon", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_ExitGame, "ItemType.Equipment.Weapon", "Item type tag ");

	//MovementMode
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Default, "Movement.Mode.Default", "Character movement mode tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Strafe, "Movement.Mode.Strafe", "Character movement mode tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Block, "Movement.Mode.Block", "Character movement mode tag ");

	//ItemType
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Equipment_Weapon, "ItemType.Equipment.Weapon", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Equipment_Helmet, "ItemType.Equipment.Helmet", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Equipment_Armor, "ItemType.Equipment.Armor", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Equipment_Gloves, "ItemType.Equipment.Gloves", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Equipment_Shoes, "ItemType.Equipment.Shoes", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Tool, "ItemType.Tool", "Item type tag ");


	//Input
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_RightClick, "Input.LightClick", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_LeftClick, "Input.RightClick", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Move, "Input.Move", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Look, "Input.Look", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Esc, "Input.Esc", "Input type tag ");

	//GameplayCue
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Parry, "GameplayCue.Parry", "GameplayCue.Parry Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Block, "GameplayCue.Block", "GameplayCue.Block Tag");
}