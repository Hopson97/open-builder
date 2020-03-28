local WIDGET_WIDTH = 800
local WIDGET_HEIGHT = 90
local BUTTON_SIZE = GuiDim.new(0, WIDGET_WIDTH, 0, WIDGET_HEIGHT)
local widgetTexture = game.getTexture("res/button.png")
local checkboxTexture_unchecked = game.getTexture("res/checkbox-unchecked.png")
local checkboxTexture_checked = game.getTexture("res/checkbox-checked.png")

StackMenu = {}
StackMenu.__index = StackMenu

local function getWidgetCentre(widgetWidth, y) 
    return GuiDim.new(0, 1920 / 2 - widgetWidth / 2, 0, y)
end

function StackMenu:create(yStart, overlay, gap, title)
    local vals = {}
    setmetatable(vals, StackMenu)
    vals.y = yStart
    vals.overlay = overlay
    vals.widgetGap = WIDGET_HEIGHT + gap
    if title ~= nil then
        local label = createTitle(overlay, title, vals.y)
        vals.y = vals.y - 140
    end
    return vals
end

function StackMenu:nextWidgetPosition()
    local position = getWidgetCentre(WIDGET_WIDTH, self.y)
    self.y = self.y - self.widgetGap
    return position
end

function StackMenu:setBackground(image)
    return createBackground(self.overlay, image)
end

function StackMenu:addImage(texture, width, height)
    local image = self.overlay:addImage()
    image.size = GuiDim.new(0, width, 0, height)
    image.position = getWidgetCentre(width, self.y)
    image.image = texture
    self.y = self.y - height
    return image
end

function StackMenu:initBasicWidget(widget)
    widget.size = BUTTON_SIZE
    widget.position = self:nextWidgetPosition()
    widget.textSize = 50
    widget.image = widgetTexture
    setHighlightOnMouseOver(widget)
end

function StackMenu:addTextBox(label, placeholder)
    self:pad(25)
    local textBox = self.overlay:addTextBox()
    self:initBasicWidget(textBox)
    textBox.placeholder = placeholder
    textBox.label = label
    textBox.maxLength = 25

    return textBox
end

function StackMenu:addButton(text)
    local button = self.overlay:addButton()
    self:initBasicWidget(button)
    button.text = text

    return button
end

function StackMenu:addCheckBox(text)
    local checkBox = self.overlay:addCheckBox()
    self:initBasicWidget(checkBox)
    checkBox.position = GuiDim.new(0, (1920 / 2 - (WIDGET_WIDTH - 500) / 2), 0, self.y + self.widgetGap) -- LEFT OFF HERE!
    checkBox.size = GuiDim.new(0, WIDGET_WIDTH - 500, 0, WIDGET_HEIGHT)
    checkBox.image = checkboxTexture_unchecked
    checkBox.text = text
    checkBox.textSize = 30
    checkBox.checkedImage = checkboxTexture_checked
    checkBox.uncheckedImage = checkboxTexture_unchecked
    return checkBox
end

function StackMenu:addLabel(text)
    local label = self.overlay:addLabel()
    label.textSize = 100
    label.position = self:nextWidgetPosition()
    if text ~= nil then
        label.text = text
    end
    return label
end

function StackMenu:addBackButton()
    local backButton = self:addButton("Back")
    backButton.onClick = function()
        game.gui():pop()
    end
    return backButton
end

function StackMenu:pad(amount)
    self.y = self.y - amount
end
