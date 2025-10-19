# Скрипты и автоматизация

## Основы скриптов

```powershell
#!/usr/bin/env pwsh
[CmdletBinding()]
param(
  [Parameter(Mandatory)]
  [string]$Path,
  [switch]$Force
)

Write-Verbose "Путь: $Path"
if (-not (Test-Path $Path)) {
  New-Item -ItemType Directory -Path $Path | Out-Null
}
```

## Функции и возвращаемые значения

```powershell
function Test-ConnectionSummary {
  param([string[]]$ComputerName)
  foreach ($name in $ComputerName) {
    $result = Test-NetConnection -ComputerName $name -WarningAction SilentlyContinue
    [pscustomobject]@{
      Computer = $name
      PingSucceeded = $result.PingSucceeded
      TcpTestSucceeded = $result.TcpTestSucceeded
    }
  }
}
```

## Обработка ошибок

```powershell
try {
  Invoke-WebRequest $url -OutFile $file -ErrorAction Stop
} catch {
  Write-Error "Не удалось скачать $url: $($_.Exception.Message)"
}

$ErrorActionPreference = 'Stop'
```

## Транскрипты и логирование

```powershell
Start-Transcript -Path "$env:TEMP\session.log" -Append
Write-Output "Скрипт запущен"
Stop-Transcript
```

## Профили и dot-sourcing

```powershell
# Загрузка вспомогательных функций при запуске
. "$PSScriptRoot\lib\git-utils.ps1"
```

## Планировщик задач и автоматизация

```powershell
$trigger = New-ScheduledTaskTrigger -Daily -At 02:00
$action = New-ScheduledTaskAction -Execute 'pwsh.exe' -Argument '-File C:\Scripts\backup.ps1'
Register-ScheduledTask -TaskName 'NightlyBackup' -Trigger $trigger -Action $action -RunLevel Highest -User 'SYSTEM'
```
