// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MARCHINGCUBES_API PerlinNoise
{
public:
	PerlinNoise() = delete;
	PerlinNoise(std::vector<float> Scales, float OutScale, uint32 Seed);
	PerlinNoise(FVector Scales, float OutScale, uint32 Seed);
	float Sample(std::vector<float> Location) const;
	float Sample(FVector Location) const;

private:

	float outScale;
	uint32 seed;
	std::vector<float> dimensions;
	float reduce(std::vector<float> vals, std::vector<float> weights) const;
	float getVertexVal(std::vector<float> vertex) const;
};
