# Open Builder Format

Open builder data (.obd) is the data format used by the game for config stuff

It is very simple, example config for the client:

```
CLIENT_DATA
    cap_fps 0
    fps_limit 60
    fov 90

    fullscreen 0
    window_width 1280
    window_height 720

    skin player
    texture_pack default
end

```

The first line defines what the data is, in this case a block defintion.

The lines between the "title" (CLIENT_DATA) and "end" define the data.

Each piece of data is a key-value pair seperated by some whitespace

For example

`window_width 1280`

Tells the client window is 1280 pixels wide

