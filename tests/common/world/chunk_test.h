#pragma once

#include <catch2/catch.hpp>
#include <common/world/chunk.h>

TEST_CASE("Chunks should be able to be freely modified")
{
    
    u8 block = 10;


    SECTION("Chunk blocks can be set and recieved correctly")
    {
        Chunk chunk;
        BlockPosition blockPosition(1, 2, 3);

        chunk.setBlock(blockPosition, block);
        REQUIRE(chunk.getBlock(blockPosition) == block);
    }

    SECTION("The chunk manager will correctly set and get neighbouring blocks")
    {
        ChunkPosition left(-1, 0, 0);
        ChunkPosition middle(0, 0, 0);
        ChunkPosition right(1, 0, 0);

        ChunkManager manager;
        Chunk &leftChunk = manager.addChunk(left);
        Chunk &middleChunk = manager.addChunk(middle);
        Chunk &rightChunk = manager.addChunk(right);

        BlockPosition setPosition(CHUNK_SIZE, 0, 0);
        BlockPosition correctedPosition(0, 0, 0);

        manager.setBlock(setPosition, block);
        REQUIRE(rightChunk.getBlock(correctedPosition) == block);
        REQUIRE(manager.getBlock(setPosition) == block);

        setPosition = {-10, 0, 0};
        correctedPosition = {CHUNK_SIZE - 10, 0, 0};
        manager.setBlock(setPosition, block);
        REQUIRE(leftChunk.getBlock(correctedPosition) == block);
        REQUIRE(manager.getBlock(setPosition) == block);
    }
}