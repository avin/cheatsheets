# Текст, CSV и JSON

## Чтение и запись

```powershell
Get-Content .\notes.txt
Get-Content .\logs.txt -Tail 20 -Wait            # аналог tail -f
Set-Content .\notes.txt -Value 'Новый текст'
Add-Content .\notes.txt -Value 'Ещё одна строка'
Out-File .\report.txt -Encoding utf8
```

## Поиск и замена

```powershell
Select-String -Path .\logs\*.log -Pattern 'ERROR'
(Get-Content .\config.ini) -replace 'Port=80','Port=8080' | Set-Content .\config.ini
# Регулярные выражения с именованными группами
'10.1.2.3 - - [12/Mar/2024:12:00:00]' -match '^(?<ip>\d+\.\d+\.\d+\.\d+)' | Out-Null
$Matches.ip
```

## Работа с CSV

```powershell
Import-Csv .\users.csv | Where-Object {$_.Active -eq 'True'}
Import-Csv .\metrics.csv | Group-Object Category | Export-Csv grouped.csv -NoTypeInformation
$data = @(
  [pscustomobject]@{Name='Alice';Score=95}
  [pscustomobject]@{Name='Bob';Score=87}
)
$data | Export-Csv scores.csv -NoTypeInformation
```

## JSON

```powershell
$json = Get-Content .\settings.json | ConvertFrom-Json
$json.FeatureFlags.DarkMode = $true
$json | ConvertTo-Json -Depth 5 | Set-Content .\settings.json
```

## XML

```powershell
[xml]$xml = Get-Content .\app.config
$xml.configuration.appSettings.add | Where-Object {$_.key -eq 'ApiUrl'} | ForEach-Object {
  $_.value = 'https://api.new.local'
}
$xml.Save('.\app.config')
```

## Here-strings и шаблоны

```powershell
$template = @"
Имя: {0}
Дата: {1:dd.MM.yyyy}
"@
[string]::Format($template,'Отчёт',[datetime]::Now)
```
