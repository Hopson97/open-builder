local crosshairTexture = game.gui.getTexture("res/crosshair.png")

local function createHUD(hud) 
    local crosshair = hud:addImage()
    crosshair.position = GuiDim.new(0.5, -16, 0.5, -16)
    crosshair.size = GuiDim.new(0, 32, 0, 32)
    crosshair.image = crosshairTexture
end

game.gui.addGui{
    id = "hud",
    title = "Player HUD",
    create = createHUD,
}