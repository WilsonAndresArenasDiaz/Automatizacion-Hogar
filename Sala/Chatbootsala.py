import whisper
import speech_recognition as sr
import requests
import os
import serial
import time
import json

# =====================================================================
# CONFIGURACIÓN HARDWARE: REEMPLAZA EL 'COM8' POR EL PUERTO DE TU ARDUINO
# =====================================================================
try:
    arduino = serial.Serial('COM8', 9600, timeout=1)
    time.sleep(2)
    print(" Conexión serial con Arduino establecida.")
except Exception as e:
    print(f"⚠️ Alerta Serial: No se pudo conectar al Arduino ({e}). Modo simulación activo.")
    arduino = None

print("\n Cargando Whisper (Modelo Tiny) para el reconocimiento de voz...")
model_stt = whisper.load_model("tiny")
recognizer = sr.Recognizer()

# =====================================================================
# FUNCIÓN PARA CONSULTAR A OLLAMA (MODELO GEMMA 2B)
# =====================================================================
def consultar_ollama_gemma(texto_usuario):
    url = "http://localhost:11434/api/generate"
    
    # Creamos un prompt de sistema estricto para que actúe como clasificador inteligente
    prompt_sistema = (
        "Eres un clasificador de comandos de voz para una estación de juegos Arduino.\n"
        "Analiza la FRASE del usuario y clasifícala en una de las siguientes opciones válidas:\n"
        "- Si el usuario quiere abrir el piano (ej: '1 do piano', 'quiero el piano', 'abre piano'): responde [PIANO]\n"
        "- Si quiere abrir la consola (ej: '2 re consola', 'consola pie', 'menú consola'): responde [CONSOLA]\n"
        "- Si quiere jugar al dinosaurio (ej: 'juego 1', '1 dino', 'dinosaurio', 'jugar dino'): responde [DINO]\n"
        "- Si quiere jugar Simón Dice (ej: 'juego 2', 're simon', 'simon dice', '2 simon'): responde [SIMON]\n"
        "- Si quiere salir, cancelar, regresar o volver (ej: 'salir', 'volver', 'cancelar', 'menú principal'): responde [MENU]\n"
        "REGLA CRÍTICA: Responde ÚNICAMENTE con la etiqueta entre corchetes, por ejemplo [PIANO]. No saludes, no expliques, no agregues texto adicional."
    )
    
    payload = {
        "model": "gemma:2b", # Cambiar a gemma2:2b si descargaste esa versión específica
        "prompt": f"{prompt_sistema}\n\nFRASE DEL USUARIO: \"{texto_usuario}\"\nRESPUESTA:",
        "stream": False
    }
    
    try:
        respuesta = requests.post(url, json=payload)
        if respuesta.status_code == 200:
            resultado_texto = respuesta.json().get("response", "").strip()
            # Limpiamos posibles respuestas ruidosas del modelo
            if "[PIANO]" in resultado_texto: return "[PIANO]"
            if "[CONSOLA]" in resultado_texto: return "[CONSOLA]"
            if "[DINO]" in resultado_texto: return "[DINO]"
            if "[SIMON]" in resultado_texto: return "[SIMON]"
            if "[MENU]" in resultado_texto: return "[MENU]"
        return "NINGUNO"
    except Exception as e:
        print(f"❌ Error al conectar con Ollama: {e}")
        return "NINGUNO"

print("\n=======================================================")
print(" CHATBOT DE VOZ CON OLLAMA (GEMMA 2B) LISTO")
print("=======================================================")

ARCHIVO_AUDIO = "temp_audio.wav"

try:
    while True:
        input("\nPresiona ENTER en el teclado para empezar a hablar...")
        
        with sr.Microphone() as source:
            print("🎙️ Escuchando... Habla ahora.")
            recognizer.adjust_for_ambient_noise(source, duration=0.5)
            audio = recognizer.listen(source)
            
            with open(ARCHIVO_AUDIO, "wb") as f:
                f.write(audio.get_wav_data())
        
        if os.path.exists(ARCHIVO_AUDIO):
            print(" Transcribiendo audio con Whisper...")
            resultado = model_stt.transcribe(ARCHIVO_AUDIO, language="es", fp16=False)
            texto_dicho = resultado["text"].strip()
            print(f"-> Tú dijiste: \"{texto_dicho}\"")
            try: os.remove(ARCHIVO_AUDIO)
            except: pass
        else:
            continue
            
        # Procesamos el texto con la Inteligencia Artificial de Ollama
        print("🧠 Analizando comando con Ollama (Gemma 2b)...")
        accion = consultar_ollama_gemma(texto_dicho)
        print(f"🤖 Decisión de la IA: {accion}")
        
        # --- ENVÍO DE ÓRDENES REALES AL ARDUINO ---
        if arduino:
            if accion == "[PIANO]":
                arduino.write(b"PIANO\n")
                print(">> Cable USB: Comando [PIANO] enviado.")
            elif accion == "[CONSOLA]":
                arduino.write(b"CONSOLA\n")
                print(">> Cable USB: Comando [CONSOLA] enviado.")
            elif accion == "[DINO]":
                arduino.write(b"DINO\n")
                print(">> Cable USB: Comando [DINO] enviado.")
            elif accion == "[SIMON]":
                arduino.write(b"SIMON\n")
                print(">> Cable USB: Comando [SIMON] enviado.")
            elif accion == "[MENU]":
                arduino.write(b"MENU\n")
                print(">> Cable USB: Comando [MENU] enviado.")
        else:
            print("⚠️ El comando no se envió físicamente porque Arduino no está conectado.")
            
except KeyboardInterrupt:
    print("\nAsistente finalizado correctamente.")
