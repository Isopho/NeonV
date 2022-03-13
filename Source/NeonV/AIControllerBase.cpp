// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

AAIControllerBase::AAIControllerBase() {
	//AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	//SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	//AIPerceptionComponent->ConfigureSense(*SightConfig);
	//AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());


	//AIPerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
	//AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::TargetPerceptionUpdated);

	//SightConfig->SightRadius = 500;
	//SightConfig->LoseSightRadius = 600;
	//SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	//SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	//SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	//AIPerceptionComponent->ConfigureSense(*SightConfig);
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay(); // We run Blueprint-beginplay, if we need it for some reason.

	// We get the perception component from the components list	
	AIPerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
	if (!ensure(AIPerceptionComponent)) { return; }
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::TargetPerceptionUpdated);

	if (!ensure(BlackboardToUse)) { return; } // We ensure that pointer isn't null
	UseBlackboard(BlackboardToUse, BB);
	if (!ensure(BehaviorTreeToUse)) { return; }// We ensure that BehaviorTree isn't null

	// Run the behavior tree
	RunBehaviorTree(BehaviorTreeToUse);



	//TArray<AActor*> ActorsSensed;
	//AIPerceptionComponent->GetPerceivedActors(HearingSense, ActorsSensed);

}

void AAIControllerBase::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("AAIControllerBase::TargetPerceptionUpdated" + Stimulus.StimulusLocation.ToString()));

	if (Stimulus.Type == UAISense::GetSenseID(SightSense) && Actor->ActorHasTag(FName("Player")))
	{
		if (!Stimulus.WasSuccessfullySensed()) {
			BB->ClearValue("SightedTarget");
			//K2_ClearFocus();
			UE_LOG(LogTemp, Warning, TEXT("Lost sight of Target"));
		}
		else
		{
			BB->SetValueAsObject("SightedTarget", Actor);
			//K2_SetFocus(Actor);
			UE_LOG(LogTemp, Warning, TEXT("Target sighted"));
		}
	}
}

