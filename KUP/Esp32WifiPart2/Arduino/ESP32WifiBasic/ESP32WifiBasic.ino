#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// SSID and password of Wifi connection:
const char* ssid = "Dormytory";
const char* password = "Khazar@2023";

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

String webpage = "<!DOCTYPE html><html> <head> <title>ESP Web Server</title> <meta name="viewport" content="width=device-width, initial-scale=1" /> <link rel="icon" href="data:," /> <!--<link rel="stylesheet" type="text/css" href="style.css" />--> <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous" /> <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/107/three.min.js"></script> <style> html { font-family: Arial; display: inline-block; text-align: center; } p { font-size: 1.2rem; } body { margin: 0; } .topnav { overflow: hidden; background-color: #003366; color: #ffd43b; font-size: 1rem; } .content { padding: 20px; } .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, 0.5); } .card-title { color: #003366; font-weight: bold; } .cards { max-width: 800px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); } .reading { font-size: 1.2rem; } .cube-content { width: 100%; background-color: white; height: 300px; margin: auto; padding-top: 2%; } #reset { border: none; color: #fefcfb; background-color: #003366; padding: 10px; text-align: center; display: inline-block; font-size: 14px; width: 150px; border-radius: 4px; } #resetX, #resetY, #resetZ { border: none; color: #fefcfb; background-color: #003366; padding-top: 10px; padding-bottom: 10px; text-align: center; display: inline-block; font-size: 14px; width: 20px; border-radius: 4px; } </style> </head> <body> <div class="topnav"> <h1> <i class="far fa-compass"></i> MPU6050 <i class="far fa-compass"></i> </h1> </div> <div class="content"> <div class="cards"> <div class="card"> <p class="card-title">GYROSCOPE</p> <p> <span class="reading">X: <span id="gyroX"></span> rad</span> </p> <p> <span class="reading">Y: <span id="gyroY"></span> rad</span> </p> <p> <span class="reading">Z: <span id="gyroZ"></span> rad</span> </p> </div> <div class="card"> <p class="card-title">ACCELEROMETER</p> <p> <span class="reading" >X: <span id="accX"></span> ms<sup>2</sup></span > </p> <p> <span class="reading" >Y: <span id="accY"></span> ms<sup>2</sup></span > </p> <p> <span class="reading" >Z: <span id="accZ"></span> ms<sup>2</sup></span > </p> </div> <div class="card"> <p class="card-title">TEMPERATURE</p> <p> <span class="reading"><span id="temp"></span> &deg;C</span> </p> <p class="card-title">3D ANIMATION</p> <button id="reset" onclick="resetPosition(this)"> RESET POSITION </button> <button id="resetX" onclick="resetPosition(this)">X</button> <button id="resetY" onclick="resetPosition(this)">Y</button> <button id="resetZ" onclick="resetPosition(this)">Z</button> </div> </div> <div class="cube-content"> <div id="3Dcube"></div> </div> </div> <script> let scene, camera, rendered, cube; function parentWidth(elem) { return elem.parentElement.clientWidth; } function parentHeight(elem) { return elem.parentElement.clientHeight; } function init3D() { scene = new THREE.Scene(); scene.background = new THREE.Color(0xffffff); camera = new THREE.PerspectiveCamera( 75, parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube")), 0.1, 1000 ); renderer = new THREE.WebGLRenderer({ antialias: true }); renderer.setSize( parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")) ); document.getElementById("3Dcube").appendChild(renderer.domElement); // Create a geometry const geometry = new THREE.BoxGeometry(5, 1, 4); // Materials of each face var cubeMaterials = [ new THREE.MeshBasicMaterial({ color: 0x03045e }), new THREE.MeshBasicMaterial({ color: 0x023e8a }), new THREE.MeshBasicMaterial({ color: 0x0077b6 }), new THREE.MeshBasicMaterial({ color: 0x03045e }), new THREE.MeshBasicMaterial({ color: 0x023e8a }), new THREE.MeshBasicMaterial({ color: 0x0077b6 }), ]; const material = new THREE.MeshFaceMaterial(cubeMaterials); cube = new THREE.Mesh(geometry, material); scene.add(cube); camera.position.z = 5; renderer.render(scene, camera); } // Resize the 3D object when the browser window changes size function onWindowResize() { camera.aspect = parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube")); //camera.aspect = window.innerWidth / window.innerHeight; camera.updateProjectionMatrix(); //renderer.setSize(window.innerWidth, window.innerHeight); renderer.setSize( parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")) ); } window.addEventListener("resize", onWindowResize, false); // Create the 3D representation init3D(); // Create events for the sensor readings if (!!window.EventSource) { var source = new EventSource("/events"); source.addEventListener( "open", function (e) { console.log("Events Connected"); }, false ); source.addEventListener( "error", function (e) { if (e.target.readyState != EventSource.OPEN) { console.log("Events Disconnected"); } }, false ); source.addEventListener( "gyro_readings", function (e) { //console.log("gyro_readings", e.data); var obj = JSON.parse(e.data); document.getElementById("gyroX").innerHTML = obj.gyroX; document.getElementById("gyroY").innerHTML = obj.gyroY; document.getElementById("gyroZ").innerHTML = obj.gyroZ; // Change cube rotation after receiving the readinds cube.rotation.x = obj.gyroY; cube.rotation.z = obj.gyroX; cube.rotation.y = obj.gyroZ; renderer.render(scene, camera); }, false ); source.addEventListener( "temperature_reading", function (e) { console.log("temperature_reading", e.data); document.getElementById("temp").innerHTML = e.data; }, false ); source.addEventListener( "accelerometer_readings", function (e) { console.log("accelerometer_readings", e.data); var obj = JSON.parse(e.data); document.getElementById("accX").innerHTML = obj.accX; document.getElementById("accY").innerHTML = obj.accY; document.getElementById("accZ").innerHTML = obj.accZ; }, false ); } function resetPosition(element) { var xhr = new XMLHttpRequest(); xhr.open("GET", "/" + element.id, true); console.log(element.id); xhr.send(); } </script> </body></html>";

void setup() {
  Serial.begin(115200);                 
  
  WiFi.begin(ssid, password);
  Serial.println("Establishing connection to WiFi with SSID: " + String(ssid));
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected to network with IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", [] () {
    server.send(200, "text\html", webpage);
  });
  server.begin();
  webSocket.begin();

}
 
void loop() {
  server.handleClient();
  webSocket.loop();
}
