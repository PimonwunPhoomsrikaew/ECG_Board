// // Basic demo for accelerometer/gyro readings from Adafruit LSM6DS3TR-C
// #include <bluefruit.h>
// #include <Adafruit_LSM6DS3TRC.h>

// // For SPI mode, we need a CS pin
// #define LSM_CS 10
// // For software-SPI mode we need SCK/MOSI/MISO pins
// #define LSM_SCK 13
// #define LSM_MISO 12
// #define LSM_MOSI 11

// Adafruit_LSM6DS3TRC lsm6ds3trc;

// void setup(void) {
//   Serial.begin(115200);
//   while (!Serial)
//     delay(10); // will pause Zero, Leonardo, etc until serial console opens

//   Serial.println("Adafruit LSM6DS3TR-C test!");

//   if (!lsm6ds3trc.begin_I2C()) {
//     // if (!lsm6ds3trc.begin_SPI(LSM_CS)) {
//     // if (!lsm6ds3trc.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
//     Serial.println("Failed to find LSM6DS3TR-C chip");
//     while (1) {
//       delay(10);
//     }
//   }

//   Serial.println("LSM6DS3TR-C Found!");

//   // lsm6ds3trc.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
//   Serial.print("Accelerometer range set to: ");
//   switch (lsm6ds3trc.getAccelRange()) {
//   case LSM6DS_ACCEL_RANGE_2_G:
//     Serial.println("+-2G");
//     break;
//   case LSM6DS_ACCEL_RANGE_4_G:
//     Serial.println("+-4G");
//     break;
//   case LSM6DS_ACCEL_RANGE_8_G:
//     Serial.println("+-8G");
//     break;
//   case LSM6DS_ACCEL_RANGE_16_G:
//     Serial.println("+-16G");
//     break;
//   }

//   // lsm6ds3trc.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
//   Serial.print("Gyro range set to: ");
//   switch (lsm6ds3trc.getGyroRange()) {
//   case LSM6DS_GYRO_RANGE_125_DPS:
//     Serial.println("125 degrees/s");
//     break;
//   case LSM6DS_GYRO_RANGE_250_DPS:
//     Serial.println("250 degrees/s");
//     break;
//   case LSM6DS_GYRO_RANGE_500_DPS:
//     Serial.println("500 degrees/s");
//     break;
//   case LSM6DS_GYRO_RANGE_1000_DPS:
//     Serial.println("1000 degrees/s");
//     break;
//   case LSM6DS_GYRO_RANGE_2000_DPS:
//     Serial.println("2000 degrees/s");
//     break;
//   case ISM330DHCX_GYRO_RANGE_4000_DPS:
//     break; // unsupported range for the DS33
//   }

//   // lsm6ds3trc.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
//   Serial.print("Accelerometer data rate set to: ");
//   switch (lsm6ds3trc.getAccelDataRate()) {
//   case LSM6DS_RATE_SHUTDOWN:
//     Serial.println("0 Hz");
//     break;
//   case LSM6DS_RATE_12_5_HZ:
//     Serial.println("12.5 Hz");
//     break;
//   case LSM6DS_RATE_26_HZ:
//     Serial.println("26 Hz");
//     break;
//   case LSM6DS_RATE_52_HZ:
//     Serial.println("52 Hz");
//     break;
//   case LSM6DS_RATE_104_HZ:
//     Serial.println("104 Hz");
//     break;
//   case LSM6DS_RATE_208_HZ:
//     Serial.println("208 Hz");
//     break;
//   case LSM6DS_RATE_416_HZ:
//     Serial.println("416 Hz");
//     break;
//   case LSM6DS_RATE_833_HZ:
//     Serial.println("833 Hz");
//     break;
//   case LSM6DS_RATE_1_66K_HZ:
//     Serial.println("1.66 KHz");
//     break;
//   case LSM6DS_RATE_3_33K_HZ:
//     Serial.println("3.33 KHz");
//     break;
//   case LSM6DS_RATE_6_66K_HZ:
//     Serial.println("6.66 KHz");
//     break;
//   }

//   // lsm6ds3trc.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
//   Serial.print("Gyro data rate set to: ");
//   switch (lsm6ds3trc.getGyroDataRate()) {
//   case LSM6DS_RATE_SHUTDOWN:
//     Serial.println("0 Hz");
//     break;
//   case LSM6DS_RATE_12_5_HZ:
//     Serial.println("12.5 Hz");
//     break;
//   case LSM6DS_RATE_26_HZ:
//     Serial.println("26 Hz");
//     break;
//   case LSM6DS_RATE_52_HZ:
//     Serial.println("52 Hz");
//     break;
//   case LSM6DS_RATE_104_HZ:
//     Serial.println("104 Hz");
//     break;
//   case LSM6DS_RATE_208_HZ:
//     Serial.println("208 Hz");
//     break;
//   case LSM6DS_RATE_416_HZ:
//     Serial.println("416 Hz");
//     break;
//   case LSM6DS_RATE_833_HZ:
//     Serial.println("833 Hz");
//     break;
//   case LSM6DS_RATE_1_66K_HZ:
//     Serial.println("1.66 KHz");
//     break;
//   case LSM6DS_RATE_3_33K_HZ:
//     Serial.println("3.33 KHz");
//     break;
//   case LSM6DS_RATE_6_66K_HZ:
//     Serial.println("6.66 KHz");
//     break;
//   }

