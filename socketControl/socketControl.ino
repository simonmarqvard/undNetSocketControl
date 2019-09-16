#include <SPI.h>
#include <WiFi101.h>

const char ssid[] = "";       //  your network SSID (name)
const char pass[] = "";   // your network password

WiFiClient client;                // instance of the WIFi client library
int status = WL_IDLE_STATUS;      // WiFi status
IPAddress server(128,122,6,155);       // address of the server

const int wifiConnectedLED = 11;
const int serverConnectLED = 10;
const int connectButton = 5;      // the pushbutton for connecting/disconnecting
const int connectionLED = 6;      // this LED indicates whether you're connected

const int sendInterval = 100;     // minimum time between messages to the server
const int debounceInterval = 15;  // used to smooth out pushbutton readings
int prevButtonState = 0;          // previous state of the pushbutton
long lastTimeSent = 0;            // timestamp of the last server message

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  // initialize digital inputs and outputs:
  pinMode(wifiConnectedLED, OUTPUT);
  pinMode(serverConnectLED, OUTPUT);
  pinMode(connectionLED, OUTPUT);
  pinMode(serverConnectLED, OUTPUT);
  pinMode(connectButton, INPUT_PULLUP);
  while (!Serial); // wait for serial port to connect.

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);     // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    digitalWrite(wifiConnectedLED, LOW);
    // wait 3 seconds for connection:
    delay(3000);
  }

  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop() {

  // message to send to the server:
  char message = 0;
  // note the current time in milliseconds:
  long now = millis();
  // check to see if the pushbutton's pressed:
  boolean buttonPushed = buttonRead(connectButton);


  // if the button's just pressed:
  if (buttonPushed) {
    // if the client's connected, disconnect:
    if (client.connected()) {
      Serial.println("disconnecting");
      client.print("x");
      client.stop();
    } // if the client's disconnected, try to connect:
    else {
      Serial.println("connecting");
      client.connect(server, 8080);
      client.print("n=SimonJensen\n");
    }
  }


//   client.connect(server, 8080);

//  // read the joystick:
  //could have been a digital read instead because these are buttons
  int sensorValue = analogRead(A1);
  int sensorValue2 = analogRead(A2);
  //must be analog
  int sensorValue3 = analogRead(A6);


  if (sensorValue == 1023) {
    Serial.println("l");
    client.print("l");
    delay(50);
  }
  if (sensorValue2 == 1023) {
    Serial.println("r");
    client.print("r");
    delay(50);
  }

  if (sensorValue3 > 950 ) {
    Serial.println("u");
    client.print("u");
    delay(50);
  }
  else if (sensorValue3 < 200) {
    Serial.println("d");
    client.print("d");
    delay(50);
  }

  if (client.connected()                     // if the client's connected
      && now - lastTimeSent > sendInterval   // and the send interval has elapased
      && message != 0) {                     // and there's a message to send
//    client.print(message);                   // send a message
//    Serial.print(message);
    //save this moment as last time you sent a message:
    lastTimeSent = now;
  }

  // set the connection LED based on the connection state:
  digitalWrite(serverConnectLED, client.connected());

  // if there's incoming data from the client, print it serially:
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
}

// this method reads the button to see if it's just changed
// from high to low, and debounces the button:
boolean buttonRead(int thisButton) {
  boolean result = false;
  int currentState = digitalRead(thisButton); // current button state

  if (currentState != prevButtonState         // if the button's changed
      && currentState == LOW) {               // and it's low
    result = true;                            // result is true
  }

  delay(debounceInterval);                    // debounce delay
  prevButtonState = currentState;             // save the button state for next time
  return result;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // When you connect to wifi turn on light
  digitalWrite(wifiConnectedLED, HIGH);
}
