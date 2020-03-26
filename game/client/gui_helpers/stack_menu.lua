local WIDGET_WIDTH = 800
local WIDGET_HEIGHT = 90
local BUTTON_SIZE = GuiDim.new(0, WIDGET_WIDTH, 0, WIDGET_HEIGHT)
local widgetTexture = game.gui.getTexture("res/button.png")
local checkboxTexture_unchecked = game.gui.getTexture("res/checkbox-unchecked.png")
local checkboxTexture_checked = game.gui.getTexture("res/checkbox-checked.png")

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

function StackMenu:nextWidgetPosition(width, centered, x, y)
    local position
    if(centered) then 
        position = getWidgetCentre(width, self.y - y)
    else
        position = GuiDim.new(0, (1920 / 2 - width / 2) + x, 0, self.y - y)
    end
    self.y = (self.y + y) - self.widgetGap
    return position
end

function StackMenu:initBasicWidget(widget, x, y, w, h, keepAlignment, texture, textSize)
    if(keepAlignment) then
        -- center the button, and place it in sequence through the nextWidgetPosition function -- 
        position = self:nextWidgetPosition(w, true, 0, 0)
        widget.size = GuiDim.new(0, w, 0, h)
        widget.position = position
        
        if(texture ~= nil) then
            widget.image = texture
        end

        if(textSize ~= nil) then
            widget.textSize = textSize
        end   

        setHighlightOnMouseOver(widget)
    else 
        -- use the x, y values as modifiers --
        position = self:nextWidgetPosition(w, false, x, y)
        widget.size = GuiDim.new(0, w, 0, h)
        widget.position = position

        if(texture ~= nil) then
            widget.image = texture
        end

        if(textSize ~= nil) then
            widget.textSize = textSize
        end

        setHighlightOnMouseOver(widget)
    end

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

function StackMenu:addTextBox(label, placeholder)
    self:pad(25)
    local textBox = self.overlay:addTextBox()
    self:initBasicWidget(textBox, 0, 0, WIDGET_WIDTH, WIDGET_HEIGHT, true, widgetTexture, 50)
    textBox.placeholder = placeholder
    textBox.label = label
    textBox.maxLength = 25

    return textBox
end

function StackMenu:addButton(text)
    local button = self.overlay:addButton()
    self:initBasicWidget(button, 0, 0, WIDGET_WIDTH, WIDGET_HEIGHT, true, widgetTexture, 50)
    button.text = text

    return button
end

function StackMenu:addCheckBox(text)
    local checkBox = self.overlay:addCheckBox()
    self:initBasicWidget(checkBox, -250, 0, WIDGET_WIDTH - 500, WIDGET_HEIGHT, false, checkboxTexture_unchecked, 30)
    checkBox.text = text
    return checkBox
end

function StackMenu:addLabel(text)
    local label = self.overlay:addLabel()
    self:initBasicWidget(label, 0, 0, WIDGET_WIDTH, WIDGET_HEIGHT, true, widgetTexture, 100)
    if(text ~= nil) then
        label.text = text
    end
    return label
end

function StackMenu:addBackButton()
    local backButton = self:addButton("Back")
    backButton.onClick = function()
        game.gui.pop()
    end
    return backButton
end

function StackMenu:pad(amount)
    self.y = self.y - amount
end