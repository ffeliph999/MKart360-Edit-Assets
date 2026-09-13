param(
    [switch]$SkipTorch
)

$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'

$Repo = Split-Path -Parent $PSScriptRoot
$Tools = Join-Path $Repo 'tools'
$Portable = Join-Path $Tools 'mingw64'
$PortableHost = Join-Path $Tools '_portable_host'
$DownloadDir = Join-Path $Tools '_downloads'

function Say([string]$Text) { Write-Host "[MK64] $Text" }

function Find-PortableRoot {
    if (Test-Path $Portable) {
        $gcc = Get-ChildItem -Path $Portable -Filter gcc.exe -Recurse -ErrorAction SilentlyContinue |
            Where-Object { $_.Directory.Name -eq 'bin' } | Select-Object -First 1
        if ($gcc) { return $gcc.Directory.Parent.FullName }
    }
    return $null
}

function Get-W64DevKit {
    $root = Find-PortableRoot
    if ($root) { Say "Portable GCC already present: $root"; return $root }

    New-Item -ItemType Directory -Force -Path $DownloadDir,$Portable | Out-Null
    Say 'Downloading the current signed w64devkit x64 toolchain from GitHub...'
    $headers = @{ 'User-Agent' = 'MK64-Xbox360-AssetSetup' }
    $release = Invoke-RestMethod -Headers $headers -Uri 'https://api.github.com/repos/skeeto/w64devkit/releases/latest'
    $asset = $release.assets | Where-Object { $_.name -match '^w64devkit-x64-.*\.7z\.exe$' } | Select-Object -First 1
    if (-not $asset) { throw 'Could not find the x64 w64devkit release asset.' }
    $archive = Join-Path $DownloadDir $asset.name
    if (-not (Test-Path $archive)) { Invoke-WebRequest -Headers $headers -Uri $asset.browser_download_url -OutFile $archive -UseBasicParsing }
    Say "Extracting $($asset.name)..."
    $proc = Start-Process -FilePath $archive -ArgumentList @('-y', "-o$Portable") -Wait -PassThru
    if ($proc.ExitCode -ne 0) { throw "w64devkit extractor exited with code $($proc.ExitCode)." }
    $root = Find-PortableRoot
    if (-not $root) { throw 'w64devkit extracted, but gcc.exe could not be located.' }
    return $root
}

function Ensure-PortableCMake {
    $existing = Get-Command cmake.exe -ErrorAction SilentlyContinue
    if ($existing) { return $existing.Source }
    $dest = Join-Path $PortableHost 'cmake'
    $found = Get-ChildItem -Path $dest -Filter cmake.exe -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($found) { return $found.FullName }
    New-Item -ItemType Directory -Force -Path $DownloadDir,$dest | Out-Null
    Say 'Downloading portable CMake...'
    $h=@{'User-Agent'='MK64-Xbox360-AssetSetup'}
    $r=Invoke-RestMethod -Headers $h -Uri 'https://api.github.com/repos/Kitware/CMake/releases/latest'
    $a=$r.assets | Where-Object { $_.name -match '^cmake-.*-windows-x86_64\.zip$' } | Select-Object -First 1
    if(-not $a){ throw 'Could not locate portable CMake package.' }
    $z=Join-Path $DownloadDir $a.name
    Invoke-WebRequest -Headers $h -Uri $a.browser_download_url -OutFile $z -UseBasicParsing
    if(Test-Path $dest){Remove-Item $dest -Recurse -Force}; New-Item -ItemType Directory -Force -Path $dest|Out-Null
    Expand-Archive $z $dest -Force
    $found=Get-ChildItem -Path $dest -Filter cmake.exe -Recurse | Select-Object -First 1
    if(-not $found){throw 'CMake download extracted but cmake.exe was not found.'}
    return $found.FullName
}

