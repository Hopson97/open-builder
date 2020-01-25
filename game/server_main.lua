
-- Callbacks for events
game.playerJoinCallbacks = {}

--Register an event for a player joining the game
--callback: function(player)
function game.onPlayerJoin(callback)
    table.insert(game.playerJoinCallbacks, callback)
end

local function spawnPlayer(player)
    local cs = game.world.CHUNK_SIZE
    local ws = game.world.WORLD_SIZE
    local x = (ws * cs) / 2
    local z = x
    local y = 0
    while game.world.getBlock(x, y, z) ~= 0 and  game.world.getBlock(x, y + 1, z) ~= 0 do
        y = y + 1
    end
    player:setPosition(x, y + 3, z);
end

--Run callback functions
function game.runPlayerJoinCallback(player)
    for _, callback in ipairs(game.playerJoinCallbacks) 
    do
        callback(player)
    end
end

game.onPlayerJoin(spawnPlayer)

dofile("game/voxel_types.lua")