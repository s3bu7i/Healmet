// Function to make AJAX request
function fetchData() {
  // Make an AJAX request to the server-side endpoint
  fetch('/endpoint')
    .then(response => response.json())
    .then(data => {
      // Process the received data
      // You can update the HTML elements on the page with the fetched values
      console.log(data);
      // Update HTML elements with the fetched values
      document.getElementById('result').innerHTML = data.result;
    })
    .catch(error => {
      // Handle any errors that occur during the request
      console.error('Error:', error);
    });
}

// Execute fetchData function when the web page loads
document.addEventListener('DOMContentLoaded', fetchData);

// Execute fetchData function when a button is clicked
document.getElementById('fetchButton').addEventListener('click', fetchData);
