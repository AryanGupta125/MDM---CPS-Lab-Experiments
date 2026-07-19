import serial
import threading
import time
from flask import Flask, render_template_string
from flask_socketio import SocketIO

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

# Configure your Arduino Serial Port here
SERIAL_PORT = 'COM6'  # Change to your actual Arduino COM port
BAUD_RATE = 9600

# HTML Interface Template
HTML_TEMPLATE = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Arduino Gesture Monitor</title>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/socket.io/4.0.1/socket.io.js"></script>
    <style>
        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; text-align: center; background-color: #f4f6f9; padding: 50px; }
        .container { max-width: 600px; margin: auto; background: white; padding: 30px; border-radius: 12px; box-shadow: 0 4px 15px rgba(0,0,0,0.1); }
        h1 { color: #333; }
        #gesture-display { font-size: 2.5rem; font-weight: bold; color: #007bff; margin: 30px 0; min-height: 60px; transition: all 0.3s ease; }
        #log { background: #222; color: #00ff00; height: 150px; overflow-y: scroll; padding: 10px; border-radius: 6px; text-align: left; font-family: monospace; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Live Gesture Control Dashboard</h1>
        <hr>
        <div id="gesture-display">Waiting for Gesture...</div>
        <h3>Live Data Stream:</h3>
        <div id="log"></div>
    </div>

    <script>
        const socket = io();
        const display = document.getElementById('gesture-display');
        const logBox = document.getElementById('log');

        socket.on('gesture_update', function(data) {
            // Update the prominent text display
            display.innerText = data.data;
            
            // Log with timestamp
            const timestamp = new Date().toLocaleTimeString();
            logBox.innerHTML += `[${timestamp}] ${data.data}<br>`;
            logBox.scrollTop = logBox.scrollHeight; // Auto-scroll to bottom
            
            // Brief visual pop effect
            display.style.transform = "scale(1.1)";
            setTimeout(() => { display.style.transform = "scale(1)"; }, 150);
        });
    </script>
</body>
</html>
"""

@app.route('/')
def index():
    return render_template_string(HTML_TEMPLATE)

def read_from_serial():
    """Background thread to read data from Arduino and emit via WebSockets"""
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to Arduino on {SERIAL_PORT}")
        time.sleep(2) # Give the connection an instant to initialize
        
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    print(f"Arduino says: {line}")
                    # Broadcast the data straight to the web browser interface
                    socketio.emit('gesture_update', {'data': line})
            time.sleep(0.01)
    except Exception as e:
        print(f"Serial Error: {e}. Check your COM port configuration.")

if __name__ == '__main__':
    # Start the Serial background thread so it doesn't block the web server
    serial_thread = threading.Thread(target=read_from_serial, daemon=True)
    serial_thread.start()
    
    # Run the web application locally
    socketio.run(app, host='0.0.0.0', port=5000, debug=False)