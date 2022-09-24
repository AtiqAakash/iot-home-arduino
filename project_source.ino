
/* Handy Documentation for Code Developers:
   IoT Multipurpose Smart home/office/industrial automation/monitoring system.
   Developed By Mohammad Atiqur Rahman Aakash
   University of Development Alternative
   Email: aakash.unipune@gmail.com
*/
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
SoftwareSerial AA(D1, D2);
/*===============================================*/
// WiFi Credentials
const char* ssid = "Home Router";
const char* password = "homepassword";
const char *host = "maker.ifttt.com";
const char *motion_event = "motion";
const char *motion_key = "v9UQmpwdiotyY_jKNy0eY";
const char *flame_event = "flame";
const char *flame_key = "v9UQmpwdiotyY_jKNy0eY";
const char *temp_event = "temperature";
const char *temp_key = "v9UQmpwdiotyY_jKNy0eY";
const char *gas_event = "gas";
const char *gas_key = "v9UQmpwdiotyY_jKNy0eY";
const char *vibe_event = "vibration";
const char *vibe_key = "v9UQmpwdiotyY_jKNy0eY";

/*===============================================*/
// NodeMCU pin mapping
/*static const uint8_t D0 = 16;
  static const uint8_t D1 = 5;
  static const uint8_t D2 = 4;
  static const uint8_t D3 = 0;
  static const uint8_t D4 = 2;
  static const uint8_t D5 = 14;
  static const uint8_t D6 = 12;
  static const uint8_t D7 = 13;
  static const uint8_t D8 = 15;
  static const uint8_t D9 = 3;
  static const uint8_t D10 = 1; */
/*===============================================*/
// static IPAddress ip(192, 168, 0, 111); // static IP used for browser access:
// static IPAddress gateway(192, 168, 0, 1);
// static IPAddress subnet(255, 255, 255, 0);
/*===============================================*/
// Functions Declaration
void motion();
void tem(const char *e_temp);
void motions(const char *e_motion);
void flame(const char *e_flame);
void gas(const char *e_gas);
void vibe(const char *e_vibe);
void flame_sense();
void gas_sense();
//long read_vib();
void call();
void bb();

