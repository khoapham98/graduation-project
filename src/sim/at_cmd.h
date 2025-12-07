#ifndef _AT_CMD_H_
#define _AT_CMD_H_
#include <stdint.h>

#define RESP_FRAME                  128

/* SWITCH BETWEEN MODES */
#define CMD_ENTER_CMD_MODE          "+++\r\n"
#define CMD_ENTER_DATA_MODE         "ATO\r\n"

/* BASIC CHECK */
#define AT_CMD_BASIC_CHECK          "AT\r\n"
#define AT_CMD_ECHO_ON              "ATE1\r\n"
#define AT_CMD_ECHO_OFF             "ATE0\r\n"

/* SIM & SIGNAL */
#define AT_CMD_READ_ICCID           "AT+CICCID\r\n"
#define AT_CMD_CHECK_READY          "AT+CPIN?\r\n"
#define AT_CMD_CHECK_SIGNAL         "AT+CSQ\r\n"
#define AT_CMD_CHECK_REG_NET        "AT+CREG?\r\n"
#define AT_CMD_CHECK_REG_GPRS       "AT+CGREG?\r\n"
#define AT_CMD_CHECK_REG_EPS        "AT+CEREG?\r\n"
#define AT_CMD_CHECK_OPERATOR       "AT+COPS?\r\n"

/* PDP CONTEXT / PACKET DATA */
#define AT_CMD_SET_PDP_CONTEXT      "AT+CGDCONT=1,\"IP\",\"%s\"\r\n"   // APN
#define AT_CMD_ATTACH_GPRS          "AT+CGATT=1\r\n"
#define AT_CMD_DETACH_GPRS          "AT+CGATT=0\r\n"
#define AT_CMD_ACTIVATE_PDP         "AT+CGACT=1,1\r\n"
#define AT_CMD_DEACTIVATE_PDP       "AT+CGACT=0,1\r\n"
#define AT_CMD_GET_IP_ADDR          "AT+CGPADDR=1\r\n"

/* AT send APIs */
int at_send_wait(char* cmd, uint64_t timeout_ms);
int at_send(char* cmd, size_t len);

int at_read(char* buf, size_t max_len, uint64_t timeout_ms);

int sim_init(char* uart_file_path);

#endif