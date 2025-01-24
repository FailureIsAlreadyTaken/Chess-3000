//  GET method
//
//


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ledVie D0
#define button D5

#ifndef STASSID
#define STASSID "tp5&6"
#define STAPSK "geii2021"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
unsigned intervall = 500;
bool stateButton;

ESP8266WebServer server(80);
LiquidCrystal_I2C lcd(0x3F, 16, 2);


/////////////////////////////// 200 OK //////////////////////////////////////////
void handleRoot() {
  String message = "<html><head> <meta http-equiv='refresh' content='1'><title>Basic Web Server </title> </head> <body> Welcome<br> Button ";
  stateButton = digitalRead(button);
  if (stateButton) {
    message = message + " ON ";
  } else {
    message = message + " OFF ";
  }
  message = message + " </br></body></html>";

  server.send(200, "text/html", message);
}

/////////////////////////////////// Error 404 //////////////////////////////////
void handleNotFound() {
  String message = "<html> Error 404";
  server.send(404, "text/html", message);
}

//////////////////////////////// setup ////////////////////////////////////////
void setup(void) {
  //--------------------------- GPIO LCD SERIAL ------------------------------
  pinMode(ledVie, OUTPUT);  // Initialize the LED_BUILTIN pin as an output
  //pinMode(D5, OUTPUT);
  pinMode(button, INPUT);
  lcd.begin();
  lcd.backlight();
  Serial.begin(115200);

  //----------------------- WIFI connexion -----------------------------------
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  int i = 0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(10, 1);

    switch (i % 3) {
      case 0: lcd.print("|"); break;
      case 1: lcd.print("/"); break;
      case 2: lcd.print("-");
    }
    i++;
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("lolo")) {
    Serial.println("MDNS responder started");
  }
  //--------------------- server configuratiuon -------------------------------
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  lcd.setCursor(0, 0);
  lcd.print("ESP WEB Server");
  lcd.setCursor(0, 1);
}

//////////////////////////// loop ///////////////////////////////////////
void loop(void) {
  digitalWrite(ledVie, HIGH);
  delay(50);
  server.handleClient();
  MDNS.update();
  digitalWrite(ledVie, LOW);
  delay(50);
}
