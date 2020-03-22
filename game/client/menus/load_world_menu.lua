
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function onCreate(overlay, data)
    local menu = StackMenu:create(900, overlay, 15, "Load World")
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    menu:addBackButton()
end

game.gui.addGui{
    id = "load_world",
    title = "Load World Menu",
    create = onCreate,
}