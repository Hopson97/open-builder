#include "geometry_factory.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <common/debug.h>
/*
namespace {
    struct BoxArgs {
        glm::vec3 min, max;
    };

    struct ModelInfo {
        std::vector<BoxArgs> boxes;
    };

    ModelInfo loadModel(const std::string &modelName)
    {
        ModelInfo model;
        std::ifstream inFile("res/models/" + modelName + ".zmd");
        std::string line;
        std::string type;
        while (inFile >> type) {
            if (type == "box") {
                BoxArgs args;
                inFile >> args.min.x >> args.min.y >> args.min.z >>
                    args.max.x >> args.max.y >> args.max.z;
                model.boxes.push_back(args);
            }
        }
        return model;
    }
} // namespace
*/
namespace client {

} // namespace client
