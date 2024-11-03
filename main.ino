#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 21
#define RST_PIN 22
#define RXD2 12
#define TXD2 14

byte xiaomiunlock[10] = {0x55, 0xAA, 0x04, 0x20 ,0x02, 0x71, 0x01, 0x00, 0x67, 0xFF};
byte xiaomilock[10] = {0x55, 0xAA, 0x04, 0x20, 0x02, 0x70, 0x01, 0x00, 0x68, 0xFF};

byte ninebotunlock[11] = {0x5A, 0xA5, 0x02, 0x3D, 0x20, 0x02, 0x70, 0x01, 0x00, 0x2D, 0xFF};
byte ninebotlock[11] = {0x5A, 0xA5, 0x02, 0x3D, 0x20, 0x02, 0x71, 0x01, 0x00, 0x2C, 0xFF};

int status = 0;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
	SPI.begin();
	mfrc522.PCD_Init();
	delay(2);
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()){
    return;
  }
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "53 41 A8 F7"){
    if (Serial2.available()){
       status = 1;
       Serial2.write(xiaomiunlock,sizeof(xiaomiunlock));
    }
    Serial.println("Access Granted");
  }else{
    Serial.println("Access Denied");
    status = 0;
    if (Serial2.available()){
  Serial2.write(xiaomilock,sizeof(xiaomilock));

    }
    delay(3000);
  }
    if(status == 0){
      delay(10);
      Serial2.write(xiaomilock,sizeof(xiaomilock));
    }
} 
