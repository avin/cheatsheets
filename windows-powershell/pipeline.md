# Конвейер и объекты

## Базовая работа с конвейером

```powershell
Get-Process | Where-Object {$_.CPU -gt 100} | Select-Object Name,CPU
Get-Service | Sort-Object Status,DisplayName
```

## Where-Object и ForEach-Object

```powershell
Get-ChildItem *.log | Where-Object Length -gt 1MB
Get-Content urls.txt | ForEach-Object {
  Invoke-WebRequest $_ -OutFile ".\downloads\$($_.Split('/')[-1])"
}
```

## Select-Object и расчётные свойства

```powershell
Get-Process | Select-Object Name,Id,
  @{Name='CPU(s)';Expression={[math]::Round($_.CPU,2)}},
  @{Name='WS MB';Expression={[math]::Round($_.WorkingSet/1MB,1)}}
```

## Группировка и агрегаты

```powershell
Get-EventLog -LogName Application -Newest 200 |
  Group-Object Source | Sort-Object Count -Descending |
  Select-Object -First 5

Get-ChildItem *.csv | Measure-Object Length -Sum -Average
```

## Форматирование вывода

```powershell
Get-Service | Format-Table Status,Name,DisplayName -AutoSize
Get-ChildItem | Format-List *
Get-Process | Format-Wide -Column 4
```

## Экспорт и импорт

```powershell
Get-Process | Export-Csv processes.csv -NoTypeInformation
Import-Csv processes.csv | ConvertTo-Html | Out-File processes.html

Get-Service | ConvertTo-Json | Out-File services.json
Get-Content services.json | ConvertFrom-Json
```
