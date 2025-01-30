#pragma once

#include <vector>
#include <iostream>





namespace voxel_manager
{
	unsigned const int chunkSize = 16;


	struct voxel
	{
		unsigned int type;  // air or voxel


		voxel(unsigned int type = 0);
	};


	
	struct chunk
	{
		std::vector<voxel> voxels; // Use a dynamic array for efficiency
		int position = 1;
		
		chunk();

	};


}
