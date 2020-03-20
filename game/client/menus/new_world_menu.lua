
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")

local function onCreate(overlay, data)
    local menu = StackMenu:create(900, overlay, 15, "New World")
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    local worldNameBox = menu:addTextBox("World Name", "Enter name...")
    local worldSeedBox = menu:addTextBox("World Seed", "Enter seed...")
    menu:pad(10)
    local create = menu:addButton("Create World")
    menu:pad(200)
    local backButton = menu:addButton("Back")

    create.onClick = function()
        local name = worldNameBox:getText()
        local seed = worldSeedBox:getText()
        if string.len(name) > 0 then
            game.gui.change("transition", "Creating World")
            game.control.createWorld(name, seed)
        end
    end    

    backButton.onClick = function()
        game.gui.change("play_game", data)
    end
end

game.gui.addGui{
    id = "new_world",
    title = "New World Menu",
    create = onCreate,
}