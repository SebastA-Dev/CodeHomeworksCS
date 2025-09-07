# Herramientas Para el Rendimiento

## Chrono
Biblioteca estándar de C++ que permite medir intervalos de tiempo en diferentes resoluciones (ms, ns), fácil de usar y portable.

## Query Performance Counter
API de Windows que proporciona un contador de alta resolución para medir tiempos con gran precisión (microsegundos o mejor).

## Read Time-Stamp Counter
Instrucción de CPU (RDTSC) que devuelve el número de ciclos ejecutados desde el arranque, útil para mediciones muy precisas a nivel de hardware.

## GetProcessTimes
Función de Windows que obtiene el tiempo de CPU (usuario y kernel) consumido por un proceso, útil para analizar el costo real de ejecución.
