// Ce petit code permet une utilisation basique du module et des tags (détection simple d'un tag)
// Toute amélioration est la bienvenue, n'hésitez pas ~
// Ce code modifié provient de ljios : https://github.com/ljos/MFRC522

#include <MFRC522.h>
#include <SPI.h>

#define SAD 10
#define RST 5

MFRC522 nfc(SAD, RST);

void setup(){

	SPI.begin();
	Serial.begin(115200);
	Serial.println("Recherche du module RFID");

	nfc.begin();

	// Obtention du numéro de firmware du module RFID
	byte version = nfc.getFirmwareVersion();

	if(! version){

		// Si le module n'est pas trouvé
		Serial.print("Module introuvable");
      
		while(1); //Stop 
	}

	Serial.print("Module MFRC522 trouve: ");
	Serial.print("firmware version 0x");
	Serial.print(version, HEX);
	Serial.println(".");
}

byte keyA[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };
byte keyB[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };


void loop(){

	byte status;
	byte data[MAX_LEN];
	byte serial[5];

	int i, j, pos;

	// Envoie une demande générale aux fonctions
	// Si une carte est trouvée, définir l'état sur MI_OK
	status = nfc.requestTag(MF1_REQIDL, data);

	if(status == MI_OK){

		Serial.println("Tag detecte / Tag A ");
  
		status = nfc.antiCollision(data);
  
		memcpy(serial, data, 5);

		// Selection du tag qui sera lu. Si ceci n'est pas fait,
		// le module ne saura pas quel tag lire.
		nfc.selectTag(serial);

		// Supposant que la mémoire du module ne fait que 64 bits.
		for(i = 0; i < 64; i++){

			// Essai d'authentification de chaque block avec la clé A
			status = nfc.authenticate(MF1_AUTHENT1A, i, keyA, serial);

			if(status == MI_OK){
      
      			Serial.println("Acces autorise :] ");      
      			Serial.println(" ");
  
				// Lecture du block i dans le tag A
				status = nfc.readFromTag(i, data);
				if(status == MI_OK){
 
					for(j = 0; j < 15; j++){
					}
				}
				else{

  					Serial.println("Lecture impossible");      
				}
			}
			else{

				// Si l'authentification avec le tag A n'a pas réussie, 
				// essayer avec le tag B
				status = nfc.authenticate(MF1_AUTHENT1B, i, keyB, serial);

				if(status == MI_OK){
      
  					Serial.print("Authentification ");
  					Serial.println("avec la cle B");
  		
  					status = nfc.readFromTag(i, data);

					if(status == MI_OK){

  						Serial.print("Acces autorise");
					}
					else{

						Serial.println("Lecture du tag impossible ");
 						Serial.println(" ");  
					}
				}
			}

			// Arrêt de la lecture et préparation à une nouvelle lecture
			nfc.haltTag();
		}

		// Fréquence d'actualisation de la lecture de la carte
		delay(1000);
	}
}