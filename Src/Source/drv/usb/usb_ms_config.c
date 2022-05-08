// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File name    : usbdev_config.c
// Version      : V0.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "memmap.h"
#include "usb_const.h"
#include "usb_drv.h"

	//standard Device Descriptor
const unsigned char USB_Device_Descriptor[18] =
	{

	        //Device (0x12 bytes)
	        0x12,                   //bLength: 0x12 bytes
	        0x01,                   //bDescriptorType: DEVICE
#if USB_MODE_1D1
					0x10, 0x01,             //bcdUSB: version 1.1
#elif USB_MODE_2D0
	        0x00, 0x02,             //bcdUSB: version 2.0
#else
					0x10, 0x01,             //bcdUSB: version 1.1
#endif
	        0x00,                   //bDeviceClass: independent interfaces
	        0x00,                   //bDeviceSubClass: 0
	        0x00,                   //bDeviceProtocol: class specific protocols NOT used on device basis
	        0x40,                   //bMaxPacketSize0: maximum packet size for endpoint zero
	        0xe3, 0x02,             //idVendor: vendor ID
	        0x02, 0x07,             //idProduct: product ID //900515But org=503
	        0x00, 0x02,             //bcdDevice: device release number in BCD
	        0x01,                   //iManufacturer: index of string
	        0x02,                   //iProduct: index of manufacturer string
	        0x03,                   //iSerialNumber: index of product string
	        0x01,                   //bNumConfigurations: 1 configuration
	};

	//Device Qualifier Descriptor
const unsigned char USB_Device_Qualifier_Descriptor[10] =
	{

	        //Device Qualifier(0x0A bytes)
	        0x0A,                   //bLength: 0x12 bytes
	        0x06,                   //bDescriptorType: DEVICE
#if USB_MODE_1D1
					0x10, 0x01,             //bcdUSB: version 1.1
#elif USB_MODE_2D0
	        0x00, 0x02,             //bcdUSB: version 2.0
#else
					0x10, 0x01,             //bcdUSB: version 1.1
#endif
	        0x00,                   //bDeviceClass: independent interfaces
	        0x00,                   //bDeviceSubClass: 0
	        0x00,                   //bDeviceProtocol: class specific protocols NOT used on device basis
	        0x40,                   //bMaxPacketSize0 for other speed
	        0x01,                   //bNumConfigurations for other-speed configuration
	        0x00,                   //Reserved,must be 0        
	};

	//configuration descriptor
const unsigned char USB_Configuration_Descriptor [] =
	{
	        0x09,			//bLength
	        0x02,			//bDescriptor
	        0x20, 00,		//wTotalLength
			    0x01,			//bNumInterface
			    0x01,			//bConfiguration Value
	        0x00,			//iConfiguration
			    0x80,			//bmAttributes,BUS-POWER
			    0x64,			//bMaxPower,200mA

	    	//Interface 0, AlternateSetting 0 (0x09 bytes)
	        0x09,                   //bLength: 0x09 bytes
	        0x04,                   //bDescriptorType: INTERFACE
	        0x00,                   //bInterfaceNumber: interface 0
	        0x00,                   //bAlternateSetting: alternate setting 0
	        0x02,                   //bNumEndpoints: 2 endpoint + zero ep
	        0x08,                   //bInterfaceClass: vendor specific interface, Mass Storage Class
	        0x06,                   //bInterfaceSubClass: 06 SCSI Command Set
	        0x50,                   //bInterfaceProtocol: class specific protocol NOT used on this interface
	        				 //Bulk-Only-Transport
	        0x00,                   //iInterface: index of string

			//Endpoint 1 (0x07 bytes)
	        0x07,                   //bLength: 0x07 bytes
	        0x05,                   //bDescriptorType: ENDPOINT
			    0x80+USB_ENDPOINT_INDEX,
	        0x02,                   //bmAttributes: BULK-ONLY(00:Control 01:isoch 10:bulk 11:intr
#if USB_MODE_1D1
					0x40, 0x00,             //wMaxPacketSize: 64 bytes,for USB1.1
#elif USB_MODE_2D0
	        0x00, 0x02,             //wMaxPacketSize: 512 bytes for USB2.0
#else
					0x40, 0x00,             //wMaxPacketSize: 64 bytes,for USB1.1
#endif
	        0x00,                   //bInterval: polling interval is 1 ms
		
			//Endpoint 1 (0x07 bytes)
	        0x07,                   //bLength: 0x07 bytes
	        0x05,                   //bDescriptorType: ENDPOINT
			    USB_ENDPOINT_INDEX,
	        0x02,                   //bmAttributes: BULK-ONLY
#if USB_MODE_1D1
					0x40, 0x00,             //wMaxPacketSize: 64 bytes,for USB1.1
#elif USB_MODE_2D0
	        0x00, 0x02,             //wMaxPacketSize: 512 bytes for USB2.0
#else
					0x40, 0x00,             //wMaxPacketSize: 64 bytes,for USB1.1
#endif
	        0x00,					//bInterval: polling interval is 1 ms
	};

	//standard string Descriptor,serial number
