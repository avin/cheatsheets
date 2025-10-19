# Модули и пакеты

## Управление модулями PowerShell

```powershell
Get-Module                     # загруженные модули
Get-Module -ListAvailable      # все доступные
Install-Module -Name Az -Scope CurrentUser
Update-Module -Name Az
Uninstall-Module -Name Az -AllVersions
```

## Репозитории PowerShellGet

```powershell
Get-PSRepository
Register-PSRepository -Name PrivateRepo -SourceLocation 'https://nuget.contoso.com/api/v2'
Set-PSRepository -Name PSGallery -InstallationPolicy Trusted
Unregister-PSRepository -Name PrivateRepo
```

## PSResourceGet (PowerShell 7.4+)

```powershell
Register-PSResourceRepository -Name Private -Uri 'https://nuget.contoso.com/v3/index.json'
Find-PSResource -Name Pester
Install-PSResource Pester -Repository PSGallery
```

## winget

```powershell
winget search PowerShell
winget install --id Git.Git -e
winget upgrade --all
winget export --output packages.json
```

## Chocolatey

```powershell
choco search vscode
choco install googlechrome -y
choco upgrade all -y
choco list --local-only
```
