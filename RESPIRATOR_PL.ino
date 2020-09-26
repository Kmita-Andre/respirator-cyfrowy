
///////////////////////////////////////////////////////////////////////
/////        RESPIRATOR CYFROWY Z WYKORZYSTANIEM KOMPRESORA       /////
////           EWENTUALNIE BUTLI ZE SPRĘŻONYM POWIETRZEM          /////
/////       ZAPEWNIAJĄCY OD JEDNEGO DO TRZECH STANOWISK           /////
/////            PRZY ZASTOSOWANIU JEDNEGO KOMPRESORA             /////
/////                  AUTOR:  Andrzej Kmita                      /////
/////             WSPÓŁPRACA:  Krzysztof Kłopotowski              /////
/////                          Damian Karpiuk                     /////
///////////////////////////////////////////////////////////////////////
//                                                                   //
//      Przedstawiony program pozwala na użycie dowolnego kom-       //
//      presora lub butli ze sprężonym powietrzem jako respira-      //
//      tora. System umożliwia :                                     //
//                                                                   //
//        1. Regulację czasu trwania wdechu i wydechu od             //
//             0,1 s. do 30 sekund                                   //
//        2. Regulację ciśnienia wdychanego powietrza od             //
//             0,1 hPa. do 250 hPa.                                  //
//        3. Regulację górnej granicy ciśnienia po której            //
//             włącza się alarm dźwiękowy i świetlny                 //
//        4. Niezależną regulację każdej linii oddzielnie            //
//             w pełnym zakresie wymienionych parametrów             //
//        5. Zmianę poszczególnych parametrów każdej linii w         //
//             trakcie pracy                                         //
//                                                                   //
//      System wyposażony jest dodatkowo w program informujący       //
//      o ewentualnej awarii poszczególnych sond ciśnienia, z        //
//      automatycznym przełączeniem zaworów na - OTWARTY - w         //
//      celu zapewnienia dopływu powietrza z otoczenia. Jedno-       //
//      cześnie w przypadku jakichkolwiek anomalii, alarmu, czy      //
//      awarji poszczególnych linii, zapewnia niezależną pracę       //
//      pozostałych linii, ustawiając zawory uszkodzonej linii       //
//      na nadmuch - ZAMKNIĘTY, wydech - OTWARTY.                    //
//                                                                   //
///////////////////////////////////////////////////////////////////////


   #include <OneWire.h>
   #include "Wire.h"
   #include <Adafruit_BMP280.h>
   #include <SPI.h>
   #include <Adafruit_Sensor.h>
 
   #define PRZYCISK1 A4
   #define PRZYCISK2 5
   #define PRZYCISK3 6
   #define KRAN1 A3
   #define KRAN2  2
   #define KRAN3  3
   #define ALARM  4 
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
   float PRESS3 =37;
   float p5;
   float p6;
   float p7;
   float p8;
   float p9;  
   float p10;   
   float czas1 = 3800;
   float czas2 = 3500;
   float czas3 = 6000;

   int p1;
   int p2;
   int p3;
   int stop;    
   int stanKRAN1 = A3;
   int stanKRAN2 = 2;
   int stanKRAN3 = 3; 
   int licznik =0; 
   int tmp;
   char znak;
   
   unsigned long pracaKRAN1;
   unsigned long pracaKRAN2;
   unsigned long pracaKRAN3;
   unsigned long aktualnyCzas = 0;
   unsigned long zapamietanyCzasKRAN1 = 0;
   unsigned long zapamietanyCzasKRAN2 = 0;
   unsigned long zapamietanyCzasKRAN3 = 0; 
   const unsigned long shortPress = 200;
   const unsigned long longPress = 800;
   const unsigned long doublelongPress = 1000;

   boolean test1 = false; 
   boolean test2 = false;
   boolean test3 = false; 
   boolean test4 = false;   
   boolean test5 = false; 
   boolean test6 = false;
   boolean alarm1=false;
   boolean alarm2=false;   
   boolean alarm3=false;  
   boolean alarm4=false; 
   boolean alarm5=false; 
   boolean alarm6=false;   

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
   pinMode(KRAN1,OUTPUT);
   pinMode(KRAN2,OUTPUT);
   pinMode(KRAN3,OUTPUT);
   pinMode(LED4, OUTPUT);
   pinMode(LED5, OUTPUT);
   pinMode(LED6, OUTPUT);
   pinMode(ALARM, OUTPUT);
   pinMode(PRZYCISK1, INPUT_PULLUP);
   pinMode(PRZYCISK2, INPUT_PULLUP);
   pinMode(PRZYCISK3, INPUT_PULLUP);
   pinMode(button.pin, INPUT_PULLUP);
   digitalWrite (ALARM, LOW);
   digitalWrite (LED6, LOW);
   digitalWrite (LED5, LOW);
   digitalWrite (LED4, LOW);
   Serial.setTimeout(50);
   
   if (!bmp1.begin()) {
   Serial.println("Sensor BMP280 nr 1 nie dziala.");
   while (1);}
   delay(50);

   if (!bmp2.begin()) {
   Serial.println("Sensor BMP280 nr 2 nie dziala. ");
   while (1);}
   delay(50);
  
   if (!bmp3.begin()) {
   Serial.println("Sensor BMP280 nr 3 nie dziala. ");
   while (1);}
   delay(50);
    
   if (!bmp4.begin()) {
   Serial.println("Sensor BMP280 nr 4 nie dziala.");
   while (1);}
   Serial.println();
   delay(50); 

   }
 
