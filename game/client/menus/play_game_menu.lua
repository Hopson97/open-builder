
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function getTitle(data)
    local mode = data["mode"]
    if mode == "vanilla" then
        return "Vanilla Mode"
    elseif mode == "modded" then
        return "Modded Mode"
    else
        return "U N K N O W N"
    end
end

local function onCreate(overlay, data)
    local menu = StackMenu:create(900, overlay, 15, getTitle(data))
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    local playWorld = menu:addButton("Play World")
    local joinWorld = menu:addButton("Join World")
    menu:addBackButton()

    playWorld.onClick = function()
        game.gui.push("load_world", data)
    end    

    joinWorld.onClick = function()
        game.gui.push("join_world", data)
    end

    if data["mode"] == "vanilla" then
        addPlayerProfile(overlay, data)
    end
end

game.gui.addGui{
    id = "play_game",
    title = "Play Game Menu",
    create = onCreate,
}