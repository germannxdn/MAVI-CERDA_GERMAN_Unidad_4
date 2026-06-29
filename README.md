# MAVI II - Unidad 4: Eventos y Colisiones

**Autor:** German Cerda

## Descripción
Sistema de tiro al objetivo que utiliza Box2D y Raylib para detectar colisiones y reaccionar a eventos físicos.

## Mecánica del juego
- Mantené presionada la tecla **ESPACIO** para cargar potencia
- Soltá **ESPACIO** para disparar el proyectil
- El objetivo es impactar en el bloque naranja
- Al impactar, se muestra un mensaje y cambia el estado del juego
- Presioná **R** para reiniciar

## Conceptos aplicados
- b2ContactListener (detección de colisiones)
- BeginContact (detección de impacto)
- Identificación de cuerpos por tipo y forma
- Estados del juego (Waiting, Charging, Running, Hit)
- Sistema de carga de potencia

## Controles
| Tecla | Acción |
|-------|--------|
| **ESPACIO** | Cargar potencia (mantener) / Disparar (soltar) |
| **R** | Reiniciar el juego |

## Estructura del código
- `ContactListener.h`: Detecta colisiones entre proyectil y objetivo
- `PhysicsWorld.h/cpp`: Maneja el mundo físico y los cuerpos
- `Game.h/cpp`: Controla el flujo del juego y los estados
- `main.cpp`: Punto de entrada
