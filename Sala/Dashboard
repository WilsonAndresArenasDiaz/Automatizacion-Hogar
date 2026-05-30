import tkinter as tk
from tkinter import ttk
import speech_recognition as sr
import requests
import serial
import threading
import time

# =====================================================================
# CONFIGURACIÓN DEL PUERTO SERIAL (CONEXIÓN CON ARDUINO)
# =====================================================================
PUERTO_COM = "COM10"  
try:
    arduino = serial.Serial(PUERTO_COM, 9600, timeout=0.1)
    time.sleep(2)
    print(" Conexión serial con Arduino establecida correctamente.")
except Exception as e:
    print(f"⚠️ Alerta Serial: No se pudo conectar al Arduino ({e}). Modo Simulación activo.")
    arduino = None

# Inicializador de reconocimiento de voz rápido
recognizer = sr.Recognizer()

# Variables globales para el Dashboard
datos_dashboard = {
    "modo_actual": "Menú Principal",
    "ultimo_comando": "Ninguno",
    "estado_ollama": "Listo",
    "tiempo_conexion": "Conectado (COM10)" if arduino else "Simulado"
}

# =====================================================================
# LÓGICA DE INTELIGENCIA ARTIFICIAL (OLLAMA GEMMA:2B)
# =====================================================================
def consultar_ollama_gemma(texto_usuario):
    url = "http://localhost:11434/api/generate"
    prompt_sistema = (
        "Eres un clasificador de comandos de voz para una estación de juegos Arduino.\n"
        "Analiza la FRASE del usuario y clasifícala en una de las siguientes opciones válidas:\n"
        "- Si quiere abrir el piano (ej: '1 do piano', 'piano', 'abre piano'): responde [PIANO]\n"
        "- Si quiere abrir la consola (ej: '2 re consola', 'consola pie', 'menú consola'): responde [CONSOLA]\n"
        "- Si quiere jugar al dinosaurio (ej: 'juego 1', '1 dino', 'dinosaurio', 'jugar dino'): responde [DINO]\n"
        "- Si quiere jugar Simón Dice (ej: 'juego 2', 're simon', 'simon dice'): responde [SIMON]\n"
        "- Si quiere salir, cancelar, regresar o volver (ej: 'salir', 'volver', 'cancelar', 'menú'): responde [MENU]\n"
        "REGLA CRÍTICA: Responde ÚNICAMENTE con la etiqueta entre corchetes, por ejemplo [PIANO]. No agregues texto adicional."
    )
    
    payload = {
        "model": "gemma:2b",
        "prompt": f"{prompt_sistema}\n\nFRASE DEL USUARIO: \"{texto_usuario}\"\nRESPUESTA:",
        "stream": False
    }
    
    try:
        respuesta = requests.post(url, json=payload, timeout=4)
        if respuesta.status_code == 200:
            resultado_texto = respuesta.json().get("response", "").strip()
            if "[PIANO]" in resultado_texto: return "[PIANO]", "Modo Piano"
            if "[CONSOLA]" in resultado_texto: return "[CONSOLA]", "Menú Consola"
            if "[DINO]" in resultado_texto: return "[DINO]", "Juego Dinosaurio"
            if "[SIMON]" in resultado_texto: return "[SIMON]", "Simón Dice"
            if "[MENU]" in resultado_texto: return "[MENU]", "Menú Principal"
        return "NINGUNO", "Comando Desconocido"
    except Exception as e:
        return "ERROR", f"Error de conexión: {e}"

# =====================================================================
# PROCESAMIENTO DE VOZ EN SEGUNDO PLANO
# =====================================================================
def escuchar_voz_hilo():
    lbl_estado_voz.config(text="🎙️ Escuchando... Habla ahora.", foreground="#FF5722")
    
    try:
        with sr.Microphone() as source:
            recognizer.adjust_for_ambient_noise(source, duration=0.4)
            audio = recognizer.listen(source, timeout=5, phrase_time_limit=4)
        
        lbl_estado_voz.config(text="⚡ Transcribiendo audio...", foreground="#2196F3")
        
        texto_dicho = recognizer.recognize_google(audio, language="es-ES")
        lbl_transcripcion.config(text=f'Tú dijiste: "{texto_dicho}"')
        
        lbl_estado_voz.config(text="🧠 Pensando con Gemma:2b...", foreground="#9C27B0")
        
        accion, modo_nombre = consultar_ollama_gemma(texto_dicho)
        
        datos_dashboard["ultimo_comando"] = texto_dicho
        datos_dashboard["modo_actual"] = modo_nombre
        
        # --- BLOQUE DE ENVÍO CORREGIDO PARA ARDUINO ---
        if accion != "NINGUNO" and accion != "ERROR":
            datos_dashboard["estado_ollama"] = f"Exitoso: {accion}"
            if arduino:
                comando_limpio = accion.replace("[", "").replace("]", "")
                mensaje_final = comando_limpio + "\n"  # Asegura el salto de línea que espera el Arduino
                
                arduino.write(mensaje_final.encode('ascii')) # Envío explícito en formato ASCII
                arduino.flush() # Vacía el buffer para transmisión inmediata
                print(f">> Cable USB: ¡Comando [{comando_limpio}] enviado con éxito!")
        else:
            datos_dashboard["estado_ollama"] = "No se entendió el comando"
            
        actualizar_dashboard_visual()
        
    except sr.UnknownValueError:
        lbl_transcripcion.config(text='❌ No se detectó ninguna palabra clara.')
    except Exception as e:
        lbl_transcripcion.config(text=f'⚠️ Error en el proceso: {e}')
        
    lbl_estado_voz.config(text="🟢 Asistente en Espera", foreground="#4CAF50")

