#include <catch2/catch.hpp>
#include <common/debug.h>
#include <common/world/chunk_manager.h>
#include <ctime>
#include <random>

TEST_CASE("Chunks can get and set voxels")
{
    voxel_t voxel = 10;

    SECTION("Chunk voxels can be set and recieved correctly")
    {
        ChunkManager manager;
        Chunk& chunk = manager.addChunk({0, 0, 0});
        VoxelPosition voxelPosition(1, 2, 3);

        chunk.qSetVoxel(voxelPosition, voxel);
        REQUIRE(chunk.qGetVoxel(voxelPosition) == voxel);
    }

    SECTION("The chunk is able to get neighbour voxels")
    {
        ChunkPosition left(-1, 0, 0);
        ChunkPosition right(0, 0, 0);

        ChunkManager manager;
        Chunk& leftChunk = manager.addChunk(left);
        Chunk& rightChunk = manager.addChunk(right);

        VoxelPosition setPosition;
        VoxelPosition correctedPosition;

        setPosition = {-5, 5, 2};
        correctedPosition = {CHUNK_SIZE - 5, 5, 2};
        manager.setVoxel(setPosition, voxel);
        REQUIRE(rightChunk.getVoxel(setPosition) == voxel);
        REQUIRE(leftChunk.qGetVoxel(correctedPosition) == voxel);
        REQUIRE(leftChunk.getVoxel(correctedPosition) == voxel);
        REQUIRE(manager.getVoxel(setPosition) == voxel);

        setPosition = {5, 10, 3};
        correctedPosition = {CHUNK_SIZE + 5, 10, 3};
        manager.setVoxel(setPosition, voxel);
        REQUIRE(leftChunk.getVoxel(correctedPosition) == voxel);
        REQUIRE(rightChunk.qGetVoxel(setPosition) == voxel);
        REQUIRE(rightChunk.getVoxel(setPosition) == voxel);
        REQUIRE(manager.getVoxel(setPosition) == voxel);
    }

    SECTION("The chunk is able to set neighbour voxels")
    {
        ChunkPosition left(-1, 0, 0);
        ChunkPosition right(0, 0, 0);
        ChunkPosition up(0, 1, 0);

        ChunkManager manager;
        Chunk& leftChunk = manager.addChunk(left);
        Chunk& rightChunk = manager.addChunk(right);
        Chunk& upChunk = manager.addChunk(up);

        VoxelPosition setPosition;
        VoxelPosition correctedPosition;

        setPosition = {-5, 5, 2};
        correctedPosition = {CHUNK_SIZE - 5, 5, 2};
        rightChunk.setVoxel(setPosition, voxel);
        REQUIRE(leftChunk.qGetVoxel(correctedPosition) == voxel);
        REQUIRE(leftChunk.getVoxel(correctedPosition) == voxel);

        setPosition = {5, 33, 3};
        correctedPosition = {5, 1, 3};
        rightChunk.setVoxel(setPosition, voxel);
        REQUIRE(upChunk.getVoxel(correctedPosition) == voxel);
        REQUIRE(upChunk.qGetVoxel(correctedPosition) == voxel);
    }

    SECTION("Setting an out of bound voxel will create a chunk at that position")
    {
        ChunkManager manager;
        ChunkPosition bottom(0, 0, 0);
        ChunkPosition up(0, 1, 0);

        Chunk& chunk = manager.addChunk(bottom);

        VoxelPosition setPosition;
        VoxelPosition correctedPosition;
        setPosition = {5, 33, 3};
        correctedPosition = {5, 1, 3};
        chunk.setVoxel(setPosition, voxel);

        REQUIRE(manager.hasChunk(up));

        if (manager.hasChunk(up)) {
            const Chunk& newChunk = manager.getChunk(up);
            REQUIRE(newChunk.getVoxel(correctedPosition) == voxel);
            REQUIRE(newChunk.qGetVoxel(correctedPosition) == voxel);
        }
    }
}

TEST_CASE("Chunk can be compressed and uncompressed")
{

    SECTION("The chunk can compress voxels")
    {
        ChunkManager manager;
        Chunk& chunk = manager.addChunk({0, 0, 0});

        SECTION("By default, a chunk is just air, so there is just one voxel type")
        {
            auto compressed = compressVoxelData(chunk.voxels);
            REQUIRE(compressed.size() == 1);
            REQUIRE(compressed[0].second == chunk.voxels.size());
        }

        SECTION("Two voxel types")
        {
            chunk.qSetVoxel({0, 0, 0}, 1);
            auto compressed = compressVoxelData(chunk.voxels);
            REQUIRE(compressed.size() == 2);
            REQUIRE(compressed[0].second == 1);
            REQUIRE(compressed[1].second == chunk.voxels.size() - 1);
        }

        SECTION("N voxel types")
        {
            std::mt19937 rng;
            rng.seed(std::time(nullptr));
            std::uniform_int_distribution<> voxelTypeCount(20, 200);

            int n = voxelTypeCount(rng);

            std::weibull_distribution<> voxelType(0, n);

            // Ensure all voxels are there in the chunk (to avoid flakiness due
            // to random)
            for (int i = 0; i < n; i++) {
                chunk.voxels[i] = i;
            }

            for (unsigned i = n + 1; i < chunk.voxels.size(); i++) {
                chunk.voxels[i] = voxelType(rng);
            }

            auto compressed = compressVoxelData(chunk.voxels);
            // yeah no clue how to test this lol
        }
    }

    SECTION("Chunks can correctly de-compress chunks")
    {
        voxel_t voxelA = 5;
        voxel_t voxelB = 45;
        voxel_t voxelC = 123;

        VoxelPosition positionA(0, 20, 12);
        VoxelPosition positionB(12, 2, 4);
        VoxelPosition positionC(1, 30, 8);

        ChunkManager manager;
        Chunk& chunk = manager.addChunk({0, 0, 0});

        chunk.qSetVoxel(positionA, voxelA);
        chunk.qSetVoxel(positionB, voxelB);
        chunk.qSetVoxel(positionC, voxelC);

        auto compressed = compressVoxelData(chunk.voxels);

        Chunk& decompressor = manager.addChunk({1, 1, 1});
        decompressor.voxels = decompressVoxelData(compressed);

        REQUIRE(decompressor.getVoxel(positionA) == voxelA);
        REQUIRE(decompressor.getVoxel(positionB) == voxelB);
        REQUIRE(decompressor.getVoxel(positionC) == voxelC);
    }
}
