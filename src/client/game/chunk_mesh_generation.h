#pragma once

#include "chunk_mesh.h"
#include <common/world/chunk.h>

class VoxelDataManager;
class Chunk;

ChunkMeshCollection makeChunkMesh(const Chunk& chunk, const VoxelDataManager& voxelData);
