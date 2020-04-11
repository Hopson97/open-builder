#pragma once

struct ScriptEngine;
class BiomeDataManager;
class VoxelDataManager;

/**
 * @brief Gives Lua the ability to call functions on the world itself
 * For example, add and remove voxels from a chunk
 * @param script The script engine with the Lua state
 */
void luaInitWorldApi(ScriptEngine& scriptEngine);

/**
 * @brief Gives Lua the ability to add data to the game
 * For example, voxel types, biome types etc
 * @param script The script engine with the Lua state
 * @param biomeManager Container for the biome types to be used from Lua
 * @param voxelManager Container for the voxel types to be used from Lua
 */
void luaInitDataApi(ScriptEngine& scriptEngine, BiomeDataManager& biomeManager,
                    VoxelDataManager& voxelManager);
