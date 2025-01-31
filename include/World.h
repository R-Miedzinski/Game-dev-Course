#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <utility>
#include <map>

#include <ShaderProgram.h>
#include <PerlinNoise.h>
#include <CubePalette.h>
#include <Chunk.h>

class World {
    public:
        const static uint8_t WIDTH = 4;
        const static uint8_t DEPTH = 4;
        const static uint8_t HEIGHT = 8;

        std::map<std::pair<int8_t, int8_t>, Chunk<DEPTH, WIDTH, HEIGHT>> m_chunks;

        World();
        // ~World();

        void RenderWorld(ShaderProgram& shader);
        void LoadWorld(glm::vec3 player_position);

    private:
        CubePalette m_cubePalette;
        PerlinNoise m_rng;
        uint8_t m_load_radius;
        uint8_t m_unload_radius;

        void GenerateChunk(int8_t x, int8_t z);
        void UnloadChunks(std::pair<int8_t, int8_t> player_chunk_position);
};