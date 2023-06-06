document.getElementById("login-form").addEventListener("submit", function(event) {
  event.preventDefault();
  
  var username = document.getElementById("username").value;
  
  if (username === "user") {
    window.location.href = "user.html";
  } else if (username === "admin") {
    window.location.href = "admin.html";
  } else {
    alert("Invalid username. Please try again.");
  }
});

document.getElementById("login-form").addEventListener("submit", function(event) {
  event.preventDefault();
  
  var username = document.getElementById("username").value;
  
  if (username === "user") {
    window.location.href = "home.html";
  } else if (username === "admin") {
    window.location.href = "admin.html";
  } else {
    alert("Invalid username. Please try again.");
  }
});
