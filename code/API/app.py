#!flask/bin/python
from flask import Flask
from flask import jsonify
from flask import request
from flask_cors import CORS
import re
import serial

app = Flask(__name__)
cors = CORS(app, resources={r"/*": {"origins": "*"}})
ser=serial.Serial('/dev/ttyACM0',9600)

@app.route('/')
def index():
        return "Bot connected!"

@app.route('/connect', methods=['GET'])
def connect():
        if request.method == 'GET':

                return jsonify({'connection':'ready'})

@app.route('/status', methods=['POST'])
def status():

        if not request.json or not 'order' in request.json:
                return jsonify({'order':"not_valid_format"}),201

        r = re.compile('.*;.*;.*;.*')
        receivedOrder = request.json.get('order',"")
        if r.match(receivedOrder) is None:
                return jsonify({'order':"not_valid_format"}),201

        v = receivedOrder.split(';')
        if not (0<=int(v[0])<=5 and 0<=int(v[1])<=5 and 0<=int(v[2])<=5 and 0<=int(v[3])<=2):
                return jsonify({'order':"invalid_parameters"}),201

        #left,right,neck,height
        #L[1:5] R[1:5] N[1:5] H[1:2]
        lastOrder = receivedOrder
        ser.write(lastOrder.encode())
        result = {
                'received': receivedOrder,
                'status': 'executing'
        }
        print(result)
        return jsonify({'order':result}),201

if __name__ == '__main__':
        app.run(debug=True, host='0.0.0.0')
