
local texCrosshair = game.gui.getTexture("res/crosshair.png")
local hud = game.gui.makeGui()

local crosshair = hud:addRect()
crosshair.position = GuiDim.new(0.5, -16, 0.5, -16)
crosshair.size = GuiDim.new(0, 32, 0, 32)
crosshair.texture = texCrosshair

--local W = 0.2
--local H = 0.3
--
--local X = 0.085
--local Y = 0.01
--local GAP = W + 0.01
--
--local logo = hud:addRect()
--logo.position = GuiDim.new(0.01, 0, 0.85 -Y, 0)
--logo.size = GuiDim.new(W * 2, 0, W / 2, 50)
--logo.texture = game.gui.getTexture("res/logo.png")
--
--local spButtonRect = hud:addRect()
--spButtonRect.position = GuiDim.new(X, 0, Y, 0)
--spButtonRect.size = GuiDim.new(W, 0, H, 0)
--X = X + GAP
--
--local mpButtonrect = hud:addRect()
--mpButtonrect.position = GuiDim.new(X, 0, Y, 0)
--mpButtonrect.size = GuiDim.new(W, 0, H, 0)
--X = X + GAP
--
--local settingsButtonRect = hud:addRect()
--settingsButtonRect.position = GuiDim.new(X, 0, Y, 0)
--settingsButtonRect.size = GuiDim.new(W, 0, H, 0)
--X = X + GAP
--
--local exitButtonRect = hud:addRect()
--exitButtonRect.position = GuiDim.new(X, 0, Y, 0)
--exitButtonRect.size = GuiDim.new(W, 0, H, 0)
--X = X + GAP