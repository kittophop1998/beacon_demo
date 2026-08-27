#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>
#include <BLEUtils.h>

const uint8_t LINE_HWID[5] = {
  0x01,
  0x90,
  0xCC,
  0x1B,
  0x4F
};

void setup() {
  Serial.begin(115200);

  BLEDevice::init("LINE-Beacon");

  BLEAdvertising* advertising = BLEDevice::getAdvertising();
  advertising->stop();

  BLEAdvertisementData advData;
  advData.setFlags(0x06);

  BLEUUID lineUUID((uint16_t)0xFE6F);

  advData.setCompleteServices(lineUUID);

  uint8_t frame[] = {
    0x02, // LINE Simple Beacon frame type

    LINE_HWID[0],
    LINE_HWID[1],
    LINE_HWID[2],
    LINE_HWID[3],
    LINE_HWID[4],

    0x7F, // TxPower
    0x00  // Device Message
  };

  String serviceData((char*)frame, sizeof(frame));

  advData.setServiceData(lineUUID, serviceData);

  advertising->setAdvertisementData(advData);

  advertising->setMinInterval(160);
  advertising->setMaxInterval(160);

  advertising->start();

  Serial.println("LINE Simple Beacon started");
  Serial.println("HWID: 0190cc1b4f");
}

void loop() {
  delay(1000);
}
