#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ESP32Servo.h>

Servo SERVO_F;
Servo SERVO_P;
Servo SERVO_M;
Servo SERVO_LR;
Servo SERVO_B;

#define SERVO_F_PIN 13
#define SERVO_P_PIN 14
#define SERVO_M_PIN 26
#define SERVO_LR_PIN 33
#define SERVO_B_PIN 18

int SERVO_F_POS = 0;
int SERVO_P_POS = 0;
int SERVO_M_POS = 0;
int SERVO_LR_POS = 0;
int SERVO_B_POS = 0;

#define FORMAT_SPIFFS_IF_FAILED true

// Replace with your network credentials
const char* ssid = "Black_hat:)";
const char* password =  "1010101010";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void servo_balance(Servo &SERVO_, int &CRNT_POS);
void servo_control(Servo &SERVO_, int CRNT_POS);

const char index_html[] PROGMEM = R"rawliteral(
<html lang="en">

<head>
   <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Robotic hand</title>
    <style>
            body {
            display: flex;
            flex-direction: column;
            text-align: center;
            justify-content: center;
        }

        .main {
            display: flex;
            flex-direction: column;
            align-self: center;
            justify-content: center;
            align-items: center;
            min-height: 100%;
            width: 100%;
        }

        button,
        input {
            width: 25vw;
            /* height: 10vh; */
            width: 100%;
        }

        label {
            display: block;
            color: #9c8d9d;
        }

        h3 {
            color: #adadad;
            text-align: center;
            margin: 0;
        }

        .row {
            text-align: left;
            width: 100%;
            box-shadow: 0 4px 10px 0 rgb(0 0 0 / 20%), 0 4px 20px 0 rgb(0 0 0 / 19%);
            margin: 1em;
            padding: 1em;
        }
    </style>
</head>

<body>

    <div class="main">
        <div class="row">
            <h3>Robotic Hand V1</h3>
        </div>
        <div class="row">
            <label for="">FINGER</label>
            <input type="range" min="0" max="180" class="slider" onchange="movement('F',this.value)" />
        </div>
        <div class="row">
            <label for="">PALM</label>
            <input type="range" min="0" max="180" class="slider" onchange="movement('P',this.value)" />
        </div>
        <div class="row">
            <label for="">MIDDLE</label>
            <input type="range" min="0" max="180" class="slider" onchange="movement('M',this.value)" />
        </div>
        <div class="row">
            <label for="">SHOULDER</label>
            <input type="range" min="0" max="180" class="slider" onchange="movement('LR',this.value)" />
        </div>
        <div class="row">
            <label for="">BASE</label>
            <input type="range" min="0" max="180" class="slider" onchange="movement('B',this.value)" />
        </div>
    </div>

    <script>
        function movement(joint,directions) {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    //console.log(this.responseText);
                }
            };
            xhttp.open("GET", "/move?joint=" + joint + "&directions=" + directions, true);
            xhttp.send();
        }
    </script>
</body>

</html>
)rawliteral";


void setup(){

  SERVO_F.attach(SERVO_F_PIN);
  SERVO_P.attach(SERVO_P_PIN);
  SERVO_M.attach(SERVO_M_PIN);
  SERVO_LR.attach(SERVO_LR_PIN);
  SERVO_B.attach(SERVO_B_PIN);

    // Serial port for debugging purposes
  Serial.begin(115200);

  servo_balance(SERVO_F, SERVO_F_POS);
  servo_balance(SERVO_P, SERVO_P_POS);
  servo_balance(SERVO_M, SERVO_M_POS);
  servo_balance(SERVO_LR, SERVO_LR_POS);
  servo_balance(SERVO_B, SERVO_B_POS);
  
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

server.on("/move", HTTP_GET, [] (AsyncWebServerRequest *request) {
  String joint;
  String directions;
  int directions_;
  if (request->hasParam("joint")) {
    joint = request->getParam("joint")->value();
    directions = request->getParam("directions")->value();
    directions_ = directions.toInt();

    Serial.print(joint);
    Serial.print(" & ");
    Serial.println(directions);
    
    //int move_state = state.toInt();

    if(joint.equals("F")) { servo_control(SERVO_F, directions_); }
    if(joint.equals("P")) { servo_control(SERVO_P, directions_); }
    if(joint.equals("M")) { servo_control(SERVO_M, directions_); }
    if(joint.equals("B")) { servo_control(SERVO_B, directions_); }
    if(joint.equals("LR")) { servo_control(SERVO_LR, directions_); }
    
  }
  request->send(200, "text/plain", "OK");
  
});

  // Start server
  server.begin();
}

void loop() {

}
