/*********************************************************
*     File Name : FTL_pageOP.c
*     YIN  @ 2017.03.31
*     Description  : Initial create
**********************************************************/
#include <string.h>
#include <stdio.h>
#include "Driver.h"
#include "Hardware.h"
#include "FTL_ECC.h"
#include "FTL_pageOP.h"
#include "FTL_Struct.h"

/**
*读取一页的数据
*参数1：@PageAddrBase: 上层写入基地址
*参数2：@Buffer:
*返回值：0 正常， 1 不正常
*/
 uint8_t pageOP_Read(uint32_t PageAddrBase, uint8_t *pBuffer)
 {
     uint8_t m8i = 0;
     uint8_t ret = 0;
     uint32_t PageNo;
     uint16_t AddrInPage;

     SPARE_TAG_TYPE  SpareTag;
     uint8_t ECC[3];

     PageNo = PageAddrBase;
     AddrInPage = PageNo % BYTE_PER_FILEPAGE;

     for(m8i = 0; m8i < 3; m8i++)
     {
         //读取数据
         Nand_Read(pBuffer, PageNo, AddrInPage, BYTE_PER_FILEPAGE);

        if(pBuffer == NULL)
        {
            return 1;
        }

        //计算读出来数据的ECC
        nand_calculate_ecc(pBuffer, ECC);

        //读取ECC码
        ret = pageOP_ReadSpare(PageAddrBase, &SpareTag);

        if(ret != 0)
        {
            printf("%s: READ SPARE WRONG!\n", __func__);
            return 1;
        }

        //判断读取是否正确
        if((ECC[0] == SpareTag.ecc[0]) && (ECC[1] == SpareTag.ecc[1]) && (ECC[2] == SpareTag.ecc[2]))
            break;

    }

    //读取错误
    if((ECC[0] != SpareTag.ecc[0]) || (ECC[1] != SpareTag.ecc[1]) || (ECC[2] != SpareTag.ecc[2]))
    {
        return 1;
    }

    return 0;
}

/**
*写入一页的数据
*参数1：@PageAddrBase: 上层写入基地址
*参数2：@Buffer:
*返回值：0 正常， 1 不正常
*/
 uint8_t pageOP_Write(uint32_t PageAddrBase, uint8_t *pBuffer)
 {
     uint8_t m8i = 0;
     uint8_t ret = 0;
     uint32_t PageNo;
     uint16_t AddrInPage;

     SPARE_TAG_TYPE  SpareTag;
     uint8_t ECC[3];

     //？


}

/**
*读取spare区
*参数1：@PageAddrBase: 上层写入基地址
*参数2：@SpareTag: 装spare区的数据
*/

uint8_t pageOP_ReadSpare(uint32_t PageAddrBase, SPARE_TAG_TYPE *SpareTag)
{
    uint32_t PageNo;
    uint16_t AddrInPage;
    //uint16_t Block_Addr;
    //uint16_t Page_Addr;

    //将 PageAddrBase转化为实际写入的基地址(针对一颗NAND)
    //Block_Addr = (uint16_t)(PageAddrBase / MAX_PAGES_PER_BLOCK);
    //Page_Addr = (uint16_t)(PageAddrBase % MAX_PAGES_PER_BLOCK);
    //PageNo = (uint32_t)(Block_Addr * BYTE_PER_FILEBLOCK) + (Page_Addr * BYTE_PER_FILEPAGE);
    //PageNo += BYTE_PER_FILEROWPAGE;
    PageNo = PageAddrBase;
    AddrInPage = PageNo % BYTE_PER_FILEPAGE;

    Nand_ReadSpare(SpareTag, PageNo, AddrInPage, MAX_BYTES_PER_SECTORSPARE);

    return 0;
}

/**
*写入spare区
*参数1：@PageAddrBase: 上层写入基地址
*参数2：@SpareTag: 装spare区的数据
*/
uint8_t pageOP_WriteSpare(uint32_t PageAddrBase, SPARE_TAG_TYPE *SpareTag)
{
    uint32_t PageNo;
    uint16_t AddrInPage;

    PageNo = PageAddrBase;
    AddrInPage = PageNo % BYTE_PER_FILEPAGE;

    Nand_WriteSpare(SpareTag, PageNo, AddrInPage, MAX_BYTES_PER_SECTORSPARE);

    return 0;
}


