#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 10

byte readCard[4];
String item1 = "1CC6A5FE";  // Various items for scanning
String item2 = "9CC5A5FE";
String item3 = "ACBFA5FE";
String item4 = "1CBEA5FE";
String item5 = "D3FDBF60";
String tagID = "";
double Total_Cost = 0; //Total cost
int count_T = 0; //Total No. of items
int count_1 = 0; //Total No. of item1
int count_2 = 0; //Total No. of item2
int count_3 = 0; //Total No. of item3
int count_4 = 0; //Total No. of item4
int count_5 = 0; //Total No. of item5

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); //MFRC522
}

void loop() {
  getID();
  while (getID())
  {
    if (tagID == item1) {
      Total_Cost = Total_Cost + 3.3; // Compares tagID to UID and if it matches it will add to total cost
      count_1 = count_1 + 1;
      Serial.write('1');
    }
    if (tagID == item2) {
      Total_Cost = Total_Cost + 5;
      count_2 = count_2 + 1;
      Serial.write('2');
    }
    if (tagID == item3) {
      Total_Cost = Total_Cost + 10.6;
      count_3 = count_3 + 1;
      Serial.write('3');
    }
    if (tagID == item4) {
      Total_Cost = Total_Cost + 0.5;
      count_4 = count_4 + 1;
      Serial.write('4');
    }
    if (tagID == item5) {
      Total_Cost = Total_Cost + 1.9;
      count_5 = count_5 + 1;
      Serial.write('5');
    }

  }
}

boolean getID()
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
    //readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  delay(5);
  return true;
}
