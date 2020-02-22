
local crosshairTexture = game.gui.getTexture("res/crosshair.png")

local function createHud(overlay)
    local crosshair = overlay:addImage()
    crosshair.image = crosshairTexture
    crosshair.position = GuiDim.new(0.5, -16, 0.5, -16)
    crosshair.size = GuiDim.new(0, 32, 0, 32)

    local label = overlay:addLabel()
    label.text = "hello"
    label.textSize = 50
    label.position = GuiDim.new(0.1, -16, 0.9, -16)
end 

game.gui.addGui{
    id = "hud",
    title = "Player HUD",
    create = createHud
}

game.onClientStartup(function() 
    game.gui.push("hud")
end)