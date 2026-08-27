#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>
#include <BLEBeacon.h>

// =====================================================
// BEACON CONFIG
// =====================================================

// ชื่อ Beacon สำหรับให้คนอ่านง่าย
#define BEACON_NAME "BEACON-A01"

// iBeacon UUID
#define BEACON_UUID "57dcd1b1-ed48-49b6-8aff-e7a4bfb390da"

// Logical identifier
#define BEACON_MAJOR 1
#define BEACON_MINOR 1

// =====================================================
// RF TX POWER
// =====================================================
//
// นี่คือกำลังส่ง BLE จริง
//
// ESP_PWR_LVL_N12 = -12 dBm
// ESP_PWR_LVL_N9  =  -9 dBm
// ESP_PWR_LVL_N6  =  -6 dBm
// ESP_PWR_LVL_N3  =  -3 dBm
// ESP_PWR_LVL_N0  =   0 dBm
// ESP_PWR_LVL_P3  =  +3 dBm
// ESP_PWR_LVL_P6  =  +6 dBm
// ESP_PWR_LVL_P9  =  +9 dBm
//

#define BEACON_TX_POWER_LEVEL ESP_PWR_LVL_P6

// =====================================================
// iBeacon Measured Power
// =====================================================
//
// ค่านี้ไม่ใช่กำลังส่ง RF
//
// มันคือ RSSI ที่ receiver "ควรได้รับ"
// เมื่ออยู่ห่าง Beacon ประมาณ 1 เมตร
//
// ตอนนี้เริ่มไว้ที่ -59 dBm
// ควร calibration จากของจริงภายหลัง
//
#define BEACON_MEASURED_POWER -59

// =====================================================
// ADVERTISING
// =====================================================

// BLE advertising interval
// 160 * 0.625 ms = 100 ms
#define BEACON_ADV_INTERVAL 160

BLEAdvertising* advertising;


// =====================================================
// TX POWER DISPLAY
// =====================================================

int getTxPowerDbm() {
  switch (BEACON_TX_POWER_LEVEL) {
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
  Serial.println("Starting iBeacon...");

  // =====================================================
  // BLE INIT
  // =====================================================

  BLEDevice::init(BEACON_NAME);

  // กำลังส่ง RF จริง
  BLEDevice::setPower(
    BEACON_TX_POWER_LEVEL,
    ESP_BLE_PWR_TYPE_ADV
  );

  // =====================================================
  // CREATE IBEACON
  // =====================================================

  BLEBeacon beacon;

  // Apple Manufacturer ID สำหรับ iBeacon
  beacon.setManufacturerId(0x004C);

  beacon.setProximityUUID(
    BLEUUID(BEACON_UUID)
  );

  beacon.setMajor(BEACON_MAJOR);

  beacon.setMinor(BEACON_MINOR);

  // RSSI calibration ที่ระยะ 1 เมตร
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

  // ทำให้ scanner เห็นชื่อ BEACON-A01
  scanResponseData.setName(
    BEACON_NAME
  );

  // =====================================================
  // START ADVERTISING
  // =====================================================

  advertising =
    BLEDevice::getAdvertising();

  advertising->setAdvertisementData(
    advertisementData
  );

  advertising->setScanResponseData(
    scanResponseData
  );

  // Scan ได้ แต่ connect ไม่ได้
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

  Serial.println("iBeacon started");

  Serial.println("================================");

  Serial.printf(
    "Name           : %s\n",
    BEACON_NAME
  );

  Serial.printf(
    "UUID           : %s\n",
    BEACON_UUID
  );

  Serial.printf(
    "Major          : %d\n",
    BEACON_MAJOR
  );

  Serial.printf(
    "Minor          : %d\n",
    BEACON_MINOR
  );

  Serial.printf(
    "RF TX Power    : %+d dBm\n",
    getTxPowerDbm()
  );

  Serial.printf(
    "Measured Power : %d dBm @ 1m\n",
    BEACON_MEASURED_POWER
  );

  Serial.printf(
    "ADV Interval   : %.1f ms\n",
    BEACON_ADV_INTERVAL * 0.625
  );

  Serial.println("================================");
}


void loop() {
  // Broadcast only
  delay(10000);
}
