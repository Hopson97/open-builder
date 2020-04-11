
local function onCreate(overlay)
    local menu = StackMenu:create(500, overlay, 15)

    local resume = menu:addButton("Resume")
    local exit = menu:addButton("Exit")

    resume.onClick = function()
        game.gui():pop()
        game.input.hideMouse()
        game.control():resume()
    end

    exit.onClick = function()
        game.gui():change("transition", {message = "Exiting"})
        game.control():exitGame()
    end
end

game.gui():addGui{
    id = "pause",
    create = onCreate,
}
