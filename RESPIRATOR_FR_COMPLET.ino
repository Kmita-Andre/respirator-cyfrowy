///////////////////////////////////////////////////////////////////////////////////////////
//     Respirateur numérique utilisant un compresseur ou une bouteille d'air comprimé    //
// Pouvant assuré la respiration de une à trois poste pour aération avec un compresseur  //
//                                                                                       //
//                              Auteur : Andrzej kmita                                   //
//                          Coopération: Krzysztof Klopotowski                           //
//                                       Damian Karpiuk                                  //
//                                                                                       //
// Le programme présenté permet l'utilisation de n'importe quel compresseur ou cylindre  //
//                        avec de l'air comprimé comme respirateur.                      //
//                                                                                       //
//                                Le système permet:                                     //
//                                                                                       //
//    1. Régulations de la durée d'inspiration et d'expiration de 0,1 à 30 seconde       //
//                                                                                       //
//    2. régulation de la pression d'air inhalé à partir de 0,1 jusqu'à 250 hPa.         //
//                                                                                       //
//    3. Réglages de la limite de pression supérieur à laquelle une alarme sonore        //
//       et visuel est activé                                                            //
//                                                                                       //
//    4. Réglage indépendant de chaque ligne séparément dans la gamme complète           //
//       des paramètres liste.                                                           //
//                                                                                       //
//    5. Changement des paramètres individuels de chaque ligne pendant le déroulement    //
//       de la respiration                                                               //
//                                                                                       //
//    6. Le système est en outre équipé d'un programme informant de l'éventuelle         //
//       défaillance de sonde de pression individuelle avec station automatique des      //
//       des vannes sur ouverture. Pour fournir l'air  des environs.                     //
//       En même temps en quadrille ma liste d'alarmes ou de défaillance de lignes       //
//       individuelle à sur le fonctionnement indépendance des autres lignes             //
//       en réglant les vins de la ligne endommagé à souffler fermer échappement         //
//       ouverture lol.                                                                  //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////   
   
   
   
   
   #include <OneWire.h>
   #include "Wire.h"
   #include <Adafruit_BMP280.h>
   #include <SPI.h>
   #include <Adafruit_Sensor.h>
 
   #define BOUTON1 A4
   #define BOUTON2 5
   #define BOUTON3 6
   #define BOUTON4 A5
   #define VANNE1 A3
   #define VANNE2  2
   #define VANNE3  3
   #define ALARM  4  
   #define BMP_SCL 13
   #define BMP_SDO 12
   #define BMP_SDA 11
   #define BMP_CSB1 10
   #define BMP_CSB2 9
   #define BMP_CSB3 8
   #define BMP_CSB4 7
   #define PRESSED LOW
   #define NOT_PRESSED HIGH
   const byte LED4 = A0;
   const byte LED5 = A1;
   const byte LED6 = A2;

   
   const byte SONDY_PRESSURE = 4;
   OneWire ds(SONDY_PRESSURE);
   void startPomiarPRESSURE(const byte (SONDY_PRESSURE)) {
   ds.select(SONDY_PRESSURE);}
   float readPomiarPRESSURE(const byte (SONDY_PRESSURE)); 
   byte data(SONDY_PRESSURE); 
   Adafruit_BMP280 bmp1(BMP_CSB1, BMP_SDA, BMP_SDO, BMP_SCL);
   Adafruit_BMP280 bmp2(BMP_CSB2, BMP_SDA, BMP_SDO, BMP_SCL);
   Adafruit_BMP280 bmp3(BMP_CSB3, BMP_SDA, BMP_SDO, BMP_SCL);
   Adafruit_BMP280 bmp4(BMP_CSB4, BMP_SDA, BMP_SDO, BMP_SCL);

   float pressure;
   float PRESS1 =27;
   float PRESS2 =32;
   float PRESS3 =50;
   float p5;
   float p6;
   float p7;
   float p8;
   float p9;  
   float p10;   
   float temps1 = 3800;
   float temps2 = 4500;
   float temps3 = 3500;

   int p1;
   int p2;
   int p3;
   int stop;    
   int etatVANNE1 = A3;
   int etatVANNE2 = 2;
   int etatVANNE3 = 3; 
   int compteur =0; 
   int tmp;
   char symbole;
   
   unsigned long travailVANNE1;
   unsigned long travailVANNE2;
   unsigned long travailVANNE3;
   unsigned long actueltemps = 0;
   unsigned long memorisertempsVANNE1 = 0;
   unsigned long memorisertempsVANNE2 = 0;
   unsigned long memorisertempsVANNE3 = 0; 
   const unsigned long shortPress = 200;
   const unsigned long longPress = 800;
   const unsigned long doublelongPress = 1000;
    
   boolean test1 = false; 
   boolean test2 = false;
   boolean test3 = false; 
   boolean test4 = false;   
   boolean test5 = false; 
   boolean test6 = false;
   boolean alarm1 = false;
   boolean alarm2 = false;   
   boolean alarm3 = false;  
   boolean alarm4 = false; 
   boolean alarm5 = false; 
   boolean alarm6 = false;
   bool LEDState4 = true;
   bool LEDState5 = true;
   bool LEDState6 = true;
   
   typedef struct Buttons {
   const byte pin = A5;
   const int debounce = 10;
   unsigned long counter=0;
   bool prevState = NOT_PRESSED;
   bool currentState;}
   Button;
   Button button;   

