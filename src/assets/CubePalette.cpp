#include <CubePalette.h>
#include <string>

std::string grass_texture = "src/graphics/textures/grass.jpg";
std::string stone_texture = "src/graphics/textures/stone.jpg";

CubePalette::CubePalette() {
    m_palette.emplace(Cube::Type::Grass, Cube(grass_texture));
    m_palette.emplace(Cube::Type::Stone, Cube(stone_texture));
}

const Cube& CubePalette::LookUp(Cube::Type type) {
    if (m_palette.contains(type)) {
        return m_palette.at(type);
    } else {
        return m_palette.at(Cube::Type::None);
    }
}

void CubePalette::RegisterCube(const std::string& texturePath, Cube::Type type) {
    if(m_palette.contains(type)) {
        return;
    }

    m_palette.emplace(type, Cube(texturePath));
}