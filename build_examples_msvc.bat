@echo off

set CFLAGS=/W3 /WX /O2
set INCLUDES=/I src\include

if "%*"=="" (
	set argv=all
) else (
	set argv=%*
)

if not exist out (
	mkdir out
)

setlocal EnableDelayedExpansion
set verbose=0

if %argv%==all (
	set argv= 
	for %%f in (examples\*) do (
		set file=%%~nf
		set argv=!argv! !file:_example=!%%~xf
	)
) else if %argv:~0,2%==-v (
	set verbose=1
	set argv=%argv:~0,2%
)

for %%a in (!argv!) do (
	if %%a==help (
		echo "Usage: %~n0%~x0 "[all|help|examples]
		echo Builds given examples. Defaults to all.
		exit 0
	)

	set file=%%~na
	set example_file=examples\!file!_example.c
	if not exist !example_file! (
		echo Error: unknown example `%file%` 1>&2
		exit 1
	)

	set source_file=src\!file!.c
	if exist !source_file! (
		set command=cl.exe %CFLAGS% %INCLUDES% /Fe"out\!file!_example.exe" !example_file! !source_file!
	) else (
		set command=cl.exe %CFLAGS% %INCLUDES% /Fe"out\!file!_example.exe" !example_file!
	)

	if %verbose%==1 (
		echo [CMD] !command!
		!command!
	) else (
		!command! >NUL
	)
)
