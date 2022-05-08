/*
 * blue_demo.c
 *
 *  Created on: 2018年3月29日
 *      Author: CCORE YY
 */

//#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdarg.h>
#include "string.h"
#include "debug.h"
#include "uart_drv.h"
#include "bt_drv.h"
#include "delay.h"
#include "uart_demo.h"
#include "ioctrl_drv.h"

#define UART_MAX_BUFLEN 1024

extern UINT32 g_ips_clk;//系统时钟全局变量
volatile unsigned char g_uart_buffer[UART_MAX_BUFLEN];

static void cmd_clear_patch(const char *arg)
{
	bt_cmd_clear_patch();
	printf("please download patch again...");
}

static void cmd_dl_patchcfg(const char *arg)
{
	unsigned char patch;

	if (!arg[0]) {
		printf("%s: Invalid parameter", __func__);
		return;
	}

	patch = (uint8_t)strtoul(arg, NULL, 10);

	bt_cmd_dl_patchcfg(patch);
}

static void cmd_wakeup(const char *arg)
{
	bt_cmd_wakeup();
}

static void cmd_read_sw_version(const char *arg)
{
	bt_cmd_read_sw_version();
}

static void cmd_erase_paired_device(const char *arg)
{
	uint8_t device_type;

	if (!arg[0]) {
		printf("%s: Invalid parameter", __func__);
		return;
	}

	device_type = (uint8_t)strtoul(arg, NULL, 10);
	if (device_type != 1 && device_type !=2) {
		printf("%s: unknown device type", __func__);
		return;
	}

	bt_cmd_erase_paired_device(device_type);
	printf("%s", __func__);
}

static void cmd_read_bt_status(const char *arg)
{
	bt_cmd_read_bt_status();
}

static void cmd_set_iocap(const char *arg)
{
	unsigned long val;

	val = (uint8_t)strtoul(arg, NULL, 10);
	bt_cmd_set_iocap(val);
	printf("Sent iocap set cmd");
}

static void cmd_baud_set(const char *arg)
{
	uint32_t val;

	val = strtoul(arg, NULL, 10);
	bt_cmd_baud_set(val);
}

static void cmd_flowctl_set(const char *arg)
{
	uint32_t val;

	val = strtoul(arg, NULL, 10);
	bt_cmd_flowctl_set(val);
	printf("set flow ctl");
}

static void cmd_bt_reset(const char *arg)
{
	bt_cmd_bt_reset();
}

static void cmd_set_logmask(const char *arg)
{
	bt_cmd_set_logmask();
}

static void cmd_ble_set_sc(const char *arg)
{
	char *p;
	char *delim = " ";
	unsigned char sc, mitm;

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid param", __func__);
		return;
	}
	sc = (uint8_t)strtoul(p, NULL, 10);

	p = strtok(NULL, delim);
	if (!p) {
		printf("%s: No MITM flag, set it to 1", __func__);
		mitm = 1;
	} else
		mitm = (uint8_t)strtoul(p, NULL, 10);

	bt_cmd_ble_set_sc(sc, mitm);
}

static void cmd_read_le_addr(const char *arg)
{
	bt_cmd_read_le_addr();
}

static void cmd_read_ble_remote_name(const char *arg)
{
	bt_cmd_read_ble_remote_name();
}

static void cmd_gatt_send(const char *arg)
{
//	uint16_t i;
//	uint8_t buff[512];

//	for (i = 0; i < sizeof(buff); i++)
//		buff[i] = i;

//	bt_cmd_gatt_send(buff, sizeof(buff));
//	printf("send gatt data");
	int len = strlen(arg);
	bt_cmd_gatt_send((unsigned char *)arg, len);
}

static void cmd_ble_set_adv_mode(const char *arg)
{
	uint8_t adv_mode;

	if (!arg[0]) {
		printf("%s: Invalid parameter", __func__);
		return;
	}

	adv_mode =  (uint8_t)strtoul(arg, NULL, 10);
	bt_cmd_ble_set_adv_mode(adv_mode);
}

static void cmd_ble_adv_enable(const char *arg)
{
	uint8_t adv_enable;

	if (!arg[0]) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	adv_enable = (uint8_t)strtoul(arg, NULL, 10);

	bt_cmd_ble_adv_enable(adv_enable);

	printf("adv_enable = %d",adv_enable);
}

