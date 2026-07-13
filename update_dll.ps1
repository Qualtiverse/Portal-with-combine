param([switch]$watch)

$repo = "SonicEraZoR/Portal-Base"
$gameDir = "G:\SteamLibrary\steamapps\SourceMods\hl2_with_ashpd"
$dllDest = Join-Path $gameDir "bin\server.dll"
$stateFile = Join-Path $PSScriptRoot ".last_dll_id"
$pollInterval = 60

$apiUrl = "https://api.github.com/repos/$repo/releases/latest"
$tagFile = Join-Path $PSScriptRoot ".last_tag"

function Download-And-Deploy {
    Write-Host "$(Get-Date -Format HH:mm:ss) Fetching latest release info..." -ForegroundColor Cyan
    $release = Invoke-RestMethod -Uri $apiUrl -ErrorAction Stop
    $zipUrl = $release.assets[0].browser_download_url
    if (-not $zipUrl) { throw "No hl2_with_ashpd.zip found in release" }
    
    Write-Host "$(Get-Date -Format HH:mm:ss) Downloading $($release.tag_name)..." -ForegroundColor Cyan
    $zipPath = [System.IO.Path]::GetTempFileName() + ".zip"
    Invoke-WebRequest -Uri $zipUrl -OutFile $zipPath -ErrorAction Stop
    
    $extractDir = [System.IO.Path]::GetTempFileName()
    Remove-Item $extractDir -Force -ErrorAction SilentlyContinue
    New-Item -ItemType Directory -Path $extractDir -Force | Out-Null
    Expand-Archive -Path $zipPath -DestinationPath $extractDir -Force
    Remove-Item $zipPath -Force
    
    $dllPath = Join-Path $extractDir "hl2_with_ashpd\bin\server.dll"
    if (-not (Test-Path $dllPath)) { throw "server.dll not found in zip" }
    
    try { [System.IO.File]::Delete($dllDest) } catch {
        Write-Host "$(Get-Date -Format HH:mm:ss) Game is running - close it first!" -ForegroundColor Red
        Remove-Item $extractDir -Recurse -Force
        return
    }
    
    Copy-Item $dllPath $dllDest -Force
    $hash = (Get-FileHash $dllDest -Algorithm SHA256).Hash
    $release.tag_name | Out-File $tagFile -Encoding utf8
    $hash | Out-File $stateFile -Encoding utf8
    Remove-Item $extractDir -Recurse -Force
    
    Write-Host "$(Get-Date -Format HH:mm:ss) Deployed server.dll ($($release.tag_name))" -ForegroundColor Green
    [System.Console]::Beep(800, 200)
    Start-Process powershell -ArgumentList "-NoProfile -WindowStyle Hidden -Command `"Add-Type -AssemblyName System.Windows.Forms; [System.Windows.Forms.MessageBox]::Show('server.dll updated to $($release.tag_name)','DLL Updated')`""
}

function Check-For-Update {
    $currentTag = Get-Content $tagFile -ErrorAction SilentlyContinue
    $release = Invoke-RestMethod -Uri $apiUrl -ErrorAction Stop
    if ($release.tag_name -ne $currentTag) {
        return $release
    }
    return $null
}

if (-not $watch) {
    try { Download-And-Deploy } catch { Write-Host "Error: $_" -ForegroundColor Red; exit 1 }
    exit
}

Write-Host "Watching $repo for new releases (poll every ${pollInterval}s)..." -ForegroundColor Magenta
Write-Host "Close this window to stop." -ForegroundColor Gray
while ($true) {
    try {
        $update = Check-For-Update
        if ($update) { Download-And-Deploy }
    } catch { Write-Host "Check failed: $_" -ForegroundColor Gray }
    Start-Sleep -Seconds $pollInterval
}
