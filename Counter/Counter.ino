#include <SevSeg.h>    // Bibliothèque pour l'afficheur 7 segments
#include <RTClib.h>    // Bibliothèque pour le module RTC

RTC_DS3231 rtc;         // Créer une instance de l'objet RTC_DS3231
SevSeg sevseg;          // Créer une instance de l'objet SevSeg

const int numDigits = 3;  // Nombre de digits (3 pour un afficheur 3 digits)
const int digitPins[] = {2, 3, 4};     // Broches de commande des digits
const int segmentPins[] = {5, 6, 7, 8}; // Broches de commande des segments

// Heures de début et de fin du compteur (de 0 à 23)
const int startHour = 10; // Début du compteur à 10h
const int endHour = 12;   // Fin du compteur à 12h

// Compteur initial à 300
int counter = 300;
#define BUTTON_INCREMENT_PIN 24
#define BUTTON_DECREMENT_PIN 25

void setup() {
  pinMode(BUTTON_INCREMENT_PIN,INPUT_PULLUP);
  pinMode(BUTTON_DECREMENT_PIN,INPUT_PULLUP);
  byte numDigitsInUse = numDigits;
  bool resistorsOnSegments = false;
  bool updateWithDelays = false;
  byte hardwareConfig = COMMON_ANODE; // Utiliser COMMON_ANODE pour un afficheur à anode commune

  sevseg.begin(hardwareConfig, numDigitsInUse, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90); // Ajuster la luminosité (de 0 à 100)

  // Initialiser le module RTC
  rtc.begin();
}

void loop() {
  // Obtenir l'heure actuelle à partir du module RTC
  DateTime now = rtc.now();
  
  // Vérifier si l'heure actuelle est comprise entre les heures de début et de fin du compteur
  if (now.hour() >= startHour && now.hour() <= endHour){
    // Compteur actif : incrémenter de 1 chaque seconde jusqu'à 999
    if (counter < 999) {
      sevseg.setNumber(counter);
      sevseg.refreshDisplay();
      delay(10000); // Délai d'une 10 seconde
      counter++;
    } else {
      // Si le compteur atteint 999, réinitialiser à 300
        counter = 300;
    } 
  }
  else {
    // Compteur inactif : afficher "OFF" sur l'afficheur
    sevseg.setChars("OFF");
    sevseg.refreshDisplay();
    delay(1000); // Délai d'une seconde
  }

  if (digitalRead(BUTTON_INCREMENT_PIN) == LOW) {
    // Bouton d'incrémentation enfoncé : incrémenter le compteur
    incrementCounter();
    delay(250); // Délai pour éviter les rebonds
  }

  if (digitalRead(BUTTON_DECREMENT_PIN) == LOW) {
    // Bouton de décrémentation enfoncé : décrémenter le compteur
    decrementCounter();
    delay(250); // Délai pour éviter les rebonds
  }

  // Afficher la valeur du compteur sur l'afficheur 7 segments
  sevseg.setNumber(counter);
  sevseg.refreshDisplay();
}

void incrementCounter() {
  if (counter < 999) {
    counter++;
  }
  
}

void decrementCounter() {
  if (counter > 300) {
    counter--;
  }
}
