#Requires -Version 5.1
$ErrorActionPreference = "Stop"

$RootDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BuildDir = Join-Path $RootDir "build"
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

$Gcc = "Z:\REA\tools\w64devkit\bin\gcc.exe"
$Ar  = "Z:\REA\tools\w64devkit\bin\ar.exe"

if (-not (Test-Path $Gcc)) {
    $Gcc = "gcc"
    $Ar  = "ar"
}

$Sources = @(
    "snes_runtime",
    "snes_ppu",
    "snes_apu",
    "snes_movie",
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
    & $Gcc -std=c11 -O2 -Wall -Wextra -Werror -I"$RootDir\include" $AppSrc $GlSrc $LibPath -lopengl32 -lgdi32 -lwinmm -o $AppExe
    if ($LASTEXITCODE -ne 0) { throw "Desktop app build failed" }
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
