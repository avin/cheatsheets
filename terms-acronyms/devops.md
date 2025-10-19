# DevOps и инфраструктура

**CI/CD** — Continuous Integration/Continuous Delivery (или Deployment); автоматизация сборки, тестирования и доставки. Jenkins, GitHub Actions, GitLab CI помогают быстро и безопасно выкатывать изменения.

**IaC** — Infrastructure as Code (Terraform, Ansible, Pulumi): инфраструктура описывается кодом и управляется через VCS, что даёт повторяемость и audit-trail.

**Immutable Infrastructure** — серверы/контейнеры не обновляются "наживую"; вместо этого создаются новые экземпляры. Уменьшает дрейф конфигураций.

**GitOps** — Git как источник правды для инфраструктуры и приложений. ArgoCD/Flux отслеживают репозитории и приводят окружения к нужному состоянию.

**SRE** — Site Reliability Engineering: практики Google по балансировке надежности и скорости поставки. Включают error budget, toil reduction.

**SLA/SLO/SLI** — Service Level Agreement/Objective/Indicator; SLA — обещание бизнесу, SLO — конкретное численное целевое значение, SLI — фактические измерения.

**Kubernetes (K8s)** — оркестратор контейнеров с ресурсами Deployment, Service, Ingress, StatefulSet. Де-факто стандарт для cloud-native.

**Helm** — пакетный менеджер для Kubernetes; Charts помогaют шаблонизировать манифесты и переиспользовать их.

**Service Mesh** — см. архитектура; на практике добавляет mTLS, ретраи, наблюдаемость без изменения кода.

**Blue/Green Deployment** — поддержка двух сред (Blue — текущая, Green — новая) с мгновенным переключением трафика. Полезно для крупных релизов с rollback.

**Canary Release** — постепенный rollout новой версии к небольшой аудитории, чтобы проверить метрики и ошибки до полного релиза.

**Chaos Engineering** — намеренное внесение сбоев (Chaos Monkey, Litmus) для проверки устойчивости системы.

**MTTR/MTBF** — Mean Time To Recovery / Between Failures; ключевые показатели зрелости SRE-команды.

**Observability** — совокупность логов, метрик, трасс, позволяющая отвечать на неожиданные вопросы о системе. Инструменты: Prometheus, Grafana, OpenTelemetry.

**APM** — Application Performance Monitoring (Datadog, New Relic); показывает транзакции, задержки, профилирование.

**MTTA** — Mean Time To Acknowledge: сколько времени уходит, чтобы заметить проблему и взять её в работу. Дополняет MTTR.

**Sidecar** — вспомогательный контейнер/процесс рядом с основным (например, Envoy proxy в Istio, лог-коллектор), добавляет функциональность без изменения основного приложения.

**Serverless / FaaS** — Function-as-a-Service (AWS Lambda, Google Cloud Functions). Удобно для событийных задач, платить только за время выполнения.

**Runtime** — среда выполнения (JVM, Node.js, .NET CLR, WASM runtime), которая исполняет приложение и управляет ресурсами.

**Shift-left** — перенос проверок (тесты, безопасность, код-ревью) на более ранние этапы разработки, чтобы ловить дефекты раньше.

**Runbook** — документ с пошаговыми инструкциями, как реагировать на типовые инциденты или выполнять рутинные операции (перезапуск сервиса, восстановление).
