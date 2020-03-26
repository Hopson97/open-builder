
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local buttonTexture = game.gui.getTexture("res/button_small.png")


local function createWorldSelectButton(menu, slot, data)
    local selector = menu:addButton("Create New World")

    if slot == 0 then
        local name = "World Name"
        local date = "01/01/20"
        selector.text = name .. '/' .. date
        selector.onClick = function()
            game.gui.change("transition", { message = "Starting Game" } )
            game.control.loadWorld("Test")
        end
    else
        selector.onClick = function()
            game.gui.push("new_world", data )
        end
    end
end

local function onCreate(overlay, data)
    local menu = StackMenu:create(900, overlay, 15, "Play World")
    menu:pad(50)
    menu:setBackground(backgroundTexture)

    for characterSlot = 0, 4 do
        createWorldSelectButton(menu, characterSlot, data)
    end

    local joinWorld = overlay:addButton()
    joinWorld.textSize = 50
    joinWorld.text = "Join World"
    joinWorld.image = buttonTexture
    joinWorld.size = GuiDim.new(0, 400, 0, 90)
    joinWorld.position = GuiDim.new(0.75, 0, 0.8, 0)
    joinWorld.onClick = function()
        game.gui.push("join_world", data)
    end

    menu:pad(50)
    menu:addBackButton()
end


game.gui.addGui{
    id = "world_select",
    create = onCreate,
}