void setup(){
  
   Serial.begin(9600);

   Wire.begin();
   pinMode(VANNE1,OUTPUT);
   pinMode(VANNE2,OUTPUT);
   pinMode(VANNE3,OUTPUT);
   pinMode(LED4, OUTPUT);
   pinMode(LED5, OUTPUT);
   pinMode(LED6, OUTPUT);
   pinMode(ALARM, OUTPUT);
   pinMode(BOUTON1, INPUT_PULLUP);
   pinMode(BOUTON2, INPUT_PULLUP);
   pinMode(BOUTON3, INPUT_PULLUP);
   pinMode(button.pin, INPUT_PULLUP);
   digitalWrite (ALARM, LOW);
   digitalWrite (LED6, LOW);
   digitalWrite (LED5, LOW);
   digitalWrite (LED4, LOW);
   if (!bmp1.begin()) {
   Serial.println("Sensor BMP280 device 1 was not found.");
   while (1);}
   delay(50);

   if (!bmp2.begin()) {
   Serial.println("Sensor BMP280 device 2 was not found.");
   while (1);}
   delay(50);
  
   if (!bmp3.begin()) {
   Serial.println("Sensor BMP280 device 3 was not found.");
   while (1);}
   delay(50);
    
   if (!bmp4.begin()) {
   Serial.println("Sensor BMP280 device 4 was not found.");
   while (1);}
   Serial.println();
   delay(50); 
   if (!bmp1.begin()) {
   Serial.println("Sensor BMP280 device 1 was not found.");
   while (1);}
   delay(50);

   if (!bmp2.begin()) {
   Serial.println("Sensor BMP280 device 2 was not found.");
   while (1);}
   delay(50);
  
   if (!bmp3.begin()) {
   Serial.println("Sensor BMP280 device 3 was not found.");
   while (1);}
   delay(50);
    
   if (!bmp4.begin()) {
   Serial.println("Sensor BMP280 device 4 was not found.");
   while (1);}
   Serial.println();
   delay(50); 
   } 
   
 
