
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function onCreate(overlay)
    local menu = StackMenu:create(900, overlay, 15)
    menu:setBackground(backgroundTexture)
    menu:addImage(logoTexture, 1064, 138)
    menu:pad(150)

    local startGame   = menu:addButton("Singleplayer")
    local multiplayer = menu:addButton("Multiplayer")
    local settings    = menu:addButton("Settings")
    local exitGame    = menu:addButton("Exit Game")

    startGame.onClick = function()
        print("Singleplayer clicked")
        game.gui.change("hud")
        game.input.lockMouse()
        game.input.hideMouse()
    end

    multiplayer.onClick = function()
        print("Multiplayer clicked")
    end

    settings.onClick = function()
        print("Settings clicked")
    end

    exitGame.onClick = function()
        print("Exit clicked")
    end
end

game.gui.addGui{
    id = "main_menu",
    title = "Main Menu",
    create = onCreate,
}