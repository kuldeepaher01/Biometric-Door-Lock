# Biometric-Door-Lock

### To make this Biometric Door Lock follow these steps

#### Components Used

> 1. Arduino UNO (You can use any other arduino board)
> 2. Fingerprint Sensor R307
> 3. 16*4 LCD (You can use 16*2)
> 4. Solenoid Door Lock (DO check the locking orientation while purchasing)
> 5. 5VDC Relay
> 6. RGB LED (I used common anode LED)
> 7. Buzzer
> 8. I2C module
> 9. Some Jumper Cables
> 10. Female DC jack
> 11. Power distribution board/ Buck Converter

- Install [Adafruit-Fingerprint-Sensor-Library](https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library), [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C). After that make the connections as shown below.
- Now, go to file -> examples -> Adafruit-Fingerprint-Sensor-Library -> enroll (To enroll fingerprint(s)). *And follow the instructions from the ino file*. Upload the code open serial monitor and follow the instructions.
- After that, upload the [Finger_LCD.ino](Finger_LCD.ino).
- And boom!!! You r good to go......

  New Image ->
  ![Biometric Door Lock New](fpscan.png)
  Pinout Diagram ->
  ![Biometric_DoorLock_PinOut-Diagram](https://user-images.githubusercontent.com/76651819/167033100-134c7eff-bb31-4e46-9b15-56d685369604.jpg)
  Circuit Image ->
  ![Biometric Door Lock Circuitry](circuit.png)
  Bare Minimum Circuit
  ![Biometric Door Lock OLD](https://user-images.githubusercontent.com/76651819/167027591-c3dae8c5-87e6-49a8-8a53-3b7080208734.jpg)

## *Author*
Made With ❤️ By [Kuldeep](https://github.com/kuldeepaher01)
