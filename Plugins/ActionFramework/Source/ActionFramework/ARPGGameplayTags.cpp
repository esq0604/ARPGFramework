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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Execute, "Abilities.Execute", "Execute Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Executed, "Abilities.Executed", "Executed Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_HitReact, "Abilities.HitReact", "HitReact Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Parry, "Abilities.Parry", "Parry Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Sprint, "Abilities.Sprint", "Sprint Ability Tag");

	//Effect
	
	//Status
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Crouch, "Status.Crouch", "Target has the Crouch status.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Target has the death status.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Parry, "Status.Parry", "Target has the block state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Block, "Status.Block", "Target has the parry state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Sprint, "Status.Sprint", "Target has the sprint state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Attack_CanNextAttack, "Status.Attack.CanNextAttack", "Target can  combo attack");

	
	//Event
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Attack_Hit, "GameplayEvent.AttackHit", "AttackHit Event For WaitGameplayEvent");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Block, "GameplayEvent.Block", "Block Event For ActivateAbility");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Parry, "GameplayEvent.Parry", "Parry Event For ActivateAbility");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_HitReact, "GameplayEvent.HitReact", "HitReact For ActivateAbility");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Executed, "GameplayEvent.Executed", "Executed Event For WaitGameplayEvent");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Execution, "GameplayEvent.Execution", "Execution EventFor WaitGameplayEvent");


	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_OpenInventory, "ItemType.Equipment.Weapon", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_OpenEquipment, "ItemType.Equipment.Weapon", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_ExitGame, "ItemType.Equipment.Weapon", "Item type tag ");

	//MovementMode
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Default, "Movement.Mode.Default", "Character movement mode tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Strafe, "Movement.Mode.Strafe", "Character movement mode tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Block, "Movement.Mode.Block", "Character movement mode tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Sprint, "Movement.Mode.Sprint", "Character movement mode tag ");


	//ItemType
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Equipment_Weapon, "ItemType.Equipment.Weapon", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Equipment_Helmet, "ItemType.Equipment.Helmet", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Equipment_Armor, "ItemType.Equipment.Armor", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Equipment_Gloves, "ItemType.Equipment.Gloves", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Equipment_Shoes, "ItemType.Equipment.Shoes", "Item type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemType_Tool, "ItemType.Tool", "Item type tag ");


	//Input
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Attack, "Input.Attack", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Block, "Input.Block", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Move, "Input.Move", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Crouch, "Input.Crouch", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Look, "Input.Look", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Esc, "Input.Esc", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Targeting, "Input.Targeting", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Sprint, "Input.Sprint", "Input type tag ");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_MouseWheelMove, "Input.MouseWheelMove", "Input type tag ");


	//GameplayCue
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Parry, "GameplayCue.Parry", "GameplayCue.Parry Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Block, "GameplayCue.Block", "GameplayCue.Block Tag");


	//UI State
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StatusBar_State_None, "StatusBar.State.None", "StatusBar UI State Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StatusBar_State_Targeted, "StatusBar.State.Targeted", "StatusBar UI State Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StatusBar_State_DamageResponse, "StatusBar.State.DamageResponse", "StatusBar UI State Type");
}