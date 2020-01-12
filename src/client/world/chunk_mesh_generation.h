#pragma once

#include "chunk_mesh.h"

class Chunk;

#include <common/world/voxel_types.h>
#include "client_voxel.h"

ChunkMesh makeChunkMesh(const Chunk &chunk, const VoxelRegistry<ClientVoxel>& voxelData);