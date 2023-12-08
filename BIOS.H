/******************************************************************************/
/****************** Переменные,константы,определения,переменные, function i/o */
/******************************************************************************/
#ifndef         __BIOS_H
#define __BIOS_H

#include <AT89S8253.h>
//
#define true    1
#define false   0
// Биты  управляющих регистров Xilinx
// _RG_CTRL
//
#define _B_MODE_ADJ    1    // 1 - режим регулировок , 0 - нет режима
#define _B_H0_S1_WDI   2    // 1 - вывод на WDI soft , 0 - xilinx
#define _B_MODE_UVKV   4    // 1 - режим УВКВ вкл.   , 0 - выкл.
#define _B_MODE_TAS    8    //
#define _B_CLK_WDI     16   // При _B_H0_S1_WDI=1,выдается частота на WDT
#define _B_DC_BV       32   // Сигнал DC из БВ 1- DC 0- нет DC
#define _B_MODE_DUBL_TKN    64   // 1 - режим ДУБЛЬ ТКН вкл.   , 0 - выкл.
#define _B_LED         128  // Светодиод
//
// Регистр  маски прерываний(разрешение) (R/W)
// _RG_MASK_INT
//#define _B_M_BV_TX     1               // Передача BV
#define _B_M_BV_RX     2               // Прием BV
#define _B_M_TAS_TX    4               // Передача TAS
#define _B_M_TAS_RX    8               // Прием  TAS
#define _B_M_ADJ       16              // Переключение режима ADJ
#define _B_M_UVKV      32              // Переключение режима UVKV
#define _B_M_TAS       64              // Переключение режима TAS
#define _B_M_DUBL_TKN  128             //             //
//
//
// Регистр флагов прерываний (R)
//_RG_FLAG_INT
//
//#define _B_F_BV_TX     1               // Передача BV
#define _B_F_BV_RX     2               // Прием BV
#define _B_F_TAS_TX    4               // Передача TAS
#define _B_F_TAS_RX    8               // Прием  TAS
#define _B_F_ADJ       16              // Переключение режима ADJ
#define _B_F_UVKV      32              // Переключение режима UVKV
#define _B_F_TAS       64              //
#define _B_F_DUBL_TKN  128             //

#define _B_F_MODE      32             //
// Регистр управления передачи в TAS (R/W)
// Старшая тетрада: количество передаваемых байт
// _RG_CTRL_TAS_TX
#define _B_START_TAS_TX 1              // Старт передачи
#define _B_RSTI_TAS_TX  2              // Сброс прерывания
// Регистр управления приемника от TASа  (R/W)
// Старшая тетрада: количество принимаемых байт без START и CRC
// _RG_CTRL_TAS_RX
#define _B_RSTI_TAS_RX  1              // Сброс прерывания
//
// Регистр управления обменом с CU  (R/W)
// _RG_CTRL_BV
#define _B_START_BV_TX  1              // Старт передачи
#define _B_RSTI_BV_TX   2              // Сброс готовности на передачу
#define _B_RSTI_BV_RX   4              // Cброс прерывания на прием
#define _B_BUSY_BV_TX   128            // 1 - линия передачи занята,0 - передача завершена
//
//
typedef unsigned char uchar;
typedef unsigned int  uint;
// состояние системы
typedef
enum  {sys0,sys1,sys2,sys3,sys4,sys5,sys6} enum_sost_syst;
// состояние обмена с БОМом
typedef
enum  {s_waitbom,s_receiptbom,s_endbom,s_faultbom} enum_sost_rx_bom;

// sys0 - до получения старта цикла
// sys1 - старт получен ->  посылка в БВ блока
// sys2 - ожидание ответа БВ
// sys3 - блок от БВ получен
// sys4 - критическая ошибка обмена с БВ
// sys5 - режим регулировок
// sys6 - режим УВКВ
//
//
//
//
#define _C_SPEED_UARTL  0xFA // 115200
#define _C_SPEED_UARTH  0xFF

