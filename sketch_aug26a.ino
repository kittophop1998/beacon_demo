#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>
#include <BLEBeacon.h>

// =====================================================
// BEACON CONFIG
// =====================================================

#define BEACON_NAME "BEACON-A01-R5M"

#define BEACON_UUID "57dcd1b1-ed48-49b6-8aff-e7a4bfb390da"

#define BEACON_MAJOR 1
#define BEACON_MINOR 1

// ระยะที่ตั้งใจใช้งาน
#define BEACON_TARGET_RANGE_M 5

// iBeacon Measured Power
// ค่า RSSI ที่ calibration ประมาณ 1 เมตร
#define BEACON_SIGNAL_POWER 0xC5  // -59

BLEAdvertising* advertising;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting iBeacon...");

  // =====================================================
  // BLE INIT
  // =====================================================

  BLEDevice::init(BEACON_NAME);

  // กำลังส่งจริง
  // -9 dBm เริ่มทดลองสำหรับประมาณ 3-5 เมตร
  BLEDevice::setPower(
    ESP_PWR_LVL_N9,
    ESP_BLE_PWR_TYPE_ADV
  );

  // =====================================================
  // IBEACON
  // =====================================================

  BLEBeacon beacon;

  beacon.setManufacturerId(0x004C);

  beacon.setProximityUUID(
    BLEUUID(BEACON_UUID)
  );

  beacon.setMajor(BEACON_MAJOR);
  beacon.setMinor(BEACON_MINOR);

  // ค่า calibration สำหรับให้ receiver คำนวณระยะ
  beacon.setSignalPower(
    BEACON_SIGNAL_POWER
  );

  // =====================================================
  // MAIN ADVERTISEMENT
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

  // จะเห็นชื่อประมาณ:
  // BEACON-A01-R5M
  scanResponseData.setName(BEACON_NAME);

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

  // Scan ได้ แต่ connect ไม่ได้
  advertising->setAdvertisementType(
    ADV_TYPE_SCAN_IND
  );

  // 100 ms
  advertising->setMinInterval(160);
  advertising->setMaxInterval(160);

  advertising->start();

  // =====================================================
  // DEBUG
  // =====================================================

  Serial.println();
  Serial.println("==============================");
  Serial.println("iBeacon started!");
  Serial.println("==============================");

  Serial.println("Name : " BEACON_NAME);
  Serial.println("UUID : " BEACON_UUID);

  Serial.printf(
    "Major: %d\n",
    BEACON_MAJOR
  );

  Serial.printf(
    "Minor: %d\n",
    BEACON_MINOR
  );

  Serial.println("RF TX Power : -9 dBm");

  Serial.printf(
    "Target Range: ~%d meters\n",
    BEACON_TARGET_RANGE_M
  );

  Serial.println("==============================");
}

void loop() {
  // Beacon broadcast only
  delay(10000);
}
