
#include "voxel_manager.h"





voxel_manager::face::face(faceDirection direction) {
	
	this->direction = direction;
}




voxel_manager::voxel::voxel(int x, int y, int z, unsigned int type) {
	this->type = type;
}


void voxel_manager::voxel::makeMesh(unsigned int VAO, glm::vec3 cubePosition, unsigned int shaderProgram, glm::mat4 view, voxel_manager::chunk& thisChunk) {


	glBindVertexArray(VAO);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, cubePosition);
	/*float angle = 20.0f * i;
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));*/

	// retrieve the matrix uniform locations
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");

	// pass them to the shaders (3 different ways)
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

}

void voxel_manager::voxel::renderMesh() {

	glDrawArrays(GL_TRIANGLES, 0, 36);
}










voxel_manager::chunk::chunk() {

	float vertices[6][30] = {
		{-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f},

	    {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f},

		{-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f},

		{ 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f},

		{-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f},

		{-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f}
	};

	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	for (unsigned int i = 0; i < chunkSize * chunkSize * chunkSize; i++) {

		unsigned int x = i % chunkSize;                               // Extract X coordinate
		unsigned int y = (i / chunkSize) % chunkSize;                 // Extract Y coordinate
		unsigned int z = (i / (chunkSize * chunkSize)) % chunkSize;   // Extract Z coordinate

		voxels[x][y][z] = voxel(x, y, z, 1); // Create solid voxel

		//std::cout << "Voxel at [" << x << ", " << y << ", " << z << "]\n";
	}


}




// Returns true if the neighbor at (nx, ny, nz) is considered solid.
bool voxel_manager::chunk::isNeighborSolid(int nx, int ny, int nz, const voxel_manager::chunk& chunk) {
	// If neighbor indices are out of bounds, consider the neighbor not solid (i.e., there's air).
	if (nx < 0 || nx >= chunkSize || ny < 0 || ny >= chunkSize || nz < 0 || nz >= chunkSize) {
		return false;
	}
	return (chunk.voxels[nx][ny][nz].type == 1);
}



void voxel_manager::chunk::createAndRenderMesh(unsigned int shaderProgram, glm::mat4 view) {

	glBindVertexArray(VAO);

	for (int z = 0; z < chunkSize; z++) {
		for (int y = 0; y < chunkSize; y++) {
			for (int x = 0; x < chunkSize; x++) {


				voxel myVoxel = voxels[x][y][z];


				// Only process voxels that are "solid" (type 1).
				if (myVoxel.type != 1)
					continue;


				// Check all 6 neighbor directions.
				bool leftSolid = isNeighborSolid(x - 1, y, z, *this);
				bool rightSolid = isNeighborSolid(x + 1, y, z, *this);
				bool bottomSolid = isNeighborSolid(x, y - 1, z, *this);
				bool topSolid = isNeighborSolid(x, y + 1, z, *this);
				bool backSolid = isNeighborSolid(x, y, z - 1, *this);
				bool frontSolid = isNeighborSolid(x, y, z + 1, *this);


				// If all 6 neighbors are solid, this voxel is completely enclosed.
				bool fullyEnclosed = leftSolid && rightSolid && bottomSolid &&
					topSolid && backSolid && frontSolid;

				// If the voxel is not fully enclosed, generate and render its mesh.
				if (!fullyEnclosed) {
					// Compute world position for this voxel (if needed).
					glm::vec3 cubePosition(x, y, z);
					myVoxel.makeMesh(VAO, cubePosition, shaderProgram, view, *this);
					myVoxel.renderMesh();
				}

			}
		}
	}
}