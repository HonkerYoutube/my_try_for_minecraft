
#include "voxel_manager.h"














void voxel_manager::voxel::makeAndRenderMesh() {
	glm::vec3 pos = glm::vec3(x, y, z);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
	setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}



void voxel_manager::voxel::makeFacesFunc(std::vector<float>* vertices, std::vector<unsigned int>* indices, int f) {
	const int floatsPerVertex = 6;
	unsigned int base = vertices->size() / floatsPerVertex;

	vertices->insert(vertices->end(), std::begin(voxelVertices[f]), std::end(voxelVertices[f]));
	for (int i = 0; i < 6; ++i)
		indices->push_back(voxelIndices[f][i] + base);
}





 





voxel_manager::chunk::chunk(int cx, int cy, int cz) {
	x = cx; y = cy; z = cz;

}

void voxel_manager::chunk::generate_voxels() {
	// Initialize voxels
	for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
			for (int z = 0; z < chunkSize; ++z)
				voxels[x][y][z] = voxel(x, y, z, SOLID);
}

void voxel_manager::chunk::make_faces() {
	if (glGenBuffers == nullptr) {
		std::cerr << "❌ glGenBuffers is null — OpenGL not initialized!\n";
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	if (VAO != 0 && VBO != 0 && EBO != 0)
		glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);



	

	const int floatsPerVertex = 6;

	for (int x = 0; x < chunkSize; ++x)
		for (int y = 0; y < chunkSize; ++y)
			for (int z = 0; z < chunkSize; ++z) {
				voxel* v = &voxels[x][y][z];
				if (v->type == AIR) continue;

				for (int f = 0; f < 6; ++f) {
					int nx = x, ny = y, nz = z;
					switch (f) {
					case 0: nz = z - 1; break; // back
					case 1: nz = z + 1; break; // front
					case 2: nx = x + 1; break; // left (your swapped layout)
					case 3: nx = x - 1; break; // right (your swapped layout)
					case 4: ny = y - 1; break; // bottom
					case 5: ny = y + 1; break; // top
					}


					// Check if neighbor is out of bounds
					bool outOfBounds = (nx < 0 || nx >= chunkSize ||
						ny < 0 || ny >= chunkSize ||
						nz < 0 || nz >= chunkSize);



					// LEFT (-X)
					if (nx < 0) {
						chunk* n_chunk = getChunkSafe(this->x - 1, this->z);
						if (n_chunk && y >= 0 && y < chunkSize && z >= 0 && z < chunkSize) {
							if (n_chunk->voxels[chunkSize - 1][y][z].type == SOLID) {
								continue; // Face is hidden by neighbor chunk
							}
						}
					}
					// RIGHT (+X)
					if (nx >= chunkSize) {
						chunk* n_chunk = getChunkSafe(this->x + 1, this->z);
						if (n_chunk && y >= 0 && y < chunkSize && z >= 0 && z < chunkSize) {
							if (n_chunk->voxels[0][y][z].type == SOLID) {
								continue; // Face is hidden by neighbor chunk
							}
						}
					}
					// BACK (-Z)
					if (nz < 0) {
						chunk* n_chunk = getChunkSafe(this->x, this->z - 1);
						if (n_chunk != nullptr) {
							if (n_chunk && y >= 0 && y < chunkSize && x >= 0 && x < chunkSize) {
								if (n_chunk->voxels[x][y][chunkSize - 1].type == SOLID) {
									continue; // Face is hidden by neighbor chunk
								}
							}
						}
					}
					// FRONT (+Z)
					if (nz >= chunkSize) {
						chunk* n_chunk = getChunkSafe(this->x, this->z + 1);
						if (n_chunk && y >= 0 && y < chunkSize && x >= 0 && x < chunkSize) {
							if (n_chunk->voxels[x][y][0].type == SOLID) {
								continue; // Face is hidden by neighbor chunk
							}
						}
					}



					// Only access voxel if in bounds
					bool neighborIsAir = outOfBounds || voxels[nx][ny][nz].type == AIR;
					bool visible = neighborIsAir;

					if (visible && !outOfBounds && voxels[nx][ny][nz].type != AIR) {
						std::cout << "❗ Face " << f << " at voxel (" << x << ", " << y << ", " << z
							<< ") is marked visible, but neighbor (" << nx << ", " << ny << ", " << nz
							<< ") is not AIR (type = " << voxels[nx][ny][nz].type << ")\n";
					}

					/*if (!visible && this->x == 1 && this->z == 0 && x == 0) {
						std::cout << "Culled face " << f << " at voxel (" << x << ", " << y << ", " << z << ")\n";
					}*/

					if (visible) {
						v->faces[f] = SOLID;
						unsigned int base = vertices.size() / floatsPerVertex;

						for (int i = 0; i < 4; ++i) {
							int vi = i * 6;
							float vx = voxelVertices[f][vi + 0];
							float vy = voxelVertices[f][vi + 1];
							float vz = voxelVertices[f][vi + 2];
							float r = voxelVertices[f][vi + 3];
							float g = voxelVertices[f][vi + 4];
							float b = voxelVertices[f][vi + 5];

							// Offset vertex position by voxel's position in chunk
							vertices.push_back(vx + x);
							vertices.push_back(vy + y);
							vertices.push_back(vz + z);
							vertices.push_back(r);
							vertices.push_back(g);
							vertices.push_back(b);
						}

						for (int i = 0; i < 6; ++i)
							indices.push_back(voxelIndices[f][i] + base);

						v->visibleFaces++;

					}
				}

			}

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}


