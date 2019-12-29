#pragma once

#include <catch2/catch.hpp>
#include <common/world/coordinate.h>
#include <common/world/world_constants.h>

TEST_CASE("Coordinate systems can be correctly converted between")
{
    SECTION("Block positions can be correctly converted to chunk positions")
    {
        BlockPosition position;
        ChunkPosition converted;

        position = {0, 0, 0};
        converted = {0, 0, 0};
        REQUIRE(toChunkPosition(position) == converted);

        position = {CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE};
        converted = {1, 1, 1};
        REQUIRE(toChunkPosition(position) == converted);

        position = {-1, CHUNK_SIZE, CHUNK_SIZE};
        converted = {-1, 1, 1};
        REQUIRE(toChunkPosition(position).x == converted.x);
    }

    SECTION("Block positions can be correctly converted to an array index")
    {
        BlockPosition position;
        int converted;

        position = {0, 0, 0};
        converted = 0;
        REQUIRE(toLocalBlockIndex(position) == converted);
    }

    SECTION("Block positions can be converted to local block postions")
    {
        BlockPosition position;
        BlockPosition converted;

        position = {0, 0, 0};
        converted = {0, 0, 0};
        REQUIRE(toLocalBlockPosition(position) == converted);

        position = {CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE};
        converted = {0, 0, 0};
        REQUIRE(toLocalBlockPosition(position) == converted);

        position = {CHUNK_SIZE + 10, CHUNK_SIZE - 1, CHUNK_SIZE};
        converted = {10, CHUNK_SIZE - 1, 0};
        REQUIRE(toLocalBlockPosition(position) == converted);

        position = {-1, 0, 0};
        converted = {CHUNK_SIZE - 1, 0, 0};
        REQUIRE(toLocalBlockPosition(position) == converted);
    }
}