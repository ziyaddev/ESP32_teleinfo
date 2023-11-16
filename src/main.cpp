#include <Arduino.h>
#include "teleinfo.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

typedef struct teleinfo
{
  int	total_kwh;
  int inst_amp_1;
  int inst_amp_2;
  int inst_amp_3;
  int	amp_max_1;
  int amp_max_2;
  int amp_max_3;
  int max_pow;
  int app_pow;
} teleinfo;

teleinfo Teleinfo;

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

// void loop()
// {
//   static char c;
//   static unsigned long previousMillis = 0;
//   unsigned long currentMillis = millis();
  
//   // On a reçu un caractère ?
//   if ( Serial1.available() )
//   {
//     if (c == '\n')
//       digitalWrite(LED_PIN, HIGH);

//     // Le lire
//     c = Serial1.read();

//     // L'affcher dans la console
//     Serial.print(c);
//   }
//   digitalWrite(LED_PIN, LOW);

//   // if (currentMillis - previousMillis > 1000 ) {
//   //   // save the last time you blinked the LED 
//   //   previousMillis = currentMillis;   
//   //   tick1sec = true;
//   // }
// }

void loop()
{
  char *incoming_data;
  char *nrj_meter_data;

  Serial.println("loop begin");

  incoming_data = (char *)malloc(sizeof(char) * (209 + 1));
  if (!incoming_data)
    Serial.println("malloc error");

  Serial.println("1st malloc");

  // This function is used to fill in an empty string with serial incoming data
  incoming_data = ft_deserializer(incoming_data);
  Serial.println("ft_deserializer");

  nrj_meter_data = (char *)malloc(sizeof(char) * (9 + 1));
  Serial.println("malloc error\n");
  // if (!nrj_meter_data)
  //   Serial.println("malloc error after\n");
	Teleinfo.total_kwh = atoi(ft_teleinfo_extract(incoming_data, "\nBASE", 9, nrj_meter_data));
  Serial.println("teleinfo extract ok");

  free(nrj_meter_data);
  Serial.println("2nd malloc");

  nrj_meter_data = (char *)malloc(sizeof(char) * (3 + 1));
  // if (!nrj_meter_data)
  //   Serial.println("malloc error\n");
  Teleinfo.inst_amp_1 = atoi(ft_teleinfo_extract(incoming_data, "\nIINST1", 3, nrj_meter_data));
  Teleinfo.inst_amp_2 = atoi(ft_teleinfo_extract(incoming_data, "\nIINST2", 3, nrj_meter_data));
	Teleinfo.inst_amp_3 = atoi(ft_teleinfo_extract(incoming_data, "\nIINST3", 3, nrj_meter_data));
	Teleinfo.amp_max_1 = atoi(ft_teleinfo_extract(incoming_data, "\nIMAX1", 3, nrj_meter_data));
	Teleinfo.amp_max_2 = atoi(ft_teleinfo_extract(incoming_data, "\nIMAX2", 3, nrj_meter_data));
	Teleinfo.amp_max_3 = atoi(ft_teleinfo_extract(incoming_data, "\nIMAX3", 3, nrj_meter_data));
  free(nrj_meter_data);
  Serial.println("3rd malloc");

  nrj_meter_data = (char *)malloc(sizeof(char) * (5 + 1));
  // if (!nrj_meter_data)
  //   printf("malloc error\n");
	Teleinfo.app_pow = atoi(ft_teleinfo_extract(incoming_data, "\nPAPP", 5, nrj_meter_data));
  Teleinfo.max_pow = atoi(ft_teleinfo_extract(incoming_data, "\nPMAX", 5, nrj_meter_data));
  free(nrj_meter_data);

	printf("\nBASE : %d\n", Teleinfo.total_kwh);
	printf("Inst Amp 1 : %d\n", Teleinfo.inst_amp_1);
	printf("Inst Amp 2 : %d\n", Teleinfo.inst_amp_2);
	printf("Inst Amp 3 : %d\n", Teleinfo.inst_amp_3);
	printf("Max Amp 1 : %d\n", Teleinfo.amp_max_1);
	printf("Max Amp 2 : %d\n", Teleinfo.amp_max_2);
	printf("Max Amp 3 : %d\n", Teleinfo.amp_max_3);
	printf("Max Power : %d\n", Teleinfo.max_pow);
	printf("App Power : %d\n", Teleinfo.app_pow);
	printf("sizeof data : %ld\n", sizeof(incoming_data));

  free(incoming_data);
}
