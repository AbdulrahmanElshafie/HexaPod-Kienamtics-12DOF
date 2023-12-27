#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>
#include <localization_inferencing.h>

// Define the PCA9685 PWM controller
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// PWM parameters
#define SERVOMIN  150  // Minimum pulse length
#define SERVOMAX  600  // Maximum pulse length
#define SERVO_FREQ 60   // Analog servos run at 60 Hz updates

const char *ssid = "Ankb 28";
const char *password = "12345678";

int defaultAngle = 90, riseAngle_4_6 = 180, riseAngle_1_3 = 0, rollAngle1 = 120, rollAngle2 = 60,
timeDelay = 700;

// Variables definitions:
// defaultAngle --> The default angle for all legs, don't change it!
// riseAngle_1_3 --> The rise anlge for the legs from 1 - 3 
// riseAngle_4_6 --> The rise anlge for the legs from 4 - 6 
// rollAngle1 --> The rolling anlge to move the leg to front (in case of looking at the front of the Hexa) 
// it could move the leg to the back in case your're looking at the back of the Hexa
// rollAngle2 --> The rolling anlge to move the leg to back (opposite effect for rollAngle1)
// timeDelay --> The time gap between each part of the movement 

static float features[3];

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

void handle_OnConnect();
void handle_forward();
void handle_backward();
void handle_left();
void handle_right();
void handle_rssi();
void handle_NotFound();
String SendHTML();
void turnRight();
void turnLeft();
void moveBackward();
void moveForward();
void handle_stand();
void handle_locate();
void handle_auto_move();

static int get_signal_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

void setPWM(int channel, int on, int off) {
  pwm.setPWM(channel, on, off);
}

void setServoAngle(int servoNum, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  setPWM(servoNum, 0, pulse);
}

// A class defining the leg of the HexaPod part, made for easy use, edits, and useability 
class Leg{
  public:
  int roll, rise;

  // Set the pins for rise joint and roll joint of the leg 
  void prepare(int rollBin, int riseBin){
    rise = riseBin;
    roll = rollBin;
  }
};

// A class defining the the HexaPod object, made for easy use, edits, and useability. Easy setup for HexaPod legs and controlling functionality. 
class HexaPod{
  public:
  Leg leg1, leg2, leg3, leg4, leg5, leg6;

  // set the Pins for all the 6 legs of the Hexa
  void prepare(int riseBin1, int rollBin1, int riseBin2, int rollBin2,
    int riseBin3, int rollBin3, int riseBin4, int rollBin4,
    int riseBin5, int rollBin5, int riseBin6, int rollBin6){
      leg1.prepare(riseBin1, rollBin1);
      leg2.prepare(riseBin2, rollBin2);
      leg3.prepare(riseBin3, rollBin3);
      leg4.prepare(riseBin4, rollBin4);
      leg5.prepare(riseBin5, rollBin5);
      leg6.prepare(riseBin6, rollBin6);
  }
  
  void Spin(int direction){

    int right = 0, left = 1, oddAngle, evenAngle;
    if(direction == right){
        oddAngle = rollAngle1;
        evenAngle = rollAngle2;
    } else if( direction == left){
        oddAngle = rollAngle2;
        evenAngle = rollAngle1;
    }

    // Spin pt1

    // Rise even legs of the ground
    setServoAngle(leg2.rise, riseAngle_1_3);
    setServoAngle(leg4.rise, riseAngle_4_6);
    setServoAngle(leg6.rise, riseAngle_4_6);
    delay(timeDelay);

    // Roll even legs to the front where should land later 
    setServoAngle(leg2.roll, evenAngle);
    setServoAngle(leg4.roll, evenAngle);
    setServoAngle(leg6.roll, evenAngle);
    
    // Roll odd legs to the back, to generate a pushiNM,./ng effect for the HexaPod 
    setServoAngle(leg1.roll, oddAngle);
    setServoAngle(leg3.roll, oddAngle);
    setServoAngle(leg5.roll, oddAngle);
    delay(timeDelay);

    // Put down even legs
    setServoAngle(leg2.rise, defaultAngle);
    setServoAngle(leg4.rise, defaultAngle);
    setServoAngle(leg6.rise, defaultAngle);
    delay(timeDelay);

    // Spin pt2

    // Rise odd legs of the ground
    setServoAngle(leg1.rise, riseAngle_1_3);
    setServoAngle(leg3.rise, riseAngle_1_3);
    setServoAngle(leg5.rise, riseAngle_4_6);
    delay(timeDelay);

    // Roll odd legs to the front where should land later 
    setServoAngle(leg1.roll, evenAngle);
    setServoAngle(leg3.roll, evenAngle);
    setServoAngle(leg5.roll, evenAngle);

    // Roll even legs to the back, to generate a pushing effect for the HexaPod  
    setServoAngle(leg2.roll, oddAngle);
    setServoAngle(leg4.roll, oddAngle);
    setServoAngle(leg6.roll, oddAngle);
    delay(timeDelay);

    // Put down odd legs
    setServoAngle(leg1.rise, defaultAngle);
    setServoAngle(leg3.rise, defaultAngle);
    setServoAngle(leg5.rise, defaultAngle);
    delay(timeDelay);
  }