const unsigned char USB_String_Descriptor[] =
	{

	        //Device (0x1a bytes)
	    0x1a,                   //bLength: 0x1A bytes
	    0x03,                   //bDescriptorType: STRING
	        
	    0x33,
			0x00,

			0x35,
		 	0x00,
		 	0x35,
		 	0x00,

			0x34,
		 	0x00,
		 	0x39,
		 	0x00,

			0x39,
		 	0x00,
		 	0x34,
		 	0x00,

			0x34,
		 	0x00,
		 	0x31,
		 	0x00,

			0x34,
		 	0x00,
		 	0x39,
		 	0x00,

			0x34,
		 	0x00

	};

const unsigned char DBR[512] = {
	0xeb,0x3c,0x90,0x4d,0x53,0x57,0x49,0x4e,0x34,0x2e,0x31,0x00,0x02,0x20,0x01,0x00,
	0x02,0x00,0x02,0x00,0x00,0xf8,0xfb,0x00,0x3f,0x00,0xff,0x00,0x20,0x00,0x00,0x00,
	0x7f,0x47,0x1f,0x00,0x00,0x00,0x29,0x43,0x4f,0x52,0x45,0x4e,0x4f,0x20,0x4e,0x41,
	0x4d,0x45,0x20,0x20,0x20,0x20,0x46,0x41,0x54,0x31,0x36,0x20,0x20,0x20,0x33,0xc9,
	0x8e,0xd1,0xbc,0xfc,0x7b,0x16,0x07,0xbd,0x78,0x00,0xc5,0x76,0x00,0x1e,0x56,0x16,
	0x55,0xbf,0x22,0x05,0x89,0x7e,0x00,0x89,0x4e,0x02,0xb1,0x0b,0xfc,0xf3,0xa4,0x06,
	0x1f,0xbd,0x00,0x7c,0xc6,0x45,0xfe,0x0f,0x38,0x4e,0x24,0x7d,0x20,0x8b,0xc1,0x99,
	0xe8,0x7e,0x01,0x83,0xeb,0x3a,0x66,0xa1,0x1c,0x7c,0x66,0x3b,0x07,0x8a,0x57,0xfc,
	0x75,0x06,0x80,0xca,0x02,0x88,0x56,0x02,0x80,0xc3,0x10,0x73,0xed,0x33,0xc9,0xfe,
	0x06,0xd8,0x7d,0x8a,0x46,0x10,0x98,0xf7,0x66,0x16,0x03,0x46,0x1c,0x13,0x56,0x1e,
	0x03,0x46,0x0e,0x13,0xd1,0x8b,0x76,0x11,0x60,0x89,0x46,0xfc,0x89,0x56,0xfe,0xb8,
	0x20,0x00,0xf7,0xe6,0x8b,0x5e,0x0b,0x03,0xc3,0x48,0xf7,0xf3,0x01,0x46,0xfc,0x11,
	0x4e,0xfe,0x61,0xbf,0x00,0x07,0xe8,0x28,0x01,0x72,0x3e,0x38,0x2d,0x74,0x17,0x60,
	0xb1,0x0b,0xbe,0xd8,0x7d,0xf3,0xa6,0x61,0x74,0x3d,0x4e,0x74,0x09,0x83,0xc7,0x20,
	0x3b,0xfb,0x72,0xe7,0xeb,0xdd,0xfe,0x0e,0xd8,0x7d,0x7b,0xa7,0xbe,0x7f,0x7d,0xac,
	0x98,0x03,0xf0,0xac,0x98,0x40,0x74,0x0c,0x48,0x74,0x13,0xb4,0x0e,0xbb,0x07,0x00,
	0xcd,0x10,0xeb,0xef,0xbe,0x82,0x7d,0xeb,0xe6,0xbe,0x80,0x7d,0xeb,0xe1,0xcd,0x16,
	0x5e,0x1f,0x66,0x8f,0x04,0xcd,0x19,0xbe,0x81,0x7d,0x8b,0x7d,0x1a,0x8d,0x45,0xfe,
	0x8a,0x4e,0x0d,0xf7,0xe1,0x03,0x46,0xfc,0x13,0x56,0xfe,0xb1,0x04,0xe8,0xc2,0x00,
	0x72,0xd7,0xea,0x00,0x02,0x70,0x00,0x52,0x50,0x06,0x53,0x6a,0x01,0x6a,0x10,0x91,
	0x8b,0x46,0x18,0xa2,0x26,0x05,0x96,0x92,0x33,0xd2,0xf7,0xf6,0x91,0xf7,0xf6,0x42,
	0x87,0xca,0xf7,0x76,0x1a,0x8a,0xf2,0x8a,0xe8,0xc0,0xcc,0x02,0x0a,0xcc,0xb8,0x01,
	0x02,0x80,0x7e,0x02,0x0e,0x75,0x04,0xb4,0x42,0x8b,0xf4,0x8a,0x56,0x24,0xcd,0x13,
	0x61,0x61,0x72,0x0a,0x40,0x75,0x01,0x42,0x03,0x5e,0x0b,0x49,0x75,0x77,0xc3,0x03,
	0x18,0x01,0x27,0x0d,0x0a,0x49,0x6e,0x76,0x61,0x6c,0x69,0x64,0x20,0x73,0x79,0x73,
	0x74,0x65,0x6d,0x20,0x64,0x69,0x73,0x6b,0xff,0x0d,0x0a,0x44,0x69,0x73,0x6b,0x20,
	0x49,0x2f,0x4f,0x20,0x65,0x72,0x72,0x6f,0x72,0xff,0x0d,0x0a,0x52,0x65,0x70,0x6c,
	0x61,0x63,0x65,0x20,0x74,0x68,0x65,0x20,0x64,0x69,0x73,0x6b,0x2c,0x20,0x61,0x6e,
	0x64,0x20,0x74,0x68,0x65,0x6e,0x20,0x70,0x72,0x65,0x73,0x73,0x20,0x61,0x6e,0x79,
	0x20,0x6b,0x65,0x79,0x0d,0x0a,0x00,0x00,0x49,0x4f,0x20,0x20,0x20,0x20,0x20,0x20,
	0x53,0x59,0x53,0x4d,0x53,0x44,0x4f,0x53,0x20,0x20,0x20,0x53,0x59,0x53,0x7f,0x01,
	0x00,0x41,0xbb,0x00,0x07,0x60,0x66,0x6a,0x00,0xe9,0x3b,0xff,0x00,0x00,0x55,0xaa,
};

