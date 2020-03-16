
local backgroundTexture = game.gui.getTexture("res/menu_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function onCreate(overlay, data)

    local menu = StackMenu:create(900, overlay, 15, "Join World")
    menu:pad(100)
    menu:setBackground(backgroundTexture)

    local serverIpBox = menu:addTextBox("Server IP", "Enter server IP...")
    serverIpBox.text = "178.62.64.146"
    menu:pad(10)
    local joinButton = menu:addButton("Join World")
    menu:pad(200)
    local backButton = menu:addButton("Back")

    joinButton.onClick = function()
        local serverIp = serverIpBox:getText()
        if string.len(serverIp) > 0 then
            game.gui.change("transition", "Joining World")
            game.control.joinGame(serverIp)
        end
    end    

    backButton.onClick = function()
        game.gui.change("play_game", data)
    end
end

game.gui.addGui{
    id = "join_world",
    title = "Join World Menu",
    create = onCreate,
}