
#include "voxel_manager.h"














void voxel_manager::voxel::makeAndRenderMesh() {
	glm::vec3 pos = glm::vec3(x, y, z);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
	setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	/*if (faces[0])
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (faces[1])
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) (sizeof(GLint) * 1));
	if (faces[2])
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) (sizeof(GLint) * 2));
	if (faces[3])
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(GLint) * 3));
	if (faces[4])
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(GLint) * 4));
	if (faces[5])
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(GLint) * 5));*/
}






 





voxel_manager::chunk::chunk(int x, int y, int z) {
	this->x = x;
	this->y = y;
	this->z = z;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);



	for (int x = 0; x < chunkSize; x++) {
		for (int y = 0; y < chunkSize; y++) {
			for (int z = 0; z < chunkSize; z++) {
				voxels[x][y][z] = voxel(x, y, z, SOLID);
			}
		}
	}

	for (int x = 0; x < chunkSize; x++) {
		for (int y = 0; y < chunkSize; y++) {
			for (int z = 0; z < chunkSize; z++) {

				voxel *thisVoxel = &voxels[x][y][z];

				for (int f = 0; f < 6; f++) {
					

					// back
					if (f == 0 && x < chunkSize && x >= 0
							   && y < chunkSize && y >= 0
							   && z < chunkSize && z >= 0) {
						if (voxel_check_neighbor_type(x, y, z - 1, voxels) == AIR) {
							thisVoxel->faces[f] = SOLID;
							vertices.insert(vertices.end(), std::begin(voxelVertices[0]), std::end(voxelVertices[0]));
							
							unsigned int currentVertexCount = vertices.size() / 6; // 6 floats per vertex
							for (int i = 0; i < 6; i++) {
								indices.push_back(voxelIndices[f][i] + currentVertexCount);
							}

							thisVoxel->visibleFaces++;
						}
					}

					// front
					if (f == 1 && x < chunkSize && x >= 0
						&& y < chunkSize && y >= 0
						&& z < chunkSize && z >= 0) {
						if (voxel_check_neighbor_type(x, y, z + 1, voxels) == AIR) {
							thisVoxel->faces[f] = SOLID;
							vertices.insert(vertices.end(), std::begin(voxelVertices[1]), std::end(voxelVertices[1]));
							
							unsigned int currentVertexCount = vertices.size() / 6; // 6 floats per vertex
							for (int i = 0; i < 6; i++) {
								indices.push_back(voxelIndices[f][i] + currentVertexCount);
							}
							thisVoxel->visibleFaces++;
						}
					}

					// left
					if (f == 2 && x < chunkSize && x >= 0
						&& y < chunkSize && y >= 0
						&& z < chunkSize && z >= 0) {
						if (voxel_check_neighbor_type(x + 1, y, z, voxels) == AIR) {
							thisVoxel->faces[f] = SOLID;
							vertices.insert(vertices.end(), std::begin(voxelVertices[2]), std::end(voxelVertices[2]));
							
							unsigned int currentVertexCount = vertices.size() / 6; // 6 floats per vertex
							for (int i = 0; i < 6; i++) {
								indices.push_back(voxelIndices[f][i] + currentVertexCount);
							}
							thisVoxel->visibleFaces++;
						}
					}

					// right
					if (f == 3 && x < chunkSize && x >= 0
						&& y < chunkSize && y >= 0
						&& z < chunkSize && z >= 0) {
						if (voxel_check_neighbor_type(x - 1, y, z, voxels) == AIR) {
							thisVoxel->faces[f] = SOLID;
							vertices.insert(vertices.end(), std::begin(voxelVertices[3]), std::end(voxelVertices[3]));
							
							unsigned int currentVertexCount = vertices.size() / 6; // 6 floats per vertex
							for (int i = 0; i < 6; i++) {
								indices.push_back(voxelIndices[f][i] + currentVertexCount);
							}
							thisVoxel->visibleFaces++;
						}
					}

					// bottom
					if (f == 4 && x < chunkSize && x >= 0
						&& y < chunkSize && y >= 0
						&& z < chunkSize && z >= 0) {
						if (voxel_check_neighbor_type(x, y - 1, z, voxels) == AIR) {
							thisVoxel->faces[f] = SOLID;
							vertices.insert(vertices.end(), std::begin(voxelVertices[4]), std::end(voxelVertices[4]));
							
							unsigned int currentVertexCount = vertices.size() / 6; // 6 floats per vertex
							for (int i = 0; i < 6; i++) {
								indices.push_back(voxelIndices[f][i] + currentVertexCount);
							}
							thisVoxel->visibleFaces++;
						}
					}

					// top
					if (f == 5 && x < chunkSize && x >= 0
						&& y < chunkSize && y >= 0
						&& z < chunkSize && z >= 0) {
						if (voxel_check_neighbor_type(x, y + 1, z, voxels) == AIR) {
							thisVoxel->faces[f] = SOLID;
							vertices.insert(vertices.end(), std::begin(voxelVertices[5]), std::end(voxelVertices[5]));
							
							unsigned int currentVertexCount = vertices.size() / 6; // 6 floats per vertex
							for (int i = 0; i < 6; i++) {
								indices.push_back(voxelIndices[f][i] + currentVertexCount);
							}
							thisVoxel->visibleFaces++;
						}
					}
				}

				if (thisVoxel->visibleFaces == 0) {
					thisVoxel->type = AIR;
				}
			}
		}
	}

	// Upload vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// Upload indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GLintptr vertex_color_offset = 3 * sizeof(float);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)vertex_color_offset);
	glEnableVertexAttribArray(1);
}


