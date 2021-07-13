#pragma once

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "FastNoiseLite.h"
#include "stb_perlin.h"
//#include "PerlinNoise.hpp"
#define STB_PERLIN_IMPLEMENTATION

class Terrain
{
public:
	Terrain();
	Terrain(GLfloat Width, GLfloat Length);
	~Terrain();
	void GenerateTerrain();
	void RenderTerrain();
	float GetMaxHeight();

	void SetSeed(int S);
private:
	std::vector<float> TerrainPoints;
	GLfloat TerrainWidth;
	GLfloat TerrainLength;
	GLuint Vao;
	GLuint Vbo;
	FastNoiseLite noise1;
	FastNoiseLite noise2;
	float GenerateHeight(float x, float z);
	float Distance = 0.1f;
	float MaxHeight=0.f;
	void CalculateAverageNormals();
	void CalcMaxHeight();
	float Seed = 1337;
	bool IsChanged = true;
	int PtCt=0;
	int NCt = 0;
	//siv::PerlinNoise perlinA;
};

