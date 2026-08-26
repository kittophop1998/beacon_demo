#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>
#include <BLEBeacon.h>

// =====================================================
// BEACON CONFIG
// =====================================================

#define BEACON_UUID "57dcd1b1-ed48-49b6-8aff-e7a4bfb390da"

#define BEACON_MAJOR 1
#define BEACON_MINOR 1

// ค่า calibration ของ iBeacon
// ไม่ใช่กำลังส่งจริง
#define BEACON_SIGNAL_POWER 0xC5

BLEAdvertising* advertising;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting iBeacon...");

  // =====================================================
  // INIT BLE
  // =====================================================

  BLEDevice::init("ESP32-C3-BEACON");

  // =====================================================
  // RF TX POWER
  //
  // เริ่มที่ -9 dBm สำหรับระยะประมาณ 3-5 เมตร
  //
  // ถ้าไกลเกินไป  -> เปลี่ยนเป็น ESP_PWR_LVL_N12
  // ถ้าสั้นเกินไป -> เปลี่ยนเป็น ESP_PWR_LVL_N6
  // =====================================================

  BLEDevice::setPower(
    ESP_PWR_LVL_N9,
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

  beacon.setSignalPower(
    BEACON_SIGNAL_POWER
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
  // START ADVERTISING
  // =====================================================

  advertising = BLEDevice::getAdvertising();

  advertising->setAdvertisementData(
    advertisementData
  );

  advertising->start();

  // =====================================================
  // DEBUG
  // =====================================================

  Serial.println();
  Serial.println("============================");
  Serial.println("iBeacon started!");
  Serial.println("============================");
  Serial.println("UUID : " BEACON_UUID);
  Serial.printf("Major: %d\n", BEACON_MAJOR);
  Serial.printf("Minor: %d\n", BEACON_MINOR);
  Serial.println("TX Power: -9 dBm");
  Serial.println("Target range: ~3-5 meters");
  Serial.println("============================");
}

void loop() {
  // Broadcast อย่างเดียว
  delay(10000);
}