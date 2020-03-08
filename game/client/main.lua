--
-- Helpers
--
dofile("game/client/stack_menu.lua")   

--
--  Menus
--
dofile("game/client/menus/main_menu.lua")
dofile("game/client/menus/play_game_menu.lua")
dofile("game/client/menus/settings_menu.lua")
dofile("game/client/menus/new_world_menu.lua")
dofile("game/client/menus/pause_menu.lua")

-- 
-- In Game GUIs
--
dofile("game/client/hud.lua")

local function onStartup()
    game.gui.push("main_menu")
end

game.onClientStartup(onStartup)

