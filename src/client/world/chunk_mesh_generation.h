#pragma once

#include "chunk_mesh.h"
#include "client_voxel.h"
#include <common/world/voxel_registry.h>

class Chunk;

ChunkMeshCollection makeChunkMesh(const Chunk &chunk,
                        const VoxelRegistry<ClientVoxel> &voxelData);