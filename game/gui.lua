-- gui.lua

-- This crosshair rotates around the main crosshair
local test = openbuilder.GUI.Image.new()
test:setSource("crosshair")
test:setPosition(openbuilder.GUI.GDim.new(0.5, -16, 0.5, -16))
test:setSize(openbuilder.GUI.GDim.new(0, 32, 0, 32))
test:setColor(openbuilder.GUI.Color3.new(0,0,0));
openbuilder.GUI.addImage(test)

-- Main crosshair (to be used in final)
local f_crosshair = openbuilder.GUI.Image.new()
f_crosshair:setSource("crosshair")
f_crosshair:setPosition(openbuilder.GUI.GDim.new(0.5, -16, 0.5, -16))
f_crosshair:setSize(openbuilder.GUI.GDim.new(0, 32, 0, 32))
openbuilder.GUI.addImage(f_crosshair)

-- Spin and colour the crosshairs
local p = 0
function update(delta)
    f_crosshair:setColor(openbuilder.GUI.Color3.new(math.sin(p)/(math.pi/2) * 0.5 + 0.5, 0, 0));
	test:setPosition(openbuilder.GUI.GDim.new(0.5, -16 + math.sin(p)*50, 0.5, -16 + math.cos(p)*50))
	p = p + (delta * math.pi*2)
end