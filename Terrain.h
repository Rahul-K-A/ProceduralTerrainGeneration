#pragma once

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "FastNoiseLite.h"


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
	float GenerateHeight(float x, float z);
	float Distance = 0.2f;
	float MaxHeight=0.f;
	void CalculateAverageNormals();
	void CalcMaxHeight();
};

