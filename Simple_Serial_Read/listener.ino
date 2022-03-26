/*This Program runs on a Arduino nano connected to an OledScreen
 * it displays information of  a Co2 sensor tranmited via Serial
 * the serial informations comes from the SoftwareSerial(PIN RX, PIN tx)
 * the receiver should read faster as the transmission rate otherwise
 * wont get the excat number of bytes that are transmited!
*/

#include <SoftwareSerial.h>
#include "oled_stuff.h"

char rx_buffer[22]; //Initialized variable to store recieved data
String filtered_string1;
uint8_t received_bytes = 0U;

SoftwareSerial secondarySerial(3, 4); // RX, TX

void setup() {
  Serial.begin(9600);
  secondarySerial.begin(115200); 
  /*
  *USE TWO SERIAL AT SAME TIME MAKES PROBLEMS ON BOTH!
  *For this reaason uncoment "Serial.println()" lines just
  *for debbuging
   */
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);
  display.clearDisplay();
  //testdrawstyles();    // Draw 'stylized' characters
}

void loop() {
  //secondarySerial.readBytes(rx_buffer,10); //this does nothing!
  //mystr.toCharArray(rx_buffer, sizeof(rx_buffer));
  //Serial.println(rx_buffer); //Print data on Serial Monitor
  if(secondarySerial.available()>0)
  {
    uint8_t i = 0U;
    while (secondarySerial.available()>0)////should read 11 bytes
    {
      rx_buffer[i] = secondarySerial.read();  
      if(i>=22){break;}
      i++;
    }
    received_bytes = i;
    uint16_t co2_value = getmetheValue('t', rx_buffer, received_bytes);
    if(co2_value>399U)
    {
      printvalue(String("Received Bytes: "),received_bytes);
      print_anothervalue(filtered_string1+" : ", co2_value); 
      //print_anothervalue("Buffer: "+String(rx_buffer)+" ",0);
      print_anothervalue(String("Co2_value: "),co2_value);
      delay(500); 
    }
    else{display.clearDisplay();}  
  }
  
  delay(200);
}

void printvalue(String label, int val)
{
  display.clearDisplay();
  display.setTextSize(1);                 // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);    // Draw white text
  display.setCursor(0,0);  
  display.print(label);          
  display.println(val);
  display.display();
}
void print_anothervalue(String label, int val)
{
  display.print(label);          
  display.println(val);
  display.display();
}

uint16_t getmetheValue(char startCharacter, char *buf, uint8_t buf_size)
{
  uint16_t result = 0;
  String sub_string;
  int8_t s_index =-1;
  int8_t e_index =-1;
    
  for(uint8_t i=0; i<buf_size;i++)
  {
    if(i<=buf_size-3 && buf[i] == 'a' && buf[i+1]== 'r' && buf[i+2]== 't' ){s_index=i+3;} 
    if(i>=1 && i>s_index && buf[i] == 'e' && buf[i+1]== 'n' && buf[i+2]== 'd' ){e_index=i-1;} 
    if(s_index!=-1 & e_index!=-1 & e_index>s_index){break;}  
  }
  
  if(s_index!=-1 & e_index!=-1)
  {
    for(uint8_t u=s_index; u<=e_index;u++)
    {
      sub_string+=buf[u];
    }
    filtered_string1 = sub_string;
    result = sub_string.toInt();
  }    
  return result;
}
