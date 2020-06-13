/*
 * emv-tools - a set of tools to work with EMV family of smart cards
 * Copyright (C) 2012, 2015 Dmitry Eremin-Solenikov
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>     // memcpy
#include <stdlib.h>     // malloc
#include "cda_test.h"

#include "../emv_pk.h"
#include "../crypto.h"
#include "../dump.h"
#include "../tlv.h"
#include "../emv_pki.h"
#include "ui.h"             // printandlog

struct emv_pk c_mchip_05 = {
    .rid = { 0xa0, 0x00, 0x00, 0x00, 0x04, },
    .index = 5,
    .hash_algo = HASH_SHA_1,
    .pk_algo = PK_RSA,
    .hash = {
        0xeb, 0xfa, 0x0d, 0x5d,
        0x06, 0xd8, 0xce, 0x70,
        0x2d, 0xa3, 0xea, 0xe8,
        0x90, 0x70, 0x1d, 0x45,
        0xe2, 0x74, 0xc8, 0x45,
    },
    .exp = { 0x03, },
    .elen = 1,
    .mlen = 1408 / 8,
    .modulus = (unsigned char[]) {
        0xb8, 0x04, 0x8a, 0xbc, 0x30, 0xc9, 0x0d, 0x97, 0x63, 0x36, 0x54, 0x3e, 0x3f, 0xd7, 0x09, 0x1c,
        0x8f, 0xe4, 0x80, 0x0d, 0xf8, 0x20, 0xed, 0x55, 0xe7, 0xe9, 0x48, 0x13, 0xed, 0x00, 0x55, 0x5b,
        0x57, 0x3f, 0xec, 0xa3, 0xd8, 0x4a, 0xf6, 0x13, 0x1a, 0x65, 0x1d, 0x66, 0xcf, 0xf4, 0x28, 0x4f,
        0xb1, 0x3b, 0x63, 0x5e, 0xdd, 0x0e, 0xe4, 0x01, 0x76, 0xd8, 0xbf, 0x04, 0xb7, 0xfd, 0x1c, 0x7b,
        0xac, 0xf9, 0xac, 0x73, 0x27, 0xdf, 0xaa, 0x8a, 0xa7, 0x2d, 0x10, 0xdb, 0x3b, 0x8e, 0x70, 0xb2,
        0xdd, 0xd8, 0x11, 0xcb, 0x41, 0x96, 0x52, 0x5e, 0xa3, 0x86, 0xac, 0xc3, 0x3c, 0x0d, 0x9d, 0x45,
        0x75, 0x91, 0x64, 0x69, 0xc4, 0xe4, 0xf5, 0x3e, 0x8e, 0x1c, 0x91, 0x2c, 0xc6, 0x18, 0xcb, 0x22,
        0xdd, 0xe7, 0xc3, 0x56, 0x8e, 0x90, 0x02, 0x2e, 0x6b, 0xba, 0x77, 0x02, 0x02, 0xe4, 0x52, 0x2a,
        0x2d, 0xd6, 0x23, 0xd1, 0x80, 0xe2, 0x15, 0xbd, 0x1d, 0x15, 0x07, 0xfe, 0x3d, 0xc9, 0x0c, 0xa3,
        0x10, 0xd2, 0x7b, 0x3e, 0xfc, 0xcd, 0x8f, 0x83, 0xde, 0x30, 0x52, 0xca, 0xd1, 0xe4, 0x89, 0x38,
        0xc6, 0x8d, 0x09, 0x5a, 0xac, 0x91, 0xb5, 0xf3, 0x7e, 0x28, 0xbb, 0x49, 0xec, 0x7e, 0xd5, 0x97,
    },
    .expire = 0,
};

const unsigned char c_issuer_cert[] = {
    0x17, 0x14, 0x28, 0x4f, 0x76, 0x3b, 0x85, 0x86, 0xee, 0x6d, 0x31, 0x99, 0x51, 0xf7, 0xe6, 0x3f,
    0xa2, 0x50, 0x76, 0xe5, 0x0d, 0xc9, 0xd3, 0x20, 0x0b, 0xa9, 0x98, 0xd3, 0xa0, 0x52, 0xad, 0xba,
    0x9a, 0xb6, 0x9a, 0xc6, 0xad, 0x6a, 0xdd, 0x3c, 0xe0, 0x9f, 0x02, 0x78, 0xf4, 0x07, 0x4e, 0xc4,
    0xee, 0x9b, 0x1d, 0x22, 0x68, 0xa3, 0xe9, 0x53, 0x57, 0x5e, 0x45, 0x4e, 0x50, 0xcd, 0x86, 0x0b,
    0xf4, 0x24, 0xc5, 0x1c, 0x59, 0x77, 0x12, 0xd2, 0xaa, 0x05, 0x70, 0x89, 0xdd, 0x86, 0x73, 0xe5,
    0x1b, 0x1e, 0x1d, 0x71, 0x88, 0x03, 0x48, 0x92, 0x07, 0x7a, 0xc1, 0x8a, 0x6a, 0xe2, 0x34, 0x88,
    0xbe, 0xa9, 0xdf, 0x3b, 0x1a, 0x83, 0xf2, 0xc0, 0x80, 0x0c, 0xd7, 0xc5, 0xcd, 0xf2, 0xfd, 0xe0,
    0x49, 0x6f, 0x7b, 0xc3, 0x9f, 0xb4, 0xbf, 0x36, 0x32, 0x99, 0xbf, 0xa6, 0x37, 0xb2, 0xec, 0x33,
    0xc5, 0x07, 0xe3, 0x68, 0x21, 0xee, 0xc2, 0x07, 0x5f, 0x0e, 0x42, 0x0d, 0x38, 0xa1, 0xc9, 0xf3,
    0x12, 0x72, 0x61, 0xba, 0x31, 0x6c, 0x98, 0x76, 0x74, 0xfa, 0xdb, 0x20, 0xea, 0x7f, 0xeb, 0x75,
    0xee, 0x45, 0x5d, 0x12, 0x14, 0x6e, 0xa6, 0xf0, 0x2e, 0x8b, 0x01, 0xec, 0x2f, 0xa7, 0xa1, 0x15,
};

const unsigned char c_issuer_rem[] = {
    0x6e, 0x63, 0xb7, 0xbc, 0x70, 0xab, 0xdd, 0x09, 0x34, 0x1b, 0x34, 0xc0, 0x32, 0x86, 0xba, 0x9b,
    0xd8, 0x3b, 0xa7, 0x93, 0x6c, 0x5b, 0x77, 0x98, 0xfb, 0x22, 0xc5, 0xe5, 0x3f, 0xf2, 0x40, 0xa2,
    0x6d, 0xbd, 0x64, 0x15,
};

const unsigned char c_issuer_exp[] = {
    0x03,
};

const unsigned char c_icc_cert[] = {
    0xa4, 0x2f, 0xbe, 0xb1, 0x56, 0xb9, 0x8d, 0xcb, 0x05, 0x54, 0xda, 0x06, 0x2a, 0xdc, 0xa5, 0x30,
    0x9a, 0x91, 0xf0, 0x4f, 0xa2, 0xc7, 0xbd, 0x71, 0x02, 0xa8, 0xd7, 0x3f, 0x16, 0xa3, 0xcf, 0xad,
    0xe8, 0xaa, 0xdf, 0x4f, 0x3f, 0xe2, 0xa2, 0x12, 0x5c, 0xcd, 0xd7, 0x7c, 0x6b, 0x9f, 0x78, 0xb5,
    0xb4, 0x37, 0x1c, 0xe0, 0x80, 0x57, 0x25, 0xb0, 0xf9, 0xc0, 0x27, 0xaf, 0x14, 0x7d, 0x91, 0xe1,
    0xff, 0xdb, 0x20, 0x1e, 0x9c, 0x17, 0x0c, 0xe7, 0x77, 0x05, 0x3a, 0x17, 0x2a, 0xd5, 0x26, 0xdc,
    0xaf, 0xd3, 0x38, 0x95, 0xe1, 0xa9, 0x47, 0x30, 0x5c, 0x5b, 0x16, 0x7f, 0x2e, 0x7c, 0x6f, 0x99,
    0x15, 0x81, 0xa6, 0x52, 0xee, 0x47, 0x31, 0x54, 0x76, 0x0c, 0x2e, 0xd7, 0x74, 0x21, 0x4e, 0x50,
    0xdf, 0xec, 0xdd, 0x4c, 0xf2, 0x94, 0xc9, 0x74, 0xb8, 0x9e, 0xbc, 0xa2, 0x5b, 0x5a, 0xb3, 0xc0,
    0xbe, 0xb5, 0x0d, 0xfa, 0xf7, 0x82, 0xaf, 0xde, 0x14, 0x33, 0xd9, 0x0c, 0xa2, 0xa8, 0x9d, 0x65,
    0x1e, 0x75, 0xd6, 0x7e, 0xbc, 0x7c, 0x3e, 0x36, 0xf5, 0xa1, 0x65, 0xee, 0x61, 0x32, 0x61, 0x29,
    0x39, 0xc1, 0xec, 0xd3, 0x99, 0xe4, 0x60, 0x74, 0xb9, 0x96, 0xd9, 0x3a, 0x88, 0xe0, 0x1e, 0x0a,
};

const unsigned char c_icc_exp[] = {
    0x03,
};

const unsigned char c_sdad_cr[] = {
    0x1c, 0x00, 0x9f, 0xc4, 0x86, 0x79, 0x15, 0x7d, 0xbf, 0xf4, 0x5f, 0x65, 0xd3, 0x3f, 0xf7, 0x8d,
    0x4f, 0xcb, 0xf0, 0xcf, 0x5e, 0xa4, 0x20, 0x8d, 0x10, 0x7a, 0xe9, 0x5a, 0xa3, 0x8c, 0x54, 0x6d,
    0x0e, 0x5a, 0x18, 0xb8, 0x74, 0x03, 0xa1, 0x2b, 0xd4, 0x47, 0xa8, 0xbb, 0xfc, 0x1e, 0x49, 0xce,
    0x0b, 0x2e, 0x25, 0x13, 0x89, 0x20, 0x57, 0x03, 0xc9, 0xbb, 0x1a, 0x88, 0xcc, 0x79, 0xf1, 0xdd,
    0xc2, 0xf9, 0x84, 0x1e, 0xad, 0xf0, 0x7c, 0xe0, 0x7b, 0x62, 0x51, 0x1d, 0xdc, 0x93, 0xdf, 0x59,
    0xf2, 0x8f, 0x0e, 0x91, 0xf9, 0x23, 0x32, 0xd2, 0x9c, 0xde, 0xf2, 0xbc, 0xcb, 0x10, 0x08, 0x85,
    0x05, 0x00, 0xef, 0x3e, 0x47, 0x0a, 0x4c, 0xb1, 0x8c, 0xd9, 0x1a, 0xa5, 0xc1, 0xa1, 0x08, 0xf3,

};

const unsigned char c_ssd1[] = {
    0x5f, 0x25, 0x03, 0x14, 0x05, 0x01, 0x5f, 0x24, 0x03, 0x15, 0x06, 0x30, 0x5a, 0x08, 0x52, 0x85,
    0x88, 0x12, 0x54, 0x34, 0x56, 0x53, 0x5f, 0x34, 0x01, 0x01, 0x8e, 0x0c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x1e, 0x03, 0x1f, 0x03, 0x9f, 0x07, 0x02, 0xff, 0x00, 0x9f, 0x0d, 0x05,
    0xbc, 0x50, 0xbc, 0x00, 0x00, 0x9f, 0x0e, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9f, 0x0f, 0x05,
    0xbc, 0x70, 0xbc, 0x98, 0x00, 0x9f, 0x4a, 0x01, 0x82, 0x5f, 0x28, 0x02, 0x06, 0x43, 0x8c, 0x21,
    0x9f, 0x02, 0x06, 0x9f, 0x03, 0x06, 0x9f, 0x1a, 0x02, 0x95, 0x05, 0x5f, 0x2a, 0x02, 0x9a, 0x03,
    0x9c, 0x01, 0x9f, 0x37, 0x04, 0x9f, 0x35, 0x01, 0x9f, 0x45, 0x02, 0x9f, 0x4c, 0x08, 0x9f, 0x34,
    0x03, 0x8d, 0x0c, 0x91, 0x0a, 0x8a, 0x02, 0x95, 0x05, 0x9f, 0x37, 0x04, 0x9f, 0x4c, 0x08,
    0x39, 0x00,
};

static const struct tlv ssd1_tlv = {
    .len = sizeof(c_ssd1),
    .value = c_ssd1,
};

const unsigned char c_pan[] = {
    0x52, 0x85, 0x88, 0x12, 0x54, 0x34, 0x56, 0x53,
};

const unsigned char c_dd1[] = {
    0x12, 0x34, 0x57, 0x79,
};

const unsigned char c_dd2[] = {
    0x9f, 0x27, 0x01, 0x40, 0x9f, 0x36, 0x02, 0x00, 0x10, 0x9f, 0x10, 0x12, 0x00, 0x10, 0x90, 0x40,
    0x01, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
};

const unsigned char c_crm1[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x43, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x06, 0x43, 0x14, 0x09, 0x25, 0x50, 0x12, 0x34, 0x57, 0x79, 0x23, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x03, 0x00,
};

static const struct tlv crm1_tlv = {
    .len = sizeof(c_crm1),
    .value = c_crm1,
};

static int cda_test_raw(bool verbose) {
    const struct emv_pk *pk = &c_mchip_05;

    struct crypto_pk *kcp = crypto_pk_open(PK_RSA,
                                           pk->modulus, pk->mlen,
                                           pk->exp, pk->elen);
    if (!kcp)
        return 1;

    unsigned char *ipk_data;
    size_t ipk_data_len;
    ipk_data = crypto_pk_encrypt(kcp, c_issuer_cert, sizeof(c_issuer_cert), &ipk_data_len);
    crypto_pk_close(kcp);

    if (!ipk_data)
        return 1;

    if (verbose) {
        PrintAndLogEx(INFO, "issuer cert:");
        dump_buffer(ipk_data, ipk_data_len, stdout, 0);
    }

    size_t ipk_pk_len = ipk_data[13];
    unsigned char *ipk_pk = malloc(ipk_pk_len);
    memcpy(ipk_pk, ipk_data + 15, ipk_data_len - 36);
    memcpy(ipk_pk + ipk_data_len - 36, c_issuer_rem, sizeof(c_issuer_rem));

    struct crypto_hash *ch;
    ch = crypto_hash_open(HASH_SHA_1);
    if (!ch) {
        free(ipk_pk);
        free(ipk_data);
        return 1;
    }

    crypto_hash_write(ch, ipk_data + 1, 14);
    crypto_hash_write(ch, ipk_pk, ipk_pk_len);
    crypto_hash_write(ch, c_issuer_exp, sizeof(c_issuer_exp));

    unsigned char *h = crypto_hash_read(ch);
    if (!h) {
        crypto_hash_close(ch);
        free(ipk_pk);
        free(ipk_data);
        return 1;
    }

    if (verbose) {
        PrintAndLogEx(INFO, "crypto hash:");
        dump_buffer(h, 20, stdout, 0);
    }

    if (memcmp(ipk_data + ipk_data_len - 21, h, 20)) {
        crypto_hash_close(ch);
        free(ipk_pk);
        free(ipk_data);
        return 1;
    }

    crypto_hash_close(ch);
    free(ipk_data);

    struct crypto_pk *ikcp = crypto_pk_open(PK_RSA, ipk_pk, (int) ipk_pk_len,
                                            c_issuer_exp, (int) sizeof(c_issuer_exp));
    free(ipk_pk);
    if (!ikcp)
        return 1;

    unsigned char *iccpk_data;
    size_t iccpk_data_len;
    iccpk_data = crypto_pk_encrypt(ikcp, c_icc_cert, sizeof(c_icc_cert), &iccpk_data_len);
    crypto_pk_close(ikcp);

    if (!iccpk_data)
        return 1;

    if (verbose) {
        PrintAndLogEx(INFO, "icc cert:");
        dump_buffer(iccpk_data, iccpk_data_len, stdout, 0);
    }

    size_t iccpk_pk_len = iccpk_data[19];
    unsigned char *iccpk_pk = malloc(iccpk_pk_len);
    memcpy(iccpk_pk, iccpk_data + 21, /*iccpk_data_len - 36*/iccpk_pk_len);
    /*memcpy(iccpk_pk + iccpk_data_len - 36, icc_rem, sizeof(icc_rem));*/

    ch = crypto_hash_open(HASH_SHA_1);
    if (!ch) {
        free(iccpk_pk);
        free(iccpk_data);
        return 1;
    }

    crypto_hash_write(ch, iccpk_data + 1, iccpk_data_len - 22);
    crypto_hash_write(ch, c_icc_exp, sizeof(c_icc_exp));
    crypto_hash_write(ch, c_ssd1, sizeof(c_ssd1));

    h = crypto_hash_read(ch);
    if (!h) {
        crypto_hash_close(ch);
        free(iccpk_pk);
        free(iccpk_data);
        return 1;
    }

    if (verbose) {
        PrintAndLogEx(INFO, "crypto hash1.1:");
        dump_buffer(h, 20, stdout, 0);
    }

    if (memcmp(iccpk_data + iccpk_data_len - 21, h, 20)) {
        crypto_hash_close(ch);
        free(iccpk_pk);
        free(iccpk_data);
        return 1;
    }

    crypto_hash_close(ch);
    free(iccpk_data);

    struct crypto_pk *icckcp = crypto_pk_open(PK_RSA, iccpk_pk, (int) iccpk_pk_len,
                                              c_issuer_exp, (int) sizeof(c_issuer_exp));
    free(iccpk_pk);
    if (!icckcp)
        return 1;

    size_t sdad_len;
    unsigned char *sdad = crypto_pk_encrypt(icckcp, c_sdad_cr, sizeof(c_sdad_cr), &sdad_len);
    crypto_pk_close(icckcp);
    if (!sdad)
        return 1;

    if (verbose) {
        PrintAndLogEx(INFO, "SDAD:");
        dump_buffer(sdad, sdad_len, stdout, 0);
    }

    ch = crypto_hash_open(HASH_SHA_1);
    if (!ch) {
        free(sdad);
        return 1;
    }

    crypto_hash_write(ch, sdad + 1, sdad_len - 22);
    crypto_hash_write(ch, c_dd1, sizeof(c_dd1));

    unsigned char *h2 = crypto_hash_read(ch);
    if (!h2) {
        crypto_hash_close(ch);
        free(sdad);
        return 1;
    }

    if (verbose) {
        PrintAndLogEx(INFO, "crypto hash2:");
        dump_buffer(h2, 20, stdout, 0);
    }

    crypto_hash_close(ch);

    ch =  crypto_hash_open(HASH_SHA_1);
    if (!ch) {
        free(sdad);
        return 1;
    }

    crypto_hash_write(ch, c_crm1, sizeof(c_crm1));
    crypto_hash_write(ch, c_dd2, sizeof(c_dd2));

    h = crypto_hash_read(ch);
    if (!h) {
        crypto_hash_close(ch);
        free(sdad);
        return 1;
    }

    if (verbose) {
        PrintAndLogEx(INFO, "crypto hash2.1:");
        dump_buffer(h, 20, stdout, 0);
    }

    if (memcmp(sdad + 5 + 8 + 1 + 8, h, 20)) {
        crypto_hash_close(ch);
        free(sdad);
        return 1;
    }

    crypto_hash_close(ch);
    free(sdad);
    return 0;
}

