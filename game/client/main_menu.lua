
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function singleplayerClicked()
    print("Singleplayer clicked")
    game.gui.pop()
    game.gui.push("hud")
    game.input.lockMouse()
    game.input.hideMouse()
   -- controller:startGame()
   -- game.gui.push("loading")
end


local function multiplayerClicked()
    print("Multiplayer clicked")
    --  controller:exit() --This should disconnect client too duh
end

local function settingClicked()
    print("Settings clicked")

    --  controller:exit() --This should disconnect client too duh
end

local function exitClicked()
    print("Exit clicked")

    --  controller:exit() --This should disconnect client too duh
end

local function onCreate(overlay)
    y = 600

    local background = overlay:addImage()
    background.size = GuiDim.new(1.0, 0, 1.0, 0)
    background.image = backgroundTexture

    --local logo = overlay:addImage()
    --logo.size = GuiDim.new(0.5, 0, 0.25, 0)
    --logo.position = GuiDim.new(0.25, 0, 0.75, 0)
    --logo.image = logoTexture


    local menu = GuiStack:create(900, overlay, 15)
    menu:addImage(logoTexture, 1064, 138)
    menu:pad(150)
    local startGame = menu:addButton("Singleplayer")
    local multiplayer = menu:addButton("Multiplayer")
    local settings = menu:addButton("Settings")
    local exitGame = menu:addButton("Exit Game")

    --local startGame = addButton(overlay, "Singleplayer")
    startGame.onClick = singleplayerClicked
    multiplayer.onClick = multiplayerClicked
    settings.onClick = settingClicked
    exitGame.onClick = exitClicked

    overlay:hide()
    overlay:show()
end

game.gui.addGui{
    id = "main_menu",
    title = "Main Menu",
    create = onCreate,
}