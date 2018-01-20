// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicSync.generated.h"

UENUM(BlueprintType)
enum class ESectionType : uint8
{
	Undefined,
	Loop,
	Through,
	End
};

UENUM(BlueprintType)
enum class ESyncType : uint8
{
	NextBeat,
	Next2Beat,
	NextBar,
	Next2Bar,
	Next4Bar,
	Next8Bar,
	SectionEnd
};

USTRUCT(BlueprintType)
struct FMusicSection
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		FString SectionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		int UnitPerBeat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		int UnitPerBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		float Tempo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		int StartBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		int StartTimeSamples;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		ESectionType LoopType;

	FMusicSection()
	{
		SectionName = "None";
		UnitPerBeat = 4;
		UnitPerBar = 16;
		Tempo = 120.0;
	}
};

USTRUCT(BlueprintType)
struct FMusicTiming
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		int Beat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		int Bar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		int Unit;
	
	bool operator >(const FMusicTiming t2) 
	{ 
		return Bar > t2.Bar || (Bar == t2.Bar && Beat > t2.Beat) || (Bar == t2.Bar && Beat == t2.Beat && Unit > t2.Unit); 
	}
	bool operator ==(const FMusicTiming t2)
	{
		return (Bar == t2.Bar && Beat == t2.Beat && Unit == t2.Unit);
	}
	bool operator <(const FMusicTiming t2) 
	{ 
		return !(*this > t2) && !(*this == t2);
	}
	bool operator <=(const FMusicTiming t2) 
	{ 
		return !(*this > t2); 
	}
	bool operator >=(const FMusicTiming t2) 
	{ 
		return (*this > t2) || (*this == t2);
	}

	FMusicTiming(int bar = 0, int beat = 0, int unit = 0)
	{
		Beat = beat;
		Bar = bar;
		Unit = unit;
	}
};

UCLASS()
class PWSHOWCASE_API AMusicSync : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AMusicSync();
#pragma region Params
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		TArray<FMusicSection> MusicSections;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		class USoundCue* MusicSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		int EntryPointSample;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicSync")
		bool CreateSectionClips;
#pragma endregion

#pragma region Properties
	static double Lag() ;
#pragma endregion
private:
#pragma region Params
	FMusicTiming just_;
	FMusicTiming near_;
	bool isJustChanged_;
	bool isNearChanged_;
	bool isFormerHalf_;
	double timeSecFromJust_;
	int numRepeat_;
	double musicTimeUnit_;
	class USoundCue* musicSource_;
	int sectionIndex_;
	int currentSample_;
	int samplingRate_;
	int samplesPerUnit_;
	int samplesPerBeat_;
	int samplesPerBar_;
	int samplesInLoop_;
	FMusicTiming oldNear_, oldJust_;
	int numLoopBar_ = -1;
	TArray<class USoundCue*> quantizedCue_;
	const float PITCH_UNIT = FGenericPlatformMath::Pow(2.0f, 1.0f / 12.0f);
	TArray<class USoundCue*> sectionSources_;
	bool isTransitioning_ = false;
	FMusicTiming transitionTiming_ = FMusicTiming(0, 0, 0);
#pragma endregion

#pragma region Properties
	double Lag_();
	double LagAbs_();
	double LagUnit_();
	double LagUnit_();
	float MusicalTime_();
	float MusicalTimeBar_();
	float AudioTimeSec_();
	int TimeSamples_();
	int SectionCount_();
	int UnitPerBar_();
	int UnitPerBeat_();
	bool IsPlaying_();
	FMusicSection CurrentSection_();
#pragma endregion
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "MusicSync")
		UAudioComponent* MusicPlayer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma region Functions
	UFUNCTION(BlueprintCallable, Category = "MusicSync")
		void SectionValidate(FMusicSection inputSection, int startTimeSample);
	UFUNCTION(BlueprintCallable, Category = "MusicSync")
		void TimingCopy(FMusicTiming inputTiming, FMusicTiming copy);
	UFUNCTION(BlueprintCallable, Category = "MusicSync")
		void TimingSet(FMusicTiming inputTiming, int bar, int beat, int unit);
	UFUNCTION(BlueprintCallable, Category = "MusicSync")
		void TimingFix(FMusicTiming inputTiming, FMusicSection section);
	UFUNCTION(BlueprintCallable, Category = "MusicSync")
		void UnitIncrement(FMusicTiming inputTiming, FMusicSection section);
	UFUNCTION(BlueprintCallable, Category = "MusicSync")
		void UnitDecrement(FMusicTiming inputTiming, FMusicSection section);
	UFUNCTION(BlueprintCallable, Category = "MusicSync")
		void BeatIncrement(FMusicTiming inputTiming, FMusicSection section);
	UFUNCTION(BlueprintCallable, Category = "MusicSync")
		void TimingAdd(FMusicTiming inputTiming, FMusicTiming addTiming, FMusicSection section);
	UFUNCTION(BlueprintCallable, Category = "MusicSync")
		void TimingSub(FMusicTiming inputTiming, FMusicTiming subTiming, FMusicSection section);
	UFUNCTION(BlueprintPure, Category = "MusicSync")
		bool TimingEquals(FMusicTiming t1, FMusicTiming t2);
	UFUNCTION(BlueprintPure, Category = "MusicSync")
		int TimingCompare(FMusicTiming t1, FMusicTiming t2);
	UFUNCTION(BlueprintPure, Category = "MusicSync")
		int GetMusicalTime(FMusicTiming inputTiming, FMusicSection section);
#pragma endregion
private:

};