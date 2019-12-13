# Setting Up Unit Tests On Windows

This means creating a new configuration on the visual studio project for running the unit tests.

## Step 1: Open 'New Project Configuration'
1. Open the project in Visual Studio
2. Right click the solution, go to properties
3. On the top-right of the properties menu, click `Configuration Manager`
4. Click on the drop down under "Configuration" and click `<new...>`

![New Configuration](https://i.imgur.com/kPGKaVn.png)

## Step 2: Create the UnitTest configuration
1. Name the configuration `UnitTests`
2. Copy settings from `Debug`
3. Click OK
![New Config Options](https://i.imgur.com/MkoM6WO.png)

## Step 3: Exclude `src/main.cpp` from the UnitTests configuration

The unit tests define their own `int main()`entry point, so the original one must be excluded
1. In the solution explorer, right-click on `src > main.cpp`and open properties
2. Set the Configuration to `UnitTests`
3. Set Excluded From Build to `Yes`
4. ![Exclude main.cpp](https://i.imgur.com/xTNgsdn.png)

## Step 4: Include `tests/tests.cpp`for ONLY unit tests
1. In the solution explorer, right-click on the `tests`folder and click `include in project`

![tests](https://i.imgur.com/n7cI4Bl.png)

2. Right click on `tests > tests.cpp` and open properties
3. Set the configuration to `Debug`
4. Set Excluded From Project to `Yes`

![Exclude from debug](https://i.imgur.com/nJu8NsV.png)

5. Do the same for Release configuration
![Exclude from release](https://i.imgur.com/Pi9wFxA.png)

# Step 5: Run unit tests
At this point you are done! You should be able to set the config to UnitTests and run them!

1. In Visual Studio, at the top left, select `UnitTests`
2. Press `CTRL` + `F5` to run them (This will run them without closing the console window immediately).
3. It should run the unit tests
![Run unit tests](https://i.imgur.com/bJsdHrh.png)