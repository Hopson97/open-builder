
local backgroundTexture = game.gui.getTexture("res/char_select_bg.png")

local function onCreate(overlay, slot)
    local menu = StackMenu:create(900, overlay, 15, "New Character")
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    local characterName = menu:addTextBox("Character Name", "Enter name...")
    menu:pad(10)
    local create = menu:addButton("Create Character")

    create.onClick = function()
        game.gui.change("character_select")
    end    


    menu:pad(200)
    local backButton = menu:addButton("Back")
    backButton.onClick = function()
        game.gui.change("character_select")
    end
end

game.gui.addGui{
    id = "new_character",
    title = "New Character Menu",
    create = onCreate,
}