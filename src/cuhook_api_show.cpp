#include <stdio.h>
#include <dlfcn.h>
#include <cstring>

#include "cuhook_api_show.h"

void* libcuda_driver_handle = dlopen("libcuda.so", RTLD_LAZY);

// get original dlsym pointer with different libc6 versions under differnt ubuntu system
// More information of libc6: https://pkgs.org/download/libc6
oriDlsymFn* get_dlsym_ptr(){
    const char* libc_version = gnu_get_libc_version();
    LOG_INFO("The libc Version in the system is:{}", libc_version);
    if(strcmp(libc_version,"2.31") == 0){
        return (oriDlsymFn*) __libc_dlsym(libdl_handle, "dlsym");  // ubuntu 20.04
    }else if(strcmp(libc_version, "2.35") == 0 || strcmp(libc_version, "2.39") == 0){
        return (oriDlsymFn*) dlvsym(libc_handle, "dlsym", "GLIBC_2.34");  // ubuntu 22.04 (2.35), ubuntu 24.04 (2.39)
    }else{ 
        return (oriDlsymFn*) __libc_dlsym(libdl_handle, "dlsym");  // lower than ubuntu 20.04
    }
}

CUresult getProcAddressBySymbol(const char *symbol, void **pfn, int driverVersion, cuuint64_t flags,
                                    CUdriverProcAddressQueryResult *symbolStatus){
    
    if(libcuda_driver_handle == nullptr){                                                                
        LOG_ERROR("Interception error (libcuda_driver_handle is null): {}", dlerror());                                                                                    
    }
    if(symbol == nullptr){
        LOG_ERROR("Symbol is empty.");
        return CUDA_ERROR_UNKNOWN;
    }
    LOG_INFO("GetMethodBySymbol is called:  symbol_name = {}, cudaVersion = {}, flags = {}.", symbol, driverVersion, flags);

     if(strcmp(symbol, "cuGetProcAddress") == 0){
        *pfn = (void *) &getProcAddressBySymbol;
        return CUDA_SUCCESS;
    }

    CUresult res_tmp = cuGetProcAddress_v2(symbol, pfn, driverVersion, flags, symbolStatus);
    if(res_tmp == CUDA_SUCCESS){
        // LOG_DEBUG("cuda api {} is found, pfn= {}, symbolStatus = {}", symbol, *pfn, symbolStatus);
    }else{
        LOG_DEBUG("cuda api {} is not found.");
    }
    return res_tmp;

    /*  manual check */ 
    // // 1. try to find "symbol" + _v3
    // char symbolExtV3[strlen(symbol) + 3] = {};
    // strcat(symbolExtV3, symbol);
    // strcat(symbolExtV3, "_v3");
    // *pfn = dlsym(libcuda_driver_handle, symbolExtV3);
    // if (*pfn != nullptr)
    // {
    //     return CUDA_SUCCESS;
    // }

    // // 2. try to find "symbol" + _v2
    // char symbolExtV2[strlen(symbol) + 3] = {};
    // strcat(symbolExtV2, symbol);
    // strcat(symbolExtV2, "_v2");
    // *pfn = dlsym(libcuda_driver_handle, symbolExtV2);
    // if (*pfn != nullptr)
    // {
    //     return CUDA_SUCCESS;
    // }

    // *pfn = dlsym(libcuda_driver_handle, symbol);
    // if (*pfn != nullptr)
    // {
    //     return CUDA_SUCCESS;
    // }

    // if(strcmp(symbol, "")){
    //     LOG_ERROR("symbol not found: {}", symbol);
    // } 
    // return CUDA_ERROR_UNKNOWN;
}


