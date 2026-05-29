# Laboratorio Final: Casa con IoT



<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135303.png" alt="Captura 1" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135342.png" alt="Captura 2" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135431.png" alt="Captura 3" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135542.png" alt="Captura 4" width="900" />
</p>
# 🚽 Baño Inteligente con ESP32

Sistema de automatización para baño inteligente usando **ESP32**, **LED RGB**, **pantalla OLED** y comunicación vía **WiFi + API REST**.  
El proyecto permite cambiar los modos del baño mediante comandos de voz o peticiones HTTP.

---

# 📌 Descripción del Proyecto

Este laboratorio implementa un sistema de iluminación inteligente controlado por un **ESP32**.

El usuario puede seleccionar distintos modos ambientales:

- 🌅 **Modo Mañana** → Luz Amarilla
- 🧖 **Modo Spa** → Luz Roja
- 🌙 **Modo Noche** → Luz Verde

Además:

- Se muestra el estado actual en una pantalla **OLED**
- El ESP32 expone una API REST
- Los cambios pueden hacerse mediante voz o desde navegador/web

---

# 🏗 Arquitectura del Proyecto

![Arquitectura](https://raw.githubusercontent.com/USER/REPO/main/arquitectura.png)

> Reemplaza la imagen por la tuya en el repositorio.

---

# 🧰 Componentes Utilizados

| Componente | Descripción |
|---|---|
| ESP32 | Microcontrolador principal |
| Pantalla OLED SSD1306 | Visualización de estados |
| LEDs RGB | Indicadores de modo |
| Resistencias | Protección LEDs |
| WiFi | Comunicación inalámbrica |
| API REST | Control remoto |

---

# ⚡ Conexiones

## 🔌 LEDs

| LED | GPIO ESP32 |
|---|---|
| Rojo | GPIO 25 |
| Verde | GPIO 26 |
| Amarillo | GPIO 27 |

---

## 🖥 OLED SSD1306

| OLED | ESP32 |
|---|---|
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| VCC | 3.3V |
| GND | GND |

---

# 📚 Librerías Utilizadas

Instalar desde el **Library Manager** de Arduino IDE:

```cpp
WiFi.h
WebServer.h
Wire.h
Adafruit_GFX.h
Adafruit_SSD1306.h
```

---

# 🌐 Funcionamiento

El ESP32:

1. Se conecta a la red WiFi
2. Inicia un servidor web en el puerto 80
3. Espera comandos HTTP
4. Interpreta el modo solicitado
5. Activa LEDs
6. Muestra información en la OLED

---

# 🧠 Modos del Sistema

| Comando | Acción | LED |
|---|---|---|
| spa | Ambiente relajante | 🔴 Rojo |
| mañana / manana | Ambiente día | 🟡 Amarillo |
| noche | Ambiente nocturno | 🟢 Verde |

---

# 🚀 API REST

## Endpoint

```bash
GET /modo?modo=<valor>
```

---

## 📌 Ejemplos

### Activar modo SPA

```bash
http://IP_ESP32/modo?modo=spa
```

### Activar modo mañana

```bash
http://IP_ESP32/modo?modo=manana
```

### Activar modo noche

```bash
http://IP_ESP32/modo?modo=noche
```

---

# 📟 Respuestas JSON

## ✅ Correcta

```json
{
  "status": "ok",
  "modo": "spa"
}
```

---

## ❌ Error

```json
{
  "status": "error"
}
```

---

# 🧠 Lógica Inteligente

El sistema interpreta palabras clave usando:

```cpp
entrada.indexOf("spa")
entrada.indexOf("mañana")
entrada.indexOf("noche")
```

Esto permite integrar asistentes de voz fácilmente.

---

# 🔥 Código Principal

## Activación de Modos

```cpp
void activarModo(String modo) {

  apagarTodo();

  if (modo == "spa") {
    digitalWrite(LED_ROJO, HIGH);
    mostrarOLED("SPA");
  }
  else if (modo == "manana") {
    digitalWrite(LED_AMARILLO, HIGH);
    mostrarOLED("DIA");
  }
  else if (modo == "noche") {
    digitalWrite(LED_VERDE, HIGH);
    mostrarOLED("NOCHE");
  }
}
```

---

# 📡 Monitor Serial

El ESP32 muestra:

```bash
Conectando a WiFi...
✅ Conectado a WiFi
📡 IP ESP32: 192.168.X.X
```



<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135604.png" alt="Captura 5" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135619.png" alt="Captura 6" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135637.png" alt="Captura 7" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135647.png" alt="Captura 8" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135656.png" alt="Captura 9" width="900" />
</p>
