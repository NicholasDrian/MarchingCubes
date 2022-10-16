// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class MARCHINGCUBES_API Tests
{
public:
	Tests() = delete;
	static void RunTests();
private:
	static void TestMeshGen();
	static void TestPerlin();
	static void PrintMesh(const struct Mesh& mesh);
};
