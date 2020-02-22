
local crosshairTexture = game.gui.getTexture("res/crosshair.png")

--local hud = game.gui.makeGui()
--
--local crosshair = hud:addRect()
--crosshair.position = GuiDim.new(0.5, -16, 0.5, -16)
--crosshair.size = GuiDim.new(0, 32, 0, 32)
--crosshair.texture = texCrosshair
--


local function createHud(overlay)
    local crosshair = overlay:addImage()
    crosshair.image = crosshairTexture
    crosshair.position = GuiDim.new(0.5, -16, 0.5, -16)
    crosshair.size = GuiDim.new(0, 32, 0, 32)
end 

game.gui.addGui{
    id = "hud",
    title = "Player HUD",
    create = createHud
}

game.onClientStartup(function() 
    game.gui.push("hud")
end)