local BUTTON_WIDTH = 800
local BUTTON_HEIGHT = 90
local BUTTON_SIZE = GuiDim.new(0, BUTTON_WIDTH, 0, BUTTON_HEIGHT)
local buttonTexture = game.gui.getTexture("res/button.png")

StackMenu = {}
StackMenu.__index = StackMenu

local function getWidgetCentre(widgetWidth, y) 
    return GuiDim.new(0, 1920 / 2 - widgetWidth / 2, 0, y)
end

function StackMenu:create(yStart, overlay, gap)
    local vals = {}
    setmetatable(vals, StackMenu)
    vals.y = yStart
    vals.overlay = overlay
    vals.buttonGap = BUTTON_HEIGHT + gap
    return vals
end

function StackMenu:nextButtonPosition()
    local position = getWidgetCentre(BUTTON_WIDTH, self.y)
    self.y = self.y - self.buttonGap
    return position
end

function StackMenu:setBackground(image)
    local background = self.overlay:addImage()
    background.size = GuiDim.new(1.0, 0, 1.0, 0)
    background.image = image
    return background
end

function StackMenu:addImage(texture, width, height)
    local image = self.overlay:addImage()
    image.size = GuiDim.new(0, width, 0, height)
    image.position = getWidgetCentre(width, self.y)
    image.image = texture
    self.y = self.y - height
    return image
end

function StackMenu:addButton(text)
    local button = self.overlay:addButton()
    button.size = BUTTON_SIZE
    button.position = self:nextButtonPosition()
    button.text = text
    button.textSize = 50
    button.image = buttonTexture

    button.onMouseOver = function()
        button:setColour(1.25, 1.25, 1.25)
    end

    button.onMouseOff = function()
        button:setColour(1, 1, 1)
    end

    return button
end

function StackMenu:pad(amount)
    self.y = self.y - amount
end