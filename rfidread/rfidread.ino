#include <SPI.h>

#include <MFRC522.h>

#define RST_PIN 9           

#define SS_PIN 10          

MFRC522 mfrc522(SS_PIN, RST_PIN);   

MFRC522::MIFARE_Key key;

MFRC522::StatusCode card_status;



void setup(){

  Serial.begin(9600);                                           

  SPI.begin();                                                 

  mfrc522.PCD_Init();                                             

  Serial.println(F("READING DATA FROM RC522 CARD"));

}



void loop(){

  for (byte i = 0; i < 6; i++){

    key.keyByte[i] = 0xFF;

  }

  if(!mfrc522.PICC_IsNewCardPresent()){

    return;

  }  

  if(!mfrc522.PICC_ReadCardSerial()){

    return;

  } 

  Serial.println(F("**Start Reading:**"));

  byte block = 4;

  byte buffr[18];

  String data_on_card = readBytesFromBlock(block, buffr);

  Serial.print(data_on_card);

  Serial.println(F("\n**End Reading**\n")); 

  delay(1000); 

  mfrc522.PICC_HaltA();

  mfrc522.PCD_StopCrypto1();

}



String readBytesFromBlock(byte block, byte buff[]){

  card_status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));

  if(card_status != MFRC522::STATUS_OK){

    Serial.print(F("Authentication failed: "));

    Serial.println(mfrc522.GetStatusCodeName(card_status));

    return "";

  }

  // Read from block

  byte len = 18;

  card_status = mfrc522.MIFARE_Read(block, buff, &len);

  if (card_status != MFRC522::STATUS_OK) {

    Serial.print(F("Reading failed: "));

    Serial.println(mfrc522.GetStatusCodeName(card_status));

    return "";

  } 

  String value = "";

  for (uint8_t i = 0; i < 16; i++){

    value += (char)buff[i];

  }

  value.trim();

  return value;

}  
