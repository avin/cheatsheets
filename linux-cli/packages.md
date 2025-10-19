# Пакетные менеджеры

## Debian/Ubuntu (apt)

```bash
sudo apt update && sudo apt upgrade         # обновить индексы и пакеты
apt search ripgrep                          # поиск пакета
apt show ripgrep                            # сведения о пакете
sudo apt install ripgrep                    # установка
apt-cache policy openssl                    # показать доступные версии и источник
dpkg -L ripgrep                             # список файлов установленного пакета
```

## Fedora/RHEL (dnf)

```bash
sudo dnf check-update                       # проверка обновлений
sudo dnf search ripgrep                     # поиск
sudo dnf install ripgrep                    # установка
sudo dnf info ripgrep                       # информация
```

## Arch (pacman)

```bash
sudo pacman -Syu                            # синхронизировать и обновить
pacman -Ss ripgrep                          # поиск в репозиториях
sudo pacman -S ripgrep                      # установка
pacman -Qi ripgrep                          # информация об установленном пакете
```

## Snap и Flatpak

```bash
snap find docker                            # поиск в snap
sudo snap install docker                    # установка
flatpak search org.gimp.GIMP                # поиск flatpak
flatpak install flathub org.gimp.GIMP       # установка приложения
```
