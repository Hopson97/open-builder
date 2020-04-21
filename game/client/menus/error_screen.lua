
local backgroundTexture = game.getTexture("res/error_bg.png")
local logoTexture = game.getTexture("res/logo.png")

local function onCreate(overlay, data)

    local menu = StackMenu:create(900, overlay, 15, "Error")
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    local errorMessage = menu:addLabel("Server IP", "Enter server IP...")
    errorMessage.text = data["message"]
    menu:pad(200)

    local backButton = menu:addButton("Main Menu")
    backButton.onClick = function()
        game.gui():change("main_menu")
    end
end

game.gui():addGui{
    id = "error_screen",
    create = onCreate,
}
