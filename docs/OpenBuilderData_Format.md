# Open Builder Format

Open builder data (.obd) is the data format used by the game for general data such as block defintions and configurations.

It is very simple, example grass block:

```
block
    name grass
    texture_top grass
    texture_side grass_side
    texture_bottom dirt
    mesh block
    type solid
end
```

The first line defines what the data is, in this case a block defintion.

The lines between the "title" (block) and "end" define the data.

Each piece of data is a key-value pair seperated by some whitespace

For example

`name grass` 

Tells the engine that this block has the name grass.

