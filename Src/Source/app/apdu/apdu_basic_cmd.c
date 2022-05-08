
#include "type.h"
#include "string.h"
#include "id2.h"


U8 vendor_id[2] = {0x81,0x88};  
U8 ver_info[8] = "87654321";						//for test,need todo
U8 id_conf[4] = "1234";						//for test,need todo
U8 ava_spc[2] = "78";						//for test,need todo
U8 reserve_b[4] = "4321";						//for test,need todo
U8 pbuffer1[256]={0};
#define writel(addr,value) (*(volatile unsigned long *)(addr))=((unsigned long)(value))
#define LP_EFAPR 						(0x40003804)
#define WAFER_ID_ADDR				(0x08200098)
#define WAFER_ID_LEN				(8)

void read_cid(UINT8 *cid)//get sn
{
	UINT32 i;
	
	writel(LP_EFAPR,0xd8d89930);
	
	for(i = 0;i < WAFER_ID_LEN; i ++)
  {
		*(cid + i) = *((UINT8 *)(WAFER_ID_ADDR + i));
  }
	
	writel(LP_EFAPR,0xd8d89900);
}
/*********************************************************/
/*函数: void apdu_get_ID(uint8_t len)
 *功能：
 *入参：
 *返回：
 *备注：
 */
/*********************************************************/
void apdu_get_ID(uint8_t len)
{
	U8 p1;
	U32 *pbuf;
  len = 0x0c;
  
	memcpy((U8*)(pbuffer1), (U8*)vendor_id, 2);
	memcpy((U8*)(pbuffer1+2), &len, 1);
	//pbuffer1[2] = sizeof(sci_id);
	
	memcpy((U8*)(pbuffer1+3), (U8*)EFLASH_ID2_SN_ADDR, 24);
}

