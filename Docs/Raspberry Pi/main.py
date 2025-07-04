try:
  import usocket as socket
except:
  import socket

import network
from machine import Pin, I2C


ssid = 'Maxwell'   
password = 'a1b2c3d7'

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass

print('Conexion correcta')
print(station.ifconfig())

bme = ("21","130","0.7")


def leer_sensor():
  global temp, hum, pres,bme
  temp = bme[0]
  pres = bme[1]
  hum = bme[2]
  return()

def pagina_web():
  html = """<!DOCTYPE HTML><html>
<head>
  <meta http-equiv=\"refresh\" content=\"10\">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 2.0rem; }
    p { font-size: 2.0rem; }
    .units { font-size: 1.2rem; }
    .bme-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Estación 
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="bme-labels">Temperatura:</span> 
    <span>"""+str(temp)+"""</span>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="bme-labels">Humedad:</span>
    <span>"""+str(hum)+"""</span>
  </p>
  <p>
    <i class="fas fa-tachometer-alt"></i>
    <span class="bme-labels">Presi&oacute;n:</span>
    <span>"""+str(pres)+"""</span>
  </p>
</body>
</html>"""
  return html

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
s.listen(5)

while True:
  conexion, direccion = s.accept()
  request = conexion.recv(1024)
  leer_sensor()
  respuesta = pagina_web()
  conexion.send('HTTP/1.1 200 OK\n')
  conexion.send('Content-Type: text/html\n')
  conexion.send('Connection: close\n\n')
  conexion.sendall(respuesta)
  conexion.close()