static void cmd_ble_write_device_name(const char *arg)
{
	int len = strlen(arg);
	uint8_t buff[40] = { 0 };

	if (len > 39)
		len = 39;
	memcpy(buff, arg, len);
	buff[len] = '\0';

	bt_cmd_ble_write_device_name(buff, len+1);
}

static void cmd_write_le_addr(const char *arg)
{
	unsigned char type;
	unsigned char addr[6];
	char *delim = " ";
	char *p;

	p = strtok((char *)arg, delim);

	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}

	type = (uint8_t)strtol(p, NULL, 16);

	p = strtok(NULL, delim);
	if (!p) {
		printf("%s: No address parameter", __func__);
		return;
	}

	memcpy(addr, p, strlen(p));

	bt_cmd_write_le_addr(type,(char *)addr);

	printf("set addr type: %u, address: %s", type, addr);
}

static void cmd_ble_set_adv_inerval(const char *arg)
{
	uint16_t adv_interval;

	if (!arg[0]) {
		printf("%s: Invalid parameter", __func__);
		return;
	}

	adv_interval = (uint16_t)strtoul(arg, NULL, 10);
	bt_cmd_ble_set_adv_inerval(adv_interval);
}

static void cmd_ble_write_adv_data(const char *arg)
{
	bt_cmd_ble_write_adv_data();
}

static void cmd_ble_write_scan_rsp_data(const char *arg)
{
	bt_cmd_ble_write_scan_rsp_data();
}

static void cmd_ble_disconnect(const char *arg)
{
	bt_cmd_ble_disconnect();
}

static void cmd_bleconn_upd(const char *arg)
{
	char *delim = " ";
	char *p;
	unsigned short interval_min,interval_max,latency,timeout;

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	interval_min = (uint16_t)strtol(p, NULL, 10);

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	interval_max = (uint16_t)strtol(p, NULL, 10);

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	latency = (uint16_t)strtol(p, NULL, 10);

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	timeout = (uint16_t)strtol(p, NULL, 10);

	bt_cmd_bleconn_upd(interval_min, interval_max, latency, timeout);

	printf("Send ble conn upd test");
}

static void cmd_ble_read_rssi(const char *arg)
{
	bt_cmd_ble_read_rssi();
}

static void cmd_ble_set_dis(const char *arg)
{
	char *delim = " ";
	char *p;
	int type;
	int len;
	uint8_t buff[40];

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	type = (int)strtol(p, NULL, 10);

	p = strtok(NULL, delim);
	if (!p) {
		printf("No manufacture name string");
		return;
	}
	len = strlen(p) + 1;
	memcpy(buff, p, len);

	bt_cmd_ble_set_dis(type, buff, len);
}

static void cmd_gatttptx_test(const char *arg)
{
	uint32_t val;

	if (arg[0])
		val = strtoul(arg, NULL, 10);
	else
		val = 80;

	bt_cmd_gatttptx_test(val);
}

static void cmd_write_pin(const char *arg)
{
	bt_cmd_write_pin();
	printf("Send pin code");
}

static void cmd_legacy_write_device_name(const char *arg)
{
	int len = strlen(arg);
	uint8_t buff[40] = { 0 };

	if (len > 39)
		len = 39;
	memcpy(buff, arg, len);
	buff[len] = '\0';

	bt_cmd_legacy_write_device_name(buff, len);
}

static void cmd_legacy_set_radio_mode(const char *arg)
{
	uint8_t radio_mode;
	if (!arg[0]) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	radio_mode =  (uint8_t)strtoul(arg, NULL, 10);;

	bt_cmd_legacy_set_radio_mode(radio_mode);
}

static void cmd_legacy_set_page_scan_para(const char *arg)
{
	char *delim = " ";
	char *p;
	unsigned char type;
	unsigned short interval, window, timeout;

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	type = (uint8_t)strtol(p, NULL, 10);

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	interval = (uint16_t)strtol(p, NULL, 10);

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	window = (uint16_t)strtol(p, NULL, 10);

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	timeout = (uint16_t)strtol(p, NULL, 10);

	bt_cmd_legacy_set_page_scan_para(type, interval, window, timeout);
}

static void cmd_legacy_set_inq_scan_para(const char *arg)
{
	char *delim = " ";
	char *p;
	unsigned char type;
	unsigned short interval, window;

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	type = (uint8_t)strtol(p, NULL, 10);

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	interval = (uint16_t)strtol(p, NULL, 10);

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	window = (uint16_t)strtol(p, NULL, 10);

	bt_cmd_legacy_set_inq_scan_para(type, interval, window);
}

