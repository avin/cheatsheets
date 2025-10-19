# Файлы и каталоги

## Создание и копирование

```powershell
New-Item -ItemType Directory -Path . -Name 'Reports'
New-Item -ItemType File -Path .\Reports\summary.txt
Copy-Item -Path .\config.json -Destination .\backup\config.json -Force
Copy-Item -Path .\Templates -Destination .\Projects -Recurse
# Дубликат списка с сохранением структуры
Get-ChildItem -Path .\Source -Recurse | Copy-Item -Destination { $_.FullName -replace 'Source','Target' }
```

## Перемещение и переименование

```powershell
Move-Item -Path .\draft.docx -Destination .\Archive\draft-$(Get-Date -Format yyyyMMdd).docx
Rename-Item -Path .\*.log -NewName { $_.BaseName + '.bak' }
Move-Item -Path (Get-ChildItem *.tmp) -Destination .\Temp\
```

## Удаление

```powershell
Remove-Item -Path .\old.txt
Remove-Item -Path .\cache -Recurse -Force
Remove-Item -Path .\*.log -WhatIf   # показать, что будет удалено
Clear-RecycleBin -Force             # очистить корзину
```

## Атрибуты и временные метки

```powershell
(Get-Item .\report.pdf).Attributes
Set-ItemProperty .\report.pdf -Name IsReadOnly -Value $true
(Get-Item .\report.pdf).CreationTime = [datetime]::Now
```

## Сравнение и хеши

```powershell
Compare-Object (Get-ChildItem .\Prod\*.json) (Get-ChildItem .\Stage\*.json) -Property Name,Length
Get-FileHash .\image.iso -Algorithm SHA256
```

## Архивация

```powershell
Compress-Archive -Path .\Logs\* -DestinationPath .\backup\logs.zip
Expand-Archive -Path .\backup\logs.zip -DestinationPath .\Restored -Force
```
