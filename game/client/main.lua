
local texCrosshair = game.gui.getTexture("res/crosshair.png")
local hud = game.gui.makeGui()

local crosshair = hud:addRect()
crosshair.position = GuiDim.new(0.5, -16, 0.5, -16)
crosshair.size = GuiDim.new(0, 32, 0, 32)
crosshair.texture = texCrosshair

local crossSpinner = hud:addRect()
crossSpinner.position = GuiDim.new(0.5, -16, 0.5, -16)
crossSpinner.size = GuiDim.new(0, 32, 0, 32)
crossSpinner.texture = texCrosshair

local crossSpinner2 = hud:addRect()
crossSpinner2.position = GuiDim.new(0.5, -16, 0.5, -16)
crossSpinner2.size = GuiDim.new(0, 32, 0, 32)
crossSpinner2.texture = texCrosshair

local crossSpinner3 = hud:addRect()
crossSpinner3.position = GuiDim.new(0.5, -16, 0.5, -16)
crossSpinner3.size = GuiDim.new(0, 32, 0, 32)
crossSpinner3.texture = texCrosshair

local crossSpinner4 = hud:addRect()
crossSpinner4.position = GuiDim.new(0.5, -16, 0.5, -16)
crossSpinner4.size = GuiDim.new(0, 32, 0, 32)
crossSpinner4.texture = texCrosshair

local p = 0
function spinner(delta)
	crossSpinner.position = GuiDim.new(0.5, -16 + math.sin(p) * 50, 0.5, -16 + math.cos(p)*50)
	crossSpinner2.position = GuiDim.new(0.2, -16 + math.sin(p) * 50, 0.5, -16 + math.cos(p)*50)
	crossSpinner3.position = GuiDim.new(0.2, -16 + math.sin(p) * 50, 0.3, -16 + math.cos(p)*50)
	crossSpinner4.position = GuiDim.new(0.8, -16 + math.sin(p) * 50, 0.5, -16 + math.cos(p)*50)
	p = p + (delta * math.pi*2)
end

--local crossSpinner = GuiRectangle.new()
--crossSpinner.position = GuiDim.new(0.5, -16, 0.5, -16)
--crossSpinner.size = GuiDim.new(0, 32, 0, 32)
--crossSpinner.texture = texCrosshair
--
--local p = 0
--function spinner(delta)
--    --f_crosshair.colour = (math.sin(p) / (math.pi/2) * 0.5 + 0.5, 0, 0));
--	crossSpinner.position = GuiDim.new(0.5, -16 + math.sin(p) * 50, 0.5, -16 + math.cos(p)*50)
--	p = p + (delta * math.pi*2)
--end

--local hud = GuiContainer.new()
--hud:add(crosshair)
--hud:add(crossSpinner)

--game.gui.add(hud)

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