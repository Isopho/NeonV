// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class NEONV_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	// ======================================================================================
	//	public Properties
	// ======================================================================================

	UAISenseConfig_Sight* SightConfig;

	// We can set Blackboard in BP
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
		UBlackboardData* BlackboardToUse;

	// We can set BehaviorTree in BP
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
		UBehaviorTree* BehaviorTreeToUse;

	// This is the Blackboardcomponent that will be as Return value later. 
	UPROPERTY()
		UBlackboardComponent* BB;

	// Perception component. 
	UPROPERTY(VisibleAnywhere, Category = "AI")
		UAIPerceptionComponent* AIPerceptionComponent;

	UFUNCTION()
		void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
		TSubclassOf<UAISense> SightSense = UAISense_Sight::StaticClass();
	// ======================================================================================
	//	General Functions
	// ======================================================================================
	AAIControllerBase();

	virtual void BeginPlay() override;
};
