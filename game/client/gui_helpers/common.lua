
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

function addPlayerProfile(overlay, data)
    local playerName = overlay:addLabel()
    playerName.text = data["characterName"]
    playerName.textSize = 45
    playerName.position = GuiDim.new(0, 10, 0, 800)
    
    local playerLevel = overlay:addLabel()
    playerLevel.text = "Level " .. data["characterLevel"]
    playerLevel.textSize = 45
    playerLevel.position = GuiDim.new(0, 10, 0, 750)
    
    local playerImage = overlay:addImage()
    playerImage.image = game.gui.getTexture("res/skins/player.png")
    playerImage.size = GuiDim.new(0, 200, 0, 400)
    playerImage.position = GuiDim.new(0, 110, 0, 320)
end