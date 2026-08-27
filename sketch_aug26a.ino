#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>
#include <BLEBeacon.h>

// =====================================================
// BEACON IDENTITY
// =====================================================

#define BEACON_NAME "BEACON-A01"

#define BEACON_UUID "57dcd1b1-ed48-49b6-8aff-e7a4bfb390da"

#define BEACON_MAJOR 1
#define BEACON_MINOR 1


// =====================================================
// BLE RF TX POWER
// =====================================================
//
// กำลังส่งจริงของ ESP32-C3
//
// N12 = -12 dBm
// N9  =  -9 dBm
// N6  =  -6 dBm
// N3  =  -3 dBm
// N0  =   0 dBm
// P3  =  +3 dBm
// P6  =  +6 dBm
// P9  =  +9 dBm
//

#define BEACON_RF_TX_LEVEL ESP_PWR_LVL_P6


// =====================================================
// IBEACON MEASURED POWER
// =====================================================
//
// ค่านี้ไม่ใช่กำลังส่งของ ESP32
//
// เป็น RSSI calibration ที่ receiver วัดได้
// เมื่ออยู่ห่าง Beacon 1 เมตร
//
// ต้องวัดจาก Beacon ตัวจริงแล้วมาแก้ค่านี้
//
// ตัวอย่าง:
// ถ้าวัดที่ 1m แล้วค่าเฉลี่ยเป็น -63
// ให้เปลี่ยนเป็น:
//
// #define BEACON_MEASURED_POWER -63
//
// ตอนนี้ใช้ -59 เป็นค่าเริ่มต้นชั่วคราวเท่านั้น
//

#define BEACON_MEASURED_POWER -59


// =====================================================
// ADVERTISING INTERVAL
// =====================================================
//
// BLE unit = 0.625 ms
//
// 160 = 100 ms
// ประมาณ 10 advertisements / second
//

#define BEACON_ADV_INTERVAL 160


BLEAdvertising* advertising;


// =====================================================
// RF POWER DISPLAY
// =====================================================

int getRfTxPowerDbm() {
  switch (BEACON_RF_TX_LEVEL) {
    case ESP_PWR_LVL_N12:
      return -12;

    case ESP_PWR_LVL_N9:
      return -9;

    case ESP_PWR_LVL_N6:
      return -6;

    case ESP_PWR_LVL_N3:
      return -3;

    case ESP_PWR_LVL_N0:
      return 0;

    case ESP_PWR_LVL_P3:
      return 3;

    case ESP_PWR_LVL_P6:
      return 6;

    case ESP_PWR_LVL_P9:
      return 9;

    default:
      return 0;
  }
}


// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Starting Beacon...");


  // =====================================================
  // BLE INIT
  // =====================================================

  BLEDevice::init(BEACON_NAME);


  // =====================================================
  // SET REAL RF TX POWER
  // =====================================================

  BLEDevice::setPower(
    BEACON_RF_TX_LEVEL,
    ESP_BLE_PWR_TYPE_ADV
  );


  // =====================================================
  // CREATE IBEACON
  // =====================================================

  BLEBeacon beacon;

  // Apple Manufacturer ID
  beacon.setManufacturerId(0x004C);

  // UUID
  beacon.setProximityUUID(
    BLEUUID(BEACON_UUID)
  );

  // Beacon identity
  beacon.setMajor(BEACON_MAJOR);
  beacon.setMinor(BEACON_MINOR);

  // RSSI reference at 1 meter
  beacon.setSignalPower(
    BEACON_MEASURED_POWER
  );


  // =====================================================
  // ADVERTISEMENT DATA
  // =====================================================

  BLEAdvertisementData advertisementData;

  advertisementData.setFlags(0x06);

  advertisementData.setManufacturerData(
    beacon.getData()
  );


  // =====================================================
  // SCAN RESPONSE
  // =====================================================

  BLEAdvertisementData scanResponseData;

  // ให้ Scanner เห็นชื่อ BEACON-A01
  scanResponseData.setName(
    BEACON_NAME
  );


  // =====================================================
  // START ADVERTISING
  // =====================================================

  advertising = BLEDevice::getAdvertising();

  advertising->setAdvertisementData(
    advertisementData
  );

  advertising->setScanResponseData(
    scanResponseData
  );

  // Scanner อ่านได้ แต่ Connect ไม่ได้
  advertising->setAdvertisementType(
    ADV_TYPE_SCAN_IND
  );

  advertising->setMinInterval(
    BEACON_ADV_INTERVAL
  );

  advertising->setMaxInterval(
    BEACON_ADV_INTERVAL
  );

  advertising->start();


  // =====================================================
  // DEBUG
  // =====================================================

  Serial.println();
  Serial.println("================================");

  Serial.println("Beacon started");

  Serial.println("================================");

  Serial.printf(
    "Name            : %s\n",
    BEACON_NAME
  );

  Serial.printf(
    "UUID            : %s\n",
    BEACON_UUID
  );

  Serial.printf(
    "Major           : %d\n",
    BEACON_MAJOR
  );

  Serial.printf(
    "Minor           : %d\n",
    BEACON_MINOR
  );

  Serial.printf(
    "RF TX Power     : %+d dBm\n",
    getRfTxPowerDbm()
  );

  Serial.printf(
    "Measured Power  : %d dBm\n",
    BEACON_MEASURED_POWER
  );

  Serial.printf(
    "ADV Interval    : %.1f ms\n",
    BEACON_ADV_INTERVAL * 0.625
  );

  Serial.println("================================");
}


// =====================================================
// LOOP
// =====================================================

void loop() {
  // BLE stack ทำ Advertising อยู่เบื้องหลัง
  delay(10000);
}
