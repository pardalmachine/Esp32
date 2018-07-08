#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 1 ; //In seconds
BLEScan* pBLEScan;
int PinoPorta = 23;
bool PortaAberta = false;
bool AchouChave = false;
#define QuantidadeChaves 1
String Chaves[] = {"4c0002150065875ecf544baaa1a8fb908ea8d05700000000bf", "outra chave"};

bool ChaveCombina(char* chave) {
  String tChave = String(chave);

  //Serial.printf("%s<-->%s\n", chave, Chaves[0]);
  //Serial.printf("%d,%d", tChave.length(), Chaves[0].length() );
  for (int i = 0; i < QuantidadeChaves; i++) {
    if (Chaves[i] == tChave) {
      return true;
    }
  }
  return false;
}


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.printf("achou algum dispositivo\n");
      if (advertisedDevice.haveManufacturerData()) {
        char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
        Serial.printf("Chave Distpositivo: %s \n", pHex);
        if (ChaveCombina(pHex)){
          AchouChave = true;  
        }
        //Serial.printf("tem dado de fabricação: %s, %d \n", pHex, ChaveCombina(pHex));
      }
      //Serial.printf(manufacture);
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Inicio config");
  pinMode(PinoPorta, OUTPUT);
  
  BLEDevice::init("meuteste");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  Serial.println("Fim config");
  
}

void loop() {
  AchouChave = false;
  Serial.println("Inicio Scan...");
  BLEScanResults foundDevices = pBLEScan->start(scanTime);
  Serial.println("Fim Scan...");

  if (AchouChave != PortaAberta){
    if (AchouChave){
      PortaAberta=true;
      digitalWrite(PinoPorta, HIGH);
    } else {
      PortaAberta=false;
      digitalWrite(PinoPorta, LOW);
    }
  }
  Serial.printf("Valor da chave: %d, Valor Porta: %d \n", AchouChave, PortaAberta);

  delay(3000);
}