  void Step(int direction){

    int forward = 0, backward = 1, leftAngle, rightAngle;
      if(direction == forward){
        rightAngle = rollAngle1;
        leftAngle = rollAngle2;
    } else if( direction == backward){
        rightAngle = rollAngle2;
        leftAngle = rollAngle1;
    }

    // Step pt1

    // Rise even legs of the ground
    setServoAngle(leg2.rise, riseAngle_1_3);
    setServoAngle(leg4.rise, riseAngle_4_6);
    setServoAngle(leg6.rise, riseAngle_4_6);
    delay(timeDelay);

    // Roll right side legs
    setServoAngle(leg2.roll, rightAngle);
    setServoAngle(leg4.roll, leftAngle);
    setServoAngle(leg6.roll, leftAngle);

    setServoAngle(leg1.roll, defaultAngle);
    setServoAngle(leg3.roll, defaultAngle);
    setServoAngle(leg5.roll, defaultAngle);
    delay(timeDelay);
    
    // Put down even legs of the ground
    setServoAngle(leg2.rise, defaultAngle);
    setServoAngle(leg4.rise, defaultAngle);
    setServoAngle(leg6.rise, defaultAngle);
    delay(timeDelay);

    // Step pt2

    // Rise odd legs of the ground 
    setServoAngle(leg1.rise, riseAngle_1_3);
    setServoAngle(leg3.rise, riseAngle_1_3);
    setServoAngle(leg5.rise, riseAngle_4_6);
    delay(timeDelay);

    // Roll right side legs
    setServoAngle(leg1.roll, rollAngle1);
    setServoAngle(leg3.roll, rightAngle);
    setServoAngle(leg5.roll, leftAngle);

    setServoAngle(leg2.roll, defaultAngle);
    setServoAngle(leg4.roll, defaultAngle);
    setServoAngle(leg6.roll, defaultAngle);
    delay(timeDelay);

    // Put down odd legs 
    setServoAngle(leg1.rise, defaultAngle);
    setServoAngle(leg3.rise, defaultAngle);
    setServoAngle(leg5.rise, defaultAngle);
    delay(timeDelay);
  }

  void dance(){
    setServoAngle(leg1.roll, rollAngle1 - 10);
    setServoAngle(leg2.roll, rollAngle1 - 10);
    setServoAngle(leg3.roll, rollAngle1 - 10);

    setServoAngle(leg4.roll, rollAngle2 + 10);
    setServoAngle(leg5.roll, rollAngle2 + 10);
    setServoAngle(leg6.roll, rollAngle2 + 10);
    
    delay(timeDelay);
    
    setServoAngle(leg1.roll, rollAngle2 + 10);
    setServoAngle(leg2.roll, rollAngle2 + 10);
    setServoAngle(leg3.roll, rollAngle2 + 10);

    setServoAngle(leg4.roll, rollAngle1 - 10);
    setServoAngle(leg5.roll, rollAngle1 - 10);
    setServoAngle(leg6.roll, rollAngle1 - 10);
    
    delay(timeDelay);
  }
  // use to reset all angles to the defaults and finout the indices for each leg.
  // Should be used only once at the first, then reset the legs on this angle 
  void resetAngles(){
  setServoAngle(leg2.rise, defaultAngle);
  setServoAngle(leg4.rise, defaultAngle);
  setServoAngle(leg6.rise, defaultAngle);
  delay(timeDelay);

  setServoAngle(leg2.roll, defaultAngle);
  setServoAngle(leg4.roll, defaultAngle);
  setServoAngle(leg6.roll, defaultAngle);
  delay(timeDelay);

  setServoAngle(leg1.rise, defaultAngle);
  setServoAngle(leg3.rise, defaultAngle);
  setServoAngle(leg5.rise, defaultAngle);
  delay(timeDelay);

  setServoAngle(leg1.roll, defaultAngle);
  setServoAngle(leg3.roll, defaultAngle);
  setServoAngle(leg5.roll, defaultAngle);
  delay(timeDelay);
  }

