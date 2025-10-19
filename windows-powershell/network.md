# Сеть и удалённый доступ

## Сетевые адаптеры и диагностика

```powershell
Get-NetAdapter | Select-Object Name,Status,LinkSpeed
Get-NetIPAddress | Where-Object {$_.AddressFamily -eq 'IPv4'}
Test-NetConnection -ComputerName example.com -Port 443
Resolve-DnsName -Name contoso.com
```

## HTTP и REST

```powershell
Invoke-WebRequest 'https://httpbin.org/get' | Select-Object -ExpandProperty Content
Invoke-RestMethod 'https://api.github.com/repos/PowerShell/PowerShell' | Select-Object name,stargazers_count
Invoke-WebRequest -Uri $url -OutFile (Split-Path $url -Leaf)
```

## SMB и сетевые ресурсы

```powershell
New-SmbShare -Name Deploy -Path 'C:\Deploy' -FullAccess 'CONTOSO\DevOps'
Get-SmbShare
Revoke-SmbShareAccess -Name Deploy -AccountName 'CONTOSO\DevOps'
```

## PowerShell Remoting (WinRM)

```powershell
Enable-PSRemoting -Force
Enter-PSSession -ComputerName server01 -Credential (Get-Credential)
Invoke-Command -ComputerName server01 -ScriptBlock { Get-Service WinRM }
$session = New-PSSession -ComputerName server01,server02
Invoke-Command -Session $session -ScriptBlock { Get-Process powershell }
Remove-PSSession $session
```

## Копирование файлов

```powershell
$session = New-PSSession -ComputerName server01
Copy-Item -Path .\deploy.zip -Destination 'C:\Temp\' -ToSession $session
Copy-Item -Path 'C:\Logs\*.log' -Destination .\logs\server01\ -FromSession $session
```

## SSH

```powershell
ssh user@linuxhost
Enter-PSSession -HostName linuxhost -UserName user
ssh-keygen -t ed25519 -C 'workstation'
```
