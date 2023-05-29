from mpu6050 import mpu6050

sensor = mpu6050(0x68)

while True:
    accelerometer_data = sensor.get_accel_data()
    gyro_data = sensor.get_gyro_data()

    print("Accelerometer data: X=%.2f, Y=%.2f, Z=%.2f" % (
        accelerometer_data['x'],
        accelerometer_data['y'],
        accelerometer_data['z']))
    print("Gyroscope data: X=%.2f, Y=%.2f, Z=%.2f" % (
        gyro_data['x'],
        gyro_data['y'],
        gyro_data['z']))


