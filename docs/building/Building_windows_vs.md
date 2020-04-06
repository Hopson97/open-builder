
# Why
Sometimes CMake doesn't work or can be a pain on Windows.

This article will talk you through a somewhat hacky way of doing it, but it will work if you follow the guide exactly.

**Do not skip over any text, or this will not work.**

# The Guide

## Prerequisites
Make sure you have this librarydownloaded somewhere on your machine. Remember where it is downloaded to.

https://www.sfml-dev.org/download/sfml/2.5.1/

## Step 1: Make an empty project

Start by opening visual studio, create a new project, and selecting 'Empty C++ Project". Call the project "open-builder' and tick the box for "Place solution and project in the same directory.

Create the project.

![alt text](https://i.imgur.com/LoaLc8T.png "Config")

## Step 2: Clone the GitHub repository and copy the files into the project
Open a terminal (eg powershell) somewhere (NOT the same folder as your project) and run command:

`git clone https://github.com/Hopson97/open-builder.git`

Open file explorer to this location, and also where the visual studio project. Ensure you can seen hidden files eg .git (View> Tick "Hidden Items".

Move/ Copy the ALL files from the Git repo folder to the Visual Studio Project:

Before
![Folders Before](https://i.imgur.com/GOzTP2H.png)

After
![Folder After](https://i.imgur.com/R794bae.png)

## Step 3: Ensure you can see them in visual studio

Open up the visual studio project and click on "Show All Files" on the solution explorer.

![Show All Files](https://i.imgur.com/izlbSYN.png)

This should show all the files.

## Step 4: Include the `src/` and `deps/` in the project

Right click on the `src` and `deps` folders and press "Include In Project".

This will take a few seconds. It will make sure the visual studio builds these files

Eg for `src`:
![Include In Project](https://i.imgur.com/IgQ4P1C.png)



## Step 5: Set up C++17

1. Right click the `open-builder` solution in the solution explorer, and select `Properties` (The wrench symbol).
2. This will open the `open-builder Property Pages`modal.
3. Ensure that "All Configurations" is selected from the drop-down at the top of the modal.
4. Go to `Configuration Properties > General`
5. Select `ISO C++17 Standard (std:c++17)` as the C++ Language Standard.
6. Click "Apply" (Leave the modal open though as it is needed for the next few steps)

![enter image description here](https://i.imgur.com/n4UARgf.png)


## Step 6: Include directories

1. Ensure that "All Configurations" is selected from the drop-down at the top of the modal.
2. In the property pages, go to `C/C++ > General`, click the `Additional Include Directories` and click on the arrow, then click `<Edit>`

This will open up another window.

In here, you'll need to add **5** include directories:

1. Where you installed SFML, add the `include`folder
2. The `deps`directory (Which is part of this project)
3. The `deps\lua` directory
4. The `src\common`directory
5. The `src\server` directory

It should look like this:

![Include Directories](https://i.imgur.com/tWVWXzj.png)


Click "OK".

Keep the Property Pages modal open.

## Step 7: Linker directories

Visual Studio needs to know where SFML libs are.

1. Ensure that "All Configurations" is selected from the drop-down at the top of the modal.
2. Go to `Linker > General`
3. In the `Additional Library Directories` row, add the `SFML/lib` directory:

![Library](https://i.imgur.com/ScTxBcG.png)

Keep the modal open

## Step 8: Link SFML

You'll now need to link the SFML libraries.

### Debug Mode

Go to `Linker > Input` and set the configuration mode to "Debug":

![Debug](https://i.imgur.com/SGjpi69.png)

Open `Additional Dependencies`, copy paste these into the text box

```
sfml-graphics-d.lib
sfml-system-d.lib
sfml-audio-d.lib
sfml-window-d.lib
sfml-network-d.lib
```

 **Make sure they are on different lines** like this:

![Different Lines](https://i.imgur.com/4qPWoAp.png)

Click Ok.

After, it should look like this:

![debug](https://i.imgur.com/y184ozY.png)

### Release Mode
We now need to do the same for release mode.

1. Set the configuration mode to "Release":
2. Open `Additional Dependencies`, copy paste these into the text box
3. **Make sure they are on different lines**

```
sfml-graphics.lib
sfml-system.lib
sfml-audio.lib
sfml-window.lib
sfml-network.lib
```

After, it should look like this:

![Release](https://i.imgur.com/lIm0lpJ.png)

You may now press OK to close the modal.

At this point, the project should compile. (`CTRL + SHIFT + B`)

However, it will not run until the last step is completed:

## Step 9: Copy SFML .dll files into the project

Open where you have SFML downloaded to, and open the `bin/`directory.

You should see a bunch of .dll files:

![.dll Files](https://i.imgur.com/PbN6KyB.png)

Copy and paste all of these into the visual studio studio project file:

![.dll](https://i.imgur.com/Z1CxSWX.png)

## Step 10

You are done!

You will now be able to build and run the project, which will launch a localhost server and client.

If you cannot, then it means a step may have been followed incorrectly.

In this case, go back through at a slower pace and make sure you did every step.

You will get there in the end :)

Enjoy!
