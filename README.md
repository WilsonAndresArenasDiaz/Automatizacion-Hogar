# Automatización Hogar - Casa IoT

Proyecto final de domótica con 5 escenarios inteligentes: Baño, Cocina, Habitación, Sala y Garaje.
<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/b1301bba-75af-4bea-8539-3dbb96010881" />

## 🧠 Resumen
- **Baño**: ESP32 con LED RGB y OLED. Control por voz (vía chatbot) para modos: día, noche, spa.
- **Cocina**: ESP32 con sensores de gas (MQ-135), fuego (KY-026) y humedad de suelo (MH). Activa extractor automático, bomba de riego, alarmas. Control por voz y vía web.
- **Habitación**: ESP32 con sensor de luz BH1750, persiana motorizada, display TM1637 (reloj NTP) y comandos de voz desde la nube.
- **Sala**: Arduino UNO con 4 juegos: piano electrónico, Simon Dice, Dino multiobstáculos y animación de caritas (simula TV).
- **Garaje**: ESP32 + sensores IR para conteo de autos HotWheels, servomotor para talanquera, LCD 16x2. Además, script Python con YOLO para detección de vehículos.

Todos los módulos (excepto Sala) se comunican por WiFi y envían datos a un servidor central en Hostinger. El dashboard web permite visualizar el estado de toda la casa en tiempo real.

## 📡 Enlaces
- **Repositorio**: https://github.com/WilsonAndresArenasDiaz/Automatizacion-Hogar
- **Dashboard**: https://grupoeos.com.co/dashboard (solo interno)

## 🛠️ Instalación
1. Clona el repositorio.
2. Abre cada carpeta y carga el código correspondiente en la placa respectiva (ESP32 o Arduino UNO).
3. Instala las librerías listadas en `librerias_requeridas.txt`.
4. Para el garaje, ejecuta `yolo_detector.py` en un PC con cámara para detectar autos y enviar comandos por serial.

## 📸 Fotos
Ver carpeta `Fotos/`.

## 📜 Licencia
Proyecto educativo de código abierto.
