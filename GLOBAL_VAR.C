 /* описание регистров в Xilinx*/
//
//
// регистры масок и флагов внешних прерываний

#include <bios.h>
//_____________________________________________________________________________
// Описание структуры Xilinx
//_____________________________________________________________________________
// Регистр управления (R/W)
xdata volatile uchar _RG_CTRL      _at_ 0x02100;
//
// #define _B_MODE_ADJ    1    // 1 - режим регулировок , 0 - нет режима
// #define _B_H0_S1_WDI   2    // 1 - вывод на WDI soft , 0 - xilinx
// #define _B_MODE_UVKV   4    // 1 - режим УВКВ вкл.   , 0 - выкл.
// #define _B_MODE_TAS    8    // 1 - режим TAS вкл.   , 0 - выкл.
// #define _B_CLK_WDI     16   // При _B_H0_S1_WDI=1,выдается частота на WDT
// #define _B_MODE_DUBL_TKN    64   // 1 - режим ДУБЛЬ ТКН вкл.   , 0 - выкл.
// #define _B_LED         128  // Светодиод
//
// Регистр  маски прерываний(разрешение) (R/W)
xdata volatile uchar _RG_MASK_INT _at_ 0x02101;
// #define _B_M_BV_TX     1               // Передача BV
// #define _B_M_BV_RX     2               // Прием BV
// #define _B_M_TAS_TX    4               // Передача TAS
// #define _B_M_TAS_RX    8               // Прием  TAS
// #define _B_M_ADJ       16              // Переключение режима ADJ
// #define _B_M_UVKV      32              // Переключение режима UVKV
// #define _B_M_TAS       64              //
// #define _B_M_NOP2      128             //
//
// Регистр флагов прерываний (R)
xdata volatile uchar _RG_FLAG_INT _at_ 0x02102;
// #define _B_F_BV_TX     1               // Передача BV
// #define _B_F_BV_RX     2               // Прием BV
// #define _B_F_TAS_TX    4               // Передача TAS
// #define _B_F_TAS_RX    8               // Прием  TAS
// #define _B_F_ADJ       16              // Переключение режима ADJ
// #define _B_F_UVKV      32              // Переключение режима UVKV
// #define _B_F_TAS       64              //
// #define _B_F_DUBL_TKN      128             //

//
// Регистр управления передачи в TAS (R/W)
// Старшая тетрада: количество передаваемых байт
xdata volatile uchar _RG_CTRL_TAS_TX _at_ 0x02103;
// #define _B_START_TAS_TX 1              // Старт передачи
//
// Регистр управления приемника от TASа  (R/W)
// Старшая тетрада: количество принимаемых байт без START и CRC
xdata volatile uchar _RG_CTRL_TAS_RX _at_ 0x02104;
// #define _B_RSTI_TAS_RX  1                       // Сброс прерывания
xdata volatile uchar _RG_CTRL_TAS_RX_START   _at_ 0x0210D;  // стартовый байт
//
// Регистр управления обменом с CU  (R/W)
xdata volatile uchar _RG_CTRL_BV  _at_ 0x02105;
// #define _B_START_BV_TX  1              // Старт передачи
// #define _B_RSTI_BV_TX   2              // Сброс готовности на передачу
// #define _B_RSTI_BV_RX   4              // Cброс прерывания на прием
// #define _B_BUSY_BV_TX   128            // 1 - линия передачи занята,0 - передача завершена
//
// Служебные регистры
xdata volatile uchar _RG_DAC_L     _at_ 0x02106;
xdata volatile uchar _RG_DAC_H     _at_ 0x02107;
// регистры для обмена с БВ (R/W)
xdata volatile uchar _RG_ERR       _at_ 0x02108; // регистр ошибок
// регистры пультов наводчика
xdata volatile uchar _RG_PU_GN_H   _at_ 0x02109;
xdata volatile uchar _RG_PU_GN_L   _at_ 0x0210A;
xdata volatile uchar _RG_PU_VN_H   _at_ 0x0210B;
xdata volatile uchar _RG_PU_VN_L   _at_ 0x0210C;
//
//
xdata volatile uchar _RG_VER_X   _at_ 0x0210E;
xdata volatile uchar _RG_VER_MU  _at_ 0x0210F;
//регистры пультов командира
xdata volatile uchar _RG_PU_CC_GN_H   _at_ 0x02200;
xdata volatile uchar _RG_PU_CC_GN_L   _at_ 0x02201;
xdata volatile uchar _RG_PU_CC_VN_H   _at_ 0x02202;
xdata volatile uchar _RG_PU_CC_VN_L   _at_ 0x02203;
//
//регистры наведения от ТАСа
xdata volatile uchar _RG_PU_TAS_GN_H   _at_ 0x02204;
xdata volatile uchar _RG_PU_TAS_GN_L   _at_ 0x02205;
xdata volatile uchar _RG_PU_TAS_VN_H   _at_ 0x02206;
xdata volatile uchar _RG_PU_TAS_VN_L   _at_ 0x02207;
//_____________________________________________________________________________
// RAM, распределенная в Xilinx
//_____________________________________________________________________________
xdata volatile uchar tas_tx[31]     _at_ 0x02000;    // Буфер передачи в TAS
xdata volatile uchar tas_rx[15]     _at_ 0x02020;    // Буфер приема от TASа
xdata volatile uchar bv_tx[15]      _at_ 0x02040;    // Буфер передачи в BV
xdata volatile uchar bv_rx[15]      _at_ 0x02060;    // Буфер приема от BV
//
xdata volatile uchar ram_reg[15]    _at_ 0x02080;    // Буфер от МУ
//
// ------------- Конец описания структуры Xilinx
//
// Ячейки для регулировок в EEPROM
xdata volatile uchar reg_eeprom[15] _at_ 0x00000;
// 0 мл. Рег. 0 ВН
// 1 ст.
// 2 мл. Рег. 0 ГН
// 3 ст.
// 4 мл. Рег. УВКВ ВН
// 5 ст.
// 6 мл. Рег. УВКВ ГН
// 7 ст.
// 8 мл. Рег. МАСШТАБИРОВАНИЕ
// 9 ст.
// 10 мл. Рег. ПС
// 11 ст.
// 12 мл. Рег. УВод ВН
// 13 ст.
// 14 мл. Рег. УВод ГН
// 15 ст.
//
int _reg_value=0; //  текущее значение регулировки
//
uchar num_reg=0,num_dac=0; // номер активной регулировки и ЦАПа
uchar com_bom=0; // полученная команда

