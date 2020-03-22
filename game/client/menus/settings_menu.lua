
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function onCreate(overlay)
    local menu = StackMenu:create(900, overlay, 15, "Settings")
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    menu:addBackButton("Back")
end

game.gui.addGui{
    id = "settings_menu",
    title = "Settings Menu",
    create = onCreate,
}