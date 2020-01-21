#include <catch2/catch.hpp>
#include <common/debug.h>
#include <common/world/chunk_manager.h>
#include <ctime>
#include <random>

TEST_CASE("Chunks can get and set blocks")
{
    block_t block = 10;

    SECTION("Chunk blocks can be set and recieved correctly")
    {
        ChunkManager manager;
        Chunk &chunk = manager.addChunk({0, 0, 0});
        BlockPosition blockPosition(1, 2, 3);

        chunk.qSetBlock(blockPosition, block);
        REQUIRE(chunk.qGetBlock(blockPosition) == block);
    }

    SECTION("The chunk is able to get neighbour blocks")
    {
        ChunkPosition left(-1, 0, 0);
        ChunkPosition right(0, 0, 0);

        ChunkManager manager;
        Chunk &leftChunk = manager.addChunk(left);
        Chunk &rightChunk = manager.addChunk(right);

        BlockPosition setPosition;
        BlockPosition correctedPosition;

        setPosition = {-5, 5, 2};
        correctedPosition = {CHUNK_SIZE - 5, 5, 2};
        manager.setBlock(setPosition, block);
        REQUIRE(rightChunk.getBlock(setPosition) == block);
        REQUIRE(leftChunk.qGetBlock(correctedPosition) == block);
        REQUIRE(leftChunk.getBlock(correctedPosition) == block);
        REQUIRE(manager.getBlock(setPosition) == block);

        setPosition = {5, 10, 3};
        correctedPosition = {CHUNK_SIZE + 5, 10, 3};
        manager.setBlock(setPosition, block);
        REQUIRE(leftChunk.getBlock(correctedPosition) == block);
        REQUIRE(rightChunk.qGetBlock(setPosition) == block);
        REQUIRE(rightChunk.getBlock(setPosition) == block);
        REQUIRE(manager.getBlock(setPosition) == block);
    }
}

TEST_CASE("Chunk can be compressed and uncompressed")
{

    SECTION("The chunk can compress blocks")
    {
        ChunkManager manager;
        Chunk &chunk = manager.addChunk({0, 0, 0});

        SECTION(
            "By default, a chunk is just air, so there is just one block type")
        {
            auto compressed = compressBlockData(chunk.blocks);
            REQUIRE(compressed.size() == 1);
            REQUIRE(compressed[0].second == chunk.blocks.size());
        }

        SECTION("Two block types")
        {
            chunk.qSetBlock({0, 0, 0}, 1);
            auto compressed = compressBlockData(chunk.blocks);
            REQUIRE(compressed.size() == 2);
            REQUIRE(compressed[0].second == 1);
            REQUIRE(compressed[1].second == chunk.blocks.size() - 1);
        }

        SECTION("N block types")
        {
            std::mt19937 rng;
            rng.seed(std::time(nullptr));
            std::uniform_int_distribution<> blockTypeCount(20, 200);

            int n = blockTypeCount(rng);

            std::weibull_distribution<> blockType(0, n);

            // Ensure all blocks are there in the chunk (to avoid flakiness due
            // to random)
            for (int i = 0; i < n; i++) {
                chunk.blocks[i] = i;
            }

            for (unsigned i = n + 1; i < chunk.blocks.size(); i++) {
                chunk.blocks[i] = blockType(rng);
            }

            auto compressed = compressBlockData(chunk.blocks);
            // yeah no clue how to test this lol
        }
    }

    SECTION("Chunks can correctly de-compress chunks")
    {
        block_t blockA = 5;
        block_t blockB = 45;
        block_t blockC = 123;

        BlockPosition positionA(0, 20, 12);
        BlockPosition positionB(12, 2, 4);
        BlockPosition positionC(1, 30, 8);

        ChunkManager manager;
        Chunk &chunk = manager.addChunk({0, 0, 0});

        chunk.qSetBlock(positionA, blockA);
        chunk.qSetBlock(positionB, blockB);
        chunk.qSetBlock(positionC, blockC);

        auto compressed = compressBlockData(chunk.blocks);

        Chunk& decompressor = manager.addChunk({1, 1, 1});
        decompressor.blocks = decompressBlockData(compressed);

        REQUIRE(decompressor.getBlock(positionA) == blockA);
        REQUIRE(decompressor.getBlock(positionB) == blockB);
        REQUIRE(decompressor.getBlock(positionC) == blockC);
    }
}
