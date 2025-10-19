# Docker шпаргалка

Подборка рецептов и команд для администрирования Docker: работа с образами и контейнерами, Compose/Swarm, CI/CD и безопасность.

### 🚀 [Базовые команды и контейнеры](./containers-basics.md)

* 📌  **Запуск, остановка, инспекция контейнеров**
* 📌  **Копирование файлов, exec, логирование**
* 📌  **Namespaces, capabilities, cgroups (кратко)**

***

### 🧱 [Образы и Dockerfile](./images-dockerfile.md)

* 📌  **Многоступенчатая сборка, кеширование**
* 📌  **Оптимизация размеров, best practices**
* 📌  **Работа с Registry (`docker login`, `push`, `buildx`)**

***

### 🧩 [Volumes и сети](./volumes-networking.md)

* 📌  **Named volumes, bind mounts, tmpfs**
* 📌  **Создание сетей, bridge/host/overlay**
* 📌  **Проброс портов, firewall, dns**

***

### 🧾 [Docker Compose](./docker-compose.md)

* 📌  **Структура `compose.yaml`, профили, расширения**
* 📌  **Переменные окружения, секции `deploy`**
* 📌  **Темплейты, overrides, команды `compose`**

***

### ⚓ [Docker Swarm](./docker-swarm.md)

* 📌  **Инициализация и управление кластером**
* 📌  **Стэки, секреты, конфиги**
* 📌  **Secrets/configs, rolling updates, scaling**

***

### 🔐 [Безопасность и управление доступом](./security.md)

* 📌  **Пользователи, capabilities, seccomp/AppArmor**
* 📌  **Подпись образов (Notary, cosign)**
* 📌  **Сканирование уязвимостей, практика обновлений**

***

### 🧭 [Диагностика и мониторинг](./diagnostics-monitoring.md)

* 📌  **Логи, `docker stats`, события**
* 📌  **Troubleshooting network/storage**
* 📌  **Интеграция с Prometheus, Grafana, ELK**

***

### 🔄 [CI/CD и автоматизация](./ci-cd-automation.md)

* 📌  **BuildKit, кэширование, multi-platform**
* 📌  **GitHub Actions/GitLab CI pipelines**
* 📌  **Тегирование, lifecycle hooks**
