
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

    local button = overlay:addButton()
    button.text = "hello"
    button.textSize = 50
    button.position = GuiDim.new(0.6, 0, 0.6, 0)
    button.size = GuiDim.new(0.2, 0, 0.1, 0)
    button.onClick = function() print("Clicked") end

end 

game.gui.addGui{
    id = "hud",
    title = "Player HUD",
    create = createHud
}

game.onClientStartup(function() 
    game.gui.push("hud")
end)