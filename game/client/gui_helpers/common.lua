
function createTitle(overlay, text, y)
    local label = overlay:addCenteredLabel()
    label.position = GuiDim.new(0, 0, 0, y)
    label.text = text
    label.textSize = 100
    return label
end

function createBackground(overlay, image)
    local background = overlay:addImage()
    background.size = GuiDim.new(1.0, 0, 1.0, 0)
    background.image = image
    return background
end

function setHighlightOnMouseOver(widget) 
    widget.onMouseOver = function()
        widget:setColour(1.25, 1.25, 1.25)
    end

    widget.onMouseOff = function()
        widget:setColour(1, 1, 1)
    end
end
