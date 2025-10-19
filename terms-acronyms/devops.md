# DevOps и инфраструктура

**CI/CD** — Continuous Integration/Continuous Delivery (или Deployment); автоматизация сборки, тестирования и доставки.

**IaC** — Infrastructure as Code (Terraform, Ansible, Pulumi): управление инфраструктурой через декларативные/императивные конфигурации.

**Immutable Infrastructure** — инфраструктура, которая не обновляется на месте, а заменяется новыми идентичными экземплярами.

**GitOps** — управление инфраструктурой и приложениями через Git как единственный источник правды, автоматизация sync (ArgoCD, Flux).

**SRE** — Site Reliability Engineering; инженеры, балансирующие надежность и скорость разработки.

**SLA/SLO/SLI** — Service Level Agreement/Objective/Indicator; договорные показатели и метрики надежности.

**Kubernetes (K8s)** — оркестратор контейнеров (Deployment, Service, Ingress).

**Helm** — пакетный менеджер для Kubernetes (Charts).

**Service Mesh** — см. архитектура.

**Blue/Green Deployment** — одновременное поддержание двух окружений (синий & зелёный) с переключением трафика.

**Canary Release** — постепенный rollout новой версии к части пользователей для проверки.

**Chaos Engineering** — тестирование отказоустойчивости путем введения намеренных сбоев (Chaos Monkey, Litmus).

**MTTR/MTBF** — Mean Time To Recovery / Between Failures; метрики надежности.

**Observability** — способность понимать внутреннее состояние системы по логам, метрикам, трассам.

**APM** — Application Performance Monitoring (Datadog, New Relic, AppDynamics).

**Sidecar** — вспомогательный контейнер/процесс, который размещается рядом с основным для добавления функционала (например, proxy, logging).

**Serverless / FaaS** — Function-as-a-Service: AWS Lambda, Google Cloud Functions, Azure Functions.

**Runtime** — среда выполнения (JVM, Node.js, WASM runtime).
