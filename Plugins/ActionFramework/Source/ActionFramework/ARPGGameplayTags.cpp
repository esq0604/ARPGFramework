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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Attack, "Abilities.Attack", "Attack Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_HitReact, "Abilities.HitReact", "HitReact Ability Tag");

	//Status
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Target has the death status.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Block_Parry, "Status.Block.Parry", "Target has the parry in block state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Attack_CanNextAttack, "Status.Attack.CanNextAttack", "Target can  combo attack");

	
	//Event
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Attack_Hit, "GameplayEvent.Attack.Hit", "Event on target hit");
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

}