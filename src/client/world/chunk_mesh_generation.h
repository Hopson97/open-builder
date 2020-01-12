#pragma once

#include "chunk_mesh.h"

class Chunk;

#include <common/data/voxel_types.h>
#include "../data/client_voxel_data.h"

ChunkMesh makeChunkMesh(const Chunk &chunk, const VoxelRegistry<ClientVoxelData>& voxelData);