local size = openbuilder.GUI.GDim.new(0.1, 0, 0.1, 0)

for i = 0, 1, 0.1 do
	local crosshair = openbuilder.GUI.Image.new()
	crosshair:setSource("crosshair")
	crosshair:setPosition(openbuilder.GUI.GDim.new(i, 0, i, 0))
	crosshair:setSize(size)
	openbuilder.GUI.addImage(crosshair)
end


