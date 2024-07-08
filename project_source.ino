/*
 * IoT Multipurpose Smart Home/Office/Industrial Automation/Monitoring System
 * Developed By Atiqur Rahman Aakash
 * University of Development Alternative
 * Email: aakash.unipune@gmail.com
 */

#include <ESP8266WiFi.h>   // Library for ESP8266 WiFi module
#include <DHT.h>           // Library for DHT temperature and humidity sensor
#include <SoftwareSerial.h> // Library for SoftwareSerial communication

// WiFi Credentials
const char* ssid = "Home Router";    // WiFi network SSID
const char* password = "homepassword";  // WiFi network password

// IFTTT Configuration
const char* host = "maker.ifttt.com"; // IFTTT Maker service endpoint
const char* motion_event = "motion";  // IFTTT event name for motion detection
const char* motion_key = "v9UQmpwdiotyY_jKNy0eY";  // IFTTT secret key for motion event
const char* flame_event = "flame";    // IFTTT event name for flame detection
const char* flame_key = "v9UQmpwdiotyY_jKNy0eY";  // IFTTT secret key for flame event
const char* temp_event = "temperature";  // IFTTT event name for temperature detection
const char* temp_key = "v9UQmpwdiotyY_jKNy0eY";  // IFTTT secret key for temperature event
const char* gas_event = "gas";        // IFTTT event name for gas detection
const char* gas_key = "v9UQmpwdiotyY_jKNy0eY";    // IFTTT secret key for gas event
const char* vibe_event = "vibration";  // IFTTT event name for vibration detection
const char* vibe_key = "v9UQmpwdiotyY_jKNy0eY";  // IFTTT secret key for vibration event

// Pin Definitions
#define DHTPIN D4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT sensor type

int redLED = D1;        // Red LED pin
int greenLED = D7;      // Green LED pin
int pirSensor = D8;     // PIR motion sensor pin
int flameSensorPin = D3;  // Flame sensor pin
int gasSensorPin = D6;   // Gas sensor pin

// Sensor Values and States
int flameState = LOW;    // State of the flame sensor
int pirState = LOW;      // State of the PIR motion sensor
int pirValue = 0;        // Value read from PIR motion sensor
float humidity, temperature, fahrenheit, heatIndexF, heatIndexC, dewPoint;  // Variables for sensor data
int gasValue;            // Value read from gas sensor

// Server and Sensor Initialization
WiFiServer server(80);   // Create a WiFi server instance
DHT dht(DHTPIN, DHTTYPE, 30); // Create a DHT sensor instance
SoftwareSerial AA(D1, D2);   // SoftwareSerial instance for SIM900 module

// Function Declarations
void detectMotion();   // Function to detect motion using PIR sensor
void detectTemperature(); // Function to detect temperature and humidity
void detectFlame();    // Function to detect flame using flame sensor
void detectGas();      // Function to detect gas presence using gas sensor
void sendIFTTTEvent(const char* event, const char* key); // Function to send event to IFTTT
double calculateDewPoint(double celsius, double humidity); // Function to calculate dew point
void makePhoneCall();  // Function to initiate a phone call
void blinkLED();       // Function to blink LEDs

// Setup Function
void setup() {
    Serial.begin(115200);   // Initialize serial communication for debugging
    AA.begin(115200);       // Initialize SoftwareSerial for SIM900 module
    delay(1000);            // Short delay for initialization

    // Initialize Pins
    pinMode(flameSensorPin, INPUT); // Set flame sensor pin as input
    pinMode(gasSensorPin, INPUT);   // Set gas sensor pin as input
    pinMode(pirSensor, INPUT);      // Set PIR motion sensor pin as input
    pinMode(redLED, OUTPUT);        // Set red LED pin as output
    pinMode(greenLED, OUTPUT);      // Set green LED pin as output
    digitalWrite(greenLED, LOW);    // Turn off green LED initially

    // Initialize DHT Sensor
    dht.begin();                    // Start DHT sensor

    // Connect to WiFi
    WiFi.mode(WIFI_STA);            // Set WiFi mode to station (client)
    WiFi.begin(ssid, password);     // Connect to WiFi network
    while (WiFi.status() != WL_CONNECTED) { // Wait for WiFi connection
        Serial.print("Connecting to ");
        Serial.print(ssid);
        Serial.println("... Please Wait");
        delay(500);
    }
    Serial.println("WiFi Connected!");   // WiFi connected
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());     // Print IP address
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());  // Print MAC address

    // Start Server
    server.begin();       // Start the server
    Serial.println("Server Started"); // Print server started message
}

// Main Loop
void loop() {
    delay(100); // Short delay for stability

    detectMotion();       // Check for motion detection
    detectGas();          // Check for gas detection
    detectTemperature();  // Check for temperature and humidity
    detectFlame();        // Check for flame detection

    // Check for client connection
    WiFiClient client = server.available();
    if (client) {
        String html = generateHTMLPage(); // Generate HTML page
        client.print(html); // Send HTML page to client
        delay(1); // Short delay
        client.stop(); // Disconnect client
    }
}

