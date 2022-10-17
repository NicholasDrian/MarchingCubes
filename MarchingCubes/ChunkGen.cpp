// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkGen.h"

#include "FractalNoise.h"
#include "PerlinNoise.h"
#include "MeshGen.h"
#include "Chunk.h"

FractalNoise* AChunkGen::noise = new FractalNoise({
	PerlinNoise(FVector(5000.0, 5000.0, 5000.0), 1.0, time(0))
	});

// Sets default values
AChunkGen::AChunkGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
		if (Distance(e.Key, PawnLocation) > loadDist * chunkSize) {
			trash.Add(e.Key);
		}
	}
	for (FIntVector key : trash) {
		AChunk* chunk = chunks[key];
		chunks.Remove(key);
		chunk->Destroy();
	}
}

void AChunkGen::AddChunks(const FIntVector& PawnLocation)
{
	for (int32 i = -loadDist / 2; i <= loadDist / 2; i++) {
		for (int32 j = -loadDist / 2; j <= loadDist / 2; j++) {
			FIntVector chunk(PawnLocation[0] + i, PawnLocation[1] + j, 0);
			if (Distance(chunk, PawnLocation) < loadDist && !chunks.Contains(chunk)) {
				FVector worldLocation((float)chunk[0], (float)chunk[1], 0.0);
				worldLocation *= chunkSize;
				UE_LOG(LogTemp, Warning, TEXT("chunk spawning at %f, %f"), worldLocation[0], worldLocation[1]);
				chunks.Add(chunk, GetWorld()->SpawnActor<AChunk>(worldLocation, FRotator(), FActorSpawnParameters()));
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

FIntVector AChunkGen::NearestChunk(FVector location) const {
	int32 x = (int32)(location[0] / chunkSize);
	int32 y = (int32)(location[1] / chunkSize);
	return { x, y, 0 };
}

float AChunkGen::Distance(FIntVector a, FIntVector b) const {
	float dx = (float)(a[0] - b[0]);
	float dy = (float)(a[1] - b[1]);
	float dz = (float)(a[2] - b[2]);
	return FMath::Sqrt(dx * dx + dy * dy + dz * dz);
}

