#ifndef CUDAHOOK_H
#define CUDAHOOK_H


#pragma once

#include <string.h>
#include <dlfcn.h>
#include <gnu/libc-version.h>


#include <cuda.h>
#include <cuda_runtime.h>

#include "log_debug.h"

#define SYMBOL_TO_STRING(x) stringtify(x)
#define stringtify(x) #x

inline bool stringCompare(const char* st1, const char *st2){
    return strcmp(st1, st2) == 0;
}

extern "C"{
    void *__libc_dlsym(void *map, const char *name);
    void *__libc_dlopen_mode(const char *name, int mode);   
}

using oriDlsymFn = void *(void *, const char *);
// void *libdl_handle = __libc_dlopen_mode("libdl.so.2", RTLD_LAZY);
void *libdl_handle = dlopen("libdl.so.2", RTLD_LAZY);
void *libc_handle = dlopen("libc.so.6", RTLD_LAZY);

oriDlsymFn* get_dlsym_ptr();

CUresult getProcAddressBySymbol(const char *symbol, void **pfn, int driverVersion, cuuint64_t flags,
                                CUdriverProcAddressQueryResult *symbolStatus);


void *dlsym(void *handle, const char *symbol){
    // LOG_INFO("ALL symbol: {}", symbol);
    if(stringCompare(symbol, SYMBOL_TO_STRING(cuGetProcAddress))){
        LOG_DEBUG("dlsym symbol: {}", symbol);
        return (void *) &getProcAddressBySymbol;
    }else{
        LOG_DEBUG("dlsym other symbol: {}", symbol);
    }
    // std::cout << "all other dlsym: " << symbol << std::endl;
    // static auto normalDsym = (oriDlsymFn*) __libc_dlsym(libdl_handle, "dlsym");
    // static auto normalDsym = (oriDlsymFn*) dlvsym(libc_handle, "__libc_dlsym", NULL);
    static auto normalDlsym = get_dlsym_ptr();
    return (*normalDlsym)(handle, symbol);
}

typedef enum HookSymbolsIndexEnum {
  CU_HOOK_GET_PROC_ADDRESS,
  CU_HOOK_MEM_ALLOC,
  CU_HOOK_MEM_ALLOC_MANAGED,
  CU_HOOK_MEM_ALLOC_PITCH,
  CU_HOOK_MEM_ALLOC_ASYNC,
  CU_HOOK_MEM_FREE,
  CU_HOOK_ARRAY_CREATE,
  CU_HOOK_ARRAY3D_CREATE,
  CU_HOOK_MIPMAPPED_ARRAY_CREATE,
  CU_HOOK_ARRAY_DESTROY,
  CU_HOOK_MIPMAPPED_ARRAY_DESTROY,
  CU_HOOK_CTX_GET_CURRENT,
  CU_HOOK_CTX_CREATE,
  CU_HOOK_CTX_SET_CURRENT,
  CU_HOOK_CTX_DESTROY,
  CU_HOOK_LAUNCH_KERNEL,
  CU_HOOK_LAUNCH_COOPERATIVE_KERNEL,
  CU_HOOK_DEVICE_TOTOAL_MEM,
  CU_HOOK_MEM_INFO,
  CU_HOOK_CTX_SYNC,
  CU_HOOK_MEMCPY_ATOH,
  CU_HOOK_MEMCPY_DTOH,
  CU_HOOK_MEMCPY_DTOH_ASYNC,
  CU_HOOK_MEMCPY_HTOA,
  CU_HOOK_MEMCPY_HTOD,
  CU_HOOK_MEMCPY_HTOD_ASYNC,
  NUM_HOOK_SYMBOLS,
} HookSymbolsIndex;



#endif