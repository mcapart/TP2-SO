TP2-SO

Una vez que se realizo el pull del respositorio en una terminal se deben posicionar sobre la carpeta:
RowDaBoat-x64barebones-d4e1c147f975 la que se tendra que habre con docker. 
Dentro de esta carpeta primero se debe hacer un cd Toolchain, para ir a esa carpeta. Ejecutar make clean y make el. Ejecutar cd .. para volver a la carpeta original. Ejecutar el make clean y luego:
- make all. Si se desea utilizar el memory manager que implementa el bitmap
- make buddy. Si se desea utilizar el memory manager que implementa el buddy system. 
Una vez compilado se corre con el comando ./run.sh
