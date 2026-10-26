#Requires -Version 5.1
$ErrorActionPreference = "Stop"

$RootDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BuildDir = Join-Path $RootDir "build"
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

# Toolchain: $env:CONTRA3_CC (path to gcc.exe), else the sibling ..\tools\w64devkit, else gcc on PATH
$DevkitBin = Join-Path (Split-Path -Parent $RootDir) "tools\w64devkit\bin"
if ($env:CONTRA3_CC) {
    $Gcc = $env:CONTRA3_CC
    $Ar  = Join-Path (Split-Path -Parent $Gcc) "ar.exe"
} elseif (Test-Path (Join-Path $DevkitBin "gcc.exe")) {
    $Gcc = Join-Path $DevkitBin "gcc.exe"
    $Ar  = Join-Path $DevkitBin "ar.exe"
} else {
    $Gcc = "gcc"
    $Ar  = "ar"
}
Write-Host "[CC] using $Gcc"

$Sources = @(
    "snes_runtime",
    "snes_ppu",
    "snes_apu",
    "snes_movie",
    "snes_savestate",
    "snes_rom",
    "snes_mainthread",
    "contra3_extras",
    "contra3_events",
    "c3_config",
    "c3_overlay",
    "c3_rumble",
    "bank_00",
    "bank_01",
    "bank_02",
    "bank_03",
    "bank_04",
    "bank_1d",
    "bank_data_manifest",
    "dispatch"
)

$ObjFiles = @()
foreach ($Src in $Sources) {
    $SrcPath = Join-Path $RootDir "src\$Src.c"
    $ObjPath = Join-Path $BuildDir "$Src.o"
    Write-Host "[CC] src/$Src.c"
    & $Gcc -std=c11 -O2 -Wall -Wextra -Werror -I"$RootDir\include" -c $SrcPath -o $ObjPath
    if ($LASTEXITCODE -ne 0) { throw "Compilation failed for src/$Src.c" }
    $ObjFiles += $ObjPath
}

$LibPath = Join-Path $BuildDir "libcontra3_recomp.a"
Write-Host "[AR] build/libcontra3_recomp.a"
& $Ar rcs $LibPath $ObjFiles
if ($LASTEXITCODE -ne 0) { throw "Archive creation failed" }

$AppSrc = Join-Path $RootDir "src\win32_main.c"
if (Test-Path $AppSrc) {
    $AppExe = Join-Path $BuildDir "contra3.exe"
    Write-Host "[LD] build/contra3.exe"
    $GlSrc = Join-Path $RootDir "src\win32_gl.c"
    $GlpSrc = Join-Path $RootDir "src\gl_present.c"
    & $Gcc -std=c11 -O2 -Wall -Wextra -Werror -I"$RootDir\include" $AppSrc $GlSrc $GlpSrc $LibPath -lopengl32 -lgdi32 -lwinmm -lcomdlg32 -o $AppExe
    if ($LASTEXITCODE -ne 0) { throw "Desktop app build failed" }
}

# Optional SDL2 frontend: set $env:CONTRA3_SDL2 to an SDL2 MinGW dev folder (x86_64-w64-mingw32)
if ($env:CONTRA3_SDL2) {
    $SdlExe = Join-Path $BuildDir "contra3_sdl.exe"
    Write-Host "[LD] build/contra3_sdl.exe"
    & $Gcc -std=c11 -O2 -Wall -Wextra -Werror -I"$RootDir\include" -I"$env:CONTRA3_SDL2\include\SDL2" (Join-Path $RootDir "src\sdl_main.c") (Join-Path $RootDir "src\gl_present.c") $LibPath -L"$env:CONTRA3_SDL2\lib" -lmingw32 -lSDL2main -lSDL2 -o $SdlExe
    if ($LASTEXITCODE -ne 0) { throw "SDL2 frontend build failed" }
    Copy-Item (Join-Path $env:CONTRA3_SDL2 "bin\SDL2.dll") $BuildDir -Force
}

$UnitSrc = Join-Path $RootDir "tests\unit_test.c"
if (Test-Path $UnitSrc) {
    $UnitExe = Join-Path $BuildDir "contra3_unit_test.exe"
    Write-Host "[LD] build/contra3_unit_test.exe"
    & $Gcc -std=c11 -O2 -Wall -Wextra -Werror -I"$RootDir\include" $UnitSrc $LibPath -o $UnitExe
    if ($LASTEXITCODE -ne 0) { throw "Unit test build failed" }
    Write-Host "[RUN] contra3_unit_test.exe"
    & $UnitExe $BuildDir
    if ($LASTEXITCODE -ne 0) { throw "Unit tests failed" }
}

$TestSrc = Join-Path $RootDir "tests\smoke_test.c"
if (Test-Path $TestSrc) {
    $TestExe = Join-Path $BuildDir "contra3_smoke_test.exe"
    Write-Host "[LD] build/contra3_smoke_test.exe"
    & $Gcc -std=c11 -O2 -Wall -Wextra -Werror -I"$RootDir\include" $TestSrc $LibPath -o $TestExe
    if ($LASTEXITCODE -ne 0) { throw "Test build failed" }
    Write-Host "[RUN] contra3_smoke_test.exe"
    & $TestExe
    if ($LASTEXITCODE -ne 0) { throw "Smoke test execution failed" }
}

$ReplaySrc = Join-Path $RootDir "tests\replay_test.c"
if (Test-Path $ReplaySrc) {
    $ReplayExe = Join-Path $BuildDir "contra3_replay_test.exe"
    Write-Host "[LD] build/contra3_replay_test.exe"
    & $Gcc -std=c11 -O2 -Wall -Wextra -Werror -I"$RootDir\include" $ReplaySrc $LibPath -o $ReplayExe
    if ($LASTEXITCODE -ne 0) { throw "Replay test build failed" }
    Write-Host "[RUN] contra3_replay_test.exe (golden state hashes)"
    & $ReplayExe --golden (Join-Path $RootDir "tests\golden")
    if ($LASTEXITCODE -ne 0) { throw "Golden replay regression detected (if intentional: contra3_replay_test.exe --update)" }
}