static int cda_test_pk(bool verbose) {
    const struct emv_pk *pk = &c_mchip_05;
    struct tlvdb *db;

    db = tlvdb_external(0x90, sizeof(c_issuer_cert), c_issuer_cert);
    tlvdb_add(db, tlvdb_external(0x9f32, sizeof(c_issuer_exp), c_issuer_exp));
    tlvdb_add(db, tlvdb_external(0x92, sizeof(c_issuer_rem), c_issuer_rem));
    tlvdb_add(db, tlvdb_external(0x5a, sizeof(c_pan), c_pan));

    struct emv_pk *ipk = emv_pki_recover_issuer_cert(pk, db);
    if (!ipk) {
        PrintAndLogEx(WARNING, "Could not recover Issuer certificate!");
        tlvdb_free(db);
        return 2;
    }

    tlvdb_add(db, tlvdb_external(0x9f46, sizeof(c_icc_cert), c_icc_cert));
    tlvdb_add(db, tlvdb_external(0x9f47, sizeof(c_icc_exp), c_icc_exp));
    /*tlvdb_add(db, tlvdb_external(0x9f48, sizeof(issuer_rem), issuer_rem));*/

    struct emv_pk *iccpk = emv_pki_recover_icc_cert(ipk, db, &ssd1_tlv);
    if (!iccpk) {
        PrintAndLogEx(WARNING, "Could not recover ICC certificate!");
        emv_pk_free(ipk);
        tlvdb_free(db);
        return 2;
    }

    tlvdb_add(db, tlvdb_fixed(0x9f37, sizeof(c_dd1), c_dd1));

    struct tlvdb *cda_db;
    cda_db = tlvdb_fixed(0x9f27, 1, (unsigned char[]) { 0x40 });
    tlvdb_add(cda_db, tlvdb_fixed(0x9f36, 2, (unsigned char[]) { 0x00, 0x10 }));
    tlvdb_add(cda_db, tlvdb_external(0x9f4b, sizeof(c_sdad_cr), c_sdad_cr));
    tlvdb_add(cda_db, tlvdb_fixed(0x9f10, 0x12,
    (unsigned char[]) { 0x00, 0x10, 0x90, 0x40, 0x01, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff}));

    struct tlvdb *idndb = emv_pki_perform_cda(iccpk, db, cda_db,
                                              NULL,
                                              &crm1_tlv,
                                              NULL);
    if (!idndb) {
        PrintAndLogEx(WARNING, "Could not recover IDN!");
        tlvdb_free(cda_db);
        emv_pk_free(iccpk);
        emv_pk_free(ipk);
        tlvdb_free(db);
        return 2;
    }

    const struct tlv *idn = tlvdb_get(idndb, 0x9f4c, NULL);
    if (!idn) {
        PrintAndLogEx(WARNING, "IDN not found!");
        tlvdb_free(idndb);
        tlvdb_free(cda_db);
        emv_pk_free(iccpk);
        emv_pk_free(ipk);
        tlvdb_free(db);
        return 2;
    }

    if (verbose) {
        PrintAndLogEx(INFO, "IDN:");
        dump_buffer(idn->value, idn->len, stdout, 0);
    }

    tlvdb_free(idndb);
    tlvdb_free(cda_db);
    emv_pk_free(iccpk);
    emv_pk_free(ipk);
    tlvdb_free(db);
    return 0;
}

int exec_cda_test(bool verbose) {
    int ret = cda_test_raw(verbose);
    if (ret) {
        PrintAndLogEx(WARNING, "CDA raw test: (%s)", _RED_("failed"));
        return ret;
    }
    PrintAndLogEx(INFO, "CDA raw test: (%s)", _GREEN_("passed"));

    ret = cda_test_pk(verbose);
    if (ret) {
        PrintAndLogEx(WARNING, "CDA test pk: (%s)", _RED_("failed"));
        return ret;
    }
    PrintAndLogEx(INFO, "CDA test pk: (%s)", _GREEN_("passed"));
    return 0;
}