// Motion Detection
void detectMotion() {
    pirValue = digitalRead(pirSensor); // Read PIR motion sensor value
    if (pirValue == HIGH) { // If motion detected
        if (pirState == LOW) { // Check if motion state changed
            Serial.println("Motion Detected!"); // Print motion detected message
            blinkLED(); // Blink LEDs
            makePhoneCall(); // Initiate phone call
            sendIFTTTEvent(motion_event, motion_key); // Send IFTTT event
            pirState = HIGH; // Update motion state
        }
    } else { // If no motion detected
        digitalWrite(greenLED, LOW); // Turn off green LED
        analogWrite(redLED, LOW);    // Turn off red LED
        if (pirState == HIGH) { // Check if motion state changed
            Serial.println("No Motion Detected"); // Print no motion message
            pirState = LOW; // Update motion state
        }
    }
}

// Temperature and Humidity Detection
void detectTemperature() {
    humidity = dht.readHumidity(); // Read humidity from DHT sensor
    temperature = dht.readTemperature(); // Read temperature from DHT sensor
    fahrenheit = dht.readTemperature(true); // Read temperature in Fahrenheit
    heatIndexC = dht.computeHeatIndex(temperature, humidity, false); // Calculate heat index in Celsius
    heatIndexF = dht.computeHeatIndex(fahrenheit, humidity); // Calculate heat index in Fahrenheit
    dewPoint = calculateDewPoint(temperature, humidity); // Calculate dew point

    if (isnan(humidity) || isnan(temperature) || isnan(fahrenheit)) { // Check for DHT sensor error
        Serial.println("DHT Sensor Error!"); // Print sensor error message
        return; // Exit function
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C | Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    if (temperature > 30) { // If temperature exceeds threshold
        blinkLED(); // Blink LEDs
        makePhoneCall(); // Initiate phone call
        sendIFTTTEvent(temp_event, temp_key); // Send IFTTT event
    }
    delay(1000); // Short delay
}

// Flame Detection
void detectFlame() {
    flameState = digitalRead(flameSensorPin); // Read flame sensor state
    if (flameState == HIGH) { // If no flame detected
        Serial.println("No Flame Detected"); // Print no flame message
    } else { // If flame detected
        Serial.println("Flame Detected!"); // Print flame detected message
        blinkLED(); // Blink LEDs
        makePhoneCall(); // Initiate phone call
        sendIFTTTEvent(flame_event, flame_key); // Send IFTTT event
    }
    delay(1000); // Short delay
}

// Gas Detection
void detectGas() {
    gasValue = digitalRead(gasSensorPin); // Read gas sensor value
    Serial.println(gasValue); // Print gas sensor value
    if (gasValue > 0) { // If gas detected
        Serial.println("Gas Detected!"); // Print gas detected message
        blinkLED(); // Blink LEDs
        makePhoneCall(); // Initiate phone call
        sendIFTTTEvent(gas_event, gas_key); // Send IFTTT event
    } else { // If no gas detected
        Serial.println("No Gas Detected"); // Print no gas message
    }
    delay(1000); // Short delay
}

// Function to send event to IFTTT
void sendIFTTTEvent(const char* event, const char* key) {
    String url = "/trigger/"; // IFTTT endpoint URL
    url += event; // Append event name
    url += "/with/key/"; // Append secret key
    url += key; // Append secret key

    // HTTP request to IFTTT
    Serial.print("Connecting to IFTTT... ");
    WiFiClient client;
    if (client.connect(host, 80)) { // If connected to IFTTT server
        Serial.println("Connected!");
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");
        delay(1000); // Short delay
        Serial.println("IFTTT Event Sent!");
    } else { // If connection to IFTTT failed
        Serial.println("Failed to connect to IFTTT");
    }
}

// Function to calculate dew point
double calculateDewPoint(double celsius, double humidity) {
    double A0 = 373.15 / (273.15 + celsius);
    double SUM = -7.90298 * (A0 - 1);
    SUM += 5.02808 * log10(A0);
    SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / A0))) - 1);
    SUM += 8.1328e-3 * (pow(10, (-3.49149 * (A0 - 1))) - 1);
    SUM += log10(1013.246);

    double VP = pow(10, SUM - 3) * humidity;
    double Td = log(VP / 0.61078);
    return (241.88 * Td) / (17.558 - Td);
}

// Function to initiate a phone call
void makePhoneCall() {
    AA.println("ATD +88 01711234567;"); // Replace with your phone number
    delay(30000); // Wait for 30 seconds (adjust as necessary)
    AA.println("ATH"); // Hang up call
}

// Function to blink LEDs
void blinkLED() {
    digitalWrite(greenLED, HIGH); // Turn on green LED
    analogWrite(redLED, 512);     // Turn on red LED (adjust brightness as necessary)
    delay(500);                   // Wait for 0.5 seconds
    digitalWrite(greenLED, LOW);  // Turn off green LED
    analogWrite(redLED, LOW);     // Turn off red LED
}

// Function to generate HTML page
String generateHTMLPage() {
    String html = "<html><body>";
    html += "<h1>IoT Monitoring System</h1>";
    html += "<p>Current Status:</p>";
    html += "<ul>";
    html += "<li>Motion Sensor: ";
    html += pirState == HIGH ? "Motion Detected" : "No Motion Detected";
    html += "</li>";
    html += "<li>Temperature: ";
    html += String(temperature);
    html += " °C</li>";
    html += "<li>Humidity: ";
    html += String(humidity);
    html += " %</li>";
    html += "<li>Flame Sensor: ";
    html += flameState == HIGH ? "No Flame Detected" : "Flame Detected";
    html += "</li>";
    html += "<li>Gas Sensor: ";
    html += gasValue > 0 ? "Gas Detected" : "No Gas Detected";
    html += "</li>";
    html += "</ul></body></html>";
    return html;
}
