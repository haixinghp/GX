/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#ifndef __DPM_PLAT_H__
#define __DPM_PLAT_H__

#include "dpm.h"
#include "ls_osa.h"

#define DPM_SHA256_SIZE         32
#define DPM_SHA256_BLOCK_SIZE   64

#define DPM_LOG(_f, ...)        ls_osa_print("%s %s %d: " _f, "<LS_DPM>", \
                                         __FUNCTION__, __LINE__, ##__VA_ARGS__)

/**
 * @brief allocate a block of size bytes memory.
 *
 * @param size[in]: specify block size in bytes.
 *
 * @return: a pointer to the beginning of the block.
 */
void *dpm_plat_malloc(uint32_t size);

/**
 * @brief deallocate a block of memory.
 *
 * @param ptr[in]: pointer to a memory block.
 *
 * @return: none.
 */
void dpm_plat_free(void *ptr);

/**
 * @brief hmac-sha256.
 *
 * @param key[in]: key buffer.
 * @param key_len[in]: the length of key.
 * @param input[in]: input data.
 * @param ilen[in]: the length of input data.
 * @param output[out]: specify the buffer to contain digest, length >= 32.
 *
 * @return: DPM_SUCCESS - success, others - fail.
 */
dpm_stat_t dpm_plat_hmac_sha256(
               uint8_t *key, uint32_t key_len,
               uint8_t *input, uint32_t ilen, uint8_t *output);

#endif /* __DPM_PLAT_H__ */
