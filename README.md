## Minero
 El minero utiliza meson e instala el ejecutable en
 ~/.local/share/rolas
 Para usarse:

1.  Configuramos meson:
     `meson setup build --prefix="$HOME/.local"
    
2. Compilamos
     `meson compile -C build`

4. Instalamos:
`meson install -C build`
  
3. Ejecutamos:
`~/.local/share/rolas/Minero`
y se usa la entrada estándar para leer el directorio


   
- pra ejecutar la base de datos sola:

1. `sqlite3 bdd/rolas.db < bdd/rolas.sql`
2. `sqlite3 bdd/rolas.db`
