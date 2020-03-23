
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

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
    local menu = StackMenu:create(900, overlay, 15, "Load World")
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    for characterSlot = 0, 5 do
        createWorldSelectButton(menu, characterSlot, data)
    end

    menu:addBackButton()

    if data["mode"] == "vanilla" then
        addPlayerProfile(overlay, data)
    end
end


game.gui.addGui{
    id = "load_world",
    title = "Load World Menu",
    create = onCreate,
}