  // use this to make the HexaPod Stand up and wake up :)
  // Should only be used when starting the Bot from sleeping (idle)
  // Will make the Hexa stand up and set all angles to its starting location to start working fine later 
  void standUp(){
  // Stand Up Pt1

  // Rise the odd legs of the ground 
  setServoAngle(leg1.rise, riseAngle_1_3);
  setServoAngle(leg3.rise, riseAngle_1_3);
  setServoAngle(leg5.rise, riseAngle_4_6);
  delay(timeDelay);

  // Roll the odd legs to where they should be (default location)
  setServoAngle(leg1.roll, defaultAngle);
  setServoAngle(leg3.roll, defaultAngle);
  setServoAngle(leg5.roll, defaultAngle);
  delay(timeDelay);

  // Put the odd legs on the ground in order to make the Hexa stand up
  setServoAngle(leg1.rise, defaultAngle);
  setServoAngle(leg3.rise, defaultAngle);
  setServoAngle(leg5.rise, defaultAngle);
  delay(timeDelay);
  
  // Stand Up Pt2

  // Rise the even legs of the ground 
  setServoAngle(leg2.rise, riseAngle_1_3);
  setServoAngle(leg4.rise, riseAngle_4_6);
  setServoAngle(leg6.rise, riseAngle_4_6);
  delay(timeDelay);

  // Roll the even legs to where they should be (default location)
  setServoAngle(leg2.roll, defaultAngle);
  setServoAngle(leg4.roll, defaultAngle);
  setServoAngle(leg6.roll, defaultAngle);
  delay(timeDelay);

  // Put the even legs on the ground in order to make the Hexa stand up
  setServoAngle(leg2.rise, defaultAngle);
  setServoAngle(leg4.rise, defaultAngle);
  setServoAngle(leg6.rise, defaultAngle);
  delay(timeDelay);
  }

  int calc_distance(int rssi){
    float env_factor = 1;
    int rssi_1m_distance = -40;

    float distance = pow(10, (rssi_1m_distance-rssi)/(10*env_factor));

    Serial.print("distance ");
    Serial.print(distance);
    Serial.print(" RSSI ");
    Serial.print(rssi);
    Serial.println(" ");

    return distance;
  }

  int find_target(String target){
    int numNetworks = WiFi.scanNetworks();
    for (int i = 0; i < numNetworks; i++){

      if(WiFi.SSID(i) == target){
          return i;
      }
    }
  }

  int locate_target(String target){
    int controllerIndex = find_target(target);
    int controllerDistance =  calc_distance(WiFi.RSSI(controllerIndex));
-
    return controllerDistance;
  }

  void go_to_target(String target){
    int distance = locate_target(target), new_distance = distance;
    int forward = 0, backward = 1, right = 0, direction = 2;

    while(distance > 1){

      if(direction == 2){

        Step(forward);
        new_distance = locate_target(target);

        if(new_distance < distance + 1.){
          direction = forward;
          distance = new_distance;
          continue;
        } else {
          standUp();
          if(new_distance > distance + 1.){
            Step(backward);
          } else if (new_distance == distance + 1.) {
            Spin(right);
            continue;
          }
        }

        Step(backward);
        new_distance = locate_target(target);

        if(new_distance < distance + 1.){
          direction = backward;
          distance = new_distance;
          continue;
        } else {
          standUp();
          if(new_distance > distance + 1.){
            Step(forward);
          } else if (new_distance == distance + 1.) {
            Spin(right);
            continue;
          }
        }

        Spin(right);

      } else if (direction == forward) {
        
        Step(forward);
        new_distance = locate_target(target);

        if(new_distance < distance + 1.){
          direction = forward;
          distance = new_distance;
          continue;
        } else {
          standUp();
          if(new_distance > distance + 1.){
            Step(backward);
            direction = 2;
          } else if (new_distance == distance + 1.) {
            Spin(right);
            direction = 2;
            continue;
          }
        }

      } else {
        
        Step(backward);
        new_distance = locate_target(target);

        if(new_distance < distance + 1.){
          direction = backward;
          distance = new_distance;
          continue;
        } else {
          standUp();
          if(new_distance > distance + 1.){
            Step(forward);
            direction = 2;
          } else if (new_distance == distance + 1.) {
            Spin(right);
            direction = 2;
            continue;
          }
        }
           
      }
    }
    dance();
    dance();
    dance();
    standUp();
    Serial.println("Success");
  }

