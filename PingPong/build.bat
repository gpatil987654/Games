cl.exe /c /EHsc pingpong.c
link.exe pingpong.obj winmm.lib user32.lib gdi32.lib kernel32.lib
pingpong.exe



