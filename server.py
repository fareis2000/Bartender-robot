from flask import Flask, send_from_directory
import serial
import time
from serial.serialutil import SerialException

# ================= CONFIGURAÇÕES =================
PORTA = "COM9"
BAUD = 9600

arduino = None
drink_em_execucao = False

# ================= CONEXÃO ARDUINO ==============
def conectar_arduino():
    global arduino
    try:
        if arduino is not None and arduino.is_open:
            arduino.close()

        arduino = serial.Serial(PORTA, BAUD, timeout=1)
        time.sleep(2)
        print("✅ Conectado ao Arduino com sucesso")
        return True

    except SerialException as e:
        print(f"❌ Erro ao conectar ao Arduino: {e}")
        arduino = None
        return False


print(f"🔌 Tentando conectar ao Arduino em {PORTA}...")
conectar_arduino()

# ================= FLASK =========================
app = Flask(__name__, static_folder='static')

@app.route('/')
def home():
    return send_from_directory(app.static_folder, 'index.html')

# ================= ENVIO DE COMANDOS =============
def enviar_comando(comando):
    global arduino, drink_em_execucao

    # Bloqueia novo pedido
    if drink_em_execucao:
        return "⏳ Aguarde: drink em preparo"

    try:
        # Garante conexão
        if arduino is None or not arduino.is_open:
            print("🔄 Reconectando ao Arduino...")
            if not conectar_arduino():
                return "❌ Erro: Arduino não conectado"

        drink_em_execucao = True
        print(f"➡️ Enviando comando: {comando}")

        arduino.write((comando + '\n').encode())
        arduino.flush()

        timeout = time.time() + 40  # tempo máximo de preparo

        while time.time() < timeout:
            if arduino.in_waiting:
                resposta = arduino.readline().decode().strip()
                print(f"⬅️ Arduino: {resposta}")

                if resposta == "DONE":
                    drink_em_execucao = False
                    return "✅ Drink finalizado com sucesso 🍹"

            time.sleep(0.1)

        drink_em_execucao = False
        return "⚠️ Erro: tempo de preparo excedido"

    except Exception as e:
        drink_em_execucao = False
        arduino = None
        print(f"❌ Erro crítico: {e}")
        return f"Erro ao enviar comando: {str(e)}"

# ================= ROTAS DRINKS ==================
@app.route('/drinks/red')
def soda_italiana():
    return enviar_comando("RED")

@app.route('/drinks/yellow')
def limonada():
    return enviar_comando("YELLOW")

@app.route('/drinks/green')
def frutas_vermelhas():
    return enviar_comando("GREEN")

@app.route('/drinks/mix')
def mix_basics():
    return enviar_comando("MIX")
# ================= START SERVER ==================
if __name__ == '__main__':
    print("🚀 Servidor Flask iniciado")
    print(f"📡 Porta Serial: {PORTA} | Baudrate: {BAUD}")
    app.run(host='0.0.0.0', port=5000, debug=False)