void loop(){

   float travailVANNE1 = temps1;
   float travailVANNE2 = temps2;
   float travailVANNE3 = temps3;
   
 //introduction des changement des parmètre du travaille du respirateur                  //
 
   if(Serial.available()>0){
   symbole = Serial.read(); 
   tmp = Serial.parseInt();
   Serial.println();
   Serial.print("                ");
   Serial.print("CHANGEMENT DES PARAMETRE = " );
   Serial.print(tmp);
   Serial.print(" ");
   Serial.print(symbole);
   Serial.println();
   
//Changement du temps- "a2800" -la durée de l'inspiration du VANNE1 est changé en 2.8 sec.//

   if(tmp!=temps1 && symbole=='a')
   temps1 = tmp;
   
//Changement du temps- "b2400" -la durée de l'inspiration du VANNE2 est changé en 2.4 sec.//

   if(tmp!=temps2 && symbole=='b')   
   temps2 = tmp;
   
//Changement du temps- "c3200" -la durée de l'inspiration du VANNE3 est changé en 3.2 sec.//

   if(tmp!=temps3 && symbole=='c')   
   temps3 = tmp;
   
//Changement de pression- "x22" -la base de la pression du module1 est de 22             //

   if(tmp!= PRESS1 && symbole=='x') 
   PRESS1 = tmp;
   
//Changement de pression- "y25" -la base de la pression du module2 est de 25             //

   if(tmp!= PRESS2 && symbole=='y') 
   PRESS2 = tmp;
   
//Changement de pression- "z30" -la base de la pression du module3 est de 30             //

   if(tmp!= PRESS3 && symbole=='z') 
   PRESS3 = tmp;  }

   actueltemps = millis();
   
//synchronisation de module individuel avec la respiration du patient                    //

   if (digitalRead(BOUTON1) == LOW) {
   digitalWrite(VANNE1, HIGH); 
   } else {
   travailVANNE1 = temps1; }
  
   if (digitalRead(BOUTON2) == LOW) {
   digitalWrite(VANNE2, HIGH); 
   } else {
   travailVANNE2 = temps2;}

   if (digitalRead(BOUTON3) == LOW) {
   digitalWrite(VANNE3, HIGH); 
   } else {
   travailVANNE3 = temps3;}
  
   if (actueltemps - memorisertempsVANNE1 >= travailVANNE1) {
   memorisertempsVANNE1 = actueltemps;
   etatVANNE1 = !etatVANNE1;
   if(!alarm1){
   digitalWrite(VANNE1, etatVANNE1); }
   }
  
   if (actueltemps - memorisertempsVANNE2 >= travailVANNE2) {
   memorisertempsVANNE2 = actueltemps;
   etatVANNE2 = !etatVANNE2;
   if(!alarm2){
   digitalWrite(VANNE2, etatVANNE2);}
   }
 
   if (actueltemps - memorisertempsVANNE3 >= travailVANNE3) {
   memorisertempsVANNE3 = actueltemps;
   etatVANNE3 = !etatVANNE3;
   if(!alarm3){
   digitalWrite(VANNE3, etatVANNE3);}
   }
//Egalisation des indications du capteur                                                 //

   float pressure1 = bmp1.readPressure() / 100.0 + 0.35;
   float pressure2 = bmp2.readPressure() / 100.0 - 0.50;
   float pressure3 = bmp3.readPressure() / 100.0 + 0.50;
   float pressure4 = bmp4.readPressure() / 100.0 ;
   
//remtre a zéro tout les base pour faire une unification des donné et des indication     //

   float p1 = ( pressure1 - pressure4);
   float p2 = ( pressure2 - pressure4);
   float p3 = ( pressure3 - pressure4);
   
// L'établissement de la frontiére de la plus haut pression                              //

   p5 = (PRESS1*1.6); 
   p6 = (PRESS2*1.4);
   p7 = (PRESS3*1.5);
   
//Détermination de la pression de fonctionnement des VANNE d'alimentation en air          //

   p8 = (PRESS1*1.05);
   p9 = (PRESS2*1.05);
   p10 = (PRESS3*1.05); 
   
//Alarme de dépassant la pression maximum du module 1                                    //

   if(test1 == false){
   if (p1 >= p5){  
   test1 = true;
   etatVANNE1 = stop; 
   digitalWrite(VANNE1, LOW); 
   digitalWrite (LED4, HIGH);
   digitalWrite (ALARM, HIGH);  
   alarm1 = true; }
   }


//Alarme de dépassant la pression maximum du module 2                                    //

   if(test2 == false){
   if (p2 >= p6){
   etatVANNE2 = stop; 
   test2 = true;
   digitalWrite(VANNE2, LOW); 
   digitalWrite (LED5, HIGH);
   digitalWrite (ALARM, HIGH);  
   alarm2 = true;}
   }
  

//Alarme de dépassant la pression maximum du module 3                                    //

   if(test3 == false){
   if (p3 >= p7){
   etatVANNE3 = stop; 
   test3 = true;
   digitalWrite(VANNE3, LOW); 
   digitalWrite (LED6, HIGH);
   digitalWrite (ALARM, HIGH);  
   alarm3 = true;}
   }

//Fermeture du VANNE 1 après la pression requise                                          //

   if( p1 > p8 && p1 < p5){ 
   digitalWrite(VANNE1, LOW); 
   etatVANNE1 = stop; 
   if( p1 < p8);{
   digitalWrite(VANNE1, LOW);
   digitalWrite(VANNE1, etatVANNE1);}    
   } 
//Fermeture du VANNE 2 après la pression requise                                          //

   if( p2 > p9 && p2 < p6){ 
   digitalWrite(VANNE2, LOW); 
   etatVANNE2 = stop; 
   if( p2 < p9);{
   digitalWrite(VANNE2, LOW);
   digitalWrite(VANNE2, etatVANNE2);}    
   }  
//Fermeture du VANNE 3 après la pression requise                                          //

   if( p3 > p10 && p3 < p7){ 
   digitalWrite(VANNE3, LOW); 
   etatVANNE3 = stop; 
   if( p3 < p10);{
   digitalWrite(VANNE3, LOW);
   digitalWrite(VANNE3, etatVANNE3);}    
   }
//Fermer du du VANNE d'air et démarrer l'alarme accident du capteur BMP4                  //

   if (p1 < -10){  
   digitalWrite(VANNE1, LOW); 
   etatVANNE1 = stop;
   digitalWrite (LED4, HIGH);
   digitalWrite (ALARM, HIGH);
   digitalWrite(VANNE2, LOW); 
   etatVANNE2 = stop;
   digitalWrite (LED5, HIGH);
   digitalWrite(VANNE3, LOW); 
   etatVANNE3 = stop;
   digitalWrite (LED6, HIGH);
   
   if (p1 > -10) { 
   digitalWrite(VANNE1, etatVANNE1);
   digitalWrite(VANNE2, etatVANNE2); 
   digitalWrite(VANNE3, etatVANNE3);
   digitalWrite (LED4, LOW);
   digitalWrite (LED5, LOW);
   digitalWrite (LED6, LOW);
   digitalWrite (ALARM, LOW);}    
   }
   
   button.currentState = digitalRead(button.pin);
   if (button.currentState != button.prevState) {
   delay(button.debounce);
   button.currentState = digitalRead(button.pin);
   if (button.currentState == PRESSED) {
   button.counter = millis();}
   if (button.currentState == NOT_PRESSED) { 
   
   if ((actueltemps - button.counter <= shortPress)) {
   etatVANNE1 = !etatVANNE1;  
   alarm1 = false;
   test1=false;
   digitalWrite(VANNE1, HIGH);    
   digitalWrite (LED4, LOW);
   digitalWrite (ALARM, LOW);}

   if ((actueltemps - button.counter <= longPress) && (actueltemps - button.counter > shortPress)) {
   etatVANNE2 = !etatVANNE2; 
   alarm2 = false;
   test2=false;
   digitalWrite(VANNE2, HIGH);    
   digitalWrite (LED5, LOW);
   digitalWrite (ALARM, LOW);}

   if ((actueltemps - button.counter >= doublelongPress)) {
   etatVANNE3 = !etatVANNE3;  
   Serial.println();
   alarm3 = false;
   test3=false;
   digitalWrite(VANNE3, HIGH);  
   digitalWrite (LED6, LOW);
   digitalWrite (ALARM, LOW); }
   }
   button.prevState = button.currentState;
   } 




   if(actueltemps%200==0){
   switch(compteur%8){ 
   case 0:{

   Serial.print("   TEMPS 1,  ");
   Serial.print("        TEMPS 2, ");
   Serial.print("        TEMPS 3,");
   Serial.println();  
   Serial.print("   T1 - a,  ");   
   Serial.print("         T2 - b, ");   
   Serial.print("         T3 - c,");   
   Serial.println();
   }break;
   case 1:{
   Serial.print(" PRESSION 1,      ");
   Serial.print(" PRESSION 2,    ");
   Serial.print("   PRESSION 3,");
   Serial.println();
   Serial.print("   P1 - x");   
   Serial.print(",           P2 - y");   
   Serial.print(",          P3 - z");
   Serial.print(",");     
   Serial.println();
  
   }break;
   case 2:{
   Serial.print("temps1=");
   Serial.print(temps1);
   Serial.print(",   temps2=");
   Serial.print(temps2);
   Serial.print(",   temps3=");
   Serial.print(temps3);
   Serial.println();

   }break;
   case 3:{
 
   Serial.print(" PRESS1=");
   Serial.print(PRESS1);
   Serial.print(",     PRESS2=");
   Serial.print(PRESS2);
   Serial.print(",     PRESS3=");
   Serial.print(PRESS3);
   Serial.println();

   }break;
   case 4:{
    
   Serial.print("  "); 
   Serial.print("p5= ");
   Serial.print(p5);
   Serial.print(",        p6= ");
   Serial.print(p6);
   Serial.print(",        p7= ");
   Serial.print(p7);
   Serial.println();

   }break;
   case 5:{

   Serial.print("  "); 
   Serial.print("p8= ");
   Serial.print(p8);
   Serial.print(",        p9= ");
   Serial.print(p9);
   Serial.print(",        p10= ");
   Serial.print(p10);
   Serial.println();

   }break;
   case 6:{
   
   Serial.print("REG p1= ");
   Serial.print(p1);
   Serial.print(",     REG p2= ");
   Serial.print(p2);
   Serial.print(",     REG p3= ");
   Serial.print(p3);
   Serial.println();
   }break;
   case 7:{
    
   Serial.print("p1= ");
   Serial.print(pressure1);   
   Serial.print(",   p2= ");
   Serial.print(pressure2);
   Serial.print(",  p3= ");
   Serial.print(pressure3);
   Serial.print(",   p4= ");
   Serial.print(pressure4);
   Serial.println();
   
   }break;
   default:{

   }break; }
   
    compteur++;
   Serial.println();
 }
}  
