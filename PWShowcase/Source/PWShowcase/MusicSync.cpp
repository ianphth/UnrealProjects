// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicSync.h"

static class AMusicSync* CurrentSync;
static TArray<class AMusicSync*> SyncList;

// Sets default values
AMusicSync::AMusicSync()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMusicSync::BeginPlay()
{
	Super::BeginPlay();
	CurrentSync = this;
}

// Called every frame
void AMusicSync::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMusicSync::SectionValidate(FMusicSection inputSection, int startTimeSample)
{
	inputSection.StartTimeSamples = startTimeSample;
}

void AMusicSync::TimingCopy(FMusicTiming inputTiming, FMusicTiming copy)
{
	inputTiming = copy;
}

void AMusicSync::TimingSet(FMusicTiming inputTiming, int bar, int beat = 0, int unit = 0)
{
	inputTiming.Bar = bar;
	inputTiming.Unit = unit;
	inputTiming.Beat = beat;
}

int AMusicSync::GetMusicalTime(FMusicTiming inputTiming, FMusicSection section)
{
	return inputTiming.Bar * section.UnitPerBar + inputTiming.Beat * section.UnitPerBeat + inputTiming.Unit;
}

void AMusicSync::TimingFix(FMusicTiming inputTiming, FMusicSection section)
{
	int totalUnit = inputTiming.Bar * section.UnitPerBar + inputTiming.Beat * section.UnitPerBeat + inputTiming.Unit;
	inputTiming.Bar = totalUnit / section.UnitPerBar;
	inputTiming.Beat = (totalUnit - inputTiming.Bar * section.UnitPerBar) / section.UnitPerBeat;
	inputTiming.Unit = (totalUnit - inputTiming.Bar * section.UnitPerBar - inputTiming.Beat * section.UnitPerBeat);
}

void AMusicSync::UnitIncrement(FMusicTiming inputTiming, FMusicSection section)
{
	inputTiming.Unit++;
	TimingFix(inputTiming, section);
}

void AMusicSync::UnitDecrement(FMusicTiming inputTiming, FMusicSection section)
{
	inputTiming.Unit--;
	TimingFix(inputTiming, section);
}

void AMusicSync::BeatIncrement(FMusicTiming inputTiming, FMusicSection section)
{
	inputTiming.Beat++;
	TimingFix(inputTiming, section);
}

void AMusicSync::TimingAdd(FMusicTiming inputTiming, FMusicTiming addTiming, FMusicSection section)
{
	inputTiming.Bar += addTiming.Bar;
	inputTiming.Beat += addTiming.Beat;
	inputTiming.Unit += addTiming.Unit;
	TimingFix(inputTiming, section);
}

void AMusicSync::TimingSub(FMusicTiming inputTiming, FMusicTiming subTiming, FMusicSection section)
{
	inputTiming.Bar -= subTiming.Bar;
	inputTiming.Beat -= subTiming.Beat;
	inputTiming.Unit -= subTiming.Unit;
	TimingFix(inputTiming, section);
}

bool AMusicSync::TimingEquals(FMusicTiming t1, FMusicTiming t2)
{
	return (t1.Bar == t2.Bar && t1.Beat == t2.Beat && t1.Unit == t2.Unit);
}

int AMusicSync::TimingCompare(FMusicTiming t1, FMusicTiming t2)
{
	if (TimingEquals(t1, t2))
	{
		return 0;
	}
	else if (t1 > t2)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

double AMusicSync::Lag_()
{
	if (isFormerHalf_)
	{
		return timeSecFromJust_;
	}
	else
	{
		return timeSecFromJust_ - musicTimeUnit_;
	}
}

double AMusicSync::LagAbs_()
{
	if (isFormerHalf_)
	{
		return timeSecFromJust_;
	}
	else
	{
		return musicTimeUnit_ - timeSecFromJust_;
	}
}

double AMusicSync::Lag()
{
	return CurrentSync->Lag_();
}

double AMusicSync::LagUnit_()
{
	return Lag() / musicTimeUnit_;
}