#include <FastLED.h>

#define DATA_PIN 6
#define numberIntersections 2 //Anzahl Kreuzungen 
#define NUM_LEDS 4*numberIntersections //Anzahl LED
CRGB leds[NUM_LEDS]; //Speicherblock(Array) zum speichern/aendern der LED Daten. MUSS VIELFACHES VON 4 SEIN (4 Ampeln pro Kreuzung)
byte lastGreen[NUM_LEDS];
byte activeLED[numberIntersections];
byte oldLED[numberIntersections];
boolean tooLongRed = false;



void setup() {
    FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS); //LED Typ (WS2812B), Signalpin, Speicherarray, Anzahl LED
    for (byte i = 0; i< numberIntersections; i++){      //Eventuell nicht notwedig, kann halt sein das sonst unten eine Nullpointer Exeption bei den if vergleichen kommt
        oldLED[i] = 0;
    }
    for (byte i = 0; i< NUM_LEDS; i++){                 //Fülle Array für Anfang mit Nullen
        lastGreen[i] = 0;
    }
}


void loop() {
    for(byte i = 0; i< numberIntersections; i++){       //Iteriere durch Kreuzungen
        
        for (byte k = i; k <4*i+4;i++){                     //Überprüfe jede Ampel ob sie 6x hintereinander rot war
            if(lastGreen[k]>=6 && !tooLongRed){     
                activeLED[i] = k-4*i;
                tooLongRed = true;                      //Stoppe for schleife
            }
        }

        if (!tooLongRed){                               //Wenn nicht zu lange Rot, würfeln
            while (oldLED[i] != activeLED[i]){          //würfel bis neue Ampel (Nicht 2x hintereinander grün)
                activeLED[i] = random(4);
            }
        }
        
        for(byte j = i; j < 4*i+4; j++){
            
            if(j == oldLED[i]+4*i || j == activeLED[i]+4*i){    //Setze alte und neue Ampel auf Gelb, Rest auf Rot
                leds[j] = CRGB::Yellow;                  
                                  

            }
            else{
                leds[j] = CRGB::Red;                
            }
        }       
    }

    FastLED.show();
    delay(1500);

    for(byte i = 0; i< numberIntersections; i++){                 //Iteriere durch Kreuzungen

        for(byte j = i; j < 4*i+4; j++){                            //Setze neue auf Grün und verändere lastGreen[]
            
            if(j == activeLED[i]+4*i){
                leds[j] = CRGB::Green;
                lastGreen[j] = 0; 
            } else{
                leds[j] = CRGB::Red;
                lastGreen[j]++;
            }
        }
    }

    FastLED.show();
    for(byte i=0; i<numberIntersections;i++){
        oldLED[i] = activeLED[i];
    }
    tooLongRed = false;
    delay(10000);

}
