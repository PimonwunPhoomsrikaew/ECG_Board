// #include <bluefruit.h>
// #include <Adafruit_LSM6DS3TRC.h>

// // For SPI mode, we need a CS pin
// #define LSM_CS 10
// // For software-SPI mode we need SCK/MOSI/MISO pins
// #define LSM_SCK 13
// #define LSM_MISO 12
// #define LSM_MOSI 11

// Adafruit_LSM6DS3TRC lsm6ds3trc;

// // BLE Service
// BLEDfu bledfu;   // OTA DFU service
// BLEDis bledis;   // device information
// BLEUart bleuart; // uart over ble
// BLEBas blebas;   // battery

// // constants won't change:
// unsigned long previousTime = 0;
// const long interval_send_data = 50;

// const int ECG_PIN = A0; // Define the pin for ECG data

// // callback invoked when central connects
// void connect_callback(uint16_t conn_handle)
// {
//   // Get the reference to current connection
//   BLEConnection *connection = Bluefruit.Connection(conn_handle);

//   char central_name[32] = {0};
//   connection->getPeerName(central_name, sizeof(central_name));

//   Serial.print("Connected to ");
//   Serial.println(central_name);
// }

// void disconnect_callback(uint16_t conn_handle, uint8_t reason)
// {
//   (void)conn_handle;
//   (void)reason;

//   Serial.println();
//   Serial.print("Disconnected, reason = 0x");
//   Serial.println(reason, HEX);
// }

// void startAdv(void)
// {
//   // Advertising packet
//   Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
//   Bluefruit.Advertising.addTxPower();

//   // Include bleuart 128-bit uuid
//   Bluefruit.Advertising.addService(bleuart);

//   Bluefruit.ScanResponse.addName();
//   Bluefruit.Advertising.restartOnDisconnect(true);
//   Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
//   Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
//   Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
// }

// void setup()
// {
//   Serial.begin(115200);
//   Serial.println("START LSM6DS3TR-C ");
//   Serial.println("---------------------------\n");
//   if (!lsm6ds3trc.begin_I2C())
//   {
//     Serial.println("Failed to find LSM6DS3TR-C chip");
//     while (1)
//     {
//       delay(10);
//     }
//   }

//   Serial.println("LSM6DS3TR-C SUCCESS!");

//   lsm6ds3trc.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
//   lsm6ds3trc.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
//   lsm6ds3trc.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
//   lsm6ds3trc.setGyroDataRate(LSM6DS_RATE_12_5_HZ);

//   lsm6ds3trc.configInt1(false, false, true); // accelerometer DRDY on INT1
//   lsm6ds3trc.configInt2(false, true, false); // gyro DRDY on INT2

//   Serial.println("START BLE");
//   Serial.println("---------------------------\n");

//   // Setup the BLE LED to be enabled on CONNECT
//   Bluefruit.autoConnLed(true);

//   // Config the peripheral connection with maximum bandwidth
//   Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

//   Bluefruit.begin();
//   Bluefruit.setName("ECG SENSOR");
//   Bluefruit.setTxPower(4);
//   Bluefruit.Periph.setConnectCallback(connect_callback);
//   Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

//   bledfu.begin();

//   // Configure and Start Device Information Service
//   bledis.setManufacturer("Biomedical Engineering Lab");
//   bledis.setModel("ECG Tracker");
//   bledis.begin();

//   // Configure and Start BLE Uart Service
//   bleuart.begin();

//   // Start BLE Battery Service
//   blebas.begin();
//   blebas.write(100);
//   startAdv(); // Set up and start advertising

//   Serial.println("Waiting for connections...");
// }

// void loop()
// {
//   // Get a new normalized sensor event
//   sensors_event_t accel;
//   sensors_event_t gyro;
//   sensors_event_t temp;
//   lsm6ds3trc.getEvent(&accel, &gyro, &temp);

//   int ecg_raw = analogRead(ECG_PIN); // Read ECG data from analog pin A0

//   if (Bluefruit.connected())
//   {