  void my_location(int rssi1, int rssi2, int rssi3){
    features[0] = rssi1;
    features[1] = rssi2;
    features[2] = rssi3;

    signal_t signal;            // Wrapper for raw input buffer
    ei_impulse_result_t result = { 0 }; // Used to store inference output
    EI_IMPULSE_ERROR res;       // Return code from inference

    signal.total_length = sizeof(features) / sizeof(features[0]);
    signal.get_data = &get_signal_data;

    res = run_classifier(&signal, &result, true);

    Serial.print("I'm in ");
    Serial.println(result.timing.classification);
  
  }
  void my_coordinates(int d1, int d2, int d3, 
                      int x1, int x2, int x3,
                      int y1, int y2, int y3){

    float x = (pow(x1, 2) + (pow(d1, 2) - pow(d1, 2))) / (2 * x2);
    float y = (pow(y2, 2) + (pow(d1, 2) - pow(d3, 2))) / (2 * y2);

    Serial.print("My Coordinates: ( ");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.println(" )");
  }
};


void scanNetworks(String ap1, String ap2, String ap3) {
 int numNetworks = WiFi.scanNetworks();
 int rssi1 = 0, rssi2 = 0, rssi3 = 0;

  if (numNetworks != 0){
    for (int i = 0; i < numNetworks; ++i)
    {
          
          if(WiFi.SSID(i) == ap1)
            rssi1 = WiFi.RSSI(i);
          if(WiFi.SSID(i) == ap2)
            rssi2 = WiFi.RSSI(i);
          if(WiFi.SSID(i) == ap3)
            rssi3 = WiFi.RSSI(i);

    }
  }
  Serial.print(rssi1);
  Serial.print(" ");
  Serial.print(rssi2);
  Serial.print(" ");
  Serial.print(rssi3);
  Serial.println(" ");
}


// Create HexaPod object 
HexaPod hexa;

void setup(){
  
  pwm.begin();	

  Serial.begin(9600);

  WiFi.mode(WIFI_AP_STA);	
  WiFi.disconnect();

  WiFi.softAP(ssid, password);  
  WiFi.softAPConfig(local_ip, gateway, subnet);

  server.on("/", HTTP_GET, handle_OnConnect);
  server.on("/forward", HTTP_GET, handle_forward);
  server.on("/backward", HTTP_GET, handle_backward);
  server.on("/left", HTTP_GET, handle_left);
  server.on("/right", HTTP_GET, handle_right);
  server.on("/locate", HTTP_GET, handle_locate);
  server.on("/stand", HTTP_GET, handle_stand);
  server.on("/auto", HTTP_GET, handle_auto_move);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

  pwm.setPWMFreq(SERVO_FREQ);

  // Prepare the HexaPod, pass the indices as pairs for each leg (rise joint bin , roll joint bin) from leg 1 to leg 6
  hexa.prepare(0, 1, 2, 3, 4, 5, 
              6, 7, 8, 9, 10, 11);

  // Wake your kid up :)
  hexa.standUp();
  delay(timeDelay);
  hexa.resetAngles();
  delay(timeDelay);

}

void loop(){
  // Work is done here 
  server.handleClient();
}

void handle_OnConnect(){
  server.send(200, "text/html", SendHTML());
}

void handle_stand(){
  hexa.standUp();
  server.send(200, "text/html", SendHTML());
}

void handle_forward(){
  int forward = 0;
  hexa.Step(forward);
  server.send(200, "text/html", SendHTML());
}

void handle_backward(){
  int backward = 1;
  hexa.Step(backward);
  server.send(200, "text/html", SendHTML());
}

