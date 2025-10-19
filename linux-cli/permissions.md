# Права и пользователи

## Права доступа

```bash
chmod 644 file.txt               # rw-r--r--
chmod +x script.sh               # добавить исполняемый бит
chmod -R g+w shared/             # рекурсивно разрешить запись группе
```

## Владелец и группа

```bash
chown user:group file.txt        # сменить владельца и группу
chown -R user:devs project/      # рекурсивная смена владельца
```

## Маска по умолчанию

```bash
umask                            # текущее значение маски
umask 027                        # новые файлы -> 640, каталоги -> 750
```

## Расширенные ACL

```bash
setfacl -m u:alice:rwx shared/   # дополнительные права для пользователя
getfacl shared/                  # посмотреть действующие ACL
setfacl -b file.txt              # удалить все ACL
```

## Пользователи и sudo

```bash
sudo useradd -m -G wheel alice   # создать пользователя и добавить в группу wheel
sudo passwd alice                # задать пароль
sudo visudo                      # настроить правила sudo
```
