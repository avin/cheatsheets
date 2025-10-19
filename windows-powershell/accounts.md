# Пользователи и группы

## Локальные пользователи

```powershell
Get-LocalUser
New-LocalUser -Name 'deploy' -Password (Read-Host -AsSecureString 'Пароль')
Disable-LocalUser -Name 'guest'
Set-LocalUser -Name 'deploy' -PasswordNeverExpires $true
Remove-LocalUser -Name 'olduser'
```

## Локальные группы

```powershell
Get-LocalGroup
Add-LocalGroupMember -Group 'Administrators' -Member 'CONTOSO\svc-deploy'
Get-LocalGroupMember -Group 'Remote Desktop Users'
Remove-LocalGroupMember -Group 'Users' -Member 'guest'
```

## Active Directory (RSAT)

```powershell
Import-Module ActiveDirectory
Get-ADUser -Filter "Enabled -eq 'True'" -SearchBase 'OU=Developers,DC=contoso,DC=com'
Set-ADAccountPassword -Identity 'jdoe' -Reset -NewPassword (ConvertTo-SecureString 'Temp2024!' -AsPlainText -Force)
Add-ADGroupMember -Identity 'VPN Users' -Members 'jdoe'
```

## Аудит входов

```powershell
Get-WinEvent -FilterHashtable @{
  LogName = 'Security'
  Id      = 4624
  StartTime = (Get-Date).AddDays(-1)
} | Select-Object TimeCreated, @{Name='User';Expression={$_.Properties[5].Value}}, @{Name='LogonType';Expression={$_.Properties[8].Value}}
```

## Права NTFS

```powershell
$acl = Get-Acl C:\Data
$rule = New-Object System.Security.AccessControl.FileSystemAccessRule('CONTOSO\Ops','Modify','ContainerInherit,ObjectInherit','None','Allow')
$acl.AddAccessRule($rule)
Set-Acl -Path C:\Data -AclObject $acl
```
