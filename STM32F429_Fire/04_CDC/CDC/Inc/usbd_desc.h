#ifndef __USBD_DESC_H
#define __USBD_DESC_H

#include "usbd_def.h"

#ifdef STM32F103xE
    #define         DEVICE_ID1          (0x1FFFF7E8)
    #define         DEVICE_ID2          (0x1FFFF7EC)
    #define         DEVICE_ID3          (0x1FFFF7F0)
#endif

#if defined(STM32F412Zx)
    #define         DEVICE_ID1          (0x1FFF7A10)
    #define         DEVICE_ID2          (0x1FFF7A14)
    #define         DEVICE_ID3          (0x1FFF7A18)
#endif  

#if defined(STM32F723xx)
    #define         DEVICE_ID1          (0x1FF07A10)
    #define         DEVICE_ID2          (0x1FF07A14)
    #define         DEVICE_ID3          (0x1FF07A18)
#endif  

#define  USB_SIZ_STRING_SERIAL       0x1A
extern USBD_DescriptorsTypeDef VCP_Desc;

#endif /* __USBD_DESC_H */