void handle_left(){
  int left = 1;
  hexa.Spin(left);
  server.send(200, "text/html", SendHTML());
}

void handle_right(){
  int right = 0;
  hexa.Spin(right);
  server.send(200, "text/html", SendHTML());
}

void handle_auto_move(){
  hexa.go_to_target("ELSHAF3Y_plus");
  server.send(200, "text/html", SendHTML());
}

void handle_locate(){
  String rssiHTML = "<!DOCTYPE html> <html>\n";
  rssiHTML += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  rssiHTML += "<title>My Location</title>\n";
  rssiHTML += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}\n";
  rssiHTML += "body{margin-top: 20px; margin-left: 20px;} h2 {color: #444444;margin-bottom: 20px;}\n";
  rssiHTML += "ul {list-style-type: none;margin: 0;padding: 0;}\n";
  rssiHTML += "li {margin: 10px 0;}\n";
  rssiHTML += "</style>\n";
  rssiHTML += "</head>\n";
  rssiHTML += "<body>\n";
  rssiHTML += "<h2>Wi-Fi RSSI Values</h2>\n";

  int numNetworks = WiFi.scanNetworks();
  int rssi1 = -100, rssi2 = -100, rssi3 = -100, d1 = 20, d2 = 20, d3 = 20;

  rssiHTML += "<ul>\n";
    for (int i = 0; i < numNetworks; ++i){
          
          if(WiFi.SSID(i) == "ELSHAF3Y_plus"){
            rssi1 = WiFi.RSSI(i);
            d1 = hexa.calc_distance(rssi1);
            rssiHTML += "<li>";
            rssiHTML += "SSID: " + WiFi.SSID(i) + " | RSSI: " + String(rssi1) + " | Distance: " + String(d1);
            rssiHTML += "</li>\n";
          }
          if(WiFi.SSID(i) == "ELSHAF3Y"){
            rssi2 = WiFi.RSSI(i);
            d2 = hexa.calc_distance(rssi2);
            rssiHTML += "<li>";
            rssiHTML += "SSID: " + WiFi.SSID(i) + " | RSSI: " + String(rssi2) + " | Distance: " + String(d2);
            rssiHTML += "</li>\n";
          }
          if(WiFi.SSID(i) == "body"){
            rssi3 = WiFi.RSSI(i);
            d3 = hexa.calc_distance(rssi3);
            rssiHTML += "<li>";
            rssiHTML += "SSID: " + WiFi.SSID(i) + " | RSSI: " + String(rssi3) + " | Distance: " + String(d3);
            rssiHTML += "</li>\n";
          }
    }
  
  hexa.my_location(rssi1, rssi2, rssi3);
  hexa.my_coordinates(d1, d2, d3, 
                      0, 4, 20,
                      0, 0, 20);

  
  rssiHTML += "</ul>\n";
  rssiHTML += "";

  rssiHTML += "</body>\n";
  rssiHTML += "</html>\n";

  server.send(200, "text/html", rssiHTML);
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String html = "<!DOCTYPE html> <html>\n";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html += "<title>Hex Pod Control</title>\n";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  html += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  html += ".button {display: inline-block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 10px;cursor: pointer;border-radius: 4px;}\n";
  html += ".button-forward {background-color: #3498db;}\n";
  html += ".button-backward {background-color: #e74c3c;}\n";
  html += ".button-left {background-color: #2ecc71;}\n";
  html += ".button-right {background-color: #f39c12;}\n";
  html += "</style>\n";
  html += "</head>\n";
  html += "<body>\n";
  html += "<h1>Hexa Pod Control</h1>\n";
  html += "<a class=\"button button-forward\" href=\"/forward\">Forward</a>\n";
  html += "<br>\n";
  html += "<a class=\"button button-left\" href=\"/left\">Left</a>";
  html += "<a class=\"button button-right\" href=\"/right\">Right</a>\n";
  html += "<br>\n";
  html += "<a class=\"button button-backward\" href=\"/backward\">Backward</a>\n";
  html += "<br>\n";
  html += "<a class=\"button \" href=\"/locate\">My Location</a>\n";
  html += "<br>\n";
  html += "<a class=\"button \" href=\"/stand\">Stand</a>\n";
  html += "<a class=\"button \" href=\"/auto\">Auto Move</a>\n";
  html += "</body>\n";
  html += "</html>\n";
  return html;
}