void voxel_manager::chunk::draw() {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	int i = 0;
	for (int x = 0; x < chunkSize; x++) {
		for (int y = 0; y < chunkSize; y++) {
			for (int z = 0; z < chunkSize; z++) {

				voxel& v = voxels[x][y][z];
				v.makeAndRenderMesh();
				if (x == 0 && y == 0 && z == 0) {
					printf("first block faces: ba:%i, fr:%i, le:%i, ri:%i, bo:%i, to:%i  visibFaces: %i \n", v.faces[0], v.faces[1], v.faces[2], v.faces[3], v.faces[4], v.faces[5], v.visibleFaces);
				}
				i++;
			}
		}
	}

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}









bool voxel_manager::voxel_check_neighbor_type(int nx, int ny, int nz, voxel theseVoxels[chunkSize][chunkSize][chunkSize]) {
	if (nx >= chunkSize || nx < 0)
		return AIR;
	if (ny >= chunkSize || ny < 0)
		return AIR;
	if (nz >= chunkSize || nz < 0)
		return AIR;
	return theseVoxels[nx][ny][nz].type;
}




float voxel_manager::voxelVertices[6][24] = {
	// image a cube that you could see all the vertices like you could see through it

	// back
   {-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side bottom left vertex
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side bottom right vertex
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side top right vertex
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f},  // far back side top left vertex

	// front
   {-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side bottom left vertex
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side bottom right vertex
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side top right vertex
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f},  // front side top left vertex

	// left
   {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // left side bottom left vertex
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // left side bottom right vertex
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // left side top right vertex
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f},  // left side top left vertex

	// right
   { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // right side bottom left vertex
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // right side bottom right vertex
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // right side top right vertex
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f},  // right side top left vertex

	 // botom
	{-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // bottom side bottom left vertex
	  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // bottom side bottom right vertex
	  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // bottom side top right vertex
	 -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f},  // bottom side top left vertex

	 // top
	{-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // top side bottom left vertex
	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // top side bottom right vertex
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  // top side top right vertex
	 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f}  // top side top left vertex

};

unsigned int voxel_manager::voxelIndices[6][6] = {
	   {0, 1, 3,
		1, 2, 3},

	   {4, 5, 7,
		5, 6, 7},

	   {8, 9, 11,
		9, 10, 11},

	   {12, 13, 15,
		13, 14, 15},

	   {16, 17, 19,
		17, 18, 19},

	   {20, 21, 23,
		21, 22, 23}
};

