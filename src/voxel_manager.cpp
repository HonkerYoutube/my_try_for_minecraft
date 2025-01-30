
#include "voxel_manager.h"





voxel_manager::voxel::voxel(unsigned int type) {
	this->type = type;

}




voxel_manager::chunk::chunk() {

	voxels.resize(chunkSize * chunkSize * chunkSize);

	for (unsigned int i = 0; i < voxels.size(); i++) {

		unsigned int x = i % chunkSize;                               // Extract X coordinate
		unsigned int y = (i / chunkSize) % chunkSize;                 // Extract Y coordinate
		unsigned int z = (i / (chunkSize * chunkSize)) % chunkSize;   // Extract Z coordinate

		voxels[i] = voxel(1); // Create solid voxel

		//std::cout << "Voxel at [" << x << ", " << y << ", " << z << "]\n";
	}
}