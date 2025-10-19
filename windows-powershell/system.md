# Системная информация

## Общие сведения

```powershell
Get-ComputerInfo | Select-Object CsName,OsName,OsVersion,WindowsProductName
Get-CimInstance Win32_OperatingSystem | Select-Object LastBootUpTime
systeminfo | Select-String 'System Boot Time'
```

## Переменные окружения

```powershell
Get-ChildItem Env: | Sort-Object Name
$env:PATH = "$env:PATH;C:\Tools"
setx HTTP_PROXY 'http://proxy.local:8080'
```

## Реестр

```powershell
Get-ItemProperty -Path 'HKCU:\Software\Contoso\App' -Name Setting
New-Item -Path 'HKCU:\Software\Contoso\App' -Force
New-ItemProperty -Path 'HKCU:\Software\Contoso\App' -Name Enabled -PropertyType DWord -Value 1 -Force
Remove-Item -Path 'HKCU:\Software\Contoso' -Recurse
```

## Диски и тома

```powershell
Get-Volume | Select-Object DriveLetter,FileSystem,SizeRemaining
Get-Disk | Select-Object Number,FriendlyName,OperationalStatus
Get-Partition | Where-Object DriveLetter -eq 'C'
```

## PowerShell-профили

```powershell
$PROFILE                      # путь к профилю
Test-Path $PROFILE.CurrentUserAllHosts
New-Item -ItemType File -Path $PROFILE.CurrentUserCurrentHost -Force
# Пример инициализации
Add-Content $PROFILE.CurrentUserCurrentHost @"
Set-PSReadLineOption -HistorySearchCursorMovesToEnd
Import-Module posh-git
"@
```
