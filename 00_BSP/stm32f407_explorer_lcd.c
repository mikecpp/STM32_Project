#include <stdio.h>
#include "stm32f407_explorer_lcd.h"  
#include "../../../Utilities/Fonts/fonts.h"
#include "../../../Utilities/Fonts/font24.c"
#include "../../../Utilities/Fonts/font20.c"
#include "../../../Utilities/Fonts/font16.c"
#include "../../../Utilities/Fonts/font12.c"
#include "../../../Utilities/Fonts/font8.c"
 
#define POLY_X(Z)              ((int32_t)((Points + (Z))->X))
#define POLY_Y(Z)              ((int32_t)((Points + (Z))->Y))

#define MAX_HEIGHT_FONT         17
#define MAX_WIDTH_FONT          24
#define OFFSET_BITMAP           54

#define ABS(X)  ((X) > 0 ? (X) : -(X)) 

static LCD_DrawPropTypeDef DrawProp;
static LCD_DrvTypeDef  *lcd_drv; 

/* Max size of bitmap will based on a font24 (17x24) */
static uint8_t bitmap[MAX_HEIGHT_FONT*MAX_WIDTH_FONT*2+OFFSET_BITMAP] = {0};

static void LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode);
static void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
static void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

uint8_t BSP_LCD_Init(void)
{ 
    uint16_t lcd_id = 0x00;

#if defined (USE_STM32F407_MINI)   
    lcd_drv = &ili9341_drv;
#else // USE_STM32F407_EXPLORER
    lcd_drv = &nt35510_drv;
#endif
    
    lcd_drv->Init();
    
    lcd_id = lcd_drv->ReadID();
    printf("LCD ID = 0x%04X \r\n", lcd_id);
       
    DrawProp.BackColor = 0xFFFF;
    DrawProp.pFont     = &Font24;
    DrawProp.TextColor = 0x0000;
  
    return LCD_OK;
}

uint32_t BSP_LCD_GetXSize(void)
{
  return(lcd_drv->GetLcdPixelWidth());
}

uint32_t BSP_LCD_GetYSize(void)
{
  return(lcd_drv->GetLcdPixelHeight());
}

uint16_t BSP_LCD_GetTextColor(void)
{
  return DrawProp.TextColor;
}

uint16_t BSP_LCD_GetBackColor(void)
{
  return DrawProp.BackColor;
}

void BSP_LCD_SetTextColor(uint16_t Color)
{
    DrawProp.TextColor = Color;
}

void BSP_LCD_SetBackColor(uint16_t Color)
{
    DrawProp.BackColor = Color;
}

void BSP_LCD_SetFont(sFONT *pFonts)
{
    DrawProp.pFont = pFonts;
}

sFONT *BSP_LCD_GetFont(void)
{
    return DrawProp.pFont;
}

void BSP_LCD_Clear(uint16_t Color)
{ 
    uint32_t counter      = 0;
    uint32_t color_backup = DrawProp.TextColor; 
    uint16_t width  = BSP_LCD_GetXSize();
    uint16_t height = BSP_LCD_GetYSize();
    
    DrawProp.TextColor = Color;
  
    // printf("width = %d, height = %d \r\n", width, height);
    
    for(counter = 0; counter < height; counter++)
    {
        BSP_LCD_DrawHLine(0, counter, width);
    }

    DrawProp.TextColor = color_backup; 
    BSP_LCD_SetTextColor(DrawProp.TextColor);
}

void BSP_LCD_ClearStringLine(uint16_t Line)
{ 
    uint32_t colorbackup = DrawProp.TextColor; 
    DrawProp.TextColor = DrawProp.BackColor;;
    
    /* Draw a rectangle with background color */
    BSP_LCD_FillRect(0, (Line * DrawProp.pFont->Height), BSP_LCD_GetXSize(), DrawProp.pFont->Height);
  
    DrawProp.TextColor = colorbackup;
    BSP_LCD_SetTextColor(DrawProp.TextColor);
}

void BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
    LCD_DrawChar(Xpos, Ypos, &DrawProp.pFont->table[(Ascii-' ') *\
    DrawProp.pFont->Height * ((DrawProp.pFont->Width + 7) / 8)]);
}

void BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *pText, Line_ModeTypdef Mode)
{
    uint16_t refcolumn = 1, counter = 0;
    uint32_t size = 0, xsize = 0; 
    uint8_t  *ptr = pText;
  
    /* Get the text size */
    while (*ptr++) size ++ ;
  
    /* Characters number per line */
    xsize = (BSP_LCD_GetXSize()/DrawProp.pFont->Width);
  
    switch (Mode)
    {
    case CENTER_MODE:
        {
        refcolumn = Xpos + ((xsize - size)* DrawProp.pFont->Width) / 2;
        break;
        }
    case LEFT_MODE:
        {   
        refcolumn = Xpos;
        break;
        }
    case RIGHT_MODE:
        {
        refcolumn =  - Xpos + ((xsize - size)*DrawProp.pFont->Width);
        break;
        }    
    default:
        {
        refcolumn = Xpos;
        break;
        }
    }
  
    /* Send the string character by character on lCD */
    while ((*pText != 0) & (((BSP_LCD_GetXSize() - (counter*DrawProp.pFont->Width)) & 0xFFFF) >= DrawProp.pFont->Width))
    {
        /* Display one character on LCD */
        BSP_LCD_DisplayChar(refcolumn, Ypos, *pText);
        /* Decrement the column position by 16 */
        refcolumn += DrawProp.pFont->Width;
        /* Point on the next character */
        pText++;
        counter++;
    }
}

void BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *pText)
{
    BSP_LCD_DisplayStringAt(0, LINE(Line),pText, LEFT_MODE);
}

uint16_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
    uint16_t ret = 0;
  
    if(lcd_drv->ReadPixel != NULL)
    {
        ret = lcd_drv->ReadPixel(Xpos, Ypos);
    }
    
    return ret;
}

void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    uint32_t index = 0;
  
    if(lcd_drv->DrawHLine != NULL)
    {
        lcd_drv->DrawHLine(DrawProp.TextColor, Xpos, Ypos, Length);
    }
    else
    {
        for(index = 0; index < Length; index++)
        {
        LCD_DrawPixel((Xpos + index), Ypos, DrawProp.TextColor);
        }
    }
}

void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    uint32_t index = 0;
  
    if(lcd_drv->DrawVLine != NULL)
    {
        lcd_drv->DrawVLine(DrawProp.TextColor, Xpos, Ypos, Length);
    }
    else
    {
        for(index = 0; index < Length; index++)
        {
            LCD_DrawPixel(Xpos, Ypos + index, DrawProp.TextColor);
        }
    }
}

void BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
    yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
    curpixel = 0;
  
    deltax = ABS(x2 - x1);        /* The difference between the x's */
    deltay = ABS(y2 - y1);        /* The difference between the y's */
    x = x1;                       /* Start x off at the first pixel */
    y = y1;                       /* Start y off at the first pixel */
  
    if (x2 >= x1)                 /* The x-values are increasing */
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else                          /* The x-values are decreasing */
    {
        xinc1 = -1;
        xinc2 = -1;
    }
  
    if (y2 >= y1)                 /* The y-values are increasing */
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else                          /* The y-values are decreasing */
    {
        yinc1 = -1;
        yinc2 = -1;
    }
  
    if (deltax >= deltay)         /* There is at least one x-value for every y-value */
    {
        xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
        yinc2 = 0;                  /* Don't change the y for every iteration */
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;         /* There are more x-values than y-values */
    }
    else                          /* There is at least one y-value for every x-value */
    {
        xinc2 = 0;                  /* Don't change the x for every iteration */
        yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;         /* There are more y-values than x-values */
    }
  
    for (curpixel = 0; curpixel <= numpixels; curpixel++)
    {
        LCD_DrawPixel(x, y, DrawProp.TextColor);  /* Draw the current pixel */
        num += numadd;                            /* Increase the numerator by the top of the fraction */
        if (num >= den)                           /* Check if numerator >= denominator */
        {
            num -= den;                             /* Calculate the new numerator value */
            x += xinc1;                             /* Change the x as appropriate */
            y += yinc1;                             /* Change the y as appropriate */
        }
        x += xinc2;                               /* Change the x as appropriate */
        y += yinc2;                               /* Change the y as appropriate */
    }
}

void BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    /* Draw horizontal lines */
    BSP_LCD_DrawHLine(Xpos, Ypos, Width);
    BSP_LCD_DrawHLine(Xpos, (Ypos+ Height), Width);
  
    /* Draw vertical lines */
    BSP_LCD_DrawVLine(Xpos, Ypos, Height);
    BSP_LCD_DrawVLine((Xpos + Width), Ypos, Height);
}
                            
void BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int32_t  D;       /* Decision Variable */ 
    uint32_t  CurX;   /* Current X Value */
    uint32_t  CurY;   /* Current Y Value */ 
  
    D = 3 - (Radius << 1);
    CurX = 0;
    CurY = Radius;
  
    while (CurX <= CurY)
    {
        LCD_DrawPixel((Xpos + CurX), (Ypos - CurY), DrawProp.TextColor);
        LCD_DrawPixel((Xpos - CurX), (Ypos - CurY), DrawProp.TextColor);
        LCD_DrawPixel((Xpos + CurY), (Ypos - CurX), DrawProp.TextColor);
        LCD_DrawPixel((Xpos - CurY), (Ypos - CurX), DrawProp.TextColor);
        LCD_DrawPixel((Xpos + CurX), (Ypos + CurY), DrawProp.TextColor);
        LCD_DrawPixel((Xpos - CurX), (Ypos + CurY), DrawProp.TextColor);
        LCD_DrawPixel((Xpos + CurY), (Ypos + CurX), DrawProp.TextColor);
        LCD_DrawPixel((Xpos - CurY), (Ypos + CurX), DrawProp.TextColor);   
        /* Initialize the font */
        BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

        if (D < 0)
        { 
            D += (CurX << 2) + 6;
        }
        else
        {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    } 
}

void BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount)
{
    int16_t X = 0, Y = 0;

    if(PointCount < 2)
    {
        return;
    }

    BSP_LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
  
    while(--PointCount)
    {
        X = Points->X;
        Y = Points->Y;
        Points++;
        BSP_LCD_DrawLine(X, Y, Points->X, Points->Y);
    }
}

void BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
    int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
    float K = 0, rad1 = 0, rad2 = 0;
  
    rad1 = XRadius;
    rad2 = YRadius;
  
    K = (float)(rad2/rad1);
  
    do {      
        LCD_DrawPixel((Xpos-(uint16_t)(x/K)), (Ypos+y), DrawProp.TextColor);
        LCD_DrawPixel((Xpos+(uint16_t)(x/K)), (Ypos+y), DrawProp.TextColor);
        LCD_DrawPixel((Xpos+(uint16_t)(x/K)), (Ypos-y), DrawProp.TextColor);
        LCD_DrawPixel((Xpos-(uint16_t)(x/K)), (Ypos-y), DrawProp.TextColor);      
    
        e2 = err;
        if (e2 <= x) {
            err += ++x*2+1;
            if (-y == x && e2 <= y) e2 = 0;
        }
        if (e2 > y) err += ++y*2+1;     
    }
    while (y <= 0);
}

void BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
    uint32_t height = 0;
    uint32_t width  = 0;
  
    /* Read bitmap width */
    width  = *(uint16_t *)  (pbmp + 18);
    width |= (*(uint16_t *) (pbmp + 20)) << 16;
  
    /* Read bitmap height */
    height  = *(uint16_t *)  (pbmp + 22);
    height |= (*(uint16_t *) (pbmp + 24)) << 16; 
  
    LCD_SetDisplayWindow(Xpos, Ypos, width, height);
  
    if(lcd_drv->DrawBitmap != NULL) {
        lcd_drv->DrawBitmap(Xpos, Ypos, pbmp);
    } 
    
    LCD_SetDisplayWindow(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

void BSP_LCD_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{
  
    LCD_SetDisplayWindow(Xpos, Ypos, Xsize, Ysize);
  
    if(lcd_drv->DrawRGBImage != NULL)
    {
        lcd_drv->DrawRGBImage(Xpos, Ypos, Xsize, Ysize, pdata);
    } 
    LCD_SetDisplayWindow(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

void BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    BSP_LCD_SetTextColor(DrawProp.TextColor);
    do
    {
        BSP_LCD_DrawHLine(Xpos, Ypos++, Width);    
    }
    while(Height--);
}

void BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int32_t  D;        /* Decision Variable */ 
    uint32_t  CurX;    /* Current X Value */
    uint32_t  CurY;    /* Current Y Value */ 
  
    D = 3 - (Radius << 1);

    CurX = 0;
    CurY = Radius;
  
    BSP_LCD_SetTextColor(DrawProp.TextColor);

    while (CurX <= CurY)
    {
        if(CurY > 0) 
        {
            BSP_LCD_DrawHLine(Xpos - CurY, Ypos + CurX, 2*CurY);
            BSP_LCD_DrawHLine(Xpos - CurY, Ypos - CurX, 2*CurY);
        }

        if(CurX > 0) 
        {
            BSP_LCD_DrawHLine(Xpos - CurX, Ypos - CurY, 2*CurX);
            BSP_LCD_DrawHLine(Xpos - CurX, Ypos + CurY, 2*CurX);
        }
        if (D < 0)
        { 
            D += (CurX << 2) + 6;
        }
        else
        {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }

    BSP_LCD_SetTextColor(DrawProp.TextColor);
    BSP_LCD_DrawCircle(Xpos, Ypos, Radius);
}

void BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
    int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
    float K = 0, rad1 = 0, rad2 = 0;
  
    rad1 = XRadius;
    rad2 = YRadius;
  
    K = (float)(rad2/rad1);    
  
    do { 
        BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1));
        BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1));
    
        e2 = err;
        if (e2 <= x) 
        {
            err += ++x*2+1;
            if (-y == x && e2 <= y) e2 = 0;
        }
        if (e2 > y) err += ++y*2+1;
    } while (y <= 0);
}

