// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum Direction;
struct Connection;
struct Node;
struct Mesh;

enum Direction : uint8 {
	X, Y, Z, NegX, NegY, NegZ
};

struct Connection {
	Node* node;
	Direction direction;
};

struct Node {
	FVector position;
	float val;
	std::vector<Connection> connections;
	bool checked;
	Node(FVector Position, float Val) : position(Position), val(Val), checked(false) {}
};

struct Mesh {
	std::vector<FVector> vertices;
	std::vector<uint32> indecies;
};

class MARCHINGCUBES_API MeshGen
{
public:
	MeshGen() = delete;
	static std::vector<Mesh> Gen(std::vector<FVector> positions, std::vector<float> vals, float threshold);
private:
	static Mesh Process(Node& n, float threshold);
	static void RecursiveProcess(Node& n, std::vector<FVector>& perimiter, float threshold, int i);
	static Mesh Triangulate(const std::vector<FVector>& perimiter);
};