//   lsm6ds3trc.configInt1(false, false, true); // accelerometer DRDY on INT1
//   lsm6ds3trc.configInt2(false, true, false); // gyro DRDY on INT2
// }

// void loop() {
//   // Get a new normalized sensor event
//   sensors_event_t accel;
//   sensors_event_t gyro;
//   sensors_event_t temp;
//   lsm6ds3trc.getEvent(&accel, &gyro, &temp);

//   // Serial.print("\t\tTemperature ");
//   // Serial.print(temp.temperature);
//   // Serial.println(" deg C");

//   // /* Display the results (acceleration is measured in m/s^2) */
//   // Serial.print("\t\tAccel X: ");
//   // Serial.print(accel.acceleration.x);
//   // Serial.print(" \tY: ");
//   // Serial.print(accel.acceleration.y);
//   // Serial.print(" \tZ: ");
//   // Serial.print(accel.acceleration.z);
//   // Serial.println(" m/s^2 ");

//   // /* Display the results (rotation is measured in rad/s) */
//   // Serial.print("\t\tGyro X: ");
//   // Serial.print(gyro.gyro.x);
//   // Serial.print(" \tY: ");
//   // Serial.print(gyro.gyro.y);
//   // Serial.print(" \tZ: ");
//   // Serial.print(gyro.gyro.z);
//   // Serial.println(" radians/s ");
//   // Serial.println();

//   // delay(100);

//    // serial plotter friendly format

//   //  Serial.print(temp.temperature);
//   //  Serial.print(",");

//   //  Serial.print(accel.acceleration.x);
//   //  Serial.print(","); Serial.print(accel.acceleration.y);
//   //  Serial.print(","); Serial.print(accel.acceleration.z);
//   //  Serial.print(",");

//   // Serial.print(gyro.gyro.x);
//   // Serial.print(","); Serial.print(gyro.gyro.y);
//   // Serial.print(","); Serial.print(gyro.gyro.z);
//   // Serial.print(",");
//   Serial.print(analogRead(A0));
//   Serial.println();
//    delayMicroseconds(10000);
// }

#include <bluefruit.h>
#include <Adafruit_LSM6DS3TRC.h>

// For SPI mode, we need a CS pin
#define LSM_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define LSM_SCK 13
#define LSM_MISO 12
#define LSM_MOSI 11

Adafruit_LSM6DS3TRC lsm6ds3trc;

// BLE Service
BLEDfu bledfu;   // OTA DFU service
BLEDis bledis;   // device information
BLEUart bleuart; // uart over ble
BLEBas blebas;   // battery

// constants won't change:
unsigned long previousTime = 0;
const long interval_send_data = 1000;

// callback invoked when central connects
void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection *connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = {0};
  connection->getPeerName(central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void)conn_handle;
  (void)reason;

  Serial.println();
  Serial.print("Disconnected, reason = 0x");
  Serial.println(reason, HEX);
}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
  Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Adafruit LSM6DS3TR-C test!");

  if (!lsm6ds3trc.begin_I2C())
  {
    Serial.println("Failed to find LSM6DS3TR-C chip");
    while (1)
    {
      delay(10);
    }
  }

  Serial.println("LSM6DS3TR-C Found!");

  lsm6ds3trc.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  lsm6ds3trc.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  lsm6ds3trc.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  lsm6ds3trc.setGyroDataRate(LSM6DS_RATE_12_5_HZ);

  lsm6ds3trc.configInt1(false, false, true); // accelerometer DRDY on INT1
  lsm6ds3trc.configInt2(false, true, false); // gyro DRDY on INT2

  Serial.println("Bluefruit52 BLEUART Example");
  Serial.println("---------------------------\n");

  // Setup the BLE LED to be enabled on CONNECT
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4); 
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);
  startAdv(); // Set up and start advertising

  Serial.println("Waiting for connections...");
}

void loop()
{
  // Get a new normalized sensor event
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  lsm6ds3trc.getEvent(&accel, &gyro, &temp);

  if (Bluefruit.connected())
  {

    unsigned long currentTime = millis();
    if (currentTime - previousTime >= interval_send_data)
    {
      previousTime = currentTime;
      bleuart.printf("%.2f,%.2f\n", accel.acceleration.x, accel.acceleration.y);
    }

    Serial.print("Accel X: ");
    Serial.print(accel.acceleration.x);
    Serial.print(" \tY: ");
    Serial.print(accel.acceleration.y);
    Serial.print(" \tZ: ");
    Serial.print(accel.acceleration.z);
    Serial.println(" m/s^2 ");
  }
}