void loop(){

   float pracaKRAN1 = czas1;
   float pracaKRAN2 = czas2;
   float pracaKRAN3 = czas3;
 //WPROWADZANIE ZMIAN PARAMETROW PRACY RESPIRATOROW                  //
   if(Serial.available()>0){
   znak = Serial.read(); 
   tmp = Serial.parseInt();
   Serial.println();
   Serial.print("                ");
   Serial.print("ZMIANA PARAMETROW = " );
   Serial.print(tmp);
   Serial.print(" ");
   Serial.print(znak);
   Serial.println();
//ZMIANA CZASU- "a2800" -KRAN1 PRZELACZA SIE CO 2.8 SEKUNDY         //
   if(tmp!=czas1 && znak=='a')
   czas1 = tmp;
//ZMIANA CZASU- "b2500" -KRAN2 PRZELACZA SIE CO 2.5 SEKUNDY         //
   if(tmp!=czas2 && znak=='b')   
   czas2 = tmp;
//ZMIANA CZASU- "c4000" -KRAN3 PRZELACZA SIE CO 4.0 SEKUNDY         //
   if(tmp!=czas3 && znak=='c')   
   czas3 = tmp;
//ZMIANA CISNIENIA- "x22" -BAZA CISNIENIA LINII 1 ZMIENIONA NA 22    //
   if(tmp!= PRESS1 && znak=='x') 
   PRESS1 = tmp;
//ZMIANA CISNIENIA- "y25" -BAZA CISNIENIA LINII 2 ZMIENIONA NA 25    //
   if(tmp!= PRESS2 && znak=='y') 
   PRESS2 = tmp;
//ZMIANA CISNIENIA- "z30" -BAZA CISNIENIA LINII 3 ZMIENIONA NA 30    //
   if(tmp!= PRESS3 && znak=='z') 
   PRESS3 = tmp;  }

   aktualnyCzas = millis();
//SYNCHRONIZACJA POSZCZEGOLNYCH LINII Z ODDECHEM PACJENTA            //
   if (digitalRead(PRZYCISK1) == LOW) {
   digitalWrite(KRAN1, HIGH); 
   } else {
   pracaKRAN1 = czas1; }
  
   if (digitalRead(PRZYCISK2) == LOW) {
   digitalWrite(KRAN2, HIGH); 
   } else {
   pracaKRAN2 = czas2;}

   if (digitalRead(PRZYCISK3) == LOW) {
   digitalWrite(KRAN3, HIGH); 
   } else {
   pracaKRAN3 = czas3;}
  
   if (aktualnyCzas - zapamietanyCzasKRAN1 >= pracaKRAN1) {
   zapamietanyCzasKRAN1 = aktualnyCzas;
   stanKRAN1 = !stanKRAN1;
   if(!alarm1){
   digitalWrite(KRAN1, stanKRAN1); }
   }
  
   if (aktualnyCzas - zapamietanyCzasKRAN2 >= pracaKRAN2) {
   zapamietanyCzasKRAN2 = aktualnyCzas;
   stanKRAN2 = !stanKRAN2;
   if(!alarm2){
   digitalWrite(KRAN2, stanKRAN2);}
   }
 
   if (aktualnyCzas - zapamietanyCzasKRAN3 >= pracaKRAN3) {
   zapamietanyCzasKRAN3 = aktualnyCzas;
   stanKRAN3 = !stanKRAN3;
   if(!alarm3){
   digitalWrite(KRAN3, stanKRAN3);}
   }
//UJEDNOLICENIE WSKAZAN SENSOROW                                     //
   float cisnienie1 = bmp1.readPressure() / 100.0 + 0.3;
   float cisnienie2 = bmp2.readPressure() / 100.0 - 0.6;
   float cisnienie3 = bmp3.readPressure() / 100.0 + 0.5;
   float cisnienie4 = bmp4.readPressure() / 100.0 ;
//WYZEROWANIE BAZ DO UJEDNOLICENIA DANYCH I WSKAZAN MONITORA         //
   float p1 = ( cisnienie1 - cisnienie4);
   float p2 = ( cisnienie2 - cisnienie4);
   float p3 = ( cisnienie3 - cisnienie4);
// USTALENIE GORNEJ GRANICY CISNIENIA                                //
   p5 = (PRESS1*1.6); 
   p6 = (PRESS2*1.4);
   p7 = (PRESS3*1.5);
//USTALENIE CISNIENIA PRACY KRANOW DOPROWADZAJACYCH POWIETRZE        //
   p8 = (PRESS1*1.05);
   p9 = (PRESS2*1.05);
   p10 = (PRESS3*1.05); 
//ALARM PRZEKROCZENIA CISNIENIA MAKSYMALNEGO LINIIA 1                //
   if(test1 == false){
   if (p1 >= p5){  
   test1 = true;
   stanKRAN1 = stop; 
   digitalWrite(KRAN1, LOW); 
   digitalWrite (LED4, HIGH);
   digitalWrite (ALARM, HIGH);  
   alarm1 = true; }
   }
//ALARM PRZEKROCZENIA CISNIENIA MAKSYMALNEGO LINIIA 2                //
   if(test2 == false){
   if (p2 >= p6){
   stanKRAN2 = stop; 
   test2 = true;
   digitalWrite(KRAN2, LOW); 
   digitalWrite (LED5, HIGH);
   digitalWrite (ALARM, HIGH);  
   alarm2 = true;}
   }
//ALARM PRZEKROCZENIA CISNIENIA MAKSYMALNEGO LINIIA 3                //

   if(test3 == false){
   if (p3 >= p7){
   stanKRAN3 = stop; 
   test3 = true;
   digitalWrite(KRAN3, LOW); 
   digitalWrite (LED6, HIGH);
   digitalWrite (ALARM, HIGH);  
   alarm3 = true;}
   }
//ZAMKNIECIE KRANU 1 PO UZYSKANIU ZADANEGO CISNIENIA                 //
   if( p1 > p8 && p1 < p5){ 
   digitalWrite(KRAN1, LOW); 
   stanKRAN1 = stop; 
   if( p1 < p8);{
   digitalWrite(KRAN1, LOW);
   digitalWrite(KRAN1, stanKRAN1);}    
   } 
//ZAMKNIECIE KRANU 2 PO UZYSKANIU ZADANEGO CISNIENIA                 //
   if( p2 > p9 && p2 < p6){ 
   digitalWrite(KRAN2, LOW); 
   stanKRAN2 = stop; 
   if( p2 < p9);{
   digitalWrite(KRAN2, LOW);
   digitalWrite(KRAN2, stanKRAN2);}    
   }  
//ZAMKNIECIE KRANU 3 PO UZYSKANIU ZADANEGO CISNIENIA                 //
   if( p3 > p10 && p3 < p7){ 
   digitalWrite(KRAN3, LOW); 
   stanKRAN3 = stop; 
   if( p3 < p10);{
   digitalWrite(KRAN3, LOW);
   digitalWrite(KRAN3, stanKRAN3);}    
   }
//ZAMKNIECIE KRANOW POWIETRZNYCH I URUCHOMIENIE ALARMOW - AWARIA BMP4//
   if (p1 < -10){  
   digitalWrite(KRAN1, LOW); 
   stanKRAN1 = stop;
   digitalWrite (LED4, HIGH);
   digitalWrite (ALARM, HIGH);
   digitalWrite(KRAN2, LOW); 
   stanKRAN2 = stop;
   digitalWrite (LED5, HIGH);
   digitalWrite(KRAN3, LOW); 
   stanKRAN3 = stop;
   digitalWrite (LED6, HIGH);
   
   if (p1 > -10) { 
   digitalWrite(KRAN1, stanKRAN1);
   digitalWrite(KRAN2, stanKRAN2); 
   digitalWrite(KRAN3, stanKRAN3);
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
   
   if ((aktualnyCzas - button.counter <= shortPress)) {
   stanKRAN1 = !stanKRAN1;  
   alarm1 = false;
   test1=false;
   digitalWrite(KRAN1, HIGH);    
   digitalWrite (LED4, LOW);
   digitalWrite (ALARM, LOW);}

   if (aktualnyCzas - button.counter <= longPress && (aktualnyCzas - button.counter > shortPress)) {
   stanKRAN2 = !stanKRAN2; 
   alarm2 = false;
   test2=false;
   digitalWrite(KRAN2, HIGH);    
   digitalWrite (LED5, LOW);
   digitalWrite (ALARM, LOW);}

   if ((aktualnyCzas - button.counter >= doublelongPress)) {
   stanKRAN3 = !stanKRAN3;  
   Serial.println();
   alarm3 = false;
   test3=false;
   digitalWrite(KRAN3, HIGH);  
   digitalWrite (LED6, LOW);
   digitalWrite (ALARM, LOW); }
   }
   button.prevState = button.currentState;
   } 

   if(aktualnyCzas%200==0){
   switch(licznik%8){ 
   case 0:{

   Serial.print("  CZAS 1,  ");
   Serial.print("          CZAS 2, ");
   Serial.print("          CZAS 3,");
   Serial.println();  
   Serial.print("  T1 - a,  ");   
   Serial.print("          T2 - b, ");   
   Serial.print("          T3 - c,");   
   Serial.println();
   }break;
   case 1:{
   Serial.print(" CISNIENE 1,      ");
   Serial.print(" CISNIENE 2,    ");
   Serial.print("   CISNIENE 3,");
   Serial.println();
   Serial.print("  P1 - x");   
   Serial.print(",            P2 - y");   
   Serial.print(",           P3 - z");
   Serial.print(",");     
   Serial.println();
  
   }break;
   case 2:{
   Serial.print("CZAS1=");
   Serial.print(czas1);
   Serial.print(",     CZAS2=");
   Serial.print(czas2);
   Serial.print(",    CZAS3=");
   Serial.print(czas3);
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
   
   Serial.print(" REG p1= ");
   Serial.print(p1);
   Serial.print(",    REG p2= ");
   Serial.print(p2);
   Serial.print(",     REG p3= ");
   Serial.print(p3);
   Serial.println();
   }break;
   case 7:{
    
   Serial.print(" p1= ");
   Serial.print(cisnienie1);   
   Serial.print(",  p2= ");
   Serial.print(cisnienie2);
   Serial.print(",  p3= ");
   Serial.print(cisnienie3);
   Serial.print(",  p4= ");
   Serial.print(cisnienie4);
   Serial.println();
   
   }break;
   default:{

   }break;
   
   }
    licznik++;
   Serial.println();
 
 }
}  
