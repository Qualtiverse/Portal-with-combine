$mapsrc = "G:\GDrive\Source game\combine_aperture_mod\mapsrc"
$backupDir = Join-Path $mapsrc "autobackup"
New-Item -ItemType Directory -Path $backupDir -Force | Out-Null

Write-Host "VMF autobackup running. Backs up every 30s when changes detected."
Write-Host "Close this window to stop."

$lastSaves = @{}

while ($true)
{
    $vmfs = Get-ChildItem -Path $mapsrc -Filter "*.vmf" -ErrorAction SilentlyContinue
    foreach ($vmf in $vmfs)
    {
        $key = $vmf.FullName
        $lastTime = $lastSaves[$key]
        if (-not $lastTime -or $vmf.LastWriteTime -gt $lastTime)
        {
            $stamp = Get-Date -Format "yyyyMMdd_HHmmss"
            $backupFile = Join-Path $backupDir ("{0}_{1}.vmf" -f $vmf.BaseName, $stamp)
            Copy-Item -Path $vmf.FullName -Destination $backupFile -Force
            $lastSaves[$key] = $vmf.LastWriteTime
            Write-Host ("[{0}] Backed up {1}" -f (Get-Date -Format "HH:mm:ss"), $vmf.Name)
        }
    }
    Start-Sleep -Seconds 30
}
