// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests.h"
#include "MeshGen.h"
#include "PerlinNoise.h"

void Tests::RunTests()
{
	//TestMeshGen();
	//TestPerlin();
}

void Tests::TestPerlin()
{
	PerlinNoise noise(FVector(1.f, 1.f, 1.f), 1, 2);
	for (float i = -5.f; i < 5.f; i += 0.2f) {
		float val = noise.Sample(FVector( 0, i, 0 ));
		FString s;
		for (float j = 0.0; j < val; j += 0.05) {
			s.Append("x");
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *s);
	}
}

void Tests::TestMeshGen()
{
	//  connections go clockwise
	//    0      3        back
	//  1      2          front
	//                    X 5 -> 6
	//    4      7        Y 5 -> 4
	//  5      6          Z 5 -> 1
	UE_LOG(LogTemp, Warning, TEXT("Testing Mesh Gen"));
	std::vector<FVector> positions = {
		{ 0.0, 1.0, 1.0 },
		{ 0.0, 0.0, 1.0 },
		{ 1.0, 0.0, 1.0 },
		{ 1.0, 1.0, 1.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 1.0, 0.0, 0.0 },
		{ 1.0, 1.0, 0.0 }
	};
	std::vector<float> vals = {
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0
	};
	std::vector<Mesh> res = MeshGen::Gen(positions, vals, 0.5);
	UE_LOG(LogTemp, Warning, TEXT("\n\tTesting all below threshold"));
	for (const Mesh& m : res) {
		PrintMesh(m);
	}
	vals = {
		1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0
	};
	res = MeshGen::Gen(positions, vals, 0.5);
	UE_LOG(LogTemp, Warning, TEXT("\n\tTesting all above threshold"));
	for (const Mesh& m : res) {
		PrintMesh(m);
	}
	vals = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0
	};
	res = MeshGen::Gen(positions, vals, 0.5);
	UE_LOG(LogTemp, Warning, TEXT("\n\tTesting one above threshold"));
	for (const Mesh& m : res) {
		PrintMesh(m);
	}
	vals = {
		1.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0
	};
	res = MeshGen::Gen(positions, vals, 0.5);
	UE_LOG(LogTemp, Warning, TEXT("\n\tTesting two above threshold"));
	for (const Mesh& m : res) {
		PrintMesh(m);
	}
	vals = {
		1.0, 1.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 0.0
	};
	res = MeshGen::Gen(positions, vals, 0.5);
	UE_LOG(LogTemp, Warning, TEXT("\n\tTesting three above threshold"));
	for (const Mesh& m : res) {
		PrintMesh(m);
	}
}

void Tests::PrintMesh(const Mesh& mesh)
{
	UE_LOG(LogTemp, Warning, TEXT("\t\tVertices:"));
	for (FVector v : mesh.vertices) {
		UE_LOG(LogTemp, Warning, TEXT("\t\t\t%f, %f, %f"), v.X, v.Y, v.Z);
	}
	UE_LOG(LogTemp, Warning, TEXT("\t\tIndecies:"));
	for (int i : mesh.indecies) {
		UE_LOG(LogTemp, Warning, TEXT("\t\t\t%d"), i);
	}
}
