@echo off

set CFLAGS=/W3 /WX /O2 /Ignored 
set INCLUDES=/I src\include

if not exist out (
	mkdir out
)

cl.exe %CFLAGS% %INCLUDES% /Fe"out\vector_example.exe" examples\vector_example.c
cl.exe %CFLAGS% %INCLUDES% /Fe"out\hash_table_example.exe" examples\hash_table_example.c src\hash_table.c
