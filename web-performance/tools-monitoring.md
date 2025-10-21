# Инструменты и мониторинг

## DevTools
- Chrome DevTools → Performance, Lighthouse, Coverage tabs.
- Firefox Developer Tools, Edge DevTools.
- Performance recording, CPU throttling, network throttling (3G).
- Layout shift regions overlay.

## Lighthouse / WebPageTest
- Lighthouse CLI (`lighthouse https://example.com --preset=perf`) + CI (GitHub Actions, GitLab).
- WebPageTest.org — контрольные тесты (многократный run, моб/desktop, connection). TTFB, Speed Index.
- Calibre, SpeedCurve для synthetic.

## RUM
- Google Analytics 4: Web Vitals collection.
- Web-vitals library (`import { onCLS, onINP } from 'web-vitals';`).
- Sentry Performance, NewRelic, Datadog, Splunk RUM.
- Custom logging with `PerformanceObserver`.

## Bundler tooling
- Webpack Bundle Analyzer, Rollup Analyzer, Vite plugin visualizer.
- `npm-run build --analyze`. Tools show chunk sizes.
- Source map explorer (analyze dependencies).

## Automation
- Lighthouse CI (GitHub Action, GitLab). Performance budgets.
- Jenkins + WebPageTest plugin.
- Calibre CLI, Sitespeed.io.

## Monitoring dashboards
- Grafana dashboards for Web Vitals (RUM). Elastic/Kibana.
- Alerting on metric thresholds (Slack, pager).
- Error monitoring (Sentry/Bugsnag) + performance traces.

