--
--  Menus
--
dofile("game/client/stack_menu.lua")   
dofile("game/client/main_menu.lua")
dofile("game/client/play_game_menu.lua")
dofile("game/client/settings_menu.lua")
dofile("game/client/hud.lua")

local function onStartup()
    game.gui.push("main_menu")
end

game.onClientStartup(onStartup)


game.onKeyReleased(game.Keyboard.L, function() print("L PRESSED") end)