
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")

local function onCreate(overlay, data)
    local label = overlay:addLabel()
    label.text = data
    label.textSize = 128
    label.position = GuiDim.new(0, 10, 0, 950)

    local background = overlay:addImage()
    background.size = GuiDim.new(1.0, 0, 1.0, 0)
    background.image = backgroundTexture
end

game.gui.addGui{
    id = "transition",
    title = "Transition Screen",
    create = onCreate,
}

game.onEnterGame(function() 
    game.input.hideMouse()
    game.gui.change("hud")
end)

game.onExitGame(function() 
    game.input.showMouse()
    game.gui.change("main_menu")
end)