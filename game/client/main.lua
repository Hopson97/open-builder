
local texCrosshair = game.gui.getTexture("res/crosshair.png")
local hud = game.gui.makeGui()

local crosshair = hud:addRect()
crosshair.position = GuiDim.new(0.5, -16, 0.5, -16)
crosshair.size = GuiDim.new(0, 32, 0, 32)
crosshair.texture = texCrosshair

game.onClientStartup(function() print("Hello from the other side") end)
