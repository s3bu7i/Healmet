from flask import Flask, jsonify, request

app = Flask(__name__)

data = {}  # Variable to store received data

@app.route('/api/data', methods=['POST'])
def receive_data():
    global data
    data = request.get_json()  # Store the received data in the 'data' variable
    return jsonify({'message': 'Data received successfully'})

@app.route('/api/data', methods=['GET'])
def get_data():
    return jsonify(data)  # Return the stored data as JSON

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

