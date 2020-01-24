
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
    player:setPosition(x, 100, z);
end

--Run callback functions
function game.runPlayerJoinCallback(player)
    for _, callback in ipairs(game.playerJoinCallbacks) 
    do
        callback(player)
    end
end

game.onPlayerJoin(spawnPlayer)

dofile("game/blocks.lua")