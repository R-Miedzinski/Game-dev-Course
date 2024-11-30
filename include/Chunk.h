#pragma once
#include "Cube.h"
#include "ShaderProgram.h"
#include "PerlinNoise.h"
#include "CubePalette.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

template <uint8_t Depth, uint8_t Width, uint8_t Height>
class Chunk {
	struct CubeData {
		Cube::Type m_type{ Cube::Type::None };
		bool m_isVisible{ true };
	};

	using FlattenData_t = std::array<CubeData, Depth* Width* Height>;
public:
	Chunk(const glm::vec2& origin, CubePalette& palette);

	void Generate(const PerlinNoise& rng);

	void Draw(ShaderProgram& shader) const;

private:
	size_t CoordsToIndex(size_t depth, size_t width, size_t height) const;
	glm::vec3 IndexToCoords(size_t index) const;
	void UpdateVisibility();

	CubePalette& m_palette;
	FlattenData_t m_data;
	glm::vec2 m_origin;
};

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline Chunk<Depth, Width, Height>::Chunk(const glm::vec2& origin, CubePalette& palette)
: m_palette(palette), m_origin(origin) {}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline void Chunk<Depth, Width, Height>::Generate(const PerlinNoise& rng) {
	std::cout << "Generating chunk" << std::endl 
	<< static_cast<size_t>(Width) << "; " 
	<< static_cast<size_t>(Depth) << "; " 
	<< static_cast<size_t>(Height)
	<< std::endl << "x: " << m_origin[0] << "\n y: "<< m_origin[1] << std::endl;
	for(int x=0; x < static_cast<size_t>(Width); x++) {
		for(int z=0; z < static_cast<size_t>(Depth); z++) {
			float local_noise = rng.At(glm::vec3(
				(float)x/static_cast<size_t>(Width) + m_origin[0], 
				0.25f, 
				(float)z/static_cast<size_t>(Depth) + m_origin[1]));

			int local_height = floor(local_noise * static_cast<size_t>(Height));

			for(int y=0; y < local_height; y++) {
				m_data[CoordsToIndex(z, x, y)] = CubeData(Cube::Type::Stone, true);
			}

			m_data[CoordsToIndex(z, x, local_height)] = CubeData(Cube::Type::Grass, true);

			for(int y=local_height + 1; y < static_cast<size_t>(Height); y++) {
				m_data[CoordsToIndex(z, x, y)] = CubeData(Cube::Type::None, false);
			}
		}
	}

	UpdateVisibility();
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline void Chunk<Depth, Width, Height>::Draw(ShaderProgram& shader) const {
	for(int cube_id=0; 
		cube_id < static_cast<size_t>(Depth)
			*static_cast<size_t>(Width)
			*static_cast<size_t>(Height); 
		cube_id++) {

		CubeData cube_data = m_data[cube_id];
		if(cube_data.m_isVisible){

			const Cube& cube = m_palette.LookUp(cube_data.m_type);
			glm::mat4 model = glm::mat4(1.0f);

			glm::vec3 cube_position = IndexToCoords(cube_id);
			model[3] = glm::vec4(cube_position, 1.0f) + glm::vec4(m_origin, 0.0f, 1.0f);

			shader.SetMat4("model", model);

			shader.SetTexture("texture1", cube.Texture());

			glBindTexture(GL_TEXTURE_2D, cube.Texture());
			glBindVertexArray(cube.Vao());

			glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
		}
	}
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline size_t Chunk<Depth, Width, Height>::CoordsToIndex(size_t depth, size_t width, size_t height) const {
	return height * static_cast<size_t>(Depth) * static_cast<size_t>(Width)
		+ width * static_cast<size_t>(Depth)
		+ depth;
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline glm::vec3 Chunk<Depth, Width, Height>::IndexToCoords(size_t index) const {
	size_t x = 0;
	size_t y = 0;
	size_t z = 0;

	z = index % (static_cast<size_t>(Depth) * static_cast<size_t>(Width)) % static_cast<size_t>(Depth);
	x = (index - z) % (static_cast<size_t>(Depth) * static_cast<size_t>(Width)) / static_cast<size_t>(Depth);
	y = (index - z - x) / (static_cast<size_t>(Depth) * static_cast<size_t>(Width));

	return glm::vec3(x, y, z);
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline void Chunk<Depth, Width, Height>::UpdateVisibility() {

}