static void cmd_spp_connect(const char *arg)
{
	bt_cmd_spp_connect((char *)arg);
	printf("spp connect test");
}

static void cmd_legacy_read_rssi(const char *arg)
{
	bt_cmd_legacy_read_rssi();
}

static void cmd_send_spp_data(const char *arg)
{
	uint16_t i;
	uint8_t buff[512];

	for (i = 0; i < sizeof(buff); i++)
		buff[i] = i;

	bt_cmd_send_spp_data(buff, sizeof(buff));

	printf("spp test");
	return;
}

static void cmd_spptptx_test(const char *arg)
{
	uint32_t sz;

	if (arg[0])
		sz = strtoul(arg, NULL, 10);
	else
		sz = 80;

	bt_cmd_spptptx_test(sz);
}

static void cmd_legacy_read_remote_device_name(const char *arg)
{
	bt_cmd_legacy_read_remote_device_name();
}

static void cmd_spp_disconnect(const char *arg)
{
	bt_cmd_spp_disconnect();
}

#define UUID_SERIAL_PORT                    0x1101
#define UUID_L2CAP                          0x0100
#define UUID_RFCOMM                         0x0003

uint8_t test_spp_sdp_record[] = {
	0x35,
	0x4D,
	//attribute SDP_ATTR_SRV_CLASS_ID_LIST
	0x09,
	(uint8_t)(0x0001 >> 8),
	(uint8_t)0x0001,
	0x35,
	0x03,
	0x19,
	(uint8_t)(UUID_SERIAL_PORT >> 8),
	(uint8_t)(UUID_SERIAL_PORT),

	//attribute SDP_ATTR_PROTO_DESC_LIST
	0x09,
	(uint8_t)(0x0004 >> 8),
	(uint8_t)0x0004,
	0x35,
	0x0c,
	0x35,
	0x03,
	0x19,
	(uint8_t)(UUID_L2CAP >> 8),
	(uint8_t)UUID_L2CAP,
	0x35,
	0x05,
	0x19,
	(uint8_t)(UUID_RFCOMM >> 8),
	(uint8_t)UUID_RFCOMM,
	0x08,
	11,

	//attribute SDP_ATTR_BROWSE_GROUP_LIST
	0x09,
	(uint8_t)(0x0005 >> 8),
	(uint8_t)0x0005,
	0x35,
	0x03,
	0x19,
	(uint8_t)(0x1002 >> 8),
	(uint8_t)0x1002,

	//attribute SDP_ATTR_LANG_BASE_ATTR_ID_LIST
	0x09,
	(uint8_t)(0x0006 >> 8),
	(uint8_t)0x0006,
	0x35,
	0x09,
	0x09,
	(uint8_t)(0x656e >> 8),
	(uint8_t)0x656e,
	0x09,
	(uint8_t)(106 >> 8),
	(uint8_t)106,
	0x09,
	(uint8_t)(0x0100 >> 8),
	(uint8_t)0x0100,

	//attribute SDP_ATTR_PROFILE_DESC_LIST
	0x09,
	(uint8_t)(0x0009 >> 8),
	(uint8_t)0x0009,
	0x35,
	0x08,
	0x35,
	0x06,
	0x19,
	(uint8_t)(UUID_SERIAL_PORT >> 8),
	(uint8_t)UUID_SERIAL_PORT,
	0x09,
	0x01,//version 1.2
	0x02,

	//attribute SDP_ATTR_SRV_NAME
	0x09,
	(uint8_t)((0x0000 + 0x0100) >> 8),
	(uint8_t)(0x0000 + 0x0100),
	0x25,
	0x0B,
	0x73, 0x65, 0x72, 0x69, 0x61, 0x6c, 0x20, 0x70, 0x6f, 0x72, 0x74 //"serial port"
};

static void cmd_legacy_set_sdprec(const char *arg)
{
	bt_cmd_legacy_set_sdprec(test_spp_sdp_record, sizeof(test_spp_sdp_record));
}

static void cmd_legacy_set_did(const char *arg)
{
	char *delim = " ";
	char *p;
	uint16_t vendor_id;
	uint16_t vendor_id_src;
	uint16_t product_id;
	uint16_t product_ver;

	p = strtok((char *)arg, delim);
	if (!p) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	vendor_id = (uint16_t) strtol(p, NULL, 16);

	p = strtok(NULL, delim);
	if (!p) {
		printf("No vendor id src param");
		return;
	}
	vendor_id_src = (uint16_t)strtol(p, NULL, 16);

	p = strtok(NULL, delim);
	if (!p) {
		printf("No product id param");
		return;
	}
	product_id = (uint16_t)strtol(p, NULL, 16);

	p = strtok(NULL, delim);
	if (!p) {
		printf("No product ver param");
		return;
	}
	product_ver = (uint16_t)strtol(p, NULL, 16);

	bt_cmd_legacy_set_did(vendor_id, vendor_id_src, product_id, product_ver);
}

