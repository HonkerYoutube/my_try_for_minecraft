#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>





namespace voxel_manager
{
	unsigned const int chunkSize = 16;


	struct voxel
	{
		unsigned int type = 0;  // air or voxel
		int x = 0;
		int y = 0;
		int z = 0;

		voxel() : type(0), x(0), y(0), z(0) {}  // Initialize all members
		voxel(int x, int y, int z, unsigned int type = 0);
		void render();
	};



	struct chunk
	{
		std::vector<voxel> voxels;
		int position = 1;


		void renderMesh();

		voxel getVoxel(unsigned int x, unsigned int y, unsigned int z);

		unsigned int VBO, VAO, EBO;


		chunk();

	private:
		unsigned int extractXFromVoxels;
	};


}