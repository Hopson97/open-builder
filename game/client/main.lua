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
dofile("game/client/menus/join_world_menu.lua")
dofile("game/client/menus/load_world_menu.lua")
dofile("game/client/menus/pause_menu.lua")
dofile("game/client/menus/transition_screen.lua")

-- 
-- In Game GUIs
--
dofile("game/client/hud.lua")

game.onClientStartup(function()
    game.gui.push("main_menu")
end)