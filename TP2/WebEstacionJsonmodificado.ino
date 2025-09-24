#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define pinLed 5
#define pinTemp 21
#define pinSensor 22
#define pinSonido 23
#define DHTTYPE DHT11

#define NombreRed "WIFI-FRH"
#define ClaveRed  "eslamismadesiempre"

WebServer servidor(80); 
DHT dht(pinTemp, DHTTYPE); 

bool ledState = false;

void setup() 
{
  Serial.begin(115200);            
  pinMode(pinLed, OUTPUT);

  WiFi.mode(WIFI_STA);
  Serial.println("Conectando a la red...");
  
  WiFi.begin(NombreRed, ClaveRed);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.print("Conectado a la red WiFi ");
  Serial.println(NombreRed);
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  //definir rutas que serán atendidas por el servidor web mediante punteros a función
  servidor.on("/", handleRaiz);
  servidor.on("/on", handleLedOn);
  servidor.on("/off", handleLedOff);
  servidor.onNotFound(handleNoEncontrado); //por defecto si viene cualquier ruta que no reconoce

  if (!MDNS.begin("esp32pablo")) 
  {
     Serial.println("Error al configurar el DNS");
  }

  //iniciar servidor
  servidor.begin();
}

void loop() 
{
  servidor.handleClient();
}


// Página principal

```cpp
void handleRaiz() 
{
String estadoLed;
String puerta;
String movimiento;
String luz;

 if (ledState)
   estadoLed = "Encendido";  
 else
  estadoLed = "Apagado";  

if (digitalRead(pinSensor))
  puerta = "true";
else
  puerta = "false";

  long temperatura = random (22, 40);
  long humedad = random (30,90);
  
  if (temperatura%2)
    movimiento = "true";
  else
    movimiento = "false";
  
  if (digitalRead(pinLed))
    luz = "true";
  else
    luz = "false";

  String json = "{";
  json += "\"luzEncendida\": " + luz + ",";
  json += "\"temperatura\": " + String(temperatura) + ",";
  json += "\"humedad\": " + String(humedad) + ",";
  json += "\"movimiento\": " + movimiento + ",";
  json += "\"puertaAbierta\": " + puerta;
  json += "}";

  servidor.sendHeader("Access-Control-Allow-Origin", "*");
  servidor.send(200, "application/json", json);

  //servidor.send(200, "text/html", html);
}
```


void handleLedOn() {
  digitalWrite(pinLed,HIGH);
  ledState = true;
  handleRaiz(); // volver a la página principal
}

// Apagar LED
void handleLedOff() {
  digitalWrite(pinLed,LOW);
  ledState = false;
  handleRaiz(); // volver a la página principal
}

// Manejar 404
void handleNoEncontrado() {
  servidor.send(404, "text/plain", "Ruta no encontrada");
}
