--
-- Helpers
--
dofile("game/client/gui_helpers/common.lua")   
dofile("game/client/gui_helpers/stack_menu.lua")   

--
--  Menus
--
dofile("game/client/menus/main_menu.lua")
dofile("game/client/menus/settings_menu.lua")
dofile("game/client/menus/new_world_menu.lua")
dofile("game/client/menus/join_world_menu.lua")
dofile("game/client/menus/world_select.lua")
dofile("game/client/menus/pause_menu.lua")
dofile("game/client/menus/error_screen.lua")
dofile("game/client/menus/transition_screen.lua")

-- 
-- In Game GUIs
--
dofile("game/client/hud.lua")

game.onClientStartup(function()
    game.gui():change("main_menu")
end)


game.onError(function(errorMessage)
    if errorMessage ~= nil then
        game.gui():change("error_screen", {message = errorMessage})
    end
end)

game.onEnterGame(function() 
    game.input.hideMouse()
    game.gui():change("hud")
end)

game.onExitGame(function() 
    game.input.showMouse()
    game.gui():change("main_menu")
end)

game.onKeyReleased(game.Keyboard.Escape, function() 
    if game.control():isInGame() then
        game.control():pause()
        game.gui():push("pause")
        game.input.showMouse()
    end
end)

game.onKeyReleased(game.Keyboard.L, function() 
    if game.control():isInGame() then
        game.control():pause()
        game.input.showMouse()
    end
end)

game.onKeyReleased(game.Keyboard.U, function() 
    game.input.hideMouse()
    game.control():resume()
end)
