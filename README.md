## Minero
Lo primero es instalar el minero

El minero utiliza meson e instala el ejecutable en
~/.local/share/rolas
Debemos tener el ejecutable instalado en esa ruta para que la interfaz la pueda usar.

Para usarse:

1.  Configuramos meson:
`meson setup build --prefix="$HOME/.local"

2. Compilamos
`meson compile -C build`

3. Instalamos:
`meson install -C build`
con esto ya sirve la interfaz.
Antes de ejecutar se debe hacer el paso 1 de base de datos
4. Ejecutamos:
`~/.local/share/rolas/Minero`
y se usa la entrada estándar para leer el directorio

## Base de datos
Primero se debe tener rolas.db para que funcione el minero y la interfaz(con el minero).
Tenemos:
`rolas/bdd/rolas.sql`
Y se usa para crear rolas.db en
`~/.loca l/share/rolas`
De esta forma:
1. `sqlite3 ~/.local/share/rolas/rolas.db < bdd/rolas.sql`
Para ejecutar la base de datos con sqlite3:   
3. `sqlite3 bdd/rolas.db`

## Interfaz
1. entramos a Interfaz/
2. abre con:
`dotnet run`

## Bibliotecas necesarias:
Para minero
- sudo apt install libid3-3.8.3-dev libsqlite3-dev
Para la interfaz
vlc libvlc-dev 
- dotnet add microsoft.data.sqlite