void voxel_manager::chunk::reset_graphics() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}


void voxel_manager::chunk::draw() {

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//printf("vertices size: %i  indices size: %i \n", int(vertices.size()), int(indices.size()));

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * chunkSize, y * chunkSize, z * chunkSize));
	setMat4("model", model); // assuming this sets the uniform
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);





}








bool voxel_manager::voxel_check_neighbor_type(int nx, int ny, int nz, voxel theseVoxels[chunkSize][chunkSize][chunkSize], int chunk_x, int chunk_z) {
	if (nz < 0 || nz >= chunkSize) return AIR;
	if (ny < 0 || ny >= chunkSize) return AIR;
	if (nx < 0 || nx >= chunkSize) return AIR;
	return theseVoxels[nx][ny][nz].type;
}

voxel_manager::chunk* voxel_manager::getChunkSafe(int chunkX, int chunkZ) {
	if (chunkX < 0 || chunkX >= worldSize || chunkZ < 0 || chunkZ >= worldSize)
		return nullptr;
	return &myWorld->chunks[chunkX][chunkZ];
}








voxel_manager::world* myWorld = new voxel_manager::world;

void voxel_manager::world::make_chunks() {
	for (int x = 0; x < worldSize; x++)
		for (int z = 0; z < worldSize; z++) {
			chunks[x][z] = chunk(x, 0, z);
		}
	for (int x = 0; x < worldSize; x++)
		for (int z = 0; z < worldSize; z++) {
			chunks[x][z].generate_voxels();
		}
	for (int x = 0; x < worldSize; x++)
		for (int z = 0; z < worldSize; z++) {
			chunks[x][z].make_faces();
		}
}

void voxel_manager::world::reset_chunk_graphics() {
	for (int x = 0; x < worldSize; x++)
		for (int z = 0; z < worldSize; z++)
			chunks[x][z].reset_graphics();
}

void voxel_manager::world::draw() {
	for (int x = 0; x < worldSize; x++)
		for (int z = 0; z < worldSize; z++)
			chunks[x][z].draw();
}







float voxel_manager::voxelVertices[6][24] = {
	// image a cube that you could see all the vertices like you could see through it

	// back red
   {-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side bottom left vertex
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side bottom right vertex
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side top right vertex
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f},  // far back side top left vertex

	// front green
   {-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side bottom left vertex
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side bottom right vertex
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side top right vertex
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f},  // front side top left vertex

	// left yellow
   { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // right side bottom left vertex
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // right side bottom right vertex
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // right side top right vertex
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f},  // right side top left vertex

	// right blue
   {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // left side bottom left vertex
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // left side bottom right vertex
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // left side top right vertex
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f},  // left side top left vertex

	 // botom azure
   {-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // bottom side bottom left vertex
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // bottom side bottom right vertex
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // bottom side top right vertex
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f},  // bottom side top left vertex

	 // top white
   {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // top side bottom left vertex
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // top side bottom right vertex
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  // top side top right vertex
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f}  // top side top left vertex

};

unsigned int voxel_manager::voxelIndices[6][6] = {
	{0, 1, 2, 2, 3, 0}, // back
	{0, 1, 2, 2, 3, 0}, // front
	{0, 1, 2, 2, 3, 0}, // left
	{0, 1, 2, 2, 3, 0}, // right
	{0, 1, 2, 2, 3, 0}, // bottom
	{0, 1, 2, 2, 3, 0}  // top
};






