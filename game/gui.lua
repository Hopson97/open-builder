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
f_crosshair:setPosition(openbuilder.GUI.GDim.new(0.5, -8, 0.5, -8))
f_crosshair:setSize(openbuilder.GUI.GDim.new(0, 16, 0, 16))
openbuilder.GUI.addImage(f_crosshair)