def iniciar_escucha_voz():
    threading.Thread(target=escuchar_voz_hilo, daemon=True).start()

# =====================================================================
# INTERFAZ GRÁFICA (TKINTER)
# =====================================================================
def actualizar_dashboard_visual():
    lbl_dash_modo.config(text=datos_dashboard["modo_actual"])
    lbl_dash_comando.config(text=datos_dashboard["ultimo_comando"])
    lbl_dash_ia.config(text=datos_dashboard["estado_ollama"])
    lbl_dash_conexion.config(text=datos_dashboard["tiempo_conexion"])

def mostrar_pantalla_voz():
    frame_dashboard.pack_forget()
    frame_asistente.pack(fill="both", expand=True, padx=20, pady=20)
    btn_nav_voz.config(state="disabled")
    btn_nav_dash.config(state="normal")

def mostrar_pantalla_dashboard():
    frame_asistente.pack_forget()
    frame_dashboard.pack(fill="both", expand=True, padx=20, pady=20)
    btn_nav_dash.config(state="disabled")
    btn_nav_voz.config(state="normal")
    actualizar_dashboard_visual()

try:
    root = tk.Tk()
    root.title("PlayStation Arduino - Panel de Control")
    root.geometry("600x450")
    root.configure(bg="#1E1E24")

    style = ttk.Style()
    style.theme_use("clam")
    style.configure("Nav.TButton", background="#333533", foreground="white")

    frame_navegacion = tk.Frame(root, bg="#2A2C2E", height=50)
    frame_navegacion.pack(fill="x", side="top")

    btn_nav_voz = ttk.Button(frame_navegacion, text="🎙️ Hablar / Comandos", style="Nav.TButton", command=mostrar_pantalla_voz)
    btn_nav_voz.pack(side="left", padx=10, pady=5)

    btn_nav_dash = ttk.Button(frame_navegacion, text="📊 Ver Dashboard", style="Nav.TButton", command=mostrar_pantalla_dashboard)
    btn_nav_dash.pack(side="left", padx=10, pady=5)

    # --- PANTALLA 1: ASISTENTE ---
    frame_asistente = tk.Frame(root, bg="#1E1E24")
    lbl_titulo_voz = tk.Label(frame_asistente, text="CONTROL POR VOZ DE LA ESTACIÓN", font=("Arial", 14, "bold"), bg="#1E1E24", fg="white")
    lbl_titulo_voz.pack(pady=15)

    btn_hablar = tk.Button(frame_asistente, text="PRESIONA AQUÍ Y HABLA", font=("Arial", 12, "bold"), bg="#4CAF50", fg="white", cursor="hand2", bd=0, padx=20, pady=15, command=iniciar_escucha_voz)
    btn_hablar.pack(pady=20)

    lbl_estado_voz = tk.Label(frame_asistente, text="🟢 Asistente en Espera", font=("Arial", 11, "bold"), bg="#1E1E24", fg="#4CAF50")
    lbl_estado_voz.pack(pady=5)

    lbl_transcripcion = tk.Label(frame_asistente, text='Tú dijiste: "..."', font=("Arial", 11, "italic"), bg="#1E1E24", fg="#B0B0B0", wraplength=500)
    lbl_transcripcion.pack(pady=20)

    # --- PANTALLA 2: DASHBOARD ---
    frame_dashboard = tk.Frame(root, bg="#1E1E24")
    lbl_titulo_dash = tk.Label(frame_dashboard, text="📊 DASHBOARD EN TIEMPO REAL", font=("Arial", 14, "bold"), bg="#1E1E24", fg="white")
    lbl_titulo_dash.pack(pady=15)

    grid_container = tk.Frame(frame_dashboard, bg="#2A2C2E", bd=2, relief="groove", padx=15, pady=15)
    grid_container.pack(fill="x", pady=10)

    tk.Label(grid_container, text="Modo / Juego Activo:", font=("Arial", 11, "bold"), bg="#2A2C2E", fg="#A0A0A0").grid(row=0, column=0, sticky="w", pady=8)
    lbl_dash_modo = tk.Label(grid_container, text="Menú Principal", font=("Arial", 11, "bold"), bg="#2A2C2E", fg="#00FFFF")
    lbl_dash_modo.grid(row=0, column=1, sticky="w", padx=20)

    tk.Label(grid_container, text="Último Comando:", font=("Arial", 11, "bold"), bg="#2A2C2E", fg="#A0A0A0").grid(row=1, column=0, sticky="w", pady=8)
    lbl_dash_comando = tk.Label(grid_container, text="Ninguno", font=("Arial", 11), bg="#2A2C2E", fg="white")
    lbl_dash_comando.grid(row=1, column=1, sticky="w", padx=20)

    tk.Label(grid_container, text="Estado Ollama:", font=("Arial", 11, "bold"), bg="#2A2C2E", fg="#A0A0A0").grid(row=2, column=0, sticky="w", pady=8)
    lbl_dash_ia = tk.Label(grid_container, text="Listo", font=("Arial", 11), bg="#2A2C2E", fg="#FFEB3B")
    lbl_dash_ia.grid(row=2, column=1, sticky="w", padx=20)

    tk.Label(grid_container, text="Conexión USB:", font=("Arial", 11, "bold"), bg="#2A2C2E", fg="#A0A0A0").grid(row=3, column=0, sticky="w", pady=8)
    lbl_dash_conexion = tk.Label(grid_container, text="Conectado (COM10)", font=("Arial", 11, "bold"), bg="#2A2C2E", fg="#4CAF50")
    lbl_dash_conexion.grid(row=3, column=1, sticky="w", padx=20)

    mostrar_pantalla_voz()
    root.mainloop()

except Exception as error_grafico:
    print(f"\n❌ Error crítico al abrir la ventana visual: {error_grafico}")
