// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGen.h"

#include "PerlinNoise.h"
#include <unordered_set>


std::vector<Mesh> MeshGen::Gen(std::vector<FVector> positions, std::vector<float> vals, float threshold)
{
	Node node0(positions[0], vals[0]);
	Node node1(positions[1], vals[1]);
	Node node2(positions[2], vals[2]);
	Node node3(positions[3], vals[3]);
	Node node4(positions[4], vals[4]);
	Node node5(positions[5], vals[5]);
	Node node6(positions[6], vals[6]);
	Node node7(positions[7], vals[7]);
	//  connections go clockwise
	//    0      3        back
	//  1      2          front
	//                    X 5 -> 6
	//    4      7        Y 5 -> 4
	//  5      6          Z 5 -> 1
	// TODO: Make node topology constexpr
	node0.connections = {{&node1, NegY}, {&node4, NegZ}, {&node3, X}};
	node1.connections = {{&node0, Y}, {&node2, X}, {&node5, NegZ }};
	node2.connections = {{&node1, NegX}, {&node3, Y}, {&node6, NegZ}};
	node3.connections = {{&node2, NegY}, {&node0, NegX}, {&node7, NegZ}};
	node4.connections = {{&node5, NegY}, {&node7, X}, {&node0, Z}};
	node5.connections = {{&node1, Z}, {&node6, X}, {&node4, Y}};
	node6.connections = {{&node2, Z}, {&node7, Y}, {&node5, NegX}};
	node7.connections = {{&node3, Z}, {&node4, NegX}, {&node6, NegY}};
	std::vector<Mesh> res;
	if (!node0.checked && node0.val > threshold) res.push_back(Process(node0, threshold));
	if (!node1.checked && node1.val > threshold) res.push_back(Process(node1, threshold));
	if (!node2.checked && node2.val > threshold) res.push_back(Process(node2, threshold));
	if (!node3.checked && node3.val > threshold) res.push_back(Process(node3, threshold));
	if (!node4.checked && node4.val > threshold) res.push_back(Process(node4, threshold));
	if (!node5.checked && node5.val > threshold) res.push_back(Process(node5, threshold));
	if (!node6.checked && node6.val > threshold) res.push_back(Process(node6, threshold));
	if (!node7.checked && node7.val > threshold) res.push_back(Process(node7, threshold));
	return res;
}

Mesh MeshGen::Process(Node& n, float threshold)
{
	std::vector<FVector> perimiter;
	RecursiveProcess(n, perimiter, threshold, 0);
	return Triangulate(perimiter);
}

void MeshGen::RecursiveProcess(Node& n, std::vector<FVector>& perimiter, float threshold, int i)
{
	n.checked = true;
	for (int j = 0; j < 3; j++) {
		Connection c = n.connections[(i + j) % 3];
		if (c.node->val < threshold) {
			float distAbove = n.val - threshold;
			float distBelow = threshold - c.node->val;
			float totalDist = distAbove + distBelow;
			perimiter.push_back(n.position * distBelow / totalDist + c.node->position * distAbove / totalDist);
		}
		else {
			if (!c.node->checked) {
				int k = 0;
				switch (c.direction) {
					case X:
						while (c.node->connections[k++].direction != NegX); break;
					case Y:
						while (c.node->connections[k++].direction != NegY); break;
					case Z:
						while (c.node->connections[k++].direction != NegZ); break;
					case NegX:
						while (c.node->connections[k++].direction != X); break;
					case NegY:
						while (c.node->connections[k++].direction != Y); break;
					case NegZ:
						while (c.node->connections[k++].direction != Z); break;
				}
				RecursiveProcess(*c.node, perimiter, threshold, k);
			}
		}		
	}
}

Mesh MeshGen::Triangulate(const std::vector<FVector>& perimiter)
{
	Mesh res;
	res.vertices = perimiter;
	uint8 size = perimiter.size();
	switch (size) {
	case 3:
		res.indecies = { 2, 1, 0 }; break;
	case 4:
		res.indecies = { 0, 3, 2, 2, 1, 0 }; break;
	case 5:
		res.indecies = { 0, 4, 3, 0, 3, 2, 2, 1, 0 }; break;
	case 6:
		res.indecies = { 0, 5, 4, 4, 3, 2, 4, 2, 0, 2, 1, 0 }; break;
	}
	return res;
}
