
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function onCreate(overlay, data)

    local menu = StackMenu:create(900, overlay, 15, data)
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    local newWorld = menu:addButton("New World")
    local loadWorld = menu:addButton("Load World")
    local joinWorld = menu:addButton("Join World")
    local backButton = menu:addButton("Back")

    newWorld.onClick = function()
        game.gui.change("hud")
        game.input.lockMouse()
        game.input.hideMouse()
        game.control.startGame()
    end    
    
    loadWorld.onClick = function()
        game.gui.change("hud")
        game.input.lockMouse()
        game.input.hideMouse()
        game.control.startGame()
    end

    joinWorld.onClick = function()
        game.gui.change("hud")
        game.input.lockMouse()
        game.input.hideMouse()
        game.control.startGame()
    end


    backButton.onClick = function()
        game.gui.change("main_menu")
    end
end

game.gui.addGui{
    id = "play_game",
    title = "Play Game Menu",
    create = onCreate,
}