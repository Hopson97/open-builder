
local backgroundTexture = game.gui.getTexture("res/char_select_bg.png")
local buttonTexture = game.gui.getTexture("res/button.png")
local buttonTextureSmall = game.gui.getTexture("res/button_small.png")

local CHAR_SELECT_WIDTH = 400
local CHAR_SELECT_HEIGHT = 90
local CHAR_SLOT_SIZE = GuiDim.new(0, CHAR_SELECT_WIDTH, 0, CHAR_SELECT_HEIGHT)

local function createCharacterSelectButton(overlay, slot)
    local backButton = overlay:addButton()
    backButton.textSize = 40
    backButton.position = GuiDim.new(0, 10, 0, 870 - slot * 9 - CHAR_SELECT_HEIGHT * slot)
    backButton.size = CHAR_SLOT_SIZE
    backButton.image = buttonTextureSmall
    setHighlightOnMouseOver(backButton)

    backButton.text = "+ New Character"

    if slot == 5 then
        backButton.text = "Charles/ Lvl 10"
    end


    backButton.onClick = function()
        game.gui.change("new_character", tostring(slot))
    end
end

local function create(overlay) 
    local title = createTitle(overlay, "Select A Character", 985)
    createBackground(overlay, backgroundTexture)

    for characterSlot = 0, 8 do
        createCharacterSelectButton(overlay, characterSlot)
    end

    local backButton = overlay:addButton()
    backButton.text = "Back"
    backButton.textSize = 50
    backButton.position = GuiDim.new(0, 1900 - CHAR_SELECT_WIDTH, 0, 20)
    backButton.size = CHAR_SLOT_SIZE
    backButton.image = buttonTextureSmall
    setHighlightOnMouseOver(backButton)

    backButton.onClick = function()
        game.gui.change("main_menu")
    end
end

game.gui.addGui{
    id = "character_select",
    title = "Character Select Menu",
    create = create
}