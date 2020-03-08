
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function onCreate(overlay, data)

    local menu = StackMenu:create(900, overlay, 15, "Join World")
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    local serverIpBox = menu:addTextBox("Enter server IP...")
    menu:pad(10)
    local joinButton = menu:addButton("Join World")
    menu:pad(200)
    local backButton = menu:addButton("Back")

    joinButton.onClick = function()
        game.gui.change("hud")
        game.input.hideMouse()
        game.control.startGame()
    end    

    backButton.onClick = function()
        game.gui.change("play_game", data)
    end
end

game.gui.addGui{
    id = "join_world",
    title = "Join World Menu",
    create = onCreate,
}