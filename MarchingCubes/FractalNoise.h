// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerlinNoise.h"

/**
 * 
 */
class MARCHINGCUBES_API FractalNoise
{
public:
	FractalNoise(std::vector<class PerlinNoise> Noises);
	float Sample(FVector Location) const;
private:
	std::vector<class PerlinNoise> noises;
};