void BSP_LCD_DisplayOn(void)
{
    lcd_drv->DisplayOn();
}

void BSP_LCD_DisplayOff(void)
{
    lcd_drv->DisplayOff();
}

static void LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
    if(lcd_drv->WritePixel != NULL) {
        lcd_drv->WritePixel(Xpos, Ypos, RGBCode);
    }
}

static void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *pChar)
{
    uint32_t counterh = 0, counterw = 0, index = 0;
    uint16_t height = 0, width = 0;
    uint8_t offset  = 0;
    uint8_t *pchar  = NULL;
    uint32_t line   = 0;
  
    height = DrawProp.pFont->Height;
    width  = DrawProp.pFont->Width;
  
    /* Fill bitmap header*/
    *(uint16_t *) (bitmap + 2) = (uint16_t)(height*width*2+OFFSET_BITMAP);
    *(uint16_t *) (bitmap + 4) = (uint16_t)((height*width*2+OFFSET_BITMAP)>>16);
    *(uint16_t *) (bitmap + 10) = OFFSET_BITMAP;
    *(uint16_t *) (bitmap + 18) = (uint16_t)(width);
    *(uint16_t *) (bitmap + 20) = (uint16_t)((width)>>16);
    *(uint16_t *) (bitmap + 22) = (uint16_t)(height);
    *(uint16_t *) (bitmap + 24) = (uint16_t)((height)>>16);

    offset =  8 *((width + 7)/8) -  width ;
         
    for(counterh = 0; counterh < height; counterh++)
    {
        pchar = ((uint8_t *)pChar + (width + 7)/8 * counterh);
    
        if(((width + 7)/8) == 3)
        {
            line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
        }

        if(((width + 7)/8) == 2)
        {
            line =  (pchar[0]<< 8) | pchar[1];
        }
      
        if(((width + 7)/8) == 1)
        {   
            line =  pchar[0];
        }  
    
        for (counterw = 0; counterw < width; counterw++)
        {
            /* Image in the bitmap is written from the bottom to the top */
            /* Need to invert image in the bitmap */
            index = (((height-counterh-1)*width) + (counterw))*2 + OFFSET_BITMAP;
            if(line & (1 << (width- counterw + offset- 1))) 
            {
                bitmap[index]   = (uint8_t) DrawProp.TextColor;
                bitmap[index+1] = (uint8_t)(DrawProp.TextColor >> 8);
            }
            else
            {
                bitmap[index]   = (uint8_t) DrawProp.BackColor;
                bitmap[index+1] = (uint8_t)(DrawProp.BackColor >> 8);
            } 
        }
    }

    BSP_LCD_DrawBitmap(Xpos, Ypos, bitmap);
}

static void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    if(lcd_drv->SetDisplayWindow != NULL)
    {
        lcd_drv->SetDisplayWindow(Xpos, Ypos, Width, Height);
    }  
}
