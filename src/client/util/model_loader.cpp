#include "model_loader.h"

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../geometry/geometry_factory.h"

namespace {
    std::vector<int> splitFace(const std::string &input)
    {
        std::vector<int> tokens;
        std::stringstream stream(input);
        std::string token;
        while (std::getline(stream, token, '/') && tokens.size() < 3) {
            tokens.push_back(std::stoi(token));
        }
        return tokens;
    }

    glm::vec3 readLineVec3(const char *line)
    {
        glm::vec3 vec;
        std::sscanf(line, "%f %f %f", &vec.x, &vec.y, &vec.z);
        return vec;
    }

    glm::vec2 readLineVec2(const char *line)
    {
        glm::vec3 vec;
        std::sscanf(line, "%f %f", &vec.x, &vec.y);
        return vec;
    }
} // namespace

#include <thread>

namespace client {

    Mesh loadModel(const std::string &fileName)
    {
        std::ifstream inFile("res/models/" + fileName + ".obj");
        if (!inFile.is_open()) {
            throw std::runtime_error("Cannot open" + fileName);
        }
        Mesh mesh;

        std::vector<glm::vec3> verts;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;

        int indexCount = 0;
        std::string line;
        while (std::getline(inFile, line)) {
            if (line[0] == 'v') {

                switch (line[1]) {
                    // clang-format off
                    case ' ': verts.push_back(readLineVec3(&line[2])); break;
                    case 'n': normals.push_back(readLineVec3(&line[2])); break;
                    case 't': uvs.push_back(readLineVec2(&line[2])); break;
                    default: break;
                        // clang-format on
                }
            }
            else if (line[0] == 'f') {
                std::istringstream stream(line);
                std::string vertex;
                stream.seekg(2);
                while (std::getline(stream, vertex, ' ')) {

                    auto faceInfo = splitFace(vertex);

                    mesh.vertices.push_back(verts[faceInfo[0] - 1].x);
                    mesh.vertices.push_back(verts[faceInfo[0] - 1].y);
                    mesh.vertices.push_back(verts[faceInfo[0] - 1].z);

                    mesh.textureCoords.push_back(uvs[faceInfo[0] - 1].x);
                    mesh.textureCoords.push_back(uvs[faceInfo[0] - 1].y);

                    mesh.normals.push_back(normals[faceInfo[2] - 1].x);
                    mesh.normals.push_back(normals[faceInfo[2] - 1].y);
                    mesh.normals.push_back(normals[faceInfo[2] - 1].z);

                    mesh.indices.push_back(indexCount++);
                    mesh.indices.push_back(indexCount++);
                    mesh.indices.push_back(indexCount++);
                }
            }
        }
        return mesh;
    }
} // namespace client