$repo = "Qualtiverse/Portal-with-combine"
$gameDir = "G:\GDrive\Source game\Portal-Base-Release\hl2_with_ashpd"
$dllDest = Join-Path $gameDir "bin\server.dll"

Write-Host "Downloading latest server.dll from $repo..." -ForegroundColor Cyan

# Get latest release from GitHub API
$release = Invoke-RestMethod -Uri "https://api.github.com/repos/$repo/releases/latest" -ErrorAction Stop
$asset = $release.assets | Where-Object { $_.name -eq "server.dll" }

if (-not $asset) {
    Write-Host "No server.dll found in latest release. Has the CI build completed?" -ForegroundColor Red
    exit 1
}

$tempFile = [System.IO.Path]::GetTempFileName()
Write-Host "Downloading from $($asset.browser_download_url) ..." -ForegroundColor Cyan
Invoke-WebRequest -Uri $asset.browser_download_url -OutFile $tempFile

# Check if destination is writable (game might be running)
try {
    [System.IO.File]::Delete($dllDest)
} catch {
    Write-Host "Cannot update - game is running? Close it first." -ForegroundColor Red
    Remove-Item $tempFile -Force
    exit 1
}

Copy-Item $tempFile $dllDest -Force
Remove-Item $tempFile -Force

Write-Host "Done! Deployed server.dll to $dllDest" -ForegroundColor Green
Write-Host "Commit: $($release.tag_name) - $($release.name)" -ForegroundColor Gray
