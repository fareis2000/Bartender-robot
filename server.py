from flask import Flask, send_from_directory
import serial
import time
from serial.serialutil import SerialException

# Ajuste aqui a porta do HC-05:
PORTA = "COM9"  # substitua pelo número da sua porta
BAUD = 9600

arduino = None

def conectar_arduino():
    global arduino
    try:
        if arduino is not None:
            arduino.close()
        arduino = serial.Serial(PORTA, BAUD, timeout=1)
        time.sleep(2)
        print("Conectado com sucesso!")
        return True
    except SerialException as e:
        print(f"Erro ao conectar: {e}")
        return False

# Tenta conectar inicialmente
print(f"Conectando ao HC-05 em {PORTA}...")
conectar_arduino()

app = Flask(__name__, static_folder='static')

@app.route('/')
def home():
    return send_from_directory(app.static_folder, 'index.html')

def enviar_comando(comando):
    global arduino
    try:
        if arduino is None or not arduino.is_open:
            print(f"Tentando reconectar ao Arduino em {PORTA}...")
            if not conectar_arduino():
                print("Falha na reconexão!")
                return "Erro: Dispositivo não conectado"
        
        print(f"Enviando comando: {comando}")
        arduino.write(comando.encode() + b'\n')
        arduino.flush()  # Garante que os dados sejam enviados
        
        # Aguarda resposta com timeout
        timeout = time.time() + 2.0  # 2 segundos de timeout
        while time.time() < timeout:
            if arduino.in_waiting:
                resposta = arduino.readline().decode().strip()
                print(f"Resposta do Arduino: {resposta}")
                return "Comando enviado e resposta recebida: " + resposta
            time.sleep(0.1)
            
        print("Timeout: Nenhuma resposta do Arduino")
        return "Timeout na resposta do Arduino"
        
    except Exception as e:
        print(f"Erro ao enviar comando: {e}")
        arduino = None
        return f"Erro ao enviar comando: {str(e)}"

@app.route('/soda/italiana')
def soda_italiana():
    return enviar_comando('SODA_ITALIANA')

@app.route('/b1/off')
def b1_off():
    return enviar_comando('B1_OFF')

@app.route('/b2/on')
def b2_on():
    return enviar_comando('B2_ON')

@app.route('/b2/off')
def b2_off():
    return enviar_comando('B2_OFF')

if __name__ == '__main__':
    print(f"Iniciando servidor na porta 5000...")
    print(f"Configuração Serial: Porta={PORTA}, Baud={BAUD}")
    app.run(host='0.0.0.0', port=5000, debug=False)
