# Tarea-algoritmos

Se me olvido mencionar que las tablas fueron hechas con la herramienta NOTION, disculpas.
Para el El DP y fuerza bruta son los algoritmos implementados para la tarea, las lineas casi al final del programa son donde esta para cambiar los strings de prueba, ahora el DP reconstructor funciona igual que el DP solo que te da la secuencia de operaciones con el coste mínimo siempre aplicado por el DP (visto en catedra)

Para hacer la compilacion y el debug de los programas correspondientes de los experimentos hago lo siguiente:
"g++ -g programa.c -o programa -Wall -Wpedantic -Wextra"
luego:
"./programa"

Para saber el asignamiento de memoria ocupar los siguiente en la terminal:
"valgrind --leak-check=full --show-leak-kinds=all ./programa"
