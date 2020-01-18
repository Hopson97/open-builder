#pragma once

#include "chunk_mesh.h"

class Chunk;

#include "client_voxel.h"
#include <common/world/voxel_registry.h>

ChunkMesh makeChunkMesh(const Chunk &chunk,
                        const VoxelRegistry<ClientVoxel> &voxelData);