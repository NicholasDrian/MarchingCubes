// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(std::vector<float> Dimensions, float OutScale, uint32 Seed) :
    seed(Seed), dimensions(Dimensions.size()), outScale(OutScale) {}

PerlinNoise::PerlinNoise(FVector dimensions, float OutScale, uint32 Seed) :
    dimensions({ float(dimensions.X), float(dimensions.Y), float(dimensions.Z) }), seed(Seed), outScale(OutScale) {}

float PerlinNoise::getVertexVal(std::vector<float> vertex) const {
    long hash = CityHash64WithSeed((const char*)&(vertex[0]), vertex.size() * sizeof(float), seed);
    return FMath::Abs((double)hash / LONG_MAX);
}

float PerlinNoise::Sample(std::vector<float> Location) const
{
    check(Location.size() == dimensions.size());
    std::vector<float> min(dimensions.size());
    std::vector<float> max(dimensions.size());
    std::vector<float> weights(dimensions.size());
    for (uint32 i = 0; i < dimensions.size(); i++) {
        float mod = FMath::Fmod(Location[i], dimensions[i]);
        if (Location[i] < 0) mod += dimensions[i];
        min[i] = Location[i] - mod;
        max[i] = Location[i] - mod + dimensions[i];
        weights[i] = mod / dimensions[i];
    }
    std::vector<float> vals;
    for (uint32 i = 0; i < FMath::Pow<int, int>(2, dimensions.size()); i++) {
        std::vector<float> cornerPoint;
        for (uint32 j = 0; j < dimensions.size(); j++) {
            cornerPoint.push_back(((1 << j) & i) ? max[j] : min[j]);
        }
        vals.push_back(getVertexVal(cornerPoint));
    }
    return reduce(vals, weights) * outScale;
}

float PerlinNoise::Sample(FVector Location) const
{
    std::vector<float> v = { (float) Location.X, (float) Location.Y, (float) Location.Z };
    return Sample(v);
}

float PerlinNoise::reduce(std::vector<float> vals, std::vector<float> weights) const
{
    for (int i = 0; i < weights.size(); i++) {
        int step = 1 << i;
        for (int j = 0; j < vals.size(); j += 2 * step) {
            float res = FMath::Lerp(vals[j], vals[j + step], weights[i]);
            vals[j] = res;
        }
    }
    return vals[0];
}