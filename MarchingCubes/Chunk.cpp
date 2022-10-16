// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

#include "ChunkGen.h"
#include "MeshGen.h"
#include "FractalNoise.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	proceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = proceduralMesh;
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
	Generate();
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunk::Generate() {
	constexpr int32 count = AChunkGen::blockCount;
	float noise[count + 1][count + 1][count + 1];
	FVector locations[count + 1][count + 1][count + 1];
	for (int32 i = 0; i <= count; i++) {
		for (int32 j = 0; j <= count; j++) {
			for (int32 k = 0; k <= count; k++) {
				FVector location = GetActorLocation() + FVector(i, j, k) * AChunkGen::blockSize;
				noise[i][j][k] = AChunkGen::noise->Sample(location);
				locations[i][j][k] = location;
			}
		}
	}
	std::vector<Mesh> meshes;
	for (int32 i = 0; i < count; i++) {
		for (int32 j = 0; j < count; j++) {
			for (int32 k = 0; k < count; k++) {
				//    0      3        back
				//  1      2          front
				//                    X 5 -> 6
				//    4      7        Y 5 -> 4
				//  5      6          Z 5 -> 1
				std::vector<FVector> positions = {
					locations[i][j + 1][k + 1],
					locations[i][j][k + 1],
					locations[i + 1][j][k + 1],
					locations[i + 1][j + 1][k + 1],
					locations[i][j + 1][k],
					locations[i][j][k],
					locations[i + 1][j][k],
					locations[i + 1][j + 1][k]
				};
				std::vector<float> vals = {
					noise[i][j + 1][k + 1],
					noise[i][j][k + 1],
					noise[i + 1][j][k + 1],
					noise[i + 1][j + 1][k + 1],
					noise[i][j + 1][k],
					noise[i][j][k],
					noise[i + 1][j][k],
					noise[i + 1][j + 1][k]
				};
				std::vector<Mesh> cube = MeshGen::Gen(positions, vals, 0.5);
				meshes.insert(meshes.begin(), cube.begin(), cube.end());
			}
		}
	}
	Mesh mesh = Consolidate(meshes);
	Finalize(mesh);
}

void AChunk::Finalize(Mesh mesh)
{
	TArray<FVector> vertices;
	for (FVector v : mesh.vertices) vertices.Add(v);
	TArray<int32> indecies;
	for (int32 i : mesh.indecies) indecies.Add(i);
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;
	proceduralMesh->CreateMeshSection_LinearColor(0, vertices, indecies, normals, UV0, vertexColors, tangents, true);
	proceduralMesh->ContainsPhysicsTriMeshData(true);
}

Mesh AChunk::Consolidate(std::vector<Mesh> meshes) const
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