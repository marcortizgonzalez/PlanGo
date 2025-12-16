# PlanGo

## Autores
- Marc Ortiz González
- Eduard Pernia
- Sergio Fernández

## Configuración de la Base de Datos
Para ejecutar la aplicación, es necesario configurar la conexión a la base de datos MySQL/MariaDB.

1. Crear la base de datos `inep08` (o el nombre que uses).
2. Ejecutar los scripts SQL generados por ODB en el siguiente orden estricto:
   - `Categoria.sql`
   - `Experiencia.sql`
   - `Escapada.sql`
   - `Activitat.sql`
   - `Usuari.sql`
   - `Reserva.sql`
3. Poblar la base de datos con el script de datos de prueba adjunto (opcional).

## Archivo de Configuración (.env)
La aplicación requiere un archivo llamado `configuracio.env` en la misma carpeta que el ejecutable `.exe`.
El formato debe ser:

- DB_USER=inep08
- DB_PASS=xubaasha8Shei6
- DB_NAME=inep08
- DB_HOST=ubiwan.epsevg.upc.edu
- DB_PORT=3306