const unsigned char MBR[512] = {
	0xfa,0x33,0xc0,0x8e,0xd0,0xbc,0x00,0x7c,0x8b,0xf4,0x50,0x07,0x50,0x1f,0xfb,0xfc,
	0xbf,0x00,0x06,0xb9,0x00,0x01,0xf2,0xa5,0xea,0x1d,0x06,0x00,0x00,0xbe,0xbe,0x07,
	0xb3,0x04,0x80,0x3c,0x80,0x74,0x0e,0x80,0x3c,0x00,0x75,0x1c,0x83,0xc6,0x10,0xfe,
	0xcb,0x75,0xef,0xcd,0x18,0x8b,0x14,0x8b,0x4c,0x02,0x8b,0xee,0x83,0xc6,0x10,0xfe,
	0xcb,0x74,0x1a,0x80,0x3c,0x00,0x74,0xf4,0xbe,0x8b,0x06,0xac,0x3c,0x00,0x74,0x0b,
	0x56,0xbb,0x07,0x00,0xb4,0x0e,0xcd,0x10,0x5e,0xeb,0xf0,0xeb,0xfe,0xbf,0x05,0x00,
	0xbb,0x00,0x7c,0xb8,0x01,0x02,0x57,0xcd,0x13,0x5f,0x73,0x0c,0x33,0xc0,0xcd,0x13,
	0x4f,0x75,0xed,0xbe,0xa3,0x06,0xeb,0xd3,0xbe,0xc2,0x06,0xbf,0xfe,0x7d,0x81,0x3d,
	0x55,0xaa,0x75,0xc7,0x8b,0xf5,0xea,0x00,0x7c,0x00,0x00,0x49,0x6e,0x76,0x61,0x6c,
	0x69,0x64,0x20,0x70,0x61,0x72,0x74,0x69,0x74,0x69,0x6f,0x6e,0x20,0x74,0x61,0x62,
	0x6c,0x65,0x00,0x45,0x72,0x72,0x6f,0x72,0x20,0x6c,0x6f,0x61,0x64,0x69,0x6e,0x67,
	0x20,0x6f,0x70,0x65,0x72,0x61,0x74,0x69,0x6e,0x67,0x20,0x73,0x79,0x73,0x74,0x65,
	0x6d,0x00,0x4d,0x69,0x73,0x73,0x69,0x6e,0x67,0x20,0x6f,0x70,0x65,0x72,0x61,0x74,
	0x69,0x6e,0x67,0x20,0x73,0x79,0x73,0x74,0x65,0x6d,0x00,0x00,0x81,0x22,0x52,0x13,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x80,0x01,
	0x01,0x00,0x06,0x03,0x60,0xf6,0x20,0x00,0x00,0x00,0x7f,0x47,0x1f,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0xaa,
};
const unsigned char dirData[]=
{
	0x44,0x45,0x4D,0x4F,0x20,0x20,0x20,0x20,0x44,0x41,0x54,0x02,0x00,0x25,0x97,0x7c,
	0x8d,0x3b,0x8d,0x3b,0x00,0x00,0xf5,0x7b,0x8d,0x3b,0x02,0x00,0x00,0x00,0x08,0x00
};