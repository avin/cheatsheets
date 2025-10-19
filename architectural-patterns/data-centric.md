# Data-centric и аналитические паттерны

## Data Lake
- **Суть**: хранилище сырых данных (structured, semi-structured, unstructured) в их исходном виде.
- **Использование**: аналитика, ML-пайплайны, архивация.
- **Плюсы**: гибкость, хранение любых данных.
- **Минусы**: "data swamp" при отсутствии governance.

## Data Warehouse
- **Суть**: централизованное хранилище структурированных данных (Snowflake, BigQuery, Redshift).
- **Использование**: BI, отчётность, аналитика.

## Lambda Architecture
- **Batch layer** + **Speed layer** + **Serving layer**. Объединяет историю и realtime.

## Kappa Architecture
- **Суть**: только stream-пайплайн, batch делается поверх stream (например, Apache Kafka + Flink).

## Event Sourcing / CQRS (см. architecture)

## ETL / ELT
- **ETL**: Extract → Transform → Load (классиеский подход).
- **ELT**: Extract → Load → Transform (когда трансформации в DWH).

## Star / Snowflake schema
- **Star**: факт-таблица + dimension.
- **Snowflake**: нормализованные dimension таблицы.

## OLTP vs OLAP
- **OLTP**: transactional workloads (insert/update/read).
- **OLAP**: аналитика, большие запросы.

## CDC (Change Data Capture)
- **Суть**: отслеживание изменений в БД (Debezium, Kafka Connect) для синхронизации и event-driven систем.

## Data Mesh
- **Суть**: доменная ответственность за данные, продуктовый подход к data-наборам.
- **Принципы**: domain ownership, data as a product, self-serve platform, federated governance.
