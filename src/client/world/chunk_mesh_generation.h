#pragma once

#include "chunk_mesh.h"

class Chunk;

#include "client_voxel.h"
#include <common/world/voxel_types.h>

ChunkMesh makeChunkMesh(const Chunk &chunk,
                        const VoxelRegistry<ClientVoxel> &voxelData);