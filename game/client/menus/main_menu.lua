
local backgroundTexture = game.getTexture("res/menu_bg.png")
local logoTexture = game.getTexture("res/logo.png")
local checkboxTexture_unchecked = game.getTexture("res/checkbox-unchecked.png")
local checkboxTexture_checked = game.getTexture("res/checkbox-checked.png")

local function onCreate(overlay)
    local menu = StackMenu:create(900, overlay, 15)
    menu:setBackground(backgroundTexture)
    menu:addImage(logoTexture, 1064, 138)
    menu:pad(150)

    local continue = menu:addButton("Continue Game")
    local playGame = menu:addButton("Play Game")
    local settings  = menu:addButton("Settings")
    local exitGame  = menu:addButton("Exit Game")

    continue.onClick = function()
        game.gui():change("transition", {message = "Starting Game"})
        game.control():loadWorld("Test")
    end    
  
    playGame.onClick = function()
        game.gui():push("world_select")
    end
  
    settings.onClick = function()
        game.gui():push("settings_menu")
    end

    exitGame.onClick = function()
        game.control():shutdown()
    end
end

game.gui():addGui{
    id = "main_menu",
    create = onCreate,
}
