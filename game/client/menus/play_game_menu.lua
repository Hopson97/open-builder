
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function getTitle(data)
    local mode = data["mode"]
    if mode == "multiverse" then
        return "Multiverse Mode"
    elseif mode == "universe" then
        return "Universe Mode"
    else
        return "U N K N O W N"
    end
end

local function onCreate(overlay, data)
    local menu = StackMenu:create(900, overlay, 15, getTitle(data))
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    local newWorld = menu:addButton("New World")
    local loadWorld = menu:addButton("Load World")
    local joinWorld = menu:addButton("Join World")
    menu:addBackButton()

    newWorld.onClick = function()
        game.gui.push("new_world", data)
    end    
    
    loadWorld.onClick = function()
        game.gui.push("load_world", data)
    end

    joinWorld.onClick = function()
        game.gui.push("join_world", data)
    end

    if data["mode"] == "multiverse" then
        print("large")
        local playerName = overlay:addLabel()
        playerName.text = data["characterName"]
        playerName.textSize = 45
        playerName.position = GuiDim.new(0, 10, 0, 800)

        local playerLevel = overlay:addLabel()
        playerLevel.text = "Level " .. data["characterLevel"]
        playerLevel.textSize = 45
        playerLevel.position = GuiDim.new(0, 10, 0, 750)

        local playerImage = overlay:addImage()
        playerImage.image = game.gui.getTexture("res/skins/player.png")
        playerImage.size = GuiDim.new(0, 200, 0, 400)
        playerImage.position = GuiDim.new(0, 110, 0, 320)
    end
end

game.gui.addGui{
    id = "play_game",
    title = "Play Game Menu",
    create = onCreate,
}