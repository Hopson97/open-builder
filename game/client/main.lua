
local texCrosshair = game.gui.getTexture("res/crosshair.png")
local hud = game.gui.makeGui()

local crosshair = hud:addRect()
crosshair.position = GuiDim.new(0.5, -16, 0.5, -16)
crosshair.size = GuiDim.new(0, 32, 0, 32)
crosshair.texture = texCrosshair

--local hotbar = hud:addRect()
--hotbar.position = GuiDim.new(0.25, 0, 0.01, 0)
--hotbar.size = GuiDim.new(0.5, 0, 0.1, 0)
--
--local textTest = hud:addText()
--textTest.position = GuiDim.new(0.25, 0, 0.01, 0)
--textTest.size = 50
--textTest.text = "Hello world"
--
--
--local textTest2 = hud:addText()
--textTest2.position = GuiDim.new(0.25, 0, 0.21, 0)
--textTest2.size = 50
--textTest2.text = "Hello world"
