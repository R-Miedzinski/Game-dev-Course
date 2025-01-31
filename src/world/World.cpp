#include <World.h>

#include <iostream>

uint8_t CalculateDistance(std::pair<int8_t, int8_t> chunk,
std::pair<int8_t, int8_t> player_position) {
    return
    (chunk.first - player_position.first) ^ 2 + 
    (chunk.second - player_position.second) ^ 2;
}

World::World() {
    m_cubePalette = CubePalette();
    m_rng = PerlinNoise();
    m_load_radius = 4;
    m_unload_radius = 8;
}

void World::RenderWorld(ShaderProgram& shader) {
    for (const auto& [key, value] : m_chunks) {
        value.Draw(shader);
    }
}

void World::LoadWorld(glm::vec3 player_position) {
    std::cout << "player chunk position: " 
        << player_position.x/WIDTH << ";"
        << player_position.z/DEPTH 
        << std::endl;

    std::pair<int8_t, int8_t> player_chunk_position = std::make_pair(
        player_position.x/WIDTH, 
        player_position.z/DEPTH
    );

    UnloadChunks(player_chunk_position);

    for (int8_t i=-m_load_radius; i<=m_load_radius; i++) {
        for (int8_t j=-m_load_radius; j<=m_load_radius; j++) {
            std::pair<int8_t, int8_t> chunk_to_load = 
                std::make_pair(player_chunk_position.first + i, player_chunk_position.second + j);
            
            if (!m_chunks.contains(chunk_to_load)) {
                GenerateChunk(chunk_to_load.first, chunk_to_load.second);
            }
        }
    }
}

void World::GenerateChunk(int8_t x, int8_t z) {
    m_chunks.emplace(
        std::make_pair(x, z), 
        Chunk<DEPTH, WIDTH, HEIGHT>(glm::vec2(x * WIDTH, z * DEPTH), m_cubePalette)
    );

    m_chunks.at(std::make_pair(x, z)).Generate(m_rng);
}

void World::UnloadChunks(std::pair<int8_t, int8_t> player_chunk_position) {
    if (!m_chunks.empty()) {
        std::vector<std::pair<int8_t, int8_t>> to_remove;

        for (const auto& [key, value] : m_chunks) {
            if (CalculateDistance(key, player_chunk_position) >= m_unload_radius * m_unload_radius ) {
                to_remove.push_back(key);
            }
        }

        for (const auto& key : to_remove) {
            m_chunks.erase(key);
        }
    }
}