local size = openbuilder.GUI.GDim.new(0.1, 0, 0.1, 0)

for i = 0, 1, 0.1 do
	local crosshair = openbuilder.GUI.Image.new()
	crosshair:setSource("crosshair")
	crosshair:setPosition(openbuilder.GUI.GDim.new(0, i*10, 0, i*10))
	crosshair:setSize(size)
	openbuilder.GUI.addImage(crosshair)
end

local f_crosshair = openbuilder.GUI.Image.new()
f_crosshair:setSource("crosshair")
f_crosshair:setPosition(openbuilder.GUI.GDim.new(0.5, -16, 0.5, -16))
f_crosshair:setSize(openbuilder.GUI.GDim.new(0, 32, 0, 32))
openbuilder.GUI.addImage(f_crosshair)

local p = 0
function update()
	f_crosshair:setPosition(openbuilder.GUI.GDim.new(0.5, -16 + math.sin(p)*5, 0.5, -16 + math.cos(p)*5))
	p = p + 0.01
end