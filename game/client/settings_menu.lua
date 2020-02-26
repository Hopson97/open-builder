
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function onCreate(overlay, data)
    local menu = StackMenu:create(900, overlay, 15)
    menu:setBackground(backgroundTexture)

    print(data)

    local backButton = menu:addButton("Back")

    backButton.onClick = function()
        game.gui.change("main_menu")
    end
end

game.gui.addGui{
    id = "settings_menu",
    title = "Settings Menu",
    create = onCreate,
}