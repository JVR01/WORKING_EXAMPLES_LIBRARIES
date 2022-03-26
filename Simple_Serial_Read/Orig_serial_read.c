#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include <stdlib.h> //??

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


int main(){
    //Initialise
    stdio_init_all(); 
    // Initialise GPIO (Green LED connected to pin 25)
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    char userInput;
    char buffer[13]; //13
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    int actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);
    
    //Main Loop 
    while(1){
        
        uint8_t ch = 0;
        int i = 0;

        /*while(uart_is_readable(UART_ID)) {
           ch = uart_getc(UART_ID);  //To read one Byte
           buffer[i] = ch;
           i++;
           sleep_ms(100);
           if(i == 12){buffer[i]='\0'; printf(">%s<\n", buffer); break;}
           
        }*/


        if(uart_is_readable(UART_ID)) {
           for (i = 0; i<11;i++){//12
                ch = uart_getc(UART_ID);  //To read one Byte
                buffer[i] = ch;
                sleep_ms(100);
                //if(i == 12){buffer[i]='\0'; printf(">%s<\n", buffer); break;}
            }
            buffer[12]='\0';
            printf(">%s<\n", buffer);
            size_t size = sizeof(buffer) / sizeof(buffer[0]);
            printf("size of buffer: %d\n", size);

            printf("fuera del while, valor i: %d \n", i);
            

            /*int i = 0;
            int MAX_SIZE_OF_MY_BUFFER = 13;
            char buffer2[MAX_SIZE_OF_MY_BUFFER]; 
            char ch2;

            while (i < MAX_SIZE_OF_MY_BUFFER - 1 && (ch2 = fgetc(buffer)) != EOF) {
                buffer2[i++] = ch2;
            }
            buffer2[i] = '\0';  // terminating character
            printf("Buffer2 >%s<\n", buffer2);*/
            char hora[13];
            sprintf(hora, "-%s-\n", buffer);

            printf("Buffer2 >%s<\n", hora);
             
        }
        
        
        //char buffer2[13];
        //uart_read_blocking(UART_ID, buffer2, 8);
        //printf("Buffer2>%s<\n", buffer2);
        /*if (uart_is_readable(UART_ID)) {
            //--printf("it is readable!\n");  
            uart_read_blocking(UART_ID, buffer, 8); //12,14 would cut and generate disorder 
            //uint8_t ch = uart_getc(UART_ID);  //To read one Byte
            //uart_putc(UART_ID, ch);           //To write one Byte
            printf("%s\n", buffer); 

            char value[6]; //6
            for (int i = 0; i<=4;i++){
                //if(buffer[i] == 'H'){value[i]='\0';break;}

                value[i]= buffer[i];
                //printf("value index %u : %02x\n", i ,value[i]); //org %s
            }
            value[5]='\0';

            printf("%s\n", value);
            int myint = atoi(value);
            printf("stoi(%s) is %d \n", value, myint);
            printf("---------\n"); 
        }*/
        /*if (uart_is_writable(UART_ID)) {
            // Change it slightly first!
            printf("it was writable!\n");
        }*/
        if(userInput == '1'){
            // Turn On LED
            gpio_put(25, 1); // Set pin 25 to high
            printf("LED switched on!\n");
        }
        else if(userInput == '0'){
            // Turn Off LED
            gpio_put(25, 0); // Set pin 25 to high.
            printf("LED switched off!\n");
        }
        else{
            printf("Invalid Input!\n");
            sleep_ms(100);
        }
        
    }
}