static void cmd_legacy_set_ertm(const char *arg)
{
	uint8_t ertm;

	if (!arg[0]) {
		printf("%s: Invalid parameter", __func__);
		return;
	}
	ertm =  (uint8_t)strtoul(arg, NULL, 10);;

	bt_cmd_legacy_set_ertm(ertm);

}

static const struct {
	const char *cmd;
	const char *arg;
	void (*func) (const char *arg);
	const char *desc;
	char * (*gen) (const char *text, int state);
	void (*disp) (char **matches, int num_matches, int max_length);
} cmd_table[] = {
	/* Common */
	{ "clear_patch",    NULL,      cmd_clear_patch, "Clear patch" },
	{ "dl_patchcfg",    NULL,      cmd_dl_patchcfg, "download patch and config" },
	{ "wakeup",         NULL,      cmd_wakeup, "Wake-up" },
	{ "read_sw_ver",    NULL,      cmd_read_sw_version, "read software version" },
	{ "erase_paired",   NULL,      cmd_erase_paired_device, "erase paired device" },
	{ "read_bt_status", NULL,      cmd_read_bt_status, "read bt status" },
	{ "iocap",	    	NULL,      cmd_set_iocap, "set iocap" },
	{ "baudset",        NULL,      cmd_baud_set, "set baudrate" },
	{ "flowctl",        NULL,      cmd_flowctl_set, "set flow ctl" },
	{ "reset",          NULL,      cmd_bt_reset, "bt reset" },
	{ "set_logmask",    NULL,      cmd_set_logmask, "set log mask" },

	/* BLE */
	{ "ble_set_sc",     NULL,      cmd_ble_set_sc, "ble set sc" },
	{ "ble_read_addr",  NULL,      cmd_read_le_addr, "read le addr" },
	{ "ble_read_remote",NULL,  	   cmd_read_ble_remote_name, "read ble remote name" },
	{ "gatt_send",      NULL,      cmd_gatt_send, "gatt send" },
	{ "set_adv_mode",   NULL,      cmd_ble_set_adv_mode, "set adv mode" },
	{ "ble_adv_enable", NULL,      cmd_ble_adv_enable, "enable ble adv" },
	{ "ble_set_name",   NULL,      cmd_ble_write_device_name, "write ble name" },
	{ "ble_set_addr",   NULL,      cmd_write_le_addr, "write le addr" },
	{ "set_adv_int",    NULL,      cmd_ble_set_adv_inerval, "set adv interval" },
	{ "set_adv_data",   NULL,      cmd_ble_write_adv_data, "set adv data" },
	{ "set_scanrsp_data",  NULL,   cmd_ble_write_scan_rsp_data, "set scan rsp data" },
	{ "ble_disconn",    NULL,      cmd_ble_disconnect, "ble disconnect" },
	{ "ble_conn_upd",   NULL,      cmd_bleconn_upd, "ble conn upd" },
	{ "read_ble_rssi",  NULL,      cmd_ble_read_rssi, "ble read rssi" },
	{ "ble_set_dis",    NULL,      cmd_ble_set_dis, "ble set dis" },
	{ "gatttptx",       NULL,      cmd_gatttptx_test, "gatt tp tx test" },

	/* BR/EDR */
	{ "writepin",       NULL,      cmd_write_pin, "write pin" },
	{ "legacy_set_name",  NULL,    cmd_legacy_write_device_name, "set br/edr name" },
	{ "legacy_set_piscan", NULL,   cmd_legacy_set_radio_mode, "br/edr set radio mode" },
	{ "set_pagescan",   NULL,      cmd_legacy_set_page_scan_para, "set page scan" },
	{ "set_inqscan",   NULL,      cmd_legacy_set_inq_scan_para, "set page scan" },
	{ "sppconn",        NULL,      cmd_spp_connect, "spp conn" },
	{ "legacy_read_rssi", NULL,    cmd_legacy_read_rssi, "read br/edr rssi" },
	{ "send_spp_data",  NULL,      cmd_send_spp_data, "send spp data" },
	{ "spptptx",        NULL,      cmd_spptptx_test, "spp tp tx test" },
	{ "legacy_read_remote", NULL,  cmd_legacy_read_remote_device_name, "br/edr read remote name" },
	{ "spp_disconn",    NULL,      cmd_spp_disconnect, "spp disconnect" },
	{ "legacy_set_sdprec", NULL,   cmd_legacy_set_sdprec, "br/edr set sdp record" },
	{ "legacy_set_did", NULL,      cmd_legacy_set_did, "br/edr set did" },
	{ "legacy_set_ertm", NULL,     cmd_legacy_set_ertm, "br/edr set ertm" },

	{ "help" },
	{ }
};

