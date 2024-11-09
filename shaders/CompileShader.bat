@echo off
setlocal

:: Set the shader directory (current directory by default)
set "SHADER_DIR=%cd%"

:: Define shader file extensions you want to compile
set "EXTENSIONS=.vert .frag .comp"

:: Check if the Vulkan SDK environment variable is set
if not defined VULKAN_SDK (
    echo Error: VULKAN_SDK is not defined. Please set it in your system environment variables.
    exit /b 1
)

:: Set path to glslc compiler
set "GLSLC=%VULKAN_SDK%\Bin\glslc.exe"

:: Verify if glslc exists
if not exist "%GLSLC%" (
    echo Error: glslc not found in %VULKAN_SDK%\Bin. Please check your Vulkan SDK installation.
    exit /b 1
)

:: Loop through each shader extension
for %%E in (%EXTENSIONS%) do (
    :: Find all files with the current extension in the shader directory
    for %%F in ("%SHADER_DIR%\*%%E") do (
        :: Check if there are matching files
        if exist "%%F" (
            :: Compile shader to .spv file with the original extension included in the name
            echo Compiling %%~nxF...
            "%GLSLC%" "%%F" -o "%%F.spv"
            if %ERRORLEVEL% NEQ 0 (
                echo Failed to compile %%~nxF
            ) else (
                echo Successfully compiled %%~nxF to %%F.spv
            )
        )
    )
)

echo All shaders processed.
endlocal