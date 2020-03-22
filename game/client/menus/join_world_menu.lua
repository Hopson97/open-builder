
local backgroundTexture = game.gui.getTexture("res/join_bg.png")
local logoTexture = game.gui.getTexture("res/logo.png")

local function onCreate(overlay, data)

    local menu = StackMenu:create(900, overlay, 15, "Join World")
    menu:pad(20)
    menu:setBackground(backgroundTexture)

    local serverIpBox = menu:addTextBox("Server IP", "Enter server IP...")
    serverIpBox.text = "178.62.64.146"

    local label = menu:addLabel()
    label.text = "This will either automatically register \nyou to the server, or log you in."
    label.textSize = 35

    menu:pad(45)

    local usernameBox = menu:addTextBox("Username", "Enter username...")
    local passwordBox = menu:addTextBox("Password", "Enter password...")

    passwordBox:hideInput()

    local joinButton = menu:addButton("Join World")

    menu:pad(40)

    local backButton = menu:addButton("Back")

    joinButton.onClick = function()
        local serverIp = serverIpBox:getText()
        local username = usernameBox:getText()
        local password = passwordBox:getText()
        if string.len(serverIp) > 0 then
            game.gui.change("transition", { message = "Joining World" } )
            game.control.joinGame(serverIp, username, password)
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