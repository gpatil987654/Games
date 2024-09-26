cl.exe /c /EHsc Snake.c
link Snake.obj user32.lib gdi32.lib kernel32.lib /SUBSYSTEM:WINDOWS
snake.exe

