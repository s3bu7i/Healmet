#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

WebServer server(80);

const char* ssid = "Dormytory";         /*Enter Your SSID*/
const char* password = "Khazar@2023"; /*Enter Your Password*/

float x_acceleration, y_acceleration, z_acceleration;
float x_gyro, y_gyro, z_gyro;
float tempe;

const char html_page[] PROGMEM = R"RawString(
<!DOCTYPE html>
<html>
  <style>
    body {font-family: sans-serif;}
    h1 {text-align: center; font-size: 30px;}
    p {text-align: center; color: #4CAF50; font-size: 30px;}
  </style>

<body>
  <h1>MPU6050 with ESP32 Web Socket </h1>
  <p>Acceleration X = <span id="x_acceleration">0</span> m/s&sup2</p>
  <p>Acceleration Y = <span id="y_acceleration">0</span> m/s&sup2</p>
  <p>Acceleration Z = <span id="z_acceleration">0</span> m/s&sup2</p><br>
 
  <p>Gyro X = <span id="x_gyro">0</span> rad/s</p>
  <p>Gyro Y = <span id="y_gyro">0</span> rad/s</p>
  <p>Gyro Z = <span id="z_gyro">0</span> rad/s</p><br>
 
  <p>Temperature = <span id="temp">0</span> &#8451;</p>

<script>
  setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        const text = this.responseText;
        const myArr = JSON.parse(text);
       document.getElementById("x_acceleration").innerHTML = myArr[0];
        document.getElementById("y_acceleration").innerHTML = myArr[1];
       document.getElementById("z_acceleration").innerHTML = myArr[2];
       document.getElementById("x_gyro").innerHTML = myArr[3];
       document.getElementById("y_gyro").innerHTML = myArr[4];
       document.getElementById("z_gyro").innerHTML = myArr[5];
       document.getElementById("temp").innerHTML = myArr[6];
      }
    };
    xhttp.open("GET", "read_Web_MPU6050", true);
    xhttp.send();
  },100);
</script>
</body>
</html>
)RawString";

void MainPage() {
  String _html_page = html_page;              /*Read The HTML Page*/
  server.send(200, "text/html", _html_page);  /*Send the code to the web server*/
}

void Web_MPU6050() {
  String data = "[\""+String(x_acceleration)+"\",\""+String(y_acceleration)+"\",\""+String(z_acceleration)+"\",\""+String(x_gyro)+"\",\""+String(y_gyro)+"\",\""+String(z_gyro)+"\",\""+String(tempe)+"\"]";
  server.send(200, "text/plane", data);
}

void setup(void){
  Serial.begin(115200);                 /*Set the baudrate to 115200*/
  WiFi.mode(WIFI_STA);                  /*Set the WiFi in STA Mode*/
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(1000);                         /*Wait for 1000mS*/
  while(WiFi.waitForConnectResult() != WL_CONNECTED){Serial.print(".");} /*Wait while connecting to WiFi*/
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());       /*Print the Local IP*/
 

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);

  server.on("/", MainPage);             /*Display the Web/HTML Page*/
  server.on("/read_Web_MPU6050", Web_MPU6050); /*Display the updated Distance value(CM and INCH)*/
  server.begin();                       /*Start Server*/
  delay(1000);                          /*Wait for 1000mS*/
 
}

void loop(void){
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
 
  x_acceleration = a.acceleration.x;
  y_acceleration = a.acceleration.y;
  z_acceleration = a.acceleration.z;
  x_gyro = g.gyro.x;
  y_gyro = g.gyro.y;
  z_gyro = g.gyro.z;
  tempe = temp.temperature;
 
  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(x_acceleration);
  Serial.print(", Y: ");
  Serial.print(y_acceleration);
  Serial.print(", Z: ");
  Serial.print(z_acceleration);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(x_gyro);
  Serial.print(", Y: ");

  Serial.print(y_gyro);
  Serial.print(", Z: ");
  Serial.print(z_gyro);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(tempe);
  Serial.println(" degC");

  Serial.println("");
  delay(500);

  server.handleClient();
}