/*____общие константы ___________________*/
 #define _C_OKEY         0
//
//
/*___Система тестирования ______________*/
//
#define _C_CNT_FAULT    10      /* количество запросов до принятия решения об ошибке */
/*____Система таймеров __________________*/
#define _C_1000MKS              -1844
#define _C_20MS                 -36864
//
#define _C_BV_RX_S              0x00    // признак второй посылки из БВ
#define _C_BV_RX_F              0xFF   // признак первой пасылки из БВ
// ___________________ Обмен с TAS
#define _C_SIZE_TX_TAS            12  // количество байт в посылке БУ->TAS -1
#define _C_SIZE_RX_TAS            4   // количество байт в посылке TAS->БУ
#define _C_START_TX_TAS           0xAA // стартовый байт в посылке БУ->TAS
#define _C_START_RX_TAS           0x55 // стартовый байт в посылке TAS->БУ
/* ___________________ Обмен с BOM  электронные регулировки ________________________*/
//
#define _C_SIZE_RX_BOM           2  // количество байт в посылке БОМ->БУ
#define _C_SIZE_TX_BOM           4  // количество байт в посылке БУ->БОМ
#define _C_START_BOM           0xA5 // стартовый байт в посылке из БОМа
#define _C_START_CU_BOM        0x5A // стартовый байт в посылке из БУ
//
#define _C_SAVE_AN_L           0xFA // байты для ответа на команду ЗАПИСЬ
#define _C_SAVE_AN_H           0xAF // при её успешном завершении
//
#define _C_COM_READ             16
#define _C_COM_WRITE            32
#define _C_COM_PLUS             128
#define _C_COM_MINUS            64
//
#define _C_LIMIT_MIN            -2048
#define _C_LIMIT_MAX            2047
//
// ------------------ Добавил для УВКВ в Индии ------------
//
//Необходимо поставить нужные значения !!!!!!!!!!!!!!!!!!!!!
//
#define _C_UVKV_VN_PLUS         100 //
#define _C_UVKV_GN_PLUS         100 //
//
/*маски для анализа регистра ошибок*/
//
//добавил по просьбе Паши из Тагила 14.12.04
#define _C_UVKV                 0x40
#define _C_ADJ                  0x20
//добавил по просьбе Андрея из Тагила 2.12.05
#define _C_TAS                  0x80
// добавил в Минске 5.06.09
#define _C_D_TKN                0x10
//
#define _B_ERR1       16         // НЕТ ФАЗ
#define _B_ERR2       1          // НЕТ ТОКА ФАЗ
#define _B_ERR3       8          // ГОТОВНОСТЬ МГ
#define _B_ERR4       4          // ПРОСАДКА +27В
#define _B_ERR5       2          // ОТКАЗ АРРЕТИРА
#define _B_ERR6       32         // НЕТ СОГЛАСОВАНИЯ ЗЕРКАЛА
#define _B_OK         64         // MSS OK
//
// #define _B_F_NOP2      128             //
// ---------------------------------------------------------
void Rst_System(void);
void Begin_init(void);             /* Начальная инициализация системы */
bit  Write_bom(void);              /* Передача блока в BOM */
void Delay_1ms(uchar Par); /* Задержка 1 ms */
int Read_eep(uchar n_r);               /* Чтение EEPROM */
bit Write_eep(uchar n_r, int r_v);     /* Запись EEPROM */
 /* Запись ЦАПа */
void Write_dac(uchar num, int value);
char Analiz_bom(void);             /* Анализ получения информации от BOM */
void Change_num(uchar Par, uchar *num_r,uchar *num_d);  /* Перевод номера ЦАПа и Регулировки */
int Tran_pr_dop(uchar vol1,vol2,maska,maska_sg);/* Перевод в дополнительный код */
// маска определяет полож. знака в старшем байте
//_____ Последний в bios_.h
#endif // __BIOS_H




