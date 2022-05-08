#ifndef _ALGORITHM_BIN_INTERFACE_H_
#define _ALGORITHM_BIN_INTERFACE_H_

#include <stdint.h>
/************************************ Call by bin caller**********************************/
typedef enum
{
	LIB_READ_ID_LIST=0x01,
	LIB_PRE_ENROLL=0x02,
	LIB_PRE_MATCH=0x03,
	LIB_ENROLL =0x04,
	LIB_MATCH=0x05,
	LIB_DELETE_ID=0x06,
	LIB_IS_FINGERLEAVE=0x07,
	LIB_CLEAR_ENROLL_ALL_STEPS=0x08,
	LIB_INIT_SENSOR=0x0b,
	LIB_FRAME_MAX
} _Lib_Cmd;


//#define FUNC_TABLE_START		(0x08010000UL)
#define FUNC_TABLE_START		(0x0803C000UL)

#define FUNC(i)				(*(unsigned int *)(FUNC_TABLE_START + (i) * 4))


typedef int           (*lib_init_t)(void *const funcs_tab[]);
typedef unsigned char (*lib_api_t)(unsigned char cmd, unsigned char *buff, unsigned short buff_len);
typedef void          (*lib_timer_isr_t)(void);
typedef void          (*lib_dbg_en_t)(unsigned char en);
typedef void          (*lib_init_bss_and_data_section_t)(void);
typedef  int (*lib_set_buf_t)(int *ptr, int len);



#define binlib_init                     ((lib_init_t)FUNC(1))
#define binlib_api                      ((lib_api_t)FUNC(2))
#define binlib_timer_isr                ((lib_timer_isr_t)FUNC(3))
#define binlib_dbg_en                   ((lib_dbg_en_t)FUNC(4))
#define binlib_init_buffer              ((lib_set_buf_t)FUNC(5))
#define binlib_init_data_bss_sections   ((lib_init_bss_and_data_section_t)FUNC(6))

extern void *const bin_funcs_table[];
//int32_t mafp_get_used_enrollids(void);
int32_t mafp_get_used_enrollids(void);
int32_t mafp_get_chipid(void);
int32_t mafp_post_enroll(void);
int32_t mafp_post_authenticate(void);
int32_t mafp_get_enrollid_status(uint8_t enroll_id);
int32_t mafp_get_used_enrollids(void);
int32_t mafp_remove_enrollid(uint8_t enroll_id);
int32_t mafp_clear_all_enrollids(void);
int32_t sensor_get_download_buf(uint8_t *name, uint8_t **dst_addr, uint32_t size);
int32_t sensor_save_download_buf(void);
int32_t sensor_enroll_image_testing(uint8_t fid);
int32_t sensor_authenticate_image_testing(void);
int32_t sensor_get_upload_template_buf(uint32_t idx, uint8_t **dst_addr);
int32_t sensor_get_upload_image_buf(uint32_t idx, uint8_t **dst_addr,uint16_t *width, uint16_t *height);
int32_t mafp_detect_mode(void);
int32_t mafp_get_max_subtpl_num(void);
int32_t mafp_reset_sensor_config(void);
int32_t sensor_upload_subtpls(void);
int32_t mafp_finger_leave(void);

extern uint8_t * get_image_data(uint32_t *image_length, uint16_t *width,uint16_t *heigth);

#endif /* end of _ALGORITHM_BIN_INTERFACE_H_ */
