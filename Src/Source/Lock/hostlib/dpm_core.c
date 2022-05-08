/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#include "dpm.h"
#include "dpm_plat.h"
#include "id2_client.h"

#define DPM_OTP_MSG_LEN         10
#define DPM_OTP_KEY_LEN         64

#define DPM_MAX_INDEX_SIZE      100

static dpm_stat_t _dpm_otp_obf_enc(uint8_t *data, uint32_t len, uint32_t idx)
{
    uint32_t i, tmp;
    uint32_t mid_h, mid_l;
    uint32_t idx_h, idx_l;

    if (data == NULL || len <= 2) {
        DPM_LOG("invalid input args\n");
        return DPM_ERROR_BAD_PARAMETERS;
    }

    tmp = data[0];
    for (i = 1; i < len - 2; i++) {
        tmp = tmp ^ data[i];
    }

    mid_h = (tmp) % 10;
    mid_l = (tmp + mid_h) % 10;

    idx_h = idx / 10;
    idx_l = idx % 10;

    if (mid_h >= idx_h) {
        data[len - 2] = mid_h - idx_h + '0';
    } else {
        data[len - 2] = mid_h - idx_h + 10 + '0';
    }

    if (mid_l >= idx_l) {
        data[len - 1] = mid_l - idx_l + '0';
    } else {
        data[len - 1] = mid_l - idx_l + 10 + '0';
    }

    return DPM_SUCCESS;
}

static dpm_stat_t _dpm_otp_obf_dec(uint8_t *data, uint32_t len, uint32_t *idx)
{
    uint32_t i, tmp;
    uint32_t mid_h, mid_l;
    uint32_t idx_h, idx_l;

    if (data == NULL || len <= 2 || idx == NULL) {
        DPM_LOG("invalid input args\n");
        return DPM_ERROR_BAD_PARAMETERS;
    }

    tmp = data[0];
    for (i = 1; i < len - 2; i++) {
        tmp = tmp ^ data[i];
    }

    mid_h = (tmp) % 10;
    mid_l = (tmp + mid_h) % 10;

    if (mid_h >= data[len - 2] - '0') {
        idx_h = mid_h - data[len - 2] + '0';
    } else {
        idx_h = mid_h - data[len - 2] + 10 + '0';
    }

    if (mid_l >= data[len - 1] - '0') {
        idx_l = mid_l - data[len - 1] + '0';
    } else {
        idx_l = mid_l - data[len - 1] + 10 + '0';
    }

    *idx = idx_h * 10 + idx_l;

    return DPM_SUCCESS;
}

dpm_stat_t dpm_generate_totp(uint8_t *key, uint32_t key_len,
               uint8_t *msg, uint32_t msg_len, uint8_t *otp, uint32_t otp_len)
{
    dpm_stat_t stat;
    uint8_t hash[DPM_SHA256_SIZE];
    uint32_t i = 0;
    uint32_t digits = 0;
    uint32_t offset = 0;
    uint64_t binary = 0;
    uint64_t module = 1;

    if (key == NULL || key_len == 0 ||
        msg == NULL || msg_len == 0 ||
        otp == NULL || otp_len == 0) {
        DPM_LOG("invalid input args\n");
        return DPM_ERROR_BAD_PARAMETERS;
    }

    stat = dpm_plat_hmac_sha256(key, key_len, msg, msg_len, hash);
    if (stat != DPM_SUCCESS) {
        DPM_LOG("dpm hmac-sha256 fail, 0x%x\n", stat);
        return -1;
    }

#ifdef CONFIG_DPM_DBG_EN
    for (i = 0; i < DPM_SHA256_SIZE; i += 8) {
        DPM_LOG("hmac-sha: %02x %02x %02x %02x %02x %02x %02x %02x\n",
               hash[i+0], hash[i+1], hash[i+2], hash[i+3],
               hash[i+4], hash[i+5], hash[i+6], hash[i+7]);
    }
#endif

    for (i = 0; i < otp_len; i++) {
        module = module * 10;
    }

    offset = hash[DPM_SHA256_SIZE - 1] & 0xf;

#ifdef CONFIG_NORM_TOTP_EN
    binary = (((uint64_t)hash[offset] & 0x7f) << 24) |
	     (((uint64_t)hash[offset + 1] & 0xff) << 16) |
	     (((uint64_t)hash[offset + 2] & 0xff) << 8) |
	     (((uint64_t)hash[offset + 3] & 0xff));
#else
    binary = (((uint64_t)hash[offset] & 0x7f) << 56) |
	     (((uint64_t)hash[offset + 1] & 0xff) << 48) |
	     (((uint64_t)hash[offset + 2] & 0xff) << 40) |
	     (((uint64_t)hash[offset + 3] & 0xff) << 32) |
	     (((uint64_t)hash[offset + 4] & 0xff) << 24) |
	     (((uint64_t)hash[offset + 5] & 0xff) << 16) |
	     (((uint64_t)hash[offset + 6] & 0xff) << 8)  |
	     (((uint64_t)hash[offset + 7] & 0xff));
#endif

#ifdef CONFIG_DPM_DBG_EN
    DPM_LOG("binary: 0x%llx %lld\n", binary, binary);
    DPM_LOG("module: %lld\n", module);
#endif

    binary = binary % module;

    digits = 1;
    module = 1;
    do {
       module = module * 10;
       if (binary / module != 0) {
           digits++;
       } else {
           break;
       }
    } while(1);

    memset(otp, '0', otp_len);
    for (i = 0; i < digits; i++) {
        otp[otp_len - 1 - i] = binary % 10  + '0';
        binary = binary / 10;
    }

#ifdef CONFIG_DPM_DBG_EN
    DPM_LOG("otp_orig: %s\n", otp);
#endif

    return DPM_SUCCESS;
}

