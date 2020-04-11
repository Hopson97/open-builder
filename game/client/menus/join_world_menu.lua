
local backgroundTexture = game.getTexture("res/join_bg.png")
local logoTexture = game.getTexture("res/logo.png")

local function onCreate(overlay)

    local menu = StackMenu:create(900, overlay, 15, "Join World")
    menu:pad(20)
    menu:setBackground(backgroundTexture)

    local serverIpBox = menu:addTextBox("Server IP", "Enter server IP...")
    --serverIpBox.text = "178.62.64.146"
    serverIpBox.text = "127.0.0.1"

    local label = menu:addLabel()
    label.text = "This will either automatically register \nyou to the server, or log you in."
    label.textSize = 35

    menu:pad(45)

    --local usernameBox = menu:addTextBox("Username", "Enter username...")

    --local passwordBox = menu:addTextBox("Password", "Enter password...")
    --passwordBox:hideInput()

    local joinButton = menu:addButton("Join World")

    menu:pad(40)
    menu:addBackButton()

    joinButton.onClick = function()
        local serverIp = serverIpBox:getText()
       -- local username = usernameBox:getText()
      --  local password = passwordBox:getText()
        if string.len(serverIp) > 0 then
            game.gui():change("transition", { message = "Joining World" } )
            --game.control():joinGame(serverIp, username, password)
            game.control():joinGame(serverIp, "", "")
        end
    end    
end

game.gui():addGui{
    id = "join_world",
    create = onCreate,
}
