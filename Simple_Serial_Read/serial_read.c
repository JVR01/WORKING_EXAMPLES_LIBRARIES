#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdlib.h>

#define UART_ID uart0 //uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0 //0
#define UART_RX_PIN 1 //1
static int chars_rxed = 0;
bool led_status = false;


void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        // Can we send it back?
        if (uart_is_writable(UART_ID)) {
            // Change it slightly first!
            ch++;
            uart_putc(UART_ID, ch);
        }
        chars_rxed++;
    }
}

void toggle_led();
uint16_t getmetheValue(char *buf, uint8_t buf_size);

int main(){

    //Initialise
    stdio_init_all(); 
    // Initialise GPIO (Green LED connected to pin 25)
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    char userInput;
    char buffer[22]; //13
    uint8_t received_bytes = 0U;
    uint16_t co2_value =0U;

    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    int actual = uart_set_baudrate(UART_ID, BAUD_RATE);
    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    printf("Main executed!\n");
    
    //Main Loop 
    while(1){
        printf("***********************************************\n");
        
        if(uart_is_readable(UART_ID)) 
        {
           uint8_t i = 0;

           while (uart_is_readable(UART_ID))////should read 11 bytes. The pico does get the right message in the right orther, somehow the nano does not
            {
                buffer[i] = uart_getc(UART_ID);  
                if(i>=22){break;}
                i++;
            }

            received_bytes = i;
            co2_value = getmetheValue(buffer, received_bytes);

            printf("Buffer Content:>%s<\n", buffer);

            printf("Number of received bytes: %d\n", received_bytes);

            if(co2_value>399U)
            {
             printf("Co2: %d\n", co2_value);
            }


            buffer[received_bytes+1]='\0';
            printf("buf content + 1: >%s<\n", buffer);
            sleep_ms(100);
        }
        else
        {
            toggle_led(); 
        }
        
        
    }
}

void toggle_led()
{
    if(led_status)
    {
        // Turn Off LED
        led_status = false;
        //printf("LED switched off!\n");
    }
    else
    {
        // Turn On LED
        led_status = true;
        //printf("LED switched on!\n");
    }

    gpio_put(25, led_status);
    sleep_ms(100);
}

uint16_t getmetheValue(char *buf, uint8_t buf_size)
{
  uint16_t result = 0;
  char sub_string[buf_size];
  //char sub_string[12];
  
  int8_t s_index =-1;
  int8_t e_index =-1;
    
  for(uint8_t i=0; i<buf_size;i++)
  {
    //if(s_index!=-1 & e_index!=-1 & e_index>s_index){break;}  
    if(i<=buf_size-3 && buf[i] == 'a' && buf[i+1]== 'r' && buf[i+2]== 't'){s_index=i+3; /*printf("start charcter index [%d]<\n", s_index);*/} 
    if(i>=1 && buf[i] == 'e' && buf[i+1]== 'n' && buf[i+2]== 'd'){e_index=i-1; /*printf("end charcter index [%d] <\n", e_index);*/} 

     /*printf("Buff[%d] chrachter:>%c<\n", i, buf[i]);*/
  }
  
  if(s_index!=-1 & e_index!=-1)
  {
    uint8_t q=0;   
    for(uint8_t u=s_index; u<=e_index;u++)
    {
      sub_string[q]=buf[u];
      //printf("sub buf charcter [%c]<\n", sub_string[q]);
      //printf("sub buf charcter [%d]<\n", sub_string[q]);
      q++;
    }
    
    /*printf("sub string Content:>%s<\n", sub_string);*/

    result = atoi(sub_string);
    //sleep_ms(1000);
  }    
  return result;
}
