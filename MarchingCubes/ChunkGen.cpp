// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkGen.h"

#include "FractalNoise.h"
#include "PerlinNoise.h"
#include "MeshGen.h"
#include "Chunk.h"

// Sets default values
AChunkGen::AChunkGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	blockSize = 100.0;
	blockCount = 20;
	loadDist = 3.0; // units: blocks

	PerlinNoise noise1(FVector(1000.0, 1000.0, 1000.0), 1.0, time(0));
	noise = new FractalNoise({ noise1 });
}

// Called when the game starts or when spawned
void AChunkGen::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChunkGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FIntVector PawnLocation = NearestChunk(GetPawnLocation());
	RemoveChunks(PawnLocation);
	AddChunks(PawnLocation);
}

void AChunkGen::RemoveChunks(const FIntVector& PawnLocation)
{
	TArray<FIntVector> trash;
	for (TPair<FIntVector, AChunk*> e : chunks) {
		if (Distance(e.Key, PawnLocation) > loadDist * blockSize * blockCount) {
			trash.Add(e.Key);
		}
	}
	for (FIntVector key : trash) {
		chunks[key]->Destroy();
		chunks.Remove(key);
	}
}

void AChunkGen::AddChunks(const FIntVector& PawnLocation)
{
	for (int32 i = -blockCount / 2; i <= blockCount / 2; i++) {
		for (int32 j = -blockCount / 2; j <= blockCount / 2; j++) {
			FIntVector block(PawnLocation[0] + i, PawnLocation[1] + j, 0);
			if (Distance(block, PawnLocation) < loadDist && !chunks.Contains(block)) {
				FVector worldLocation((float)block[0], (float)block[1], 0.0);
				worldLocation *= blockSize * blockCount;
				chunks[block] = GetWorld()->SpawnActor<AChunk>(worldLocation, FRotator(), FActorSpawnParameters());
			}
		}
	}
}

FVector AChunkGen::GetPawnLocation() const
{
	APawn* pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (pawn) return pawn->GetActorLocation();
	return FVector();
}

Mesh AChunkGen::Consolidate(std::vector<Mesh> meshes) const
{
	std::vector<FVector> Vertices;
	std::vector<uint32> Indices;
	TMap<FVector, uint32> map;
	for (Mesh mesh : meshes) {
		for (FVector v : mesh.vertices) {
			if (!map.Contains(v)) {
				map.Add(v, Vertices.size());
				Vertices.push_back(v);
			}
		}
		for (uint32 i : mesh.indecies) {
			Indices.push_back(map[mesh.vertices[i]]);
		}
	}
	return { Vertices, Indices };
}

FIntVector AChunkGen::NearestChunk(FVector location) const {
	int32 x = (int32)(location[0] / blockSize / blockCount);
	int32 y = (int32)(location[1] / blockSize / blockCount);
	return { x, y, 0 };
}

float AChunkGen::Distance(FIntVector a, FIntVector b) const {
	float dx = (float)(a[0] - b[0]);
	float dy = (float)(a[1] - b[1]);
	float dz = (float)(a[2] - b[2]);
	return FMath::Sqrt(dx * dx + dy * dy + dz * dz);
}

