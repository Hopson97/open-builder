
local backgroundTexture = game.getTexture("res/menu_bg.png")
local logoTexture = game.getTexture("res/logo.png")

local function onCreate(overlay)
    local menu = StackMenu:create(900, overlay, 15, "Settings")
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    menu:addBackButton("Back")
end

game.gui():addGui{
    id = "settings_menu",
    create = onCreate,
}
