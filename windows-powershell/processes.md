# Процессы и службы

## Процессы

```powershell
Get-Process | Sort-Object CPU -Descending | Select-Object -First 5
Get-Process -Name powershell
Stop-Process -Name notepad -Force
Start-Process -FilePath 'C:\Program Files\Git\bin\bash.exe' -ArgumentList '--login'
# Запуск с захватом вывода
Start-Process pwsh -ArgumentList '-File script.ps1' -NoNewWindow -Wait -PassThru
```

## Производительность

```powershell
Measure-Command { script.ps1 }                 # длительность выполнения
Get-Counter '\Processor(_Total)\% Processor Time' -SampleInterval 1 -MaxSamples 5
Get-Process | Select-Object Name,CPU,PM,WS
```

## Службы Windows

```powershell
Get-Service | Where-Object Status -eq 'Stopped'
Restart-Service -Name Spooler
Set-Service -Name W32Time -StartupType Automatic
Get-Service -Name WinRM | Select-Object Status,StartType,DisplayName
```

## Планировщик задач

```powershell
Get-ScheduledTask | Where-Object TaskName -like '*Backup*'
Register-ScheduledTask -TaskName 'CleanupTemp' `
  -Trigger (New-ScheduledTaskTrigger -Daily -At 3am) `
  -Action (New-ScheduledTaskAction -Execute 'powershell.exe' -Argument '-File C:\Scripts\cleanup.ps1') `
  -Principal (New-ScheduledTaskPrincipal -UserId 'SYSTEM' -LogonType ServiceAccount -RunLevel Highest)
Unregister-ScheduledTask -TaskName 'CleanupTemp' -Confirm:$false
```

## Журналы событий

```powershell
Get-WinEvent -LogName Application -MaxEvents 20
Get-WinEvent -FilterHashtable @{LogName='System';ID=16} | Format-Table TimeCreated, ProviderName, Message -AutoSize
wevtutil qe Application /q:"*[System[(Level=2)]]" /f:text | Select-String 'Error'
```
