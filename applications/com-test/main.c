#include <arm_shared_memory_system.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <libpynq.h>
#include <platform.h>
#include <stdint.h>
#include <stepper.h>

void uart_read_array(const int uart, uint8_t *buf, uint8_t l) {
  for (uint8_t x = 0; x < l; x++) {
    buf[x] = uart_recv(uart);
    printf("%c",buf[x]);
  }
  printf("\n");
}

void uart_write_array(const int uart, uint8_t *buf, uint8_t l)
{
  for (uint8_t x = 0; x < l; x++) {
    uart_send(uart, buf[x]);
  }
}

int main(void) {
  pynq_init();
  switchbox_set_pin(IO_AR0, SWB_UART0_RX);
  switchbox_set_pin(IO_AR1, SWB_UART0_TX);
  // gpio_set_direction(IO_AR2, GPIO_DIR_INPUT);
  // gpio_set_direction(IO_AR3, GPIO_DIR_INPUT);
  // printf("AR2: %d\n", gpio_get_level(IO_AR2));
  // printf("AR3: %d\n", gpio_get_level(IO_AR3));

  uart_init(UART0);

  uart_reset_fifos(UART0);

  // json_tokener *tok = json_tokener_new();
  // json_object *root = json_tokener_parse_ex(tok, array, size);

  //for (int i = 0; i < 3; i++){

    // json_tokener *tok = json_tokener_new();
    // json_object *root = json_tokener_parse_ex(tok, input, size);
    json_object *jobj = json_object_new_object();

    // Add some key-value pairs to the object
    json_object_object_add(jobj, "key1", json_object_new_int(10));
    json_object_object_add(jobj, "key2", json_object_new_string("value2"));

    // Convert the JSON object to a string
    const char *json_string = json_object_to_json_string(jobj);

    printf("%s\n", json_string);
    
    char *input = json_string; //"Hello World with sauce and mayonaise";
    int size = strlen(input);
    // Print the JSON string

    uint32_t size_t = size;
    uart_write_array(UART0, (uint8_t *) &size_t, 4);
    
    char array[size];
    strcpy(array, input);
    uart_write_array(UART0, (uint8_t *) &array, size);
    sleep_msec(size*2); // min 2*size of message needed to send all info
  //}

  
  
  // uint32_t size = 1;
  // uart_write_array(UART0, (uint8_t *) &size, 4);
  // char array[1] = "H";
  // uart_write_array(UART0, (uint8_t *) &array, size);


  sleep_msec(10); //leave at 10

  pynq_destroy();
  return EXIT_SUCCESS;
}
