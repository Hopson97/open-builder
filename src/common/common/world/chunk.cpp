#include "chunk.h"
#include "chunk_manager.h"

namespace {
    // clang-format off
    bool voxelPositionOutOfChunkBounds(const VoxelPosition& voxelPosition) {
        return 
        voxelPosition.x < 0 || voxelPosition.x >= CHUNK_SIZE ||
        voxelPosition.y < 0 || voxelPosition.y >= CHUNK_SIZE ||
        voxelPosition.z < 0 || voxelPosition.z >= CHUNK_SIZE;
    }
    // clang-format on
} // namespace

Chunk::Chunk(ChunkManager& manager, const ChunkPosition& position)
    : mp_manager(manager)
    , m_position(position)
{
}

voxel_t Chunk::qGetVoxel(const VoxelPosition& voxelPosition) const
{
    assert(!voxelPositionOutOfChunkBounds(voxelPosition));
    return voxels[toLocalVoxelIndex(voxelPosition)];
}

void Chunk::qSetVoxel(const VoxelPosition& voxelPosition, voxel_t voxel)
{
    assert(!voxelPositionOutOfChunkBounds(voxelPosition));
    voxels[toLocalVoxelIndex(voxelPosition)] = voxel;
}

voxel_t Chunk::getVoxel(const VoxelPosition& voxelPosition) const
{
    if (voxelPositionOutOfChunkBounds(voxelPosition)) {
        return mp_manager.getVoxel(toGlobalVoxelPosition(voxelPosition, m_position));
    }
    return qGetVoxel(voxelPosition);
}

void Chunk::setVoxel(const VoxelPosition& voxelPosition, voxel_t voxel)
{
    if (voxelPositionOutOfChunkBounds(voxelPosition)) {
        return mp_manager.setVoxel(toGlobalVoxelPosition(voxelPosition, m_position),
                                   voxel);
    }
    qSetVoxel(voxelPosition, voxel);
}

const ChunkPosition& Chunk::getPosition() const
{
    return m_position;
}

CompressedVoxels compressVoxelData(const VoxelArray& voxels)
{
    CompressedVoxels compressed;
    voxel_t currentVoxel = voxels[0];
    u32 voxelCount = 1;

    for (unsigned i = 1; i < voxels.size(); i++) {
        auto voxel = voxels[i];
        if (voxel == currentVoxel) {
            voxelCount++;
        }
        else {
            compressed.emplace_back(currentVoxel, voxelCount);
            currentVoxel = voxels[i];
            voxelCount = 1;
        }
    }
    compressed.emplace_back(currentVoxel, voxelCount);
    return compressed;
}

VoxelArray decompressVoxelData(const CompressedVoxels& voxels)
{
    VoxelArray voxelData;
    int voxelPointer = 0;
    for (auto& voxel : voxels) {
        auto type = voxel.first;
        auto count = voxel.second;

        for (u16 i = 0; i < count; i++) {
            voxelData[voxelPointer++] = type;
        }
    }
    return voxelData;
}
