#pragma once
#include "Cube.h"
#include "ShaderProgram.h"
#include "PerlinNoise.h"
#include "CubePalette.h"
#include "Ray.h"
#include "AABB.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <utility>
#include <iostream>

template <uint8_t Depth, uint8_t Width, uint8_t Height>
class Chunk {
	struct CubeData {
		Cube::Type m_type{ Cube::Type::None };
		bool m_isVisible{ true };
	};

	using FlattenData_t = std::array<CubeData, Depth* Width* Height>;
public:
	struct HitRecord {
		glm::ivec3 m_cubeIndex;
		glm::ivec3 m_neighbourIndex;
	};

	Chunk(const glm::vec2& origin, CubePalette& palette);

	void Generate(const PerlinNoise& rng);

	void Draw(ShaderProgram& shader) const;

	Ray::HitType Hit(const Ray& ray, Ray::time_t min, Ray::time_t max, HitRecord& record) const;

	bool RemoveBlock(uint8_t width, uint8_t height, uint8_t depth);
	bool PlaceBlock(uint8_t width, uint8_t height, uint8_t depth, Cube::Type type);

private:
	size_t CoordsToIndex(size_t depth, size_t width, size_t height) const;
	glm::vec3 IndexToCoords(size_t index) const;
	void UpdateVisibility();

