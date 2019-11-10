#pragma once

#include "../renderer/mesh.h"
#include <string>

namespace client {
    Mesh loadModel(const std::string &fileName);
} // namespace client
