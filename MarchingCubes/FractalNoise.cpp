// Fill out your copyright notice in the Description page of Project Settings.


#include "FractalNoise.h"

#include "PerlinNoise.h"

FractalNoise::FractalNoise(std::vector<class PerlinNoise> Noises): noises(Noises) {
    MaxVal = 0;
    for (PerlinNoise noise : Noises) {
        MaxVal += noise.outScale;
    }
}

float FractalNoise::Sample(FVector Location) const
{
    float res = 0;
    for (PerlinNoise noise : noises) {
        res += noise.Sample(Location);
    }
    return res;
}