dpm_stat_t dpm_get_totp(uint64_t timestamp, uint32_t index,
               uint32_t otp_step, uint32_t otp_len, uint8_t *otp_data)
{
    dpm_stat_t stat;
    uint32_t i, len;
    uint64_t steps = 0;
    uint8_t msg[DPM_OTP_MSG_LEN] = {0};
    uint8_t key[DPM_OTP_KEY_LEN] = {0};
    uint8_t id2[ID2_ID_LEN + 1] = {0};
    irot_result_t result;

#ifdef CONFIG_DPM_DBG_EN
    DPM_LOG("timestamp: %lld\n", timestamp);
    DPM_LOG("index: %d\n", index);
    DPM_LOG("otp_step: %d\n", otp_step);
    DPM_LOG("otp_len: %d\n", otp_len);
#endif

    if (timestamp == 0 || otp_data == NULL) {
        DPM_LOG("invalid input args\n");
        return DPM_ERROR_BAD_PARAMETERS;
    }

    if (index >= DPM_MAX_INDEX_SIZE) {
        DPM_LOG("invalid index, %d\n", index);
        return DPM_ERROR_BAD_PARAMETERS;
    }

    if (otp_step == 0) {
        otp_step = DPM_OTP_STEP_MIN_LEN;
    }

    if (otp_step < DPM_OTP_STEP_MIN_LEN ||
        otp_step > DPM_OTP_STEP_MAX_LEN) {
        DPM_LOG("invalid otp step, %d\n", otp_step);
        return DPM_ERROR_BAD_PARAMETERS;
    }

    if (otp_len < DPM_OTP_DATA_MIN_LEN ||
        otp_len > DPM_OTP_DATA_MAX_LEN) {
        DPM_LOG("invalid otp data length, %d\n", otp_len);
        return DPM_ERROR_BAD_PARAMETERS;
    }

    steps = timestamp / otp_step;

    for (i = 0; i < DPM_OTP_MSG_LEN - 2; i++) {
        msg[7 - i] = (steps >> i * 8) & 0xFF;
    }
    msg[DPM_OTP_MSG_LEN - 2] = (index / 10) + '0';
    msg[DPM_OTP_MSG_LEN - 1] = (index % 10) + '0';

#ifdef CONFIG_DPM_DBG_EN
    DPM_LOG("otp_msg: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
            msg[0], msg[1], msg[2], msg[3], msg[4],
            msg[5], msg[6], msg[7], msg[8], msg[9]);
#endif

    result = id2_client_init();
    if (result != IROT_SUCCESS) {
        DPM_LOG("id2 init fail, 0x%x\n", result);
        return DPM_ERROR_GENERIC;
    }

    len = ID2_ID_LEN;
    result = id2_client_get_id(id2, &len);
    if (result != IROT_SUCCESS) {
        DPM_LOG("id2 get id fail, 0x%x\n", result);
        id2_client_cleanup();
        return DPM_ERROR_GENERIC;
    }

    len = DPM_OTP_KEY_LEN;
    result = id2_client_get_secret((char *)id2, key, &len);
    if (result != IROT_SUCCESS) {
        DPM_LOG("id2 get secret fail, 0x%x\n", result);
        id2_client_cleanup();
        return DPM_ERROR_GENERIC;
    }

    id2_client_cleanup();

#ifdef CONFIG_DPM_DBG_EN
    DPM_LOG("id2: %s\n", id2);
    DPM_LOG("otp secret: %s\n", key);
#endif

    stat = dpm_generate_totp(key, len,
               msg, DPM_OTP_MSG_LEN, otp_data, otp_len - 2);
    if (stat != DPM_SUCCESS) {
        DPM_LOG("generate totp fail, 0x%x\n", stat);
        return stat;
    }

    stat = _dpm_otp_obf_enc(otp_data, otp_len, index);
    if (stat != DPM_SUCCESS) {
        DPM_LOG("otp obf encode fail, 0x%x\n", stat);
        return stat;
    }

#ifdef CONFIG_DPM_DBG_EN
    DPM_LOG("otp_data: %s\n", otp_data);
#endif

    return DPM_SUCCESS;
}

dpm_stat_t dpm_get_index(uint8_t *otp_data, uint32_t otp_len, uint32_t *index)
{
    dpm_stat_t stat;

    if (otp_data == NULL || index == NULL) {
        DPM_LOG("invalid input args\n");
        return DPM_ERROR_BAD_PARAMETERS;
    }

    if (otp_len < DPM_OTP_DATA_MIN_LEN ||
        otp_len > DPM_OTP_DATA_MAX_LEN) {
        DPM_LOG("invalid otp data length, %d\n", otp_len);
        return DPM_ERROR_BAD_PARAMETERS;
    }

    stat = _dpm_otp_obf_dec(otp_data, otp_len, index);
    if (stat != DPM_SUCCESS) {
        DPM_LOG("otp obf decode fail, 0x%x\n", stat);
        return stat;
    }

    return DPM_SUCCESS;
}

