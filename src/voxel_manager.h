#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
        bool type = AIR;
        bool faces[6] = { 0, 0, 0, 0, 0, 0 };
        short visibleFaces = 0;

        short x = 0, y = 0, z = 0;

        voxel() = default;
        voxel(int voxel_x, int voxel_y, int voxel_z, bool t)
            : type(t), x(voxel_x), y(voxel_y), z(voxel_z) {
        }

        void makeAndRenderMesh();
        void makeFacesFunc(std::vector<float>* vertices, std::vector<unsigned int>* indices, int f);
    };

    struct chunk
    {
        voxel voxels[chunkSize][chunkSize][chunkSize];
        int x = 0, y = 0, z = 0;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        glm::mat4 model = 1.0f;

        GLuint VBO = 0, VAO = 0, EBO = 0;

        chunk() = default;
        chunk(int cx, int cy, int cz);

        void draw();
        void reset_graphics();
        void make_faces();
        void generate_voxels();
    };

    struct world
    {
        chunk chunks[worldSize][worldSize];


        void reset_chunk_graphics();
        void draw();
        void make_chunks();

    };


    bool voxel_check_neighbor_type(int nx, int ny, int nz, voxel theseVoxels[chunkSize][chunkSize][chunkSize], int chunk_x, int chunk_z);
    chunk* getChunkSafe(int chunkX, int chunkZ);
    enum faces {
        BACK,
        FRONT,
        LEFT,
        RIGHT,
        BOTTOM,
        TOP
    };



}

// Declare the shared pointer
extern voxel_manager::world* myWorld;