/*===============================================*/
// Definition pins
#define DHTPIN D4 // GPIO -> 2. Can be changed to any other pins.
#define DHTTYPE DHT22
int red = D1;
int green = D7;
int pir = D8;
int FlamePin = D3;
int Flame = LOW;
int gas_pin = D6;
long duration;
long cm;
long inches;
float humid;
float temp;
float fahr;
float hif;
float hic;
float dpt;
int gas_value;
int pir_state = LOW;
int pir_value = 0;
WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE, 30);
/*===============================================*/
// Setup starts here.
/*===============================================*/
// Setup starts here.
void setup() {
  Serial.begin(115200);
  while (!Serial);
  AA.begin(115200);
  delay(1000);
  Serial.println("_____");

  // Defining Pin Modes
  pinMode(FlamePin, INPUT);
  pinMode(gas_pin, INPUT);
  pinMode(pir, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  digitalWrite(green, 0);
  // Initialize DHT sensor
  dht.begin();

  // Wifi mode selection.
  WiFi.mode(WIFI_STA); // Station mode selected.
  // WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println(" Please Wait");
    delay(10);
  }

  Serial.println("Okay, I'm ON!");
  Serial.println("WiFi Connected");
  // Start the server.
  server.begin();
  Serial.println("Bingo.. Server Started");

  //Show Ip Address on serial monitor.
  Serial.println(WiFi.localIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

}

/*===============================================*/
// Main function Starts.
void loop() {
  delay(100);
  /*===============================================*/
  // Motion Sensing
  motion();
  // Gas Sensing
  gas_sense();
  // Temperature Sensing

  // Temperature and humidity section.
  float humid = dht.readHumidity();
  float temp = dht.readTemperature();
  delay(10);
  if (temp > 30)
  {
    bb();
    Serial.println("Calling for temperature detection");
    call();
    tem(temp_event);
  }
  float fahr = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(fahr, humid); // For fahrenheight
  float hic = dht.computeHeatIndex(temp, humid, false); // For Celcius
  float dpt = dewPointFast(temp, humid);

  // DHT availability.
  if (isnan(humid) || isnan(temp) || isnan(fahr))
  {
    Serial.println(" DHT not connected");
    return;
  }
  Serial.print("Temperature :");
  Serial.print(temp);
  Serial.print("°C");
  Serial.print("|| Humidity ");
  Serial.print(humid);
  Serial.println("%");
  delay(1000);
  //Flame Sensing
  flame_sense();
  // Checking if connected or not.
  WiFiClient client = server.available();
  if (!client)
    return;


  /*===============================================*/
  // Page Development.
  String html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  html += "<!DOCTYPE html>";
  html += "<head><meta charset=\"UTF-8\">";
  html += "<meta http-equiv=\"refresh\" content=\"30\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
  html += "<link rel=\"icon\" type=\"image/x-icon\" href = \"https://www.ald.softbankrobotics.com/sites/aldebaran/themes/aldebaran/favicon.ico\">";
  html += "<title>Aakash-IOT</title>";
  html += "<link rel=\"stylesheet\" href = \"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">";
  html += "<script src = \"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<div class='progress'>";
  html += "<div class='progress-bar progress-bar-striped active' role='progressbar' aria-valuenow='' aria-valuemin='0' aria-valuemax='100' style='width:100%'>";
  html += "<MARQUEE BEHAVIOR=ALTERNATE>Developped By Aakash</MARQUEE><br><br>";
  html += "</div></div></div>";
  html += "<div class=\"container\">";
  html += "<div class=\"panel panel-default\" margin:25px>";
  html += "<div class=\"panel-heading\">";
  html += "<div class=\"col-md-6\"><input class=\"btn btn-block btm-md btn-success\" type=\"button\" value=\"Go To Feedback Panel\" onclick=location.href=\"http://192.168.43.217\" ></div><br><br>";
  html += "<div class=\"container\">";
  html += "<div class=\"panel panel-default\" margin:5px>";
  html += "<div class=\"panel-heading\">";
  html += "<H2 style=\"font-family:robot; color:#ed4917\"><center>WEATHER INFO</center></H2>";
  html += "<div class=\"panel-body\" style=\"background-color: #518DC1\">";
  html += "<pre>";
  html += "Humidity : ";
  html += humid;
  html += " %\n";
  html += "Temperature : ";
  html += temp;
  html += " °C\n";
  html += "Temperature : ";
  html += fahr;
  html += " °F\n";
  html += "Heat Index : ";
  html += hic;
  html += " °C || ";
  html += hif;
  html += " °F\n";
  html += "Dew PointFast : ";
  html += dpt;
  html += " °C\n<br>";
  html += "</pre></div></div></div></div>";
  html += "</body></html>";
  client.print(html); /* Print out the page */
  delay(1);
  Serial.println("Running");
  client.stop();

}

void tem(const char *e_temp) {
  WiFiClient client;
  int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/trigger/";
  url += temp_event;
  url += "/with/key/";
  url += temp_key;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.
  while (client.connected())
  {
    if (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    else
    {
      // No data yet, wait a bit
      delay(50);
    }
  }
}

void motion() {
  pir_value = digitalRead(pir); // read sensor pir_valueue
  if (pir_value == HIGH)
  { // check if the sensor is HIGH
    if (pir_state == LOW)
    {
      Serial.println("Motion detected!");
      bb();
      Serial.println("Calling for motion detection");
      call();
      motions(motion_event);
      pir_state = HIGH; // update variable pir_state to HIGH
    }
  }
  else
  {
    analogWrite(red, 0);
    digitalWrite(green, 0);
    if (pir_state == HIGH)
    {
      Serial.println("No Motion Detected!");
      pir_state = LOW; // update variable pir_state to LOW
    }
  }
}

// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity) {
  double a = 17.271;
  double b = 237.7;
  double t = (a * celsius) / (b + celsius) + log(humidity * 0.01);
  double Td = (b * t) / (a - t);
  return Td;
}
// Flame sensing function
void flame_sense() {
  Flame = digitalRead(FlamePin);
  if (Flame == HIGH)
    Serial.println("No Flame");
  else
  {
    bb();
    Serial.println("Fire!!!!");
    Serial.println("Calling for fire detection");
    call();
    flame(flame_event);
  }
  delay(1000);
}
// Gas sensing function
void gas_sense() {
  int gas_value = digitalRead(gas_pin);
  Serial.println(gas_value);
  if (gas_value > 0)
  {
    bb();
    Serial.println("Gas Available");
    Serial.println("Calling for gas detection");
    call();
    gas(gas_event);
  }
  else
    Serial.println("Gas Not Available");
  delay(1000);
}




void motions(const char *e_motion) {

  WiFiClient client;
  int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/trigger/";
  url += motion_event;
  url += "/with/key/";
  url += motion_key;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.
  while (client.connected())
  {
    if (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    else
    {
      // No data yet, wait a bit
      delay(50);
    }
  }
}

void flame(const char *e_flame) {
  WiFiClient client;
  int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/trigger/";
  url += flame_event;
  url += "/with/key/";
  url += flame_key;

  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.
  while (client.connected())
  {
    if (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    else
    {
      // No data yet, wait a bit
      delay(50);
    }
  }
}

void gas(const char *e_gas) {
  WiFiClient client;
  int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/trigger/";
  url += gas_event;
  url += "/with/key/";
  url += gas_key;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.
  while (client.connected())
  {
    if (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    else
    {
      // No data yet, wait a bit
      delay(50);
    }
  }
}

void vibe(const char *e_vibe) {
  WiFiClient client;
  int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/trigger/";
  url += vibe_event;
  url += "/with/key/";
  url += vibe_key;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.
  while (client.connected())
  {
    if (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    else
    {
      // No data yet, wait a bit
      delay(50);
    }
  }
}

void call() {
  Serial.println();
  Serial.println("Making A Call");
  AA.println("AT");
  delay(1000);
  AA.println("ATD01521207874");
  delay(20000);
  AA.println("ATH");
}

void bb() {
  int i;
  digitalWrite(green, 1);
  for (i = 0; i < 20; i++) {
    analogWrite(red, 500);
    delay(400);
    analogWrite(red, 0);
    delay(400);
  }
}





