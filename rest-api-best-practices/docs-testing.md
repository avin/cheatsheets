# Документация и тестирование

## OpenAPI / Swagger
- Поддерживайте актуальную спецификацию (YAML/JSON).
- Используйте генераторы (Swagger UI, ReDoc) для интерактивной документации.
- Автоматизируйте валидацию схемы (openapi-cli, spectral).

## Документация
- Примеры запросов/ответов, описание параметров, типов.
- Объясните бизнес-термины, "gotchas".
- Publish Change Log (breaking changes, deprecations).

## SDK и клиенты
- Генераторы (`openapi-generator`, `swagger-codegen`, `orval`).
- Поддерживайте версии для популярных языков (TypeScript, Python, Go).

## Тестирование
- Unit тесты для бизнес-логики.
- Интеграционные тесты API (Postman/Newman, pytest + httpx, supertest).
- Контрактные тесты (Pact, Dredd) между сервисами.
- Smoke-тесты после деплоя.

## Sandbox environments
- Предоставляйте sandbox/staging для клиентов.
- Внимание к rate limit, тестовым данным.

## Monitoring Docs
- Обновляйте документацию при релизах (CI pipeline).
- Автоматическая проверка broken links.

