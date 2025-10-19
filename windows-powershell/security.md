# Безопасность и учетные данные

## Execution Policy

```powershell
Get-ExecutionPolicy
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
Set-ExecutionPolicy Bypass -Scope Process -Force
```

## Учётные данные

```powershell
$cred = Get-Credential
Invoke-Command -ComputerName server01 -Credential $cred -ScriptBlock { hostname }

# Сохранить зашифрованные креды для текущего пользователя
$cred | Export-Clixml -Path $env:USERPROFILE\cred.xml
Import-Clixml $env:USERPROFILE\cred.xml
```

## SecretManagement

```powershell
Install-Module Microsoft.PowerShell.SecretManagement -Scope CurrentUser
Register-SecretVault -Name MyVault -ModuleName SecretManagement.KeePass -VaultParameters @{ Path='C:\Secrets\vault.kdbx' }
Set-Secret -Name ApiKey -Secret '12345'
Get-Secret ApiKey
```

## Сертификаты

```powershell
Get-ChildItem Cert:\CurrentUser\My
New-SelfSignedCertificate -DnsName 'dev.contoso.com' -CertStoreLocation Cert:\LocalMachine\My
Export-PfxCertificate -Cert (Get-ChildItem Cert:\CurrentUser\My\ABC123*) -FilePath .\cert.pfx -Password (ConvertTo-SecureString 'StrongPass1!' -AsPlainText -Force)
```

## Подпись скриптов

```powershell
Set-AuthenticodeSignature -FilePath .\Deploy.ps1 -Certificate (Get-ChildItem Cert:\CurrentUser\My\ABC123*)
Get-AuthenticodeSignature .\Deploy.ps1
```
