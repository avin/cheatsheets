# WMI и CIM

## Получение данных

```powershell
Get-CimInstance -ClassName Win32_OperatingSystem
Get-CimInstance -Namespace root\CIMV2 -ClassName Win32_Service -Filter "State='Running'" | Select-Object Name,StartMode
```

## Методами CIM

```powershell
$process = Get-CimInstance Win32_Process -Filter "Name='notepad.exe'"
Invoke-CimMethod -InputObject $process -MethodName Terminate
```

## Создание сессий

```powershell
$session = New-CimSession -ComputerName server01 -Credential (Get-Credential)
Get-CimInstance -CimSession $session -ClassName Win32_LogicalDisk
Remove-CimSession $session
```

## Работа с WMI (устаревшее API)

```powershell
Get-WmiObject Win32_BIOS | Select-Object Manufacturer,SerialNumber
```

## Преобразование между API

```powershell
Get-WmiObject Win32_Service | ConvertTo-CimInstance | Export-Clixml services.xml
```
