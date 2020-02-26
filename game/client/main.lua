--
--  Menus
--
dofile("game/client/stack_menu.lua")   
dofile("game/client/main_menu.lua")
dofile("game/client/play_game_menu.lua")
dofile("game/client/settings_menu.lua")
dofile("game/client/pause_menu.lua")
dofile("game/client/hud.lua")

local function onStartup()
    game.gui.push("main_menu")
end

game.onClientStartup(onStartup)


game.onKeyReleased(game.Keyboard.Escape, function() 
    print ("ESCAPE")
    print (game.control.currentState())
    print (game.control.InGame)
    if game.control.currentState() == game.State.InGame then
        game.control.pause()
        game.gui.push("pause")
        game.input.showMouse()
    end
end)