local BUTTON_WIDTH = 800
local BUTTON_HEIGHT = 90
local BUTTON_SIZE = GuiDim.new(0, BUTTON_WIDTH, 0, BUTTON_HEIGHT)
local buttonTexture = game.gui.getTexture("res/button.png")

GuiStack = {}
GuiStack.__index = GuiStack

local function getWidgetCentre(widgetWidth, y) 
    return GuiDim.new(0, 1920 / 2 - widgetWidth / 2, 0, y)
end

function GuiStack:create(yStart, overlay, gap)
    local vals = {}
    setmetatable(vals, GuiStack)
    vals.y = yStart
    vals.overlay = overlay
    vals.buttonGap = BUTTON_HEIGHT + gap
    return vals
end

function GuiStack:nextButtonPosition()
    local position = getWidgetCentre(BUTTON_WIDTH, self.y)
    self.y = self.y - self.buttonGap
    return position
end

function GuiStack:addImage(texture, width, height)
    local image = self.overlay:addImage()
    image.size = GuiDim.new(0, width, 0, height)
    image.position = getWidgetCentre(width, self.y)
    image.image = texture
    self.y = self.y - height
    return image
end

function GuiStack:addButton(text)
    local button = self.overlay:addButton()
    button.size = BUTTON_SIZE
    button.position = self:nextButtonPosition()
    button.text = text
    button.textSize = 50
    button.image = buttonTexture

    return button
end

function GuiStack:pad(amount)
    self.y = self.y - amount
end