// Переменные для обмена с БОМом
uchar ptr_rx_bom=0,ptr_tx_bom=0;
uchar cnt_rx_bom=0,cnt_tx_bom=0;
uchar sum_rx_bom=0;
uchar err_rx_bom=_C_OKEY;
uchar buf_rx_bom[_C_SIZE_RX_BOM];
uchar buf_tx_bom[_C_SIZE_TX_BOM];
uchar cnt_err_rx_bom=0;         // счетчик ошибок при связе с бомом
//
bit fl_busy_bom=0;   // флаг передачи в БОМ
//
bit fl_mode_adj=0;         // флаг режима регулировок   0- нет 1- режим
bit fl_mode_uvkv=0;        // флаг режима УВКВ          0- нет 1- режим
bit fl_mode_dubl_tkn=0;    // флаг режима Дубль-ТКН     0- нет 1- режим

bit fl_mode_tkn_change=0;
bit fl_mode_change_mode=0; // флаг смены режима
bit fl_start=0;      // флаг начала цикла
//---------------------------------------------
// Добавил для выверки по боковому срезу ствола
//---------------------------------------------
bit fl_uvkv_gn=0;         // 1- режим УВКВ по ГН
// Когда вкл УВКВ и выбрана регулировка 0 по ГН
//
/* время ожидания на получение информации о записи в EEPROM */
bit fl_ena_timer_eeprom=0;
bit fl_stop_timer_eeprom=0;
//
bit _B_GLOBAL_ERR_HARD=0;       /* невозможность работы ПР */
bit _B_ERR_BOM=0;               /* нет связи с БОМом */
bit _B_ERR_BV=0;                /* нет связи с БВ */
// обмен с TAS
//bit  fl_mode_tas=0;            // флаг режима TAS
bit  fl_new_tas_rx=0;         // 1-пришла информация от TAS


/* ___________________ Обмен с BV ________________________*/
uchar cnt_err_rx_bv=0;          // счетчик ошибок обмена с БВ
bit  fl_new_bv=0;               // 1-пришла информация от БВ
bit  fl_ena_timer_bv_rx=0;      // 1- работа таймера БВ
bit  fl_wait_bv_rx=0;           // 1- ожидание посылки от БВ
uchar count,i;
//
uchar count_tm1=0;

// состояние системы
enum_sost_syst   sost_sys=sys1;
// состояние обмена с БОМом
enum_sost_rx_bom  sost_rx_bom=s_waitbom;
//
// переменные для хранения наведения от ТАСа
// VN
uchar nav_tas_vn_h=0;
uchar nav_tas_vn_l=0;
// GN
uchar nav_tas_gn_h=0;
uchar nav_tas_gn_l=0;


