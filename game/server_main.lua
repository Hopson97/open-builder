
-- Callbacks for events
game.playerJoinCallbacks = {}

--Register an event for a player joining the game
--callback: function(player)
function game.onPlayerJoin(callback)
    table.insert(game.playerJoinCallbacks, callback)
end

--Run callback functions
function game.runPlayerJoinCallback(player)
    for _, callback in ipairs(game.playerJoinCallbacks) 
    do
        callback(player)
    end
end

dofile("game/voxel_types.lua")
dofile("game/biome_types.lua")