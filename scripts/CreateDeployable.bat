rmdir /s /q OpenBuilder
mkdir OpenBuilder
mkdir OpenBuilder\game
mkdir OpenBuilder\shaders
mkdir OpenBuilder\res
mkdir OpenBuilder\texture_packs


xcopy /s .\Release\builderverse.exe OpenBuilder

xcopy /s /e .\game OpenBuilder\game
xcopy /s /e .\shaders OpenBuilder\shaders
xcopy /s /e .\res OpenBuilder\res
xcopy /s /e .\texture_packs OpenBuilder\texture_packs

robocopy .\ OpenBuilder credits.txt
robocopy .\ OpenBuilder config.obd

robocopy .\ OpenBuilder sfml-audio-2.dll
robocopy .\ OpenBuilder sfml-graphics-2.dll
robocopy .\ OpenBuilder sfml-network-2.dll
robocopy .\ OpenBuilder sfml-system-2.dll
robocopy .\ OpenBuilder sfml-window-2.dll
robocopy .\ OpenBuilder openal32.dll
