local crosshairTexture = game.getTexture("res/crosshair.png")
local grass = game.getTexture("texture_packs/default/voxels/grass_side.png")

local function createHUD(hud) 
    local crosshair = hud:addImage()
    crosshair.position = GuiDim.new(0.5, -16, 0.5, -16)
    crosshair.size = GuiDim.new(0, 32, 0, 32)
    crosshair.image = crosshairTexture


    for x = 0, 8 do
        local test = hud:addImage()
        test.position = GuiDim.new(0, 366 + x * 128 + 10 * x, 0.01, 0)
        test.size = GuiDim.new(0, 128, 0, 128)
        test.image = grass
    end
end

game.gui():addGui{
    id = "hud",
    create = createHUD,
}

game.onMouseWheelScroll(function(delta) 
    print(delta)
end)
