#include "lua_api.h"

#include <common/scripting/script_engine.h>
#include <common/world/biome.h>
#include <common/world/voxel_data.h>

namespace {
void addVoxelApi(sol::table& dataTable, VoxelDataManager& voxelManager)
{
    dataTable["addVoxel"] = [&](const sol::table& voxelData) {
        VoxelData voxel;

        voxel.name = voxelData["name"].get<std::string>();
        voxel.topTexture = voxelData["render"]["top"].get<std::string>();
        voxel.sideTexture = voxelData["render"]["sides"].get<std::string>();
        voxel.bottomTexture = voxelData["render"]["bottom"].get<std::string>();

        if (voxelData["collidable"].valid()) {
            voxel.isCollidable = voxelData["collidable"].get<bool>();
        }
        if (voxelData["type"].valid()) {
            voxel.type = voxelData["type"].get<VoxelType>();
        }
        if (voxelData["render"]["mesh"].valid()) {
            voxel.meshStyle = voxelData["render"]["mesh"].get<VoxelMeshStyle>();
        }

        voxelManager.addVoxelData(voxel);
    };
}

void addBiomeApi(sol::table dataTable, VoxelDataManager& voxelManager,
                 BiomeDataManager& biomeManager)
{
    dataTable["addBiome"] = [&](const sol::table& biomeData) {
        Biome biome;

        biome.name = biomeData["name"].get<std::string>();
        biome.depth = biomeData["depth"].get<int>();
        biome.onTopBlockSet = biomeData["onTopBlockSet"];

        std::string topVoxel = biomeData["top_voxel"].get<std::string>();
        std::string undergroundVoxel = biomeData["underground_voxel"].get<std::string>();
        biome.topVoxel = voxelManager.getVoxelId(topVoxel);
        biome.undergroundVoxel = voxelManager.getVoxelId(undergroundVoxel);

        biomeManager.addBiomeData(biome);
    };
}

void addEnums(ScriptEngine& script)
{
    auto meshStyle = script.addTable("MeshStyle");
    meshStyle["Block"] = VoxelMeshStyle::Block;
    meshStyle["Cross"] = VoxelMeshStyle::Cross;
    meshStyle["None"] = VoxelMeshStyle::None;

    auto voxelType = script.addTable("VoxelType");
    voxelType["Solid"] = VoxelType::Solid;
    voxelType["Flora"] = VoxelType::Flora;
    voxelType["Fluid"] = VoxelType::Fluid;
    voxelType["Gas"] = VoxelType::Gas;
}
} // namespace

void luaInitDataApi(ScriptEngine& script, BiomeDataManager& biomeManager,
                    VoxelDataManager& voxelManager)
{
    auto dataTable = script.addTable("data");
    addVoxelApi(dataTable, voxelManager);
    addBiomeApi(dataTable, voxelManager, biomeManager);
    addEnums(script);

    dataTable["getVoxel"] = [&](const std::string& voxelName) {
        return voxelManager.getVoxelId(voxelName);
    };
}