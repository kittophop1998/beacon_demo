#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>
#include <BLEUtils.h>

// =====================================================
// iBeacon Config
// =====================================================

// UUID:
// F3B8A7C1-2D4E-4A9F-8C6B-0190CC1B4F01
//
// UUID นี้ใช้เป็น UUID หลักของระบบเรา
// App iOS / Android สามารถใช้ UUID นี้ค้นหา Beacon ได้
const uint8_t IBEACON_UUID[16] = {
  0xF3, 0xB8, 0xA7, 0xC1,
  0x2D, 0x4E,
  0x4A, 0x9F,
  0x8C, 0x6B,
  0x01, 0x90, 0xCC, 0x1B, 0x4F, 0x01
};

// Major = สาขา / Location
const uint16_t IBEACON_MAJOR = 1;

// Minor = Beacon ID
const uint16_t IBEACON_MINOR = 1;

// RSSI ที่ระยะประมาณ 1 เมตร
// ต้อง calibrate อีกทีตอนติดตั้งจริง
const int8_t IBEACON_TX_POWER = -59;

// Advertising interval
// BLE ใช้หน่วย 0.625ms
// 160 = 100ms
const uint16_t ADV_INTERVAL = 160;


// =====================================================
// Setup
// =====================================================

void setup() {
  Serial.begin(115200);

  // ชื่อนี้ไม่ได้ใช้เป็น identifier หลัก
  // iBeacon ใช้ UUID + Major + Minor
  BLEDevice::init("Attendance-Beacon");

  BLEAdvertising* advertising = BLEDevice::getAdvertising();

  advertising->stop();

  BLEAdvertisementData advData;

  // General Discoverable + BR/EDR Not Supported
  advData.setFlags(0x06);


  // ===================================================
  // iBeacon Manufacturer Data
  // ===================================================
  //
  // Packet:
  //
  // 4C 00       Apple Company ID
  // 02 15       iBeacon Type
  // UUID        16 bytes
  // Major       2 bytes
  // Minor       2 bytes
  // Tx Power    1 byte
  //
  // Total = 25 bytes
  //

  uint8_t manufacturerData[25];

  // Apple Company Identifier: 0x004C
  // BLE uses little endian here
  manufacturerData[0] = 0x4C;
  manufacturerData[1] = 0x00;

  // iBeacon type
  manufacturerData[2] = 0x02;

  // Remaining iBeacon payload length
  manufacturerData[3] = 0x15;


  // UUID
  memcpy(
    &manufacturerData[4],
    IBEACON_UUID,
    16
  );


  // Major
  // Big endian
  manufacturerData[20] =
    (IBEACON_MAJOR >> 8) & 0xFF;

  manufacturerData[21] =
    IBEACON_MAJOR & 0xFF;


  // Minor
  // Big endian
  manufacturerData[22] =
    (IBEACON_MINOR >> 8) & 0xFF;

  manufacturerData[23] =
    IBEACON_MINOR & 0xFF;


  // Measured Tx Power
  manufacturerData[24] =
    (uint8_t)IBEACON_TX_POWER;


  String manufacturerDataString(
    (char*)manufacturerData,
    sizeof(manufacturerData)
  );

  advData.setManufacturerData(
    manufacturerDataString
  );


  // ===================================================
  // Advertising
  // ===================================================

  advertising->setAdvertisementData(advData);

  // 160 × 0.625ms = 100ms
  advertising->setMinInterval(ADV_INTERVAL);
  advertising->setMaxInterval(ADV_INTERVAL);

  advertising->start();


  // ===================================================
  // Debug
  // ===================================================

  Serial.println();
  Serial.println("==============================");
  Serial.println("iBeacon Started");
  Serial.println("==============================");

  Serial.println(
    "UUID: F3B8A7C1-2D4E-4A9F-8C6B-0190CC1B4F01"
  );

  Serial.print("Major: ");
  Serial.println(IBEACON_MAJOR);

  Serial.print("Minor: ");
  Serial.println(IBEACON_MINOR);

  Serial.print("TxPower: ");
  Serial.println(IBEACON_TX_POWER);

  Serial.println("Advertising Interval: 100ms");

  Serial.println("==============================");
}


// =====================================================
// Loop
// =====================================================

void loop() {
  // Beacon ไม่ต้องทำอะไร
  // BLE advertising ทำงานอยู่เบื้องหลัง
  delay(1000);
}