function Ensure-PortableNinja {
    $existing = Get-Command ninja.exe -ErrorAction SilentlyContinue
    if ($existing) { return $existing.Source }
    $dest=Join-Path $PortableHost 'ninja'; $exe=Join-Path $dest 'ninja.exe'
    if(Test-Path $exe){return $exe}
    New-Item -ItemType Directory -Force -Path $DownloadDir,$dest|Out-Null
    Say 'Downloading portable Ninja...'
    $h=@{'User-Agent'='MK64-Xbox360-AssetSetup'}
    $r=Invoke-RestMethod -Headers $h -Uri 'https://api.github.com/repos/ninja-build/ninja/releases/latest'
    $a=$r.assets|Where-Object{$_.name -eq 'ninja-win.zip'}|Select-Object -First 1
    if(-not $a){throw 'Could not locate ninja-win.zip.'}
    $z=Join-Path $DownloadDir 'ninja-win.zip'; Invoke-WebRequest -Headers $h -Uri $a.browser_download_url -OutFile $z -UseBasicParsing
    Expand-Archive $z $dest -Force
    if(-not(Test-Path $exe)){throw 'Ninja download extracted but ninja.exe was not found.'}
    return $exe
}

function Ensure-PortableGit {
    $existing = Get-Command git.exe -ErrorAction SilentlyContinue
    if ($existing) { return $existing.Source }
    $dest=Join-Path $PortableHost 'git'
    $found=Get-ChildItem -Path $dest -Filter git.exe -Recurse -ErrorAction SilentlyContinue | Where-Object{$_.FullName -match '\\cmd\\git\.exe$'} | Select-Object -First 1
    if($found){return $found.FullName}
    New-Item -ItemType Directory -Force -Path $DownloadDir,$dest|Out-Null
    Say 'Downloading portable Git (needed by Torch dependencies)...'
    $h=@{'User-Agent'='MK64-Xbox360-AssetSetup'}
    $r=Invoke-RestMethod -Headers $h -Uri 'https://api.github.com/repos/git-for-windows/git/releases/latest'
    $a=$r.assets|Where-Object{$_.name -match '^PortableGit-.*-64-bit\.7z\.exe$'}|Select-Object -First 1
    if(-not $a){throw 'Could not locate PortableGit 64-bit package.'}
    $pkg=Join-Path $DownloadDir $a.name; Invoke-WebRequest -Headers $h -Uri $a.browser_download_url -OutFile $pkg -UseBasicParsing
    if(Test-Path $dest){Remove-Item $dest -Recurse -Force}; New-Item -ItemType Directory -Force -Path $dest|Out-Null
    $proc=Start-Process -FilePath $pkg -ArgumentList @('-y', "-o$dest") -Wait -PassThru
    if($proc.ExitCode -ne 0){throw "PortableGit extractor exited with code $($proc.ExitCode)."}
    $found=Get-ChildItem -Path $dest -Filter git.exe -Recurse | Where-Object{$_.FullName -match '\\cmd\\git\.exe$'} | Select-Object -First 1
    if(-not $found){throw 'PortableGit extracted but git.exe was not found.'}
    return $found.FullName
}

function Run-Make([string[]]$MakeArgs) {
    $make = Join-Path $script:W64Root 'bin\make.exe'
    if (-not (Test-Path $make)) { throw "make.exe not found at $make" }
    & $make @MakeArgs
    if ($LASTEXITCODE -ne 0) { throw "make failed with exit code $LASTEXITCODE" }
}

function Ensure-TorchSource {
    $torch=Join-Path $Tools 'torch'
    if((Test-Path (Join-Path $torch 'CMakeLists.txt')) -and (Test-Path (Join-Path $torch 'src'))){ Say 'Torch source is already present.'; return }
    Say 'Cloning Torch and its submodules...'
    if(Test-Path $torch){Remove-Item $torch -Recurse -Force}
    & $script:GitExe clone --recursive --depth 1 https://github.com/HarbourMasters/Torch.git $torch
    if($LASTEXITCODE -ne 0){throw "git clone Torch failed with exit code $LASTEXITCODE"}
    if(-not(Test-Path (Join-Path $torch 'CMakeLists.txt'))){throw 'Torch source setup failed.'}
}


