

local hud = GuiContainer.new()

local texCrosshair = game.gui.getTexture("res/crosshair.png")


game.gui.add(hud)

--
--  GOAL
--
--local hud = GUI.container.new()
--
--local crosshair = GUI.rectangle.new()
--crosshair.image = "crosshair.png"
--crosshair.position = GDim.new(0.5, -16, 0.5, -16)
--crosshair.size = GDim.new(0, 32, 0, 32)
--hud:add(crosshair)
--
--game.gui.add(hud)
--
----eg could also do this stuff
--hud:show()
--hud:hide()

--
--  CURRENT
--
-- Main crosshair (to be used in final)
--local crosshair = game.GUI.Image.new()
--crosshair:setSource("crosshair")
--crosshair:setScaledPosition(0.5, 0.5)
--crosshair:setPixelOffset(-16.0, -16.0)
--crosshair:setPixelSize(32.0, 32.0)
--crosshair:setColor(1, 1, 1)
--game.GUI.addImage(crosshair)

-- gui.lua

---- This crosshair rotates around the main crosshair
--local test = game.GUI.Image.new()
--test:setSource("crosshair")
--test:setPosition(game.GUI.GDim.new(0.5, -16, 0.5, -16))
--test:setSize(game.GUI.GDim.new(0, 32, 0, 32))
--test:setColor(game.GUI.Color3.new(0,0,0));
--game.GUI.addImage(test)
--

---- Spin and colour the crosshairs
--local p = 0
--function update(delta)
--    f_crosshair:setColor(game.GUI.Color3.new(math.sin(p)/(math.pi/2) * 0.5 + 0.5, 0, 0));
--	test:setPosition(game.GUI.GDim.new(0.5, -16 + math.sin(p)*50, 0.5, -16 + math.cos(p)*50))
--	p = p + (delta * math.pi*2)
--end