#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>





namespace voxel_manager
{
	unsigned const int chunkSize = 16;
	enum faceDirection { LEFT, RIGHT, BOTTOM, TOP, BACK, FRONT };

	struct chunk;

	struct face
	{ 
		faceDirection direction;
		face() : direction(LEFT) {}
		face(faceDirection direction);
	};


	struct voxel
	{
		face faces[6];

		unsigned int type = 0;  // air or voxel
		int x = 0;
		int y = 0;
		int z = 0;

		voxel() : type(0), x(0), y(0), z(0) {}  // Initialize all members
		voxel(int x, int y, int z, unsigned int type = 0);
		void renderMesh();
		void makeMesh(unsigned int VAO, glm::vec3 cubePosition, unsigned int shaderProgram, glm::mat4 view, voxel_manager::chunk& thisChunk);
	};



	struct chunk
	{
		voxel voxels[16][16][16];
		int position = 1;

		
		void createAndRenderMesh(unsigned int shaderProgram, glm::mat4 view);

		bool isNeighborSolid(int nx, int ny, int nz, const voxel_manager::chunk& chunk);

		//voxel getVoxel(unsigned int x, unsigned int y, unsigned int z);

		unsigned int VBO, VAO, EBO;


		chunk();

	};


}