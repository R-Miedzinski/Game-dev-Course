#pragma once

#include "Cube.h"

#include <unordered_map>

class CubePalette {
public:
	CubePalette();

	const Cube& LookUp(Cube::Type type);

	void RegisterCube(const std::string& texturePath, Cube::Type type);

private:
	std::unordered_map<Cube::Type, Cube> m_palette;
};