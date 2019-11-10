#include "geometry_factory.h"

#include <algorithm>

// clang-format off

namespace client::GeometryFactory {
    Mesh createQuad(float width, float height)
    {
        Mesh mesh;

        mesh.vertices = {
            0,      0, 0,
            0,      height, 0,
            width,  height, 0,
            width,  0, 0
        };

        mesh.textureCoords = {
            0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
        };

        mesh.normals = {
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
        };

        mesh.indices = {
            0, 1, 2, 2, 3, 0
        };

        return mesh;
    }

    Mesh createCube(const glm::vec3 &min, const glm::vec3 &max)
    {
        Mesh cubeMesh;
		// Vertices
		cubeMesh.vertices = {

			min.x, min.y, max.z,
			max.x, min.y, max.z,
			max.x, max.y, max.z,
			min.x, max.y, max.z,

			min.x, min.y, min.z,
			max.x, min.y, min.z,
			max.x, max.y, min.z,
			min.x, max.y, min.z
		};


		// Indices
		cubeMesh.indices = {
			// front
			0, 1, 2, 2, 3, 0,
			// right
			1, 5, 6, 6, 2, 1,
			// back
			7, 6, 5, 5, 4, 7,
			// left
			4, 0, 3, 3, 7, 4,
			// bottom
			4, 5, 1, 1, 0, 4,
			// top
			3, 2, 6, 6, 7, 3,
		};

        
        for (int i = 0; i < 8; i++) {
            cubeMesh.normals.insert(cubeMesh.normals.end(), {0.0f, 1.0f, 0.0f});
        }

        for (int i = 0; i < 2; i++) {
            cubeMesh.textureCoords.insert(
                cubeMesh.textureCoords.end(),
                {1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f});
        }

        return cubeMesh;
    }

	std::vector<GLfloat> createCubeVerticies(const glm::vec3& min, const glm::vec3& max)
	{
		return {
			//Front
			min.x, min.y, max.z,
			max.x, min.y, max.z,
			max.x, max.y, max.z,
			min.x, max.y, max.z,
			// right
			max.x, min.y, min.z,
			max.x, max.y, min.z,
			max.x, max.y, max.z,
			max.x, min.y, max.z,
			// back
			min.x, min.y, min.z,
			min.x, max.y, min.z,
			max.x, max.y, min.z,
			max.x, min.y, min.z,
			// left
			min.x, min.y, min.z,
			min.x, min.y, max.z,
			min.x, max.y, max.z,
			min.x, max.y, min.z,
			// bottom
			min.x, min.y, max.z,
			min.x, min.y, min.z,
			max.x, min.y, min.z,
			max.x, min.y, max.z,
			// top
			min.x, max.y, max.z,
			max.x, max.y, max.z,
			max.x, max.y, min.z,
			min.x, max.y, min.z
		};
	}

	std::vector<GLuint> createCubeIndices(GLuint start)
	{
		std::vector<GLuint> indices;
		GLuint i = start;
        for (int itr = 0; itr < 6; itr++) {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
            indices.push_back(i + 2);
            indices.push_back(i + 3);
            indices.push_back(i);
            i += 4;
        }
		return indices;
	}

	void addBox(Mesh& mesh, const glm::vec3 &min, const glm::vec3 &max) 
	{
		auto v = createCubeVerticies(min, max);
		mesh.vertices.insert(mesh.vertices.end(), v.cbegin(), v.cend());

		mesh.normals.insert(mesh.normals.end(), {
			// front
			0, 0, 1, 
			0, 0, 1,
			0, 0, 1,
			0, 0, 1,
			// right
			1, 0, 0,
			1, 0, 0,
			1, 0, 0,
			1, 0, 0,
			// back
			0, 0, -1,
			0, 0, -1,
			0, 0, -1,
			0, 0, -1,
			// left
			-1, 0, 0,
			-1, 0, 0,
			-1, 0, 0,
			-1, 0, 0,
			// bottom
			0, -1, 0,
			0, -1, 0,
			0, -1, 0,
			0, -1, 0,
			// top
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
		});

        for (int i = 0; i < 6; i++) {
            mesh.textureCoords.insert(
                mesh.textureCoords.end(),
                {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f});
        }

        GLuint i = mesh.indices.empty() ? 0 : *std::max_element(mesh.indices.begin(), mesh.indices.end()) + 1;
        auto indices = createCubeIndices(i);
		mesh.indices.insert(mesh.indices.end(), indices.cbegin(), indices.cend());

	}

    Mesh createCubeWithNormals(const glm::vec3 &min, const glm::vec3 &max)
    {
        Mesh cubeMesh;
		addBox(cubeMesh, min, max);
        return cubeMesh;
    }
} // namespace client::GeometryFactory

// clang-format off
		