//     unsigned long currentTime = millis();
//     if (currentTime - previousTime >= interval_send_data)
//     {
//       previousTime = currentTime;
//       bleuart.printf("%.2f,%.2f,%.2f,%d\n", accel.acceleration.x, accel.acceleration.y, accel.acceleration.z, ecg_raw);
//       Serial.printf("%.2f,%.2f,%.2f,%d\n", accel.acceleration.x, accel.acceleration.y, accel.acceleration.z, ecg_raw);

//     }
//   }
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
BLEDfu bledfu;      // OTA DFU service
BLEDis bledis;      // device information
BLEUart bleuart;    // uart over ble
BLEBas blebas;      // battery

// --- Configuration Constants ---
const long interval_read_ecg = 5;   // Read ECG every 5 ms (200 Hz sample rate)
const long interval_send_data = 1000; // Send data packet every 100 ms (10 Hz)

// --- ECG Buffer ---
// We send data every 100ms and read every 5ms, so we need to store 100/5 = 20 samples.
const int ECG_BUFFER_SIZE = 20;
uint16_t ecg_buffer[ECG_BUFFER_SIZE];
int ecg_buffer_index = 0;

// --- Timers ---
unsigned long previousReadTime = 0;
unsigned long previousSendTime = 0;

const int ECG_PIN = A0; // Define the pin for ECG data

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
  // while(!Serial) delay(10); // Optional: wait for serial monitor to open

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
  lsm6ds3trc.setAccelDataRate(LSM6DS_RATE_12_5_HZ); // Note: Accel data is updated at 12.5Hz
  lsm6ds3trc.setGyroDataRate(LSM6DS_RATE_12_5_HZ);

  Serial.println("START BLE");
  Serial.println("---------------------------\n");

  // Setup the BLE LED to be enabled on CONNECT
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth
  // For lowest latency, use a smaller connection interval MIN/MAX
  // Note: This increases power consumption.
  // Bluefruit.Periph.setConnInterval(6, 12); // 7.5ms to 15ms
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setName("ECG SENSOR");
  Bluefruit.setTxPower(4);
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("Biomedical Engineering Lab");
  bledis.setModel("ECG Tracker v2");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);
  startAdv(); // Set up and start advertising

  Serial.println("Ready to collect data. Waiting for connection...");
}

void loop()
{
  // We only run the logic if a device is connected
  if (Bluefruit.connected())
  {
    unsigned long currentTime = millis();

    // --- Task 1: Read ECG data at 200Hz and fill the buffer ---
    if (currentTime - previousReadTime >= interval_read_ecg)
    {
      previousReadTime = currentTime;

      // Only add to buffer if it's not full
      if (ecg_buffer_index < ECG_BUFFER_SIZE)
      {
        int ecg_value = analogRead(ECG_PIN); // Read ECG data from analog pin A0
        ecg_buffer[ecg_buffer_index] = ecg_value;
        ecg_buffer_index++;
      }
    }

    // --- Task 2: Send the data packet at 10Hz ---
    if (currentTime - previousSendTime >= interval_send_data)
    {
      previousSendTime = currentTime;

      // Only send if the buffer is full
      if (ecg_buffer_index >= ECG_BUFFER_SIZE)
      {
        // Get the latest accelerometer data just before sending
        sensors_event_t accel;
        sensors_event_t gyro;
        sensors_event_t temp;
        lsm6ds3trc.getEvent(&accel, &gyro, &temp);

        // --- Send data using printf for better memory efficiency ---
        
        // 1. Send accelerometer data first, without a newline
        bleuart.printf("%.2f,%.2f,%.2f", accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
        Serial.printf("%.2f,%.2f,%.2f", accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);

        // 2. Loop through the ECG buffer and send each value, preceded by a comma
        for (int i = 0; i < ECG_BUFFER_SIZE; i++)
        {
          bleuart.printf(",%d", ecg_buffer[i]);
          Serial.printf(",%d", ecg_buffer[i]);
        }

        // 3. Send a final newline character to mark the end of the packet
        bleuart.println();
        Serial.println();

        // Reset the buffer index to start collecting the next batch
        ecg_buffer_index = 0;
      }
    }
  }
}
