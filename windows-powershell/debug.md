# Отладка и тестирование

## Встроенная отладка

```powershell
Set-PSDebug -Trace 1                 # трассировка выполнения
Set-PSDebug -Strict                  # строгая проверка переменных
Set-PSBreakpoint -Script .\script.ps1 -Line 12
Get-PSBreakpoint
Remove-PSBreakpoint -Id 0
```

## Pester

```powershell
Install-Module Pester -Scope CurrentUser
Invoke-Pester .\Tests

Describe 'Get-Version' {
  It 'Возвращает не пустую строку' {
    (Get-Version).Length | Should -BeGreaterThan 0
  }
}
```

## Трассировка и логирование

```powershell
Start-Transcript -Path "$env:TEMP\transcript.txt"
Trace-Command -Name ParameterBinding -Expression { Get-Process Notepad } -PSHost
Stop-Transcript
```

## Анализ ошибок

```powershell
$Error[0]
$Error[0].Exception.StackTrace
$PSItem.Exception.InnerException | Format-List *
```

## Работа с заданиями (Jobs)

```powershell
$job = Start-Job -ScriptBlock { Start-Sleep 5; Get-Date }
Receive-Job -Job $job -Wait
Get-Job | Remove-Job
```
