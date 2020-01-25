-- Test mod
-- Spawns the player in center of world when they join

local function notAir(x, y, z)
    return game.world.getBlock(x, y, z) ~= 0
end

local function spawnPlayer(player)
    local cs = game.world.CHUNK_SIZE
    local ws = game.world.WORLD_SIZE
    local x = (ws * cs) / 2
    local z = x
    local y = 0
    while notAir(x, y, z) and notAir(x, y + 1, z) do
        y = y + 1
    end
    player:setPosition(x, y + 3, z);
end

game.onPlayerJoin(spawnPlayer)