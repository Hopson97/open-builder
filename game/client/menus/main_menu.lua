
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function onCreate(overlay)
    local menu = StackMenu:create(900, overlay, 15)
    menu:setBackground(backgroundTexture)
    menu:addImage(logoTexture, 1064, 138)
    menu:pad(150)

    local continue = menu:addButton("Continue Game")

    local vanilla   = menu:addButton("Play Vanilla")
    local modded    = menu:addButton("Play Modded")
    local settings  = menu:addButton("Settings")
    local exitGame  = menu:addButton("Exit Game")

    continue.onClick = function()
        game.gui.change("transition", { message = "Starting Game" } )
        game.control.loadWorld("Test")
    end    

    vanilla.onClick = function()
        game.gui.change("character_select")
    end

    modded.onClick = function()
        game.gui.change("play_game", { mode = "universe" })
    end

    settings.onClick = function()
        game.gui.change("settings_menu")
    end

    exitGame.onClick = function()
        game.control.shutdown()
    end
end

game.gui.addGui{
    id = "main_menu",
    title = "Main Menu",
    create = onCreate,
}