// Initialize Google Maps
function initMap() {
  var mapOptions = {
    center: { lat: 37.7749, lng: -122.4194 }, // Set the initial map center
    zoom: 10 // Set the initial zoom level
  };

  var map = new google.maps.Map(document.getElementById("map"), mapOptions);

  // Add any additional map functionality or markers as needed
}

// Update motorcycle info
function updateMotorcycleInfo(distance, speed) {
  document.getElementById("distance").textContent = distance + " km";
  document.getElementById("speed").textContent = speed + " km/h";
}

// Example data for updating motorcycle info (replace with real-time data)
var distance = 150;
var speed = 80;

// Update motorcycle info initially
updateMotorcycleInfo(distance, speed);

// Update motorcycle info periodically (example every 5 seconds)
setInterval(function() {
  // Update distance and speed with new values
  distance += 10;
  speed += 5;

  updateMotorcycleInfo(distance, speed);
}, 5000);
