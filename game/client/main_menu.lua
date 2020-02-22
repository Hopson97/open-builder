
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local BUTTON_SIZE = GuiDim.new(0.2, 0, 0.1, 0)
local BUTTON_GAP = 0.21

local x = 0
local y = 0

local function addButton(overlay, label)
    local button = overlay:addButton()
    button.size = BUTTON_SIZE
    button.position = GuiDim.new(x, 0, y, 0)
    button.text = label
    button.textSize = 40

    x = x + BUTTON_GAP
    return button
end

local function singleplayerClicked()
    print("Singleplayer clicked")
    game.gui.pop()
    game.gui.push("hud")
   -- controller:startGame()
   -- game.gui.push("loading")
end


local function multiplayerClicked()
    print("Multiplayer clicked")
    --  controller:exit() --This should disconnect client too duh
end

local function settingClicked()
    print("Singleplayer clicked")

    --  controller:exit() --This should disconnect client too duh
end

local function exitClicked()
    print("Singleplayer clicked")

    --  controller:exit() --This should disconnect client too duh
end



local function onCreate(overlay)
    x = 0.085
    y = 0.05

    local background = overlay:addImage()
    background.size = GuiDim.new(1.0, 0, 1.0, 0)
    background.image = backgroundTexture

    local logo = overlay:addImage()
    logo.size = GuiDim.new(0.4, 0, 0.2, 0)
    logo.image = logoTexture
    logo.position = GuiDim.new(0.025, 0, 0.8, 0)

    local startGame = addButton(overlay, "Singleplayer")
    startGame.onClick = singleplayerClicked

    local multiplayer = addButton(overlay, "Multiplayer")
    multiplayer.onClick = multiplayerClicked

    local settings = addButton(overlay, "Settings")
    settings.onClick = settingClicked

    local exitGame = addButton(overlay, "Exit Game")
    exitGame.onClick = exitClicked
end

game.gui.addGui{
    id = "main_menu",
    title = "Main Menu",
    create = onCreate,
}


game.onClientStartup(function()
    game.gui.push("main_menu")
end)