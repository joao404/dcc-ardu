#include "S88.h"
#include "DCCpp_Uno.h"
#include "Comm.h"



void S88::init(int ModuleNumber)
{
  if(ModuleNumber>S88AdrMin && ModuleNumber<S88AdrMax)//Adresse groesser als Min und kleiner als Max
  {
    pinMode(S88ResetPin, OUTPUT);    //Reset
    pinMode(S88PSPin, OUTPUT);      //PS/LOAD
    pinMode(S88ClkPin, OUTPUT);      //Clock
    pinMode(S88DataPin, INPUT_PULLUP);    //Dateneingang
    digitalWrite(S88ResetPin, LOW);
    digitalWrite(S88PSPin, LOW);      //init
    digitalWrite(S88ClkPin, LOW);

    //Reserviere Speicher für die Daten
    S88InputBytes= ModuleNumber-S88AdrMin;
    if(S88InputArray!=NULL)
      free(S88InputArray);
    S88InputArray = new uint8_t[S88InputBytes];
    memset(S88InputArray,0xFF,sizeof(uint8_t)*S88InputBytes);
    S88previousMicros=micros();
    S88RCount=0;
    S88MCount=0;
    S88PCount=0;
    activ=true;
  }
  
}

void S88::deactivate(int ModuleNumber)
{
  if(ModuleNumber>S88AdrMin && ModuleNumber<S88AdrMax)//Adresse groesser als Min und kleiner als Max
  {
    if(S88InputArray!=NULL)
      free(S88InputArray); 
    S88InputArray==NULL;
  }
  activ=false;
}


void S88::cycle(void)
{
  if(true==activ)
  {
    unsigned long S88Micros = micros();
    if (S88Micros - S88previousMicros > S88CLOCKTIME) {
      GetInputData();    //S88 Bus Takt
      S88previousMicros = S88Micros;
    }
  }      
}


void S88::GetInputData(void)
{
  
  if (S88RCount == 3)    //Load/PS Leitung auf 1, darauf folgt ein Schiebetakt nach 10 ticks!
  digitalWrite(S88PSPin, HIGH);
  else if (S88RCount == 4)   //Schiebetakt nach 5 ticks und S88Module > 0
  digitalWrite(S88ClkPin, HIGH);       //1. Impuls
  else if (S88RCount == 5)   //Read Data IN 1. Bit und S88Module > 0
  readData();    //LOW-Flanke während Load/PS Schiebetakt, dann liegen die Daten an
  else if (S88RCount == 9)    //Reset-Plus, löscht die den Paralleleingängen vorgeschaltetetn Latches
  digitalWrite(S88ResetPin, HIGH);
  else if (S88RCount == 10)    //Ende Resetimpuls
  digitalWrite(S88ResetPin, LOW);
  else if (S88RCount == 11)    //Ende PS Phase
  digitalWrite(S88PSPin, LOW);
  else if (S88RCount >= 12)
  {    //Auslesen mit weiteren Schiebetakt der Latches links
  if (S88RCount % 2 == 0)      //wechselnder Taktimpuls/Schiebetakt
    digitalWrite(S88ClkPin, HIGH);
  else readData();    //Read Data IN 2. bis (Module*8) Bit
  }
  S88RCount++;      //Zähler für Durchläufe/Takt
  if (S88MCount == S88InputBytes)
  {  //Alle Module ausgelesen?
  S88RCount = 0;                    //setzte Zähler zurück
  S88MCount = 0;                  //beginne beim ersten Modul
  S88PCount = 0;                  //beginne beim ersten Port
  //init der Grundpegel
  digitalWrite(S88PSPin, LOW);
  digitalWrite(S88ClkPin, LOW);
  digitalWrite(S88ResetPin, LOW);
  }
}

//--------------------------------------------------------------
//Einlesen des Daten-Bit und Vergleich mit vorherigem Durchlauf
void S88::readData(void)
{
  digitalWrite(S88ClkPin, LOW);  //LOW-Flanke, dann liegen die Daten an
  byte getData = digitalRead(S88DataPin);  //Bit einlesen
  if (bitRead(S88InputArray[S88MCount],S88PCount) != getData) {     //Zustandsänderung Prüfen?
    bitWrite(S88InputArray[S88MCount],S88PCount,getData);          //Bitzustand Speichern
    //Zustand hat sich geändert
    if(0==getData){
      INTERFACE.print("<q");
      INTERFACE.print(S88MCount*8+S88PCount+S88AdrBase);
      INTERFACE.print(">");
    } else {
      INTERFACE.print("<Q");
      INTERFACE.print(S88MCount*8+S88PCount+S88AdrBase);
      INTERFACE.print(">");
    }
  }
  S88PCount++;
  if (S88PCount == 8) {
    S88PCount = 0;
    S88MCount++;
  }
}
