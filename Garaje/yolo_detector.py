import cv2
import torch
import serial
import time
import requests

# Configuración
ESP32_IP = "192.168.1.XXX"  # Cambiar por IP real
USE_SERIAL = False          # Si True, usa puerto serie; si False, usa HTTP
SERIAL_PORT = "COM5"        # Puerto serial del ESP32 (si aplica)
BAUD_RATE = 115200

# Cargar modelo YOLO (puedes usar 'yolov8n.pt' de Ultralytics)
model = torch.hub.load('ultralytics/yolov5', 'yolov5s', pretrained=True)

cap = cv2.VideoCapture(0)   # Cámara web
if not cap.isOpened():
    print("Error: No se pudo abrir la cámara")
    exit()

# Contador de autos (simula espacios ocupados)
MAX_CARS = 8
last_count = -1

def send_command_to_esp32(car_count):
    # Envía el número de autos detectados al ESP32
    # Opción 1: HTTP GET
    try:
        requests.get(f"http://{ESP32_IP}/yolo?cars={car_count}", timeout=1)
        print(f"Enviado: {car_count} autos")
    except:
        print("Error HTTP")
    # Opción 2: Serial (si ESP32 escucha)
    if USE_SERIAL:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        ser.write(f"CARS:{car_count}\n".encode())
        ser.close()

print("Iniciando detección de autos. Presiona 'q' para salir.")
while True:
    ret, frame = cap.read()
    if not ret:
        break
    results = model(frame)
    df = results.pandas().xyxy[0]
    cars = df[df['name'] == 'car']
    car_count = min(len(cars), MAX_CARS)
    # Mostrar en frame
    for _, row in cars.iterrows():
        x1, y1, x2, y2 = int(row['xmin']), int(row['ymin']), int(row['xmax']), int(row['ymax'])
        cv2.rectangle(frame, (x1,y1), (x2,y2), (0,255,0), 2)
        cv2.putText(frame, f"auto", (x1,y1-5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,255,0),2)
    cv2.putText(frame, f"Autos detectados: {car_count}/{MAX_CARS}", (10,30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,0,255),2)
    cv2.imshow("YOLO Garaje", frame)
    if car_count != last_count:
        send_command_to_esp32(car_count)
        last_count = car_count
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
