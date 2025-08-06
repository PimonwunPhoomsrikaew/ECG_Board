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

int ECG_PIN = A0; // Define the pin for ECG data

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
  Serial.println("START LSM6DS3TR-C ");
  Serial.println("---------------------------\n");
  if (!lsm6ds3trc.begin_I2C())
  {
    Serial.println("Failed to find LSM6DS3TR-C chip");
    while (1)
    {
      delay(10);
    }
  }

  Serial.println("LSM6DS3TR-C SUCCESS!");

  lsm6ds3trc.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  lsm6ds3trc.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  lsm6ds3trc.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  lsm6ds3trc.setGyroDataRate(LSM6DS_RATE_12_5_HZ);

  lsm6ds3trc.configInt1(false, false, true); // accelerometer DRDY on INT1
  lsm6ds3trc.configInt2(false, true, false); // gyro DRDY on INT2

  Serial.println("START BLE");
  Serial.println("---------------------------\n");

  // Setup the BLE LED to be enabled on CONNECT
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setName("ECG SENSOR");
  Bluefruit.setTxPower(4); 
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("Biomedical Engineering Lab");
  bledis.setModel("ECG Tracker");
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

  int ecg_raw = analogRead(ECG_PIN); // Read ECG data from analog pin A0

  if (Bluefruit.connected())
  {

    unsigned long currentTime = millis();
    if (currentTime - previousTime >= interval_send_data)
    {
      previousTime = currentTime;
      bleuart.printf("%.2f,%.2f,%.2f,%.2f\n", accel.acceleration.x, accel.acceleration.y, accel.acceleration.z, ecg_raw);
    }

    Serial.print("Accel X: ");
    Serial.print(accel.acceleration.x);
    Serial.print(" \tY: ");
    Serial.print(accel.acceleration.y);
    Serial.print(" \tZ: ");
    Serial.print(accel.acceleration.z);
    Serial.print("  m/s^2 ");
    Serial.print(" \tecg: ");
    Serial.println(ecg_raw);
    
  }
}
