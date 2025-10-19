# Шпаргалка по PowerShell

Рабочие сниппеты и команды для администрирования Windows и автоматизации через PowerShell.

### 🧭 [Навигация и файловая система](./navigation.md)

* 📌  **Провайдеры файловой системы** (`Get-Location`, `Set-Location`, алиасы)
* 📌  **Просмотр содержимого** (`Get-ChildItem`, фильтры, рекурсия)
* 📌  **Работа с путями** (`Join-Path`, `Resolve-Path`)
* 📌  **PSDrives и сетевые ресурсы**

***

### 📁 [Файлы и каталоги](./files.md)

* 📌  **Создание, копирование, перемещение** (`New-Item`, `Copy-Item`, `Move-Item`)
* 📌  **Удаление и корзина** (`Remove-Item`, `Clear-RecycleBin`)
* 📌  **Атрибуты и временные метки**
* 📌  **Сравнение каталогов (`Compare-Object`)**
* 📌  **Zip-архивация (`Compress-Archive`, `Expand-Archive`)**

***

### 🔀 [Конвейер и объекты](./pipeline.md)

* 📌  **Конвейер объектов** (`|`, `Select-Object`, `Where-Object`)
* 📌  **Сортировка и группировка** (`Sort-Object`, `Group-Object`, `Measure-Object`)
* 📌  **Форматирование вывода** (`Format-Table`, `Format-List`, `Format-Wide`)
* 📌  **Экспорт и импорт (`Export-Csv`, `ConvertTo-Json`)**
* 📌  **Расширенные выражения (`ForEach-Object`, скриптовые блоки)**

***

### ✂️ [Текст, CSV и JSON](./text.md)

* 📌  **Чтение и запись текста** (`Get-Content`, `Set-Content`, `Out-File`)
* 📌  **Построчная обработка** (`Select-String`, `-replace`)
* 📌  **Работа с CSV** (`Import-Csv`, `Export-Csv`, `ConvertFrom-Csv`)
* 📌  **JSON и объекты** (`ConvertFrom-Json`, `ConvertTo-Json`)
* 📌  **Шаблоны и here-strings**

***

### ⚙️ [Процессы и службы](./processes.md)

* 📌  **Управление процессами** (`Get-Process`, `Stop-Process`, `Start-Process`)
* 📌  **Отслеживание ресурсов (`Get-Counter`, `Measure-Command`)**
* 📌  **Службы и их состояние** (`Get-Service`, `Restart-Service`)
* 📌  **Автозагрузка и задачи (`Get-ScheduledTask`, `Register-ScheduledTask`)**
* 📌  **Журналы событий (`Get-WinEvent`)**

***

### 🖥️ [Системная информация](./system.md)

* 📌  **Сведения о системе** (`Get-ComputerInfo`, `systeminfo`)
* 📌  **Работа с реестром** (`Get-ItemProperty`, `New-ItemProperty`)
* 📌  **Управление окружением** (`Get-ChildItem Env:`, `setx`)
* 📌  **Управление дисками (`Get-Volume`, `Get-Disk`)**
* 📌  **PowerShell-профили и настройка среды**

***

### 🌐 [Сеть и удалённый доступ](./network.md)

* 📌  **IP-конфигурация и адаптеры** (`Get-NetIPAddress`, `Test-NetConnection`)
* 📌  **Запросы HTTP и REST** (`Invoke-WebRequest`, `Invoke-RestMethod`)
* 📌  **Копирование по сети (`Copy-Item -ToSession`, `New-SmbShare`)**
* 📌  **PowerShell Remoting (WinRM)** (`Enter-PSSession`, `Invoke-Command`)
* 📌  **SSH и подсистемы (OpenSSH)**

***

### 📦 [Модули и пакеты](./packages.md)

* 📌  **Управление модулями** (`Get-Module`, `Install-Module`, `Update-Module`)
* 📌  **Репозитории PowerShellGet (`Get-PSRepository`, `Register-PSRepository`)**
* 📌  **winget / Chocolatey** (`winget install`, `choco upgrade`)
* 📌  **NuGet-провайдер и хранилища (`Register-PSResourceRepository`)**

***

### 🤖 [Скрипты и автоматизация](./scripting.md)

* 📌  **Скриптовые блоки и функции** (`function`, `param`, `return`)
* 📌  **Обработка аргументов** (`[CmdletBinding()]`, `Param()`)
* 📌  **Поток ошибок и транскрипты** (`Try/Catch`, `Start-Transcript`)
* 📌  **Модули и dot-sourcing**
* 📌  **Планирование задач (`Register-ScheduledTask`, `schtasks.exe`)**

***

### 🧪 [Отладка и тестирование](./debug.md)

* 📌  **Пошаговая отладка** (`Set-PSDebug`, `Debug-Job`)
* 📌  **Точки останова (`Set-PSBreakpoint`)**
* 📌  **Unit-тесты (`Pester`)**
* 📌  **Логи и транскрипты** (`Start-Transcript`, `Trace-Command`)
* 📌  **Диагностика ошибок (`$Error`, `$PSItem.Exception`)**

***

### 🔒 [Безопасность и учетные данные](./security.md)

* 📌  **Execution Policy** (`Get-ExecutionPolicy`, `Set-ExecutionPolicy`)
* 📌  **Учётные данные** (`Get-Credential`, `Export-Clixml`)
* 📌  **Защита секретов (`SecretManagement`, `DPAPI`)**
* 📌  **Сертификаты** (`Get-ChildItem Cert:`, `New-SelfSignedCertificate`)
* 📌  **Подписывание скриптов**

***

### 📡 [WMI и CIM](./wmi.md)

* 📌  **Запросы CIM** (`Get-CimInstance`, `Invoke-CimMethod`)
* 📌  **Фильтрация и проекции (`-Filter`, `Select-Object`)**
* 📌  **Создание и удаление объектов**
* 📌  **Сессии и удалённый доступ (`New-CimSession`)**
* 📌  **Переход от WMI к CIM (`Get-WmiObject` vs `Get-CimInstance`)**

***

### 👥 [Пользователи и группы](./accounts.md)

* 📌  **Локальные пользователи** (`Get-LocalUser`, `New-LocalUser`)
* 📌  **Локальные группы** (`Get-LocalGroup`, `Add-LocalGroupMember`)
* 📌  **Active Directory (RSAT)** (`Get-ADUser`, `Set-ADAccountPassword`)
* 📌  **Аудит входов (`Get-WinEvent`, `LogonType`)**
* 📌  **Управление правами (`icacls`, `Set-Acl`)**
