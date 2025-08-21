#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include "constants.h"




namespace voxel_manager
{

	extern float voxelVertices[6][24];

	extern unsigned int voxelIndices[6][6];



	enum voxel_type {
		AIR,
		SOLID
	};

	struct voxel
	{
		bool type = 0;  // air or voxel   air = 0  solid = 1

		bool faces[6] = {0, 0, 0, 0, 0, 0};  // back, front, left, right, bottom, top
		short visibleFaces = 0;

		short x = 0;
		short y = 0;
		short z = 0;

		void makeAndRenderMesh();




		voxel() = default; // <-- default constructor
		voxel(int voxel_x, int voxel_y, int voxel_z, bool t) : type(t), x(voxel_x), y(voxel_y), z(voxel_z) {}

	};

		
	struct chunk
	{
		voxel voxels[chunkSize][chunkSize][chunkSize];
		int x = 0;
		int y = 0;
		int z = 0;


		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		GLuint VBO, VAO, EBO;

		chunk(int x, int y, int z);

		void draw();
	};


	bool voxel_check_neighbor_type(int nx, int ny, int nz, voxel theseVoxels[chunkSize][chunkSize][chunkSize]);




}
