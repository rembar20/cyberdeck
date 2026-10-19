Import("env")
import time
import serial
from serial.tools import list_ports

def before_upload(source, target, env):
    print("Auto-reset: Szukam aktywnego portu Pico, aby wysłać komendę resetu...")
    
    target_port = None
    ports = list_ports.comports()
    for port in ports:
        if "COM" in port.device or "ttyACM" in port.device:
            target_port = port.device
            break

    if not target_port:
        print("Auto-reset: Nie znaleziono portu COM! Spróbuj wcisnąć BOOTSEL ręcznie.")
        return

    print(f"Auto-reset: Znaleziono port {target_port}. Wysyłam komendę 'firmware_update'...")
    try:
        ser = serial.Serial(target_port, 115200, timeout=1)
        time.sleep(0.2)
        
        ser.write(b"firmware_update\n")
        time.sleep(0.5)
        ser.close()
        
        print("Auto-reset: Komenda wysłana, czekam na przełączenie w tryb BOOTSEL...")
        time.sleep(2.0)
    except Exception as e:
        print(f"Auto-reset: Błąd komunikacji z portem ({e}).")

env.AddPreAction("upload", before_upload)