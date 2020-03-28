
local backgroundTexture = game.getTexture("res/menu_bg.png")

local function onCreate(overlay, data)
    local label = overlay:addLabel()
    label.text = data["message"]
    label.textSize = 128
    label.position = GuiDim.new(0, 10, 0, 950)

    local background = overlay:addImage()
    background.size = GuiDim.new(1.0, 0, 1.0, 0)
    background.image = backgroundTexture
end

game.gui():addGui{
    id = "transition",
    create = onCreate,
}