void bt_cmd_handler(char *input)
{
	char *cmd, *arg;
	int i;

	if (!strlen(input))
		goto done;

	cmd = strtok_r(input, " ", &arg);
	if (!cmd)
		goto done;

	if (arg) {
		int len = strlen(arg);
		if (len > 0 && arg[len - 1] == ' ')
			arg[len - 1] = '\0';
	}

	for (i = 0; cmd_table[i].cmd; i++) {
		if (strcmp(cmd, cmd_table[i].cmd))
			continue;

		if (cmd_table[i].func) {
			printf("Run cmd [%s - %s]", cmd_table[i].cmd, arg);
			cmd_table[i].func(arg);
			goto done;
		}
	}

	if (strcmp(cmd, "help")) {
		printf("Invalid command");
		goto done;
	}

	printf("Available commands:");

	for (i = 0; cmd_table[i].cmd; i++) {
		if (cmd_table[i].desc)
			printf("  %s - %s", cmd_table[i].cmd, cmd_table[i].desc ? : "");
	}

done:
	return;
}


void BT_Demo(void)
{
	unsigned int len=0;
	
	UART_InitTypeDef  UART_InitStruct;

	g_bt_print_info = 0;
	
	//蓝牙通信端口初始化
	UART_BT_InitStruct.UART_BaudRate = 115200;
	UART_BT_InitStruct.UART_FrameLength = UART_DATA_FRAME_LEN_10BIT;
	UART_BT_InitStruct.UART_Mode = UART_INT_MODE;//采用中断方式
	UART_BT_InitStruct.UART_Parity = UART_PARITY_NONE;
	UART_Init(SCI_BT, &UART_BT_InitStruct);
	
	//上位机通信端口初始化
	UART_InitStruct.UART_BaudRate = 115200;
	UART_InitStruct.UART_FrameLength = UART_DATA_FRAME_LEN_10BIT;
	UART_InitStruct.UART_Mode = UART_INT_MODE;//采用中断方式
	UART_InitStruct.UART_Parity = UART_PARITY_NONE;
	UART_Init(SCI1, &UART_InitStruct);
	
	bt_init(BT_NORMAL_MODE);

	printf(">>>>>>>>>>>>>>>>>>>>>>>>> BT Console <<<<<<<<<<<<<<<<<<<<<<<<<\r\n");
	printf(">> ");

	while(1)
	{
		if (UART_ByteRecieved(SCI1) == TRUE)	//Console串口接收到数据
		{
			while((UART_ByteRecieved(SCI1)) && (len < UART_MAX_BUFLEN))
			{
				UART_RecvByte(SCI1, (UINT8 *)&g_uart_buffer[len++]);
				delay(500);
			}

			if((g_uart_buffer[len-2] == '\r') && (g_uart_buffer[len-1] == '\n'))
			{
				g_uart_buffer[len-2] = '\0';
			}
			else
			{
				g_uart_buffer[len] = '\0';
			}

			bt_cmd_handler((char *)g_uart_buffer);
			printf(">> ");
			len=0;
		}

		if (UART_ByteRecieved(SCI_BT) == TRUE)	//BT串口接收到数据
		{
			while((UART_ByteRecieved(SCI_BT)) && (len < UART_MAX_BUFLEN))
			{
				UART_RecvByte(SCI_BT, (UINT8 *)&g_uart_buffer[len++]);
				delay(500);
			}
			printf("\r\n");
			bt_event_handler((UINT8 *)g_uart_buffer, len);
			printf(">> ");
			len=0;
		}

		if(g_bt_init_flag == BT_PatchDownload_Complete)
		{
			bt_cmd_write_pin();
		}
		else if(g_bt_init_flag == BT_WritePin_Complete)
		{
			bt_cmd_legacy_set_radio_mode(1);
		}
	}
}