function Patch-TorchMinGWZlib {
    $cmakeFile = Join-Path (Join-Path $Tools 'torch') 'CMakeLists.txt'
    if (-not (Test-Path $cmakeFile)) { throw 'Torch CMakeLists.txt was not found.' }

    $text = Get-Content -Raw -Path $cmakeFile

    # Torch currently asks MinGW for a system ZLIB in its standalone block,
    # even though the same CMakeLists later FetchContent-builds zlib itself.
    # w64devkit intentionally does not ship the zlib development package, so
    # remove only that premature standalone ZLIB lookup. The later zlibstatic
    # FetchContent block remains intact and supplies the actual library.
    $pattern = '(?s)if\s*\(MSVC\)\s*\r?\n\s*FetchContent_Declare\(\s*\r?\n\s*zlib\s*\r?\n\s*GIT_REPOSITORY\s+"https://github\.com/madler/zlib\.git".*?\r?\n\s*endif\(\)'
    if ($text -match $pattern) {
        Say 'Patching Torch CMake so MinGW uses Torch''s bundled zlib build...'
        $replacement = @'
# MK64 Xbox 360 setup: skip Torch's premature standalone/system ZLIB lookup.
# Torch's later "Link zlib" section FetchContent-builds zlibstatic for Windows.
'@
        $text = [regex]::Replace($text, $pattern, $replacement, 1)
        Set-Content -Path $cmakeFile -Value $text -Encoding UTF8
    } elseif ($text -match 'MK64 Xbox 360 setup: skip Torch''s premature standalone/system ZLIB lookup') {
        Say 'Torch MinGW zlib patch is already applied.'
    } else {
        throw 'Torch CMake zlib section changed upstream; refusing to patch the wrong block.'
    }
}

function Build-Torch {
    $torch=Join-Path $Tools 'torch'; $build=Join-Path $torch 'cmake-build-release'
    $existing=Get-ChildItem -Path $build -Filter torch.exe -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
    if($existing){
        Say ("Reusing existing Torch executable: " + $existing.FullName)
        return [string]$existing.FullName
    }
    if(Test-Path $build){Remove-Item $build -Recurse -Force}
    New-Item -ItemType Directory -Force -Path $build|Out-Null
    Say 'Configuring Torch with portable GCC + Ninja (MK64 support only)...'
    & $script:CMakeExe -S $torch -B $build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_MK64=ON -DBUILD_SM64=OFF -DBUILD_SF64=OFF -DBUILD_PM64=OFF -DBUILD_FZERO=OFF -DBUILD_BK64=OFF -DBUILD_MARIO_ARTIST=OFF -DBUILD_NAUDIO=OFF -DBUILD_OOT=OFF -DBUILD_UI=OFF -DBUILD_STORMLIB=OFF | Out-Host
    if($LASTEXITCODE -ne 0){throw "Torch CMake configure failed with exit code $LASTEXITCODE"}
    Say 'Building Torch...'
    & $script:CMakeExe --build $build --parallel | Out-Host
    if($LASTEXITCODE -ne 0){throw "Torch build failed with exit code $LASTEXITCODE"}
    $exe=Get-ChildItem -Path $build -Filter torch.exe -Recurse -ErrorAction SilentlyContinue|Select-Object -First 1
    if(-not $exe){throw 'Torch build completed but torch.exe was not found.'}
    return $exe.FullName
}

$script:W64Root=Get-W64DevKit
$env:PATH=(Join-Path $script:W64Root 'bin')+';'+$env:PATH

Say 'Building only the three native helpers extract_assets.py actually needs...'
Push-Location $Repo
try { Run-Make @('-C','tools','n64graphics','mio0','tkmk00') } finally { Pop-Location }
foreach($base in @('n64graphics','mio0','tkmk00')){
    $candidates = @(
        (Join-Path $Tools ($base + '.exe'))
        (Join-Path $Tools $base)
    )
    $candidate = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
    if(-not $candidate){throw "Expected helper was not built: tools\\$base(.exe)"}
    Say ("Found helper: " + $candidate)
}
Say 'Legacy extraction helper tools are ready.'

if(-not $SkipTorch){
    $script:CMakeExe=Ensure-PortableCMake
    $script:NinjaExe=Ensure-PortableNinja
    $script:GitExe=Ensure-PortableGit
    $env:PATH=(Split-Path $script:CMakeExe -Parent)+';'+(Split-Path $script:NinjaExe -Parent)+';'+(Split-Path $script:GitExe -Parent)+';'+$env:PATH
    Ensure-TorchSource
    Patch-TorchMinGWZlib
    $torchExe=Build-Torch
    Set-Content -Path (Join-Path $Tools 'torch_exe_path.txt') -Value $torchExe -Encoding ASCII
    Say "Torch ready: $torchExe"
}
Say 'Windows asset tools setup completed successfully.'
