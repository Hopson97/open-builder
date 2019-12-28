#pragma once

#include <catch2/catch.hpp>
#include <common/debug.h>
#include <common/world/chunk_manager.h>

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
