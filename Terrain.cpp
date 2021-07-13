#include "Terrain.h"
#include <cmath>

Terrain::Terrain():TerrainWidth{100.f},TerrainLength{100.f},Vao{0},Vbo{0}
{
	noise1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise1.SetFractalType(FastNoiseLite::FractalType_DomainWarpProgressive);
}

Terrain::Terrain(GLfloat Width, GLfloat Length) : TerrainWidth{Width}, TerrainLength{ Length }, Vao{ 0 }, Vbo{ 0 }
{
	noise1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise1.SetFractalType(FastNoiseLite::FractalType_FBm);
}

Terrain::~Terrain()
{
	TerrainPoints.clear();
	Vao = Vbo = 0;
}


//TODO:Remove redundant points and find a way to generate Index buffer objects to increase performance 
void Terrain::GenerateTerrain()
{
	if (TerrainPoints.size() > 0)
		TerrainPoints.clear();
	for (float x = 0; x < TerrainWidth; x=x+Distance)
	{
		for (float z = 0; z < TerrainLength; z+=Distance)
		{      
			    TerrainPoints.push_back(x);
				TerrainPoints.push_back(GenerateHeight(x,z+Distance));
				TerrainPoints.push_back(z + Distance); 
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);
	
				TerrainPoints.push_back(x);
				TerrainPoints.push_back(GenerateHeight(x, z));
				TerrainPoints.push_back(z);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);

				TerrainPoints.push_back(x + Distance);
				TerrainPoints.push_back(GenerateHeight(x+Distance,z));
				TerrainPoints.push_back(z);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);

				TerrainPoints.push_back(x);
				TerrainPoints.push_back(GenerateHeight(x, z + Distance));
				TerrainPoints.push_back(z + Distance);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);

				TerrainPoints.push_back(x + Distance);
				TerrainPoints.push_back(GenerateHeight(x + Distance, z));
				TerrainPoints.push_back(z);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);

				TerrainPoints.push_back(x+Distance);
				TerrainPoints.push_back(GenerateHeight(x+Distance, z+Distance));
				TerrainPoints.push_back(z + Distance);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);
				TerrainPoints.push_back(0.f);

		}
	}


	CalculateAverageNormals();
	CalcMaxHeight();

	glGenVertexArrays(1, &Vao);
	//Links Vao to OpenGL state machine as the vertex array object
	glBindVertexArray(Vao);

	glGenBuffers(1, &Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	//Links Vbo to OpenGL State machine
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*TerrainPoints.size(),&TerrainPoints[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray enables the generic vertex attribute array specified by index. 0 is the default index but you can have many
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	//glEnableVertexAttribArray enables the generic vertex attribute array specified by index. 0 is the default index but you can have many
	glEnableVertexAttribArray(0);

	//Set texture data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(1);
	//Unlinks Vbo from OpenGL state machine
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Unlinks Vao from state machine
	glBindVertexArray(0);
}

void Terrain::RenderTerrain()
{
	glBindVertexArray(Vao);
	//Explicitly binding Ibo because some GPU have bug which require you to explicitly bind Ibo to Vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,Vbo);
	//Params 1)How to consider vertices as,2) Number of elements to be rendered,3)Data type of index (byte-wise or int-wise) 4)Address of indices.0 because indices were already bound in CreateTr
    glDrawArrays(GL_TRIANGLES,0,TerrainPoints.size());
	//Unbind Ibo and Vao to prevent accidental data modification
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

float Terrain::GetMaxHeight()
{
	return MaxHeight;
}

void Terrain::SetSeed(int S)
{
	//TODO: Implement seed input from user
	std::cout << "foo\n";
}

float Terrain::GenerateHeight(float x, float z)
{
	float TotalNoise = 0.f;
	float BaseFrequency = 0.01f;
	float Effect = 0.24;
	float Temp = 10.f;

	noise1.SetSeed(0);
	noise1.SetFrequency(0.01f);
	noise1.SetFractalLacunarity(2.f);
	noise1.SetFractalGain(0.5f);
	
	
	TotalNoise = TotalNoise +5* noise1.GetNoise(x , z );
	noise1.SetFrequency(0.05f);
	TotalNoise = TotalNoise + noise1.GetNoise(x+20, z+20);
	noise1.SetFrequency(0.1f);
	TotalNoise = TotalNoise +0.5f* noise1.GetNoise(x + 30, z + 30);
	noise1.SetFrequency(0.5f);
	TotalNoise = TotalNoise + 0.25f * noise1.GetNoise(x + 30, z + 30);
	
	TotalNoise=10.f*TotalNoise;
	
	if (TotalNoise < 1.f)
		return 0;
	return TotalNoise;
}


void Terrain::CalculateAverageNormals()
{
	for (size_t i = 0; i < TerrainPoints.size(); i += 18)
	{
		glm::vec3 p1(TerrainPoints[i], TerrainPoints[i + 1], TerrainPoints[i + 2]);
		glm::vec3 p2(TerrainPoints[i+6], TerrainPoints[i + 7], TerrainPoints[i + 8]);
		glm::vec3 p3(TerrainPoints[i + 12], TerrainPoints[i + 13], TerrainPoints[i + 14]);
		glm::vec3 U = p2 - p1;
		glm::vec3 V = p3 - p1;
		float nx = (U.y * V.z) - (U.z * V.y);
		float ny = (U.z * V.x) - (U.x * V.z);
		float nz = (U.x * V.y) - (U.y * V.x);
		glm::vec3 norm = glm::vec3(nx, ny, nz);
		norm = glm::normalize(norm);
		TerrainPoints[i + 3] = TerrainPoints[i + 9] = TerrainPoints[i + 15] = norm.x;//-norm.x;
		TerrainPoints[i + 4] = TerrainPoints[i + 10] = TerrainPoints[i + 16] = norm.y;//-norm.y;
		TerrainPoints[i + 5] = TerrainPoints[i + 11] = TerrainPoints[i + 17] = norm.z;//-norm.z;
	}

}

void Terrain::CalcMaxHeight()
{
	MaxHeight = TerrainPoints[1];
	for(size_t i = 7; i < TerrainPoints.size(); i += 6)
	{
		if (MaxHeight < TerrainPoints[i])
		{
			MaxHeight = TerrainPoints[i];
		}
	}

}
