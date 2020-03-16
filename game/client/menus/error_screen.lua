
local backgroundTexture = game.gui.getTexture("res/error_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function onCreate(overlay, data)

    local menu = StackMenu:create(900, overlay, 15, "Error")
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    local errorMessage = menu:addLabel("Server IP", "Enter server IP...")
    errorMessage.text = data
    menu:pad(200)

    local backButton = menu:addButton("Back")
    backButton.onClick = function()
        game.gui.change("main_menu")
    end
end

game.gui.addGui{
    id = "error_screen",
    title = "Error Screen",
    create = onCreate,
}

game.onError(function(errorMessage)
    game.gui.change("error_screen", errorMessage)
end)