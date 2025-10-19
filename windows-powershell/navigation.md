# Навигация и файловая система

## Текущее расположение и переходы

```powershell
Get-Location                        # аналог pwd
Set-Location C:\Projects            # перейти в каталог
Push-Location D:\Logs               # временно перейти
Pop-Location                        # вернуться назад
```

```powershell
Set-Alias ll Get-ChildItem
Set-Alias .. Set-Location ..
```

## Просмотр содержимого

```powershell
Get-ChildItem                      # список файлов
Get-ChildItem -Force               # включая скрытые/system
Get-ChildItem -Path . -File        # только файлы
Get-ChildItem -Path C:\ -Recurse -Depth 2 | Format-Table Name,Length
```

```powershell
Get-ChildItem -Path HKLM:\Software # провайдер реестра
Get-ChildItem Env:                 # переменные окружения как элементы
```

## Работа с путями

```powershell
Join-Path $env:USERPROFILE 'Documents\notes.txt'
Resolve-Path .\script.ps1          # абсолютный путь
Split-Path C:\Logs\App\error.log -Parent
Test-Path \\server\share\file.txt
```

## PSDrive и сетевые ресурсы

```powershell
Get-PSDrive                        # все подключенные провайдеры
New-PSDrive -Name Docs -PSProvider FileSystem -Root '\\server\docs' -Persist
Remove-PSDrive Docs
```
