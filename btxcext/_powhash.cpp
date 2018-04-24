// Copyright (c) 2018 The Bitcore Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Python.h"

#include "utilstrencodings.h"
#include "sph_blake.h"
#include "sph_bmw.h"
#include "sph_groestl.h"
#include "sph_jh.h"
#include "sph_keccak.h"
#include "sph_skein.h"
#include "sph_luffa.h"
#include "sph_cubehash.h"
#include "sph_shavite.h"
#include "sph_simd.h"
#include "sph_echo.h"
#include <algorithm>

#define HASH_FUNC_BASE_TIMESTAMP 1492973331 // BitCore: Genesis Timestamp
#define HASH_FUNC_COUNT 10                   // BitCore: HASH_FUNC_COUNT of 11
#define HASH_FUNC_COUNT_PERMUTATIONS 40320  // BitCore: HASH_FUNC_COUNT!

static PyObject * powhash_hash (PyObject *self, PyObject *args)
{
    sph_blake512_context     ctx_blake;
    sph_bmw512_context       ctx_bmw;
    sph_groestl512_context   ctx_groestl;
    sph_jh512_context        ctx_jh;
    sph_keccak512_context    ctx_keccak;
    sph_skein512_context     ctx_skein;
    sph_luffa512_context     ctx_luffa;
    sph_cubehash512_context  ctx_cubehash;
    sph_shavite512_context   ctx_shavite;
    sph_simd512_context      ctx_simd;
    sph_echo512_context      ctx_echo;
    static unsigned char pblank[1];

    const char *bytes;
    int bytesSize = 0;
    unsigned long timeStamp = 0;
    if (! PyArg_ParseTuple (args, "s#k", &bytes, &bytesSize, &timeStamp))
       return NULL;


    char hash[HASH_FUNC_COUNT][32];

    memset ( hash, 0, 32*HASH_FUNC_COUNT);

    // We want to permute algorithms. To get started we
    // initialize an array with a sorted sequence of unique
    // integers where every integer represents its own algorithm.
    uint32_t permutation[HASH_FUNC_COUNT];
    for (uint32_t i=0; i < HASH_FUNC_COUNT; i++) {
        permutation[i]=i;
    }

    // Compute the next permuation
    uint32_t steps = (timeStamp - HASH_FUNC_BASE_TIMESTAMP)%HASH_FUNC_COUNT_PERMUTATIONS;
    for (uint32_t i=0; i < steps; i++) {
        std::next_permutation(permutation, permutation + HASH_FUNC_COUNT);
    }

    for (uint32_t i=0; i < HASH_FUNC_COUNT; i++) {
	    switch(permutation[i]) {
            case 0:
                sph_blake512_init(&ctx_blake);
                if (i == 0)
                    sph_blake512 (&ctx_blake, (bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_blake512 (&ctx_blake, static_cast<const void*>(&hash[i-1][0]), 64);                
                sph_blake512_close(&ctx_blake, static_cast<void*>(&hash[i][0]));
            break;
            case 1:
                sph_bmw512_init(&ctx_bmw);
                if (i == 0)
                    sph_bmw512 (&ctx_bmw, ( bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_bmw512 (&ctx_bmw, static_cast<const void*>(&hash[i-1][0]), 64);
                sph_bmw512_close(&ctx_bmw, static_cast<void*>(&hash[i][0]));
            break;
            case 2:
                sph_groestl512_init(&ctx_groestl);
                if (i == 0)
                    sph_groestl512 (&ctx_groestl, (bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_groestl512 (&ctx_groestl, static_cast<const void*>(&hash[i-1][0]), 64);
                sph_groestl512_close(&ctx_groestl, static_cast<void*>(&hash[i][0]));
            break;
            case 3:
                sph_skein512_init(&ctx_skein);
                if (i == 0)
                    sph_skein512 (&ctx_skein, (bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_skein512 (&ctx_skein, static_cast<const void*>(&hash[i-1][0]), 64);
                sph_skein512_close(&ctx_skein, static_cast<void*>(&hash[i][0]));
            break;
            case 4:
                sph_jh512_init(&ctx_jh);
                if (i == 0)
                    sph_jh512 (&ctx_jh, (bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_jh512 (&ctx_jh, static_cast<const void*>(&hash[i-1][0]), 64);
                sph_jh512_close(&ctx_jh, static_cast<void*>(&hash[i][0]));
            break;
            case 5:
                sph_keccak512_init(&ctx_keccak);
                if (i == 0)
                    sph_keccak512 (&ctx_keccak, (bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_keccak512 (&ctx_keccak, static_cast<const void*>(&hash[i-1][0]), 64);
                sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[i][0]));
            break;
            case 6:
                sph_luffa512_init(&ctx_luffa);
                if (i == 0)
                    sph_luffa512 (&ctx_luffa, (bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_luffa512 (&ctx_luffa, static_cast<void*>(&hash[i-1][0]), 64);
                sph_luffa512_close(&ctx_luffa, static_cast<void*>(&hash[i][0]));
            break;
            case 7:
                sph_cubehash512_init(&ctx_cubehash);
                if (i == 0)
                    sph_cubehash512 (&ctx_cubehash, (bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_cubehash512 (&ctx_cubehash, static_cast<const void*>(&hash[i-1][0]), 64);
                sph_cubehash512_close(&ctx_cubehash, static_cast<void*>(&hash[i][0]));
            break;
            case 8:
                sph_shavite512_init(&ctx_shavite);
                if (i == 0)
                    sph_shavite512 (&ctx_shavite, (bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_shavite512(&ctx_shavite, static_cast<const void*>(&hash[i-1][0]), 64);
                sph_shavite512_close(&ctx_shavite, static_cast<void*>(&hash[i][0]));
            break;
            case 9:
                sph_simd512_init(&ctx_simd);
                if (i == 0)
                    sph_simd512 (&ctx_simd, (bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_simd512 (&ctx_simd, static_cast<const void*>(&hash[i-1][0]), 64);
                sph_simd512_close(&ctx_simd, static_cast<void*>(&hash[i][0]));
            break;
            case 10:
                sph_echo512_init(&ctx_echo);
                if (i == 0)
                    sph_echo512 (&ctx_echo, (bytesSize == 0 ? pblank : static_cast<const void*>(bytes)), bytesSize * sizeof(bytes[0]));
                else
                    sph_echo512 (&ctx_echo, static_cast<const void*>(&hash[i-1][0]), 64);
                sph_echo512_close(&ctx_echo, static_cast<void*>(&hash[i][0]));
            break;
	    }
    }
    char result[32];
    for ( int x = 0; x < 32; x++ )
            result[31-x] = hash[HASH_FUNC_COUNT-1][x];
    std::string strHex = HexStr( &result[0], &result[0]+32 );
    return Py_BuildValue("s", strHex.data());
};

PyDoc_STRVAR(hash_doc, "computes proof of work hash");

static PyMethodDef powhash_methods[] = {
    {"hash", powhash_hash, METH_VARARGS, hash_doc},
    {NULL, NULL, 0, NULL},
};

static struct PyModuleDef _powhash =
{
    PyModuleDef_HEAD_INIT,
    "_powhash",
    "usage _powhash.hash(header,timestamp)\n",
    -1,
    powhash_methods
};

PyMODINIT_FUNC PyInit__powhash(void)
{
    return PyModule_Create(&_powhash);
};
