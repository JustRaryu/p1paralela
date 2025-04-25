# p1paralela


Para correr el programa ejecutar:
```bash
g++ -O3 -fopenmp main.cpp matrix_factory.cpp matrix_multiplication.cpp matrix_parallel_multiplication.cpp -o main
```
Luego se mostrará por consola un menú que pregunta por [1] secuencial o [2] paralelizada, donde segun el número que se entrega se ejecutará el algoritmo elegido de la forma que se desee.
Despues se mostrará otro menú con 6 algoritmos para elegir con cual metodo se realizará la multiplicación de matrices con los tamaños 2, 4, 8, 16, 32, 64, 128, 256, 512.
