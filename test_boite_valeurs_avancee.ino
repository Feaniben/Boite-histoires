/*Il s'agit de programme de la boîte à histoire maison, permettant, via un questionnaire à choix multiples en 3 étapes, de choisir entre 27 histoires audios enregistrées sur la carte sd du DFPLAYER.
Le Github :................ et toute la doc sur le drive google cologuiflu (branchements, documentations des hardwares achetés etc.
Chaque mp3 prend un numéro de 0001 à 9999 dans le dossier /mp3 de la sd, il suffit d'appeler le bon numéro au bon moment pour lire le bon mp3.
 */


#include "Arduino.h"
#include "SoftwareSerial.h"  //bibliotheque pour communiquer avec le DFplayer en utilisant les pins 10 et 11 comme ports serial
#include "DFRobotDFPlayerMini.h" // biblio du hardware 

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

int bouton2=6;   
int bouton3=7;
int bouton4=8;
int ledUn=5;  // led de temoin de marche
int ledDeux=12; // led de temoin de lecture audio en cours une histoire
boolean etatBouton2;
boolean etatBouton3;
boolean etatBouton4;
int piste; // correspond au numero de la piste mp3 de 0000 à 9999
int avancee;

void setup()
{
  pinMode(bouton2,INPUT_PULLUP); // il n'y a pas de bouton1 pour rester cohérent dans les valeurs attribuées à la variable piste par les boutons, car les pistes finissant par 1 sont réservées au menu d'accueuil.
  pinMode(bouton3,INPUT_PULLUP);
  pinMode(bouton4,INPUT_PULLUP);
  pinMode(ledUn,OUTPUT);
  pinMode(ledDeux,OUTPUT);
  etatBouton2=1;
  etatBouton3=1;
  etatBouton4=1;
  digitalWrite(ledUn,HIGH);
  digitalWrite(ledDeux,LOW);
  piste=0; // la piste /MP3/0000.mp3 est le menu accueil avec le premier choix multiple
  avancee=0; // permet de savoir le stade de choix auquel on en est, en fonction duquel les boutons ne declenchent pas les memes actions
    
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  
  Serial.println();
  Serial.println(F("Boite à histoires"));
  Serial.println(F("Initialisation, peut prendre quelques secondes"));
  
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //dis au DFplayer d'utiliser les ports serial software.
    Serial.println(F("Problème:"));
    Serial.println(F("1.Verifier la connection !"));
    Serial.println(F("2.Verifier la carte SD !"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer en ligne."));
  myDFPlayer.playMp3Folder(piste);
  delay(4000);
  avancee=1;
  
  myDFPlayer.volume(30);  //de 0 to 30, reste a voir si on veut le podifier avec un potentiometre
  
}

void loop()
{
  while (avancee<4)
  {
    myDFPlayer.playMp3Folder(1);// piste=1 donc on lit /MP3/0001.mp3 qui doit être la question correspondant au premier choix à trois réponses
    delay(13000); // le delay doit être adapté à la durée de chaque piste mp3 car sinon elle ne se lit pas en entier
    Serial.println("Fin du bonjour");
    avancee=1;
    Serial.print("avancee=");
    Serial.println(avancee);
  
    while (avancee==1) // la variable "avancee" donne le niveau du menu dans lequel on se trouve
    {
      etatBouton2=digitalRead(bouton2);
      etatBouton3=digitalRead(bouton3);
      etatBouton4=digitalRead(bouton4);
      Serial.println("Choix des personnages"); //le menu un donne le premier embranchement à 3 branches des histoires : celui des unités du numéro de piste 0000.mp3
      
      if (!etatBouton2)//si bouton appuyé
      {
        piste=2; 
        Serial.println("Histoires des enfants");
        delay(200);
        avancee=2;
      }
      if (!etatBouton3)//si bouton appuyé
      {
        piste=3; 
        Serial.println("Histoires fantastiques");
        delay(200);
        avancee=2;
      }
  
      if (!etatBouton4)//si bouton appuyé
      {
        piste=4;
        Serial.println("Histoires des animaux");
        delay(200);
        avancee=2;
      }
      delay(200);
    }
    
    Serial.print("piste=");
    Serial.println(piste);
    Serial.println("Audio du menu 2 en cours de lecture");
    myDFPlayer.playMp3Folder(piste);
    delay(15000);  // à adapter à la durée du menu audio
    Serial.println("Fin du menu 2");
    Serial.print("avancee=");
    Serial.println(avancee);

  
    while (avancee==2)
    {
      Serial.println("Choix des lieux"); // les lieux correspondent aux dizaines du numéro de piste
      etatBouton2=digitalRead(bouton2);
      etatBouton3=digitalRead(bouton3);
      etatBouton4=digitalRead(bouton4);
      
      if (!etatBouton2)//si bouton appuyé
      {
        piste=piste+20;
        Serial.println("Dizaine = 2");    
        delay(200);
        avancee=3;
      }

      if (!etatBouton3)//si bouton appuyé
      {
        piste=piste+30;
        Serial.println("Dizaine=3");
        delay(200);
        avancee=3;
      }
      if (!etatBouton4)//si bouton appuyé
      {
        piste=piste+40;
        Serial.println("Dizaine=4");
        delay(200);
        avancee=3;
      }
      delay(200);    
    }
    Serial.print("piste=");
    Serial.println(piste);
    Serial.println("Audio du menu 3 en cours de lecture");
    myDFPlayer.playMp3Folder(piste);
    delay(15000);  // à adapter à la durée du menu audio
    Serial.println("Fin du menu 3");
    Serial.print("avancee=");
    Serial.println(avancee);

    while (avancee==3)
    {
      Serial.println("Choix de l'histoire"); // les histoires correspondent aux centaines du numéro de piste (ne sont pas explcicitées par leur titre mais par un détail important de l'hitoire).
      etatBouton2=digitalRead(bouton2);
      etatBouton3=digitalRead(bouton3);
      etatBouton4=digitalRead(bouton4);
      
      if (!etatBouton2)//si bouton appuyé
      {
        piste=piste+200;
        Serial.println("Centaine = 2");    
        delay(200);
        avancee=4;
      }

      if (!etatBouton3)//si bouton appuyé
      {
        piste=piste+300;
        Serial.println("Centaine=3");
        delay(200);
        avancee=4;
      }
      if (!etatBouton4)//si bouton appuyé
      {
        piste=piste+400;
        Serial.println("Centaine=4");
        delay(200);
        avancee=4;
      }
      Serial.print("piste=");
      Serial.println(piste);
      Serial.println(avancee);
      delay(200);    
        
    }

    Serial.println("Histoire en cours de lecture");
    digitalWrite(ledDeux,HIGH);
    myDFPlayer.playMp3Folder(piste);
    delay(377000);  // à adapter à la durée des histoires
    Serial.println("Fin de l'histoire");
    myDFPlayer.playMp3Folder(0011); // "Si tu veux écouter une autre histoire, appuis sur le bouton noir"
    delay(5000);
    digitalWrite(ledUn,LOW);
    avancee++;
    Serial.println(avancee);

    
   }
}

 
  static unsigned long timer = millis();
  
  /*if (millis() - timer > 3000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
  }*/
  
/*  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
       } 
    
  }



void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}*/
