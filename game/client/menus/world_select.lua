
local backgroundTexture = game.getTexture("res/menu_bg.png")
local buttonTexture = game.getTexture("res/button_small.png")


local function createWorldSelectButton(menu, slot)
    local selector = menu:addButton("Create New World")

    if slot == 0 then
        local name = "World Name"
        local date = "01/01/20"
        selector.text = name .. '/' .. date
        selector.onClick = function()
            game.gui():change("transition", { message = "Starting Game" } )
            game.control():loadWorld("Test")
        end
    else
        selector.onClick = function()
            game.gui():push("new_world")
        end
    end
end

local function onCreate(overlay)
    local menu = StackMenu:create(900, overlay, 15, "Play World")
    menu:pad(50)
    menu:setBackground(backgroundTexture)

    for characterSlot = 0, 4 do
        createWorldSelectButton(menu, characterSlot)
    end

    local joinWorldButton = overlay:addButton()
    joinWorldButton.textSize = 50
    joinWorldButton.text = "Join World"
    joinWorldButton.image = buttonTexture
    joinWorldButton.size = GuiDim.new(0, 400, 0, 90)
    joinWorldButton.position = GuiDim.new(0.75, 0, 0.8, 0)
    setHighlightOnMouseOver(joinWorldButton)
    joinWorldButton.onClick = function()
        game.gui():push("join_world")
    end

    menu:pad(50)
    menu:addBackButton()
end


game.gui():addGui{
    id = "world_select",
    create = onCreate,
}
