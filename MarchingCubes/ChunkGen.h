// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkGen.generated.h"


UCLASS()
class MARCHINGCUBES_API AChunkGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RemoveChunks(const FIntVector& PawnLocation);
	void AddChunks(const FIntVector& PawnLocation);

public:

	float blockSize;
	int32 blockCount;
	float loadDist;
	class FractalNoise* noise;

private:

	FVector GetPawnLocation() const;
	TMap<FIntVector, class AChunk*> chunks;
	struct Mesh Consolidate(std::vector<struct Mesh> meshes) const;
	FIntVector NearestChunk(FVector location) const;
	float Distance(FIntVector a, FIntVector b) const;

};
