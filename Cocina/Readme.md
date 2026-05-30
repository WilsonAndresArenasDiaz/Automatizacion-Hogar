# Cocina Inteligente con ESP32 para Proyecto de Domótica
<img width="1807" height="1355" alt="cocina" src="https://github.com/user-attachments/assets/b22cd879-3852-400c-8646-494cc0a1c6c5" />


## Descripción
Este proyecto implementa una **cocina inteligente** como parte de una casa domótica.  
Incluye:

- Sensor de gas MQ-135
- Sensor de flama (infrarrojo)
- Sensor de humedad de suelo (MH)
- LEDs simulando estufa y nevera
- Zumbador de alarma
- Extractor de aire (ventilador) controlado por TIP122
- Bomba de agua para riego automático controlada por TIP127
- Conexión WiFi y servidor web embebido
- Envío de datos a una API en la nube (Hostinger) para dashboard central
- Riego automático cuando el suelo se seca
- Alarmas por gas o fuego con activación del extractor

## Hardware necesario
- ESP32 Dev Board
- Sensor MQ-135
- Sensor KY-026 (flama)
- Sensor de humedad MH
- 2 LEDs + resistencias 220Ω
- Zumbador pasivo
- Transistor TIP122 y TIP127 + resistencias 1kΩ
- Diodos 1N4007
- Motor DC (extractor) y bomba de agua pequeña
- Fuente de alimentación externa (5V o 12V según motores)

## Esquema de conexiones (resumen)
| Componente        | Pin ESP32 |
|-------------------|-----------|
| MQ-135 (A0)       | GPIO 34   |
| Flama (DO)        | GPIO 32   |
| Humedad MH (A0)   | GPIO 35   |
| Buzzer            | GPIO 25   |
| LED estufa        | GPIO 26   |
| LED nevera        | GPIO 27   |
| Extractor (TIP122)| GPIO 33   |
| Bomba (TIP127)    | GPIO 14   |

Consulta el código para más detalles.

## Configuración WiFi
El código está preparado para la red:
- SSID: `BigDj`
- Contraseña: `BIGDJ111`

Puedes cambiarlo en las constantes `ssid` y `password`.

## Endpoints HTTP disponibles
- `/datos` → JSON con gas, fuego, humedad
- `/estufa?estado=on/off`
- `/nevera?estado=on/off`
- `/extractor` → activa 5 segundos
- `/bomba` → activa 5 segundos
- `/buzzer` → suena 0.5 segundos

## Dashboard local
Abre el archivo `dashboard_cocina.html` en tu navegador, cambia la IP de la ESP32 y podrás monitorear y controlar la cocina desde cualquier dispositivo de la misma red.

## Envío a nube (Hostinger)
Cada 10 segundos la ESP32 envía los datos a `https://grupoeos.com.co/api/recibir_datos.php` con una API key. Allí el compañero recolecta la información para el dashboard general de la casa.

## Licencia
Proyecto académico libre para uso educativo.
