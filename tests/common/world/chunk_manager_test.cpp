#include <catch2/catch.hpp>
#include <common/debug.h>
#include <common/world/chunk_manager.h>

TEST_CASE("Chunks manager tests")
{
    block_t block = 10;

    SECTION("The chunk manager will correctly set and get neighbouring blocks")
    {
        ChunkPosition left(-1, 0, 0);
        ChunkPosition middle(0, 0, 0);
        ChunkPosition right(1, 0, 0);

        ChunkManager manager;
        Chunk& leftChunk = manager.addChunk(left);
        Chunk& middleChunk = manager.addChunk(middle);
        Chunk& rightChunk = manager.addChunk(right);

        BlockPosition setPosition;
        BlockPosition correctedPosition;

        setPosition = {CHUNK_SIZE, 0, 0};
        correctedPosition = {0, 0, 0};
        manager.setBlock(setPosition, block);
        REQUIRE(rightChunk.qGetBlock(correctedPosition) == block);
        REQUIRE(manager.getBlock(setPosition) == block);

        setPosition = {-1, 20, 0};
        correctedPosition = {CHUNK_SIZE - 1, 20, 0};
        manager.setBlock(setPosition, block);
        REQUIRE(leftChunk.qGetBlock(correctedPosition) == block);
        REQUIRE(manager.getBlock(setPosition) == block);

        setPosition = {-10, 0, 0};
        correctedPosition = {CHUNK_SIZE - 10, 0, 0};
        manager.setBlock(setPosition, block);
        REQUIRE(leftChunk.qGetBlock(correctedPosition) == block);
        REQUIRE(manager.getBlock(setPosition) == block);

        setPosition = {5, 10, 20};
        correctedPosition = {5, 10, 20};
        manager.setBlock(setPosition, block);
        REQUIRE(middleChunk.qGetBlock(correctedPosition) == block);
        REQUIRE(manager.getBlock(setPosition) == block);
    }

    SECTION("The chunk manager correctly identifies it has neighbours for a "
            "given chunk")
    {
        ChunkManager manager;
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                for (int x = 0; x < 3; x++) {
                    manager.addChunk({x, y, z});
                }
            }
        }

        REQUIRE(manager.hasNeighbours({1, 1, 1}) == true);
        REQUIRE(manager.hasNeighbours({1, 2, 1}) == false);
    }

    SECTION("The chunk manager can ensure a chunk has neighbours")
    {
        ChunkManager manager;
        manager.ensureNeighbours({0, 0, 0});
        REQUIRE(manager.hasNeighbours({0, 0, 0}) == true);
    }
}