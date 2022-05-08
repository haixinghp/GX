/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#include "dpm_plat.h"

#ifdef CONFIG_USE_DPM_SHA256
#include "dpm_sha256.h"
#else
#include "ali_crypto.h"
#endif

void *dpm_plat_malloc(uint32_t size)
{
    return ls_osa_malloc((size_t)size);
}

void dpm_plat_free(void *ptr)
{
    ls_osa_free(ptr);
}

#ifdef CONFIG_USE_DPM_SHA256
dpm_stat_t dpm_plat_hmac_sha256(
               uint8_t *key, uint32_t key_len,
               uint8_t *input, uint32_t ilen, uint8_t *output)
{
    uint8_t ipad[DPM_SHA256_BLOCK_SIZE];
    uint8_t opad[DPM_SHA256_BLOCK_SIZE];
    uint8_t hash[DPM_SHA256_SIZE];
    uint32_t i = 0;
    dpm_sha256_context context;

    if (key == NULL || key_len == 0 ||
        input == NULL || ilen == 0 || output == NULL) {
        DPM_LOG("invalid input args\n");
        return DPM_ERROR_BAD_PARAMETERS;
    }

    if (key_len > DPM_SHA256_BLOCK_SIZE) {
        DPM_LOG("not support key length, %d\n", key_len);
        return DPM_ERROR_NOT_SUPPORTED;
    }

    memset(ipad, 0x36, DPM_SHA256_BLOCK_SIZE);
    memset(opad, 0x5C, DPM_SHA256_BLOCK_SIZE);

    for (i = 0; i < key_len; i++) {
        ipad[i] = ipad[i] ^ key[i];
        opad[i] = opad[i] ^ key[i];
    }

    dpm_sha256_init(&context);
    dpm_sha256_starts(&context, 0);
    dpm_sha256_update(&context, ipad, DPM_SHA256_BLOCK_SIZE);
    dpm_sha256_update(&context, input, ilen);
    dpm_sha256_finish(&context, hash);

    dpm_sha256_init(&context);
    dpm_sha256_starts(&context, 0);
    dpm_sha256_update(&context, opad, DPM_SHA256_BLOCK_SIZE);
    dpm_sha256_update(&context, hash, DPM_SHA256_SIZE);
    dpm_sha256_finish(&context, output);

    return DPM_SUCCESS;
}
#else
dpm_stat_t dpm_plat_hmac_sha256(
               uint8_t *key, uint32_t key_len,
               uint8_t *input, uint32_t ilen, uint8_t *output)
{
    ali_crypto_result result;
    size_t size;
    void *hash_ctx = NULL;
    uint8_t ipad[DPM_SHA256_BLOCK_SIZE];
    uint8_t opad[DPM_SHA256_BLOCK_SIZE];
    uint8_t hash[DPM_SHA256_SIZE];
    uint32_t i = 0;

    if (key == NULL || key_len == 0 ||
        input == NULL || ilen == 0 || output == NULL) {
        DPM_LOG("invalid input args\n");
        return DPM_ERROR_BAD_PARAMETERS;
    }

    if (key_len > DPM_SHA256_BLOCK_SIZE) {
        DPM_LOG("not support key length, %d\n", key_len);
        return DPM_ERROR_NOT_SUPPORTED;
    }

    memset(ipad, 0x36, DPM_SHA256_BLOCK_SIZE);
    memset(opad, 0x5C, DPM_SHA256_BLOCK_SIZE);

    for (i = 0; i < key_len; i++) {
        ipad[i] = ipad[i] ^ key[i];
        opad[i] = opad[i] ^ key[i];
    }

    result = ali_sha256_get_ctx_size(&size);
    if (result != ALI_CRYPTO_SUCCESS) {
        DPM_LOG("get hash ctx size fail, 0x%x\n", result);
        return DPM_ERROR_GENERIC;
    }

    hash_ctx = dpm_plat_malloc(size);
    if (hash_ctx == NULL) {
        DPM_LOG("malloc %d size\n", size);
        return DPM_ERROR_OUT_OF_MEMORY;
    }

    ali_sha256_init(hash_ctx);
    ali_sha256_update(ipad, DPM_SHA256_BLOCK_SIZE, hash_ctx);
    ali_sha256_update(input, ilen, hash_ctx);
    ali_sha256_final(hash, hash_ctx);

    /* reset context */
    memset(hash_ctx, 0, size);

    ali_sha256_init(hash_ctx);
    ali_sha256_update(opad, DPM_SHA256_BLOCK_SIZE, hash_ctx);
    ali_sha256_update(hash, DPM_SHA256_SIZE, hash_ctx);
    ali_sha256_final(output, hash_ctx);

    dpm_plat_free(hash_ctx);

    return DPM_SUCCESS;
}
#endif
