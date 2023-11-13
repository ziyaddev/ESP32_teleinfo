#include <Arduino.h>

#define TIC_RX_PIN      16
#define TIC_TX_PIN      17

#define LED_PIN         0

// #define RGB_LED_PIN     25

// Pour clignotement LED asynchrone
unsigned long blinkLed  = 0;
uint8_t blinkDelay = 0;

// Uptime timer
boolean tick1sec = 0;// one for interrupt, don't mess with 
unsigned long uptime = 0; // save value we can use in sketch even if we're interrupted

// Used to indicate if we need to send all date or just modified ones
boolean fulldata = true;

// HardwareSerial Serial1(2);  // UART1/Serial2 pins 16,17
//HardwareSerial Serial1(1);  // UART1/Serial1 pins 9,10
//HardwareSerial Serial1(1);  // UART1/Serial1 pins 9,10



/* ======================================================================
Function: sendJSON 
Purpose : dump teleinfo values on serial
Input   : linked list pointer on the concerned data
          true to dump all values, false for only modified ones
Output  : - 
Comments: -
====================================================================== */
// void sendJSON(ValueList * me, boolean all)
// {
//   bool firstdata = true;

//   // Got at least one ?
//   if (me) {
//     // Json start
//     Serial.print(F("{"));

//     if (all) {
//       Serial.print(F("\"_UPTIME\":"));
//       Serial.print(uptime, DEC);
//       firstdata = false;
//     }

//     // Loop thru the node
//     while (me->next) {
//       // go to next node
//       me = me->next;

//       // uniquement sur les nouvelles valeurs ou celles modifiées 
//       // sauf si explicitement demandé toutes
//       if ( all || ( me->flags & (TINFO_FLAGS_UPDATED | TINFO_FLAGS_ADDED) ) )
//       {
//         // First elemement, no comma
//         if (firstdata)
//           firstdata = false;
//         else
//           Serial.print(F(", ")) ;

//         Serial.print(F("\"")) ;
//         Serial.print(me->name) ;
//         Serial.print(F("\":")) ;

//         // we have at least something ?
//         if (me->value && strlen(me->value))
//         {
//           boolean isNumber = true;
//           char * p = me->value;

//           // check if value is number
//           while (*p && isNumber) {
//             if ( *p < '0' || *p > '9' )
//               isNumber = false;
//             p++;
//           }
  
//           // this will add "" on not number values
//           if (!isNumber) {
//             Serial.print(F("\"")) ;
//             Serial.print(me->value) ;
//             Serial.print(F("\"")) ;
//           }
//           // this will remove leading zero on numbers
//           else
//             Serial.print(atol(me->value));
//         }
//       }
//     }
//    // Json end
//    Serial.println(F("}")) ;
//   }
// }

/* ======================================================================
Function: setup
Purpose : Setup I/O and other one time startup stuff
Input   : -
Output  : - 
Comments: -
====================================================================== */
void setup()
{
  // Serial, pour le debug
  Serial.begin(115200);
  Serial.println(F("\r\n\r\n=============="));
  Serial.println(F("Teleinfo"));
  pinMode(LED_PIN, OUTPUT);     

  // Configure Teleinfo Soft serial 
  // La téléinfo est connectee sur D3
  // ceci permet d'eviter les conflits avec la 
  // vraie serial lors des uploads
  Serial.printf_P(PSTR("TIC RX = GPIO=%d\r\n"), TIC_RX_PIN);
  Serial1.begin(1200, SERIAL_7E1, TIC_RX_PIN);
  pinMode(TIC_RX_PIN, INPUT_PULLUP);

}

void loop()
{
  static char c;
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  
  // On a reçu un caractère ?
  if ( Serial1.available() )
  {
    if (c == '\n')
      digitalWrite(LED_PIN, HIGH);

    // Le lire
    c = Serial1.read();

    // L'affcher dans la console
    Serial.print(c);
  }
  digitalWrite(LED_PIN, LOW);

  // if (currentMillis - previousMillis > 1000 ) {
  //   // save the last time you blinked the LED 
  //   previousMillis = currentMillis;   
  //   tick1sec = true;
  // }
}
