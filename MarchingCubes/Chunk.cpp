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
	for (int32 z = 0; z <= count; z++) {
		float bias = 1.0 - (float) z / count;
		bias *= AChunkGen::noise->MaxVal;
		for (int32 x = 0; x <= count; x++) {
			for (int32 y = 0; y <= count; y++) {
				FVector location = FVector(x, y, z) * AChunkGen::blockSize;
				float sample = AChunkGen::noise->Sample(GetActorLocation() + location);
				sample += bias;
				sample *= bias;
				noise[x][y][z] = sample;
				locations[x][y][z] = location;
			}
		}
	}

	std::vector<Mesh> meshes;
	for (int32 x = 0; x < count; x++) {
		for (int32 y = 0; y < count; y++) {
			for (int32 z = 0; z < count; z++) {
				//    0      3        back
				//  1      2          front
				//                    X 5 -> 6
				//    4      7        Y 5 -> 4
				//  5      6          Z 5 -> 1
				std::vector<FVector> positions = {
					locations[x][y + 1][z + 1],
					locations[x][y][z + 1],
					locations[x + 1][y][z + 1],
					locations[x + 1][y + 1][z + 1],
					locations[x][y + 1][z],
					locations[x][y][z],
					locations[x + 1][y][z],
					locations[x + 1][y + 1][z]
				};
				std::vector<float> vals = {
					noise[x][y + 1][z + 1],
					noise[x][y][z + 1],
					noise[x + 1][y][z + 1],
					noise[x + 1][y + 1][z + 1],
					noise[x][y + 1][z],
					noise[x][y][z],
					noise[x + 1][y][z],
					noise[x + 1][y + 1][z]
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