	CubePalette& m_palette;
	FlattenData_t m_data;
	glm::vec2 m_origin;
	AABB m_aabb;
};

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline Chunk<Depth, Width, Height>::Chunk(const glm::vec2& origin, CubePalette& palette)
: m_palette(palette), m_origin(origin), 
m_aabb(
		glm::vec3(
			m_origin.x * static_cast<size_t>(Width),
			0.0f,
			m_origin.y * static_cast<size_t>(Depth)),
		glm::vec3(
			(m_origin.x + 1) * static_cast<size_t>(Width),
			static_cast<size_t>(Height),
			(m_origin.y + 1) * static_cast<size_t>(Depth))
	) {
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline void Chunk<Depth, Width, Height>::Generate(const PerlinNoise& rng) {
	std::cout << "Generating chunk" << std::endl 
	<< static_cast<size_t>(Width) << "; " 
	<< static_cast<size_t>(Depth) << "; " 
	<< static_cast<size_t>(Height)
	<< std::endl << "x: " << m_origin[0] << "\ny: "<< m_origin[1] << std::endl;
	for(int x=0; x < static_cast<size_t>(Width); x++) {
		for(int z=0; z < static_cast<size_t>(Depth); z++) {
			float local_noise = rng.At(glm::vec3(
				(float)x/static_cast<size_t>(Width) + m_origin.x, 
				0.25f, 
				(float)z/static_cast<size_t>(Depth) + m_origin.y));

			int local_height = floor(local_noise * static_cast<size_t>(Height)/4 + static_cast<size_t>(Height)/2);

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
	// std::unordered_map<Cube::Type, std::vector<glm::mat4>> model_instances;
	
	for(int cube_id=0; 
		cube_id < static_cast<size_t>(Depth)
			*static_cast<size_t>(Width)
			*static_cast<size_t>(Height); 
		cube_id++) {

		CubeData cube_data = m_data[cube_id];
		if(cube_data.m_isVisible){

			glm::mat4 model = glm::mat4(1.0f);

			glm::vec3 cube_position = IndexToCoords(cube_id);
			model[3] = glm::vec4(cube_position.x - static_cast<size_t>(Width) * m_origin.x, 
						cube_position.y, 
						cube_position.z  - static_cast<size_t>(Depth) * m_origin.y, 
						1.0f);

			const Cube& cube = m_palette.LookUp(cube_data.m_type);
			shader.SetMat4("model", model);

			shader.SetTexture("texture1", cube.Texture());

			glBindVertexArray(cube.Vao());
			glBindTexture(GL_TEXTURE_2D, cube.Texture());
			glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

			// if(!model_instances.contains(cube_data.m_type)) {
			// 	model_instances.emplace(cube_data.m_type, std::vector<glm::mat4>());
			// // 	model_instances.at(cube_data.m_type).push_back(model)
			// // } else {
			// }

			// model_instances.at(cube_data.m_type).push_back(model);

		}

		// std::unordered_map<Cube::Type, std::vector<glm::mat4>>::iterator item = model_instances.begin();

		// while(item != model_instances.end()) {
		// 	const Cube& cube = m_palette.LookUp(item->first); //cube_data.m_type);

		// 	glBindVertexArray(cube.Vao());
		// 	// glBindBuffer(GL_ARRAY_BUFFER, cube.Vbo());
		// 	// glBufferData(GL_ARRAY_BUFFER, sizeof(item->second.size()), &item->second.front(), GL_DYNAMIC_DRAW);

		// 	GLuint attribLocation = 2;
		// 	glEnableVertexAttribArray(attribLocation);
		// 	glVertexAttribIPointer(attribLocation, static_cast<int>(item->second.size()), GL_INT, sizeof(int), (void*)0);
		// 	glVertexAttribDivisor(attribLocation, 1);

		// 	shader.SetMat4Instanced("model", item->second);

		// 	shader.SetTexture("texture1", cube.Texture());

		// 	glBindTexture(GL_TEXTURE_2D, cube.Texture());
		// 	glDrawArraysInstanced(GL_TRIANGLES, 0, 6 * 6, item->second.size());

		// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
		// 	glBindVertexArray(0);
		// 	++item;
		// }
	}
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline Ray::HitType Chunk<Depth, Width, Height>::Hit(const Ray& ray, Ray::time_t min, Ray::time_t max, HitRecord& record) const {
	//	define hit record for chunk's AABB
	AABB::HitRecord chunk_record;
	AABB::HitRecord cube_record;
	//	early exit in case entire chunk was missed
	if(m_aabb.Hit(ray, min, max, chunk_record) == Ray::HitType::Miss) {
		return Ray::HitType::Miss;
	}

	//	calculate chunk shift
	glm::vec2 chunk_shift = glm::vec2(
			m_origin.x * static_cast<size_t>(Width), 
			m_origin.y * static_cast<size_t>(Depth)
		);
	//	for x,y,z in Width, Height, Depth
	for(int x=0; x < static_cast<size_t>(Width); x++) {
		for(int z=0; z < static_cast<size_t>(Depth); z++) {
			for(int y=0; y < static_cast<size_t>(Height); y++) {
		//		find CubeData in chunk
				uint8_t cube_id = CoordsToIndex(z, x, y);
				CubeData cube = m_data[cube_id];
		//		check visibility and type
				if(!cube.m_isVisible || cube.m_type == Cube::Type::None) {
					continue;
				} else {
			//		define cube AABB
					AABB cube_aabb = AABB(
						m_aabb.Min() + glm::vec3(x, y, z),
						m_aabb.Min() + glm::vec3(x + 1, y + 1, z + 1)
					);
			//		test ray with cube
					Ray::HitType cube_hit = cube_aabb.Hit(ray, min, max, cube_record);
			//		if hit
					if(cube_hit == Ray::HitType::Hit) {
			//			save last hit time to max variable
						if(cube_record.m_time > max) {
							continue;
						} else {
							max = cube_record.m_time;
				//			save cube index to hit record
							record.m_cubeIndex = glm::ivec3(z, x, y);
				//			calculate cube position in world (it's center!)
				//			calculate neighbour index based on axis
							AABB::Axis axis = cube_record.m_axis;
				//			save neighbour index to hit record
							if(axis == AABB::Axis::x) {
								record.m_neighbourIndex = glm::ivec3(z, x, y); //TODO: add neighbour recognition
							} else if(axis == AABB::Axis::y) {
								record.m_neighbourIndex = glm::ivec3(z, x, y);
							} else if(axis == AABB::Axis::z) {
								record.m_neighbourIndex = glm::ivec3(z, x, y);
							}
						}
					} 
				}
		//	if no cubes were hit return Miss
			}
		}
	}

	if (record.m_cubeIndex.x > -1 && record.m_cubeIndex.y > -1 && record.m_cubeIndex.z > -1 ) {
		return Ray::HitType::Hit;
	}

	return Ray::HitType::Miss;
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline bool Chunk<Depth, Width, Height>::RemoveBlock(uint8_t width, uint8_t height, uint8_t depth) {
	//	find CubeData in chunk
	//	check type
	//	replace type with none
	//	update visibility
	//	return operation's success
	return false;
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline bool Chunk<Depth, Width, Height>::PlaceBlock(uint8_t width, uint8_t height, uint8_t depth, Cube::Type type) {
	//	find CubeData in chunk
	//	check type
	//	replace type with new type
	//	update visibility
	//	return operation's success
	return false;
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline size_t Chunk<Depth, Width, Height>::CoordsToIndex(size_t depth, size_t width, size_t height) const {
	return height * static_cast<size_t>(Depth) * static_cast<size_t>(Width)
		+ width * static_cast<size_t>(Depth)
		+ depth;
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline glm::vec3 Chunk<Depth, Width, Height>::IndexToCoords(size_t index) const {
	size_t y = index / (static_cast<size_t>(Depth) * static_cast<size_t>(Width));
    size_t remainder = index % (static_cast<size_t>(Depth) * static_cast<size_t>(Width));
    size_t z = remainder / static_cast<size_t>(Depth);
    size_t x = remainder % static_cast<size_t>(Depth);

	return glm::vec3(x, y, z);
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline void Chunk<Depth, Width, Height>::UpdateVisibility() {
	for(int cube_id=0; 
		cube_id < static_cast<size_t>(Depth)
			*static_cast<size_t>(Width)
			*static_cast<size_t>(Height); 
		cube_id++ ) {
		CubeData& cube_data = m_data.at(cube_id);

		if(cube_data.m_type != Cube::Type::None) {
			cube_data.m_isVisible = false;

			glm::vec3 position = IndexToCoords(cube_id);

			if((position.x == 0 || position.x == static_cast<size_t>(Depth) - 1) ||
				(position.z == 0 || position.z == static_cast<size_t>(Width) - 1) ||
				(position.y == 0 || position.y == static_cast<size_t>(Height) - 1)
			) {
				cube_data.m_isVisible = true;
				continue;
			}

			const CubeData& y_p_neighbour = m_data[CoordsToIndex(position.x, position.z, position.y + 1)];
			if(y_p_neighbour.m_type == Cube::Type::None) {
				cube_data.m_isVisible = true;
				continue;
			}

			const CubeData& y_m_neighobour = m_data[CoordsToIndex(position.x, position.z, position.y - 1)];
			if(y_m_neighobour.m_type == Cube::Type::None) {
				cube_data.m_isVisible = true;
				continue;
			}

			const CubeData& x_p_neighobour = m_data[CoordsToIndex(position.x + 1, position.z, position.y)];
			if(x_p_neighobour.m_type == Cube::Type::None) {
				cube_data.m_isVisible = true;
				continue;
			}

			const CubeData& x_m_neighobour = m_data[CoordsToIndex(position.x - 1, position.z, position.y)];
			if(x_m_neighobour.m_type == Cube::Type::None) {
				cube_data.m_isVisible = true;
				continue;
			}

			const CubeData& z_p_neighobour = m_data[CoordsToIndex(position.x, position.z + 1, position.y)];
			if(z_p_neighobour.m_type == Cube::Type::None) {
				cube_data.m_isVisible = true;
				continue;
			}

			const CubeData& z_m_neighobour = m_data[CoordsToIndex(position.x, position.z - 1, position.y)];
			if(z_m_neighobour.m_type == Cube::Type::None) {
				cube_data.m_isVisible = true;
				continue;
			}
		} else {
			cube_data.m_isVisible = false;
		}
	}
}
