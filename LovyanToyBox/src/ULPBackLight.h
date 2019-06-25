#ifndef _ULPBACKLIGHT_H_
#define _ULPBACKLIGHT_H_

#include <MenuCallBack.h>
#include <driver/rtc_io.h>
#include <esp32/ulp.h>

class ULPBackLight : public MenuCallBack
{
  void initULP(void)
  {
/*

●RTC_GPIO_OUT_REG & RTC_GPIO_IN_REG matrix
bit 14   RTC_GPIO  0   GPIO 36  A/D
bit 15   RTC_GPIO  1   GPIO 37  BtnC
bit 16   RTC_GPIO  2   GPIO 38  BtnB
bit 17   RTC_GPIO  3   GPIO 39  BtnA
bit 18   RTC_GPIO  4   GPIO 34
bit 19   RTC_GPIO  5   GPIO 35  A/D
bit 20   RTC_GPIO  6   GPIO 25  D/A Speaker
bit 21   RTC_GPIO  7   GPIO 26  D/A
bit 22   RTC_GPIO  8   GPIO 33  LCD RST
bit 23   RTC_GPIO  9   GPIO 32  LCD BackLight
bit 24   RTC_GPIO 10   GPIO  4  TFCARD CS
bit 25   RTC_GPIO 11   GPIO  0
bit 26   RTC_GPIO 12   GPIO  2
bit 27   RTC_GPIO 13   GPIO 15
bit 28   RTC_GPIO 14   GPIO 13
bit 29   RTC_GPIO 15   GPIO 12
bit 30   RTC_GPIO 16   GPIO 14  LCD CS
bit 31   RTC_GPIO 17   GPIO 27  LCD DC

ex.1
I_WR_REG(RTC_GPIO_OUT_REG, 23, 23, 1) // bit23 = 1
  GPIO32 = 1

ex.2
I_WR_REG(RTC_GPIO_OUT_REG, 25, 29, 5) // bit25~29 = 5 ( 0b00101 )
  GPIO 0 = 1
  GPIO 2 = 0
  GPIO15 = 1
  GPIO13 = 0
  GPIO12 = 0

ex.3
I_RD_REG(RTC_GPIO_IN_REG, 17, 17)
  R0 = bit17 (GPIO39  BtnA status)

*/
    const ulp_insn_t ulp_bl[] = {
    I_MOVI(R1, 5),      // R1 輝度調整用
  M_LABEL(1),  // label_1
    I_MOVI(R2, 0),      // R2 押下フラグ
  M_LABEL(2),  // label_2
    I_RD_REG(RTC_GPIO_IN_REG, 16, 16), // BtnB状態をR0に取得
    M_BGE(3, 1),        // ボタン押されてなければlabel_3ジャンプ
    I_END(),            // プログラムタイマ停止
    I_HALT(),           // ULPコプロセッサ停止
  M_LABEL(3),
    I_WR_REG(RTC_GPIO_OUT_REG, 23, 23, 1), // GPIO32にHighを出力(点灯)
    I_DELAY(500),       // 待つ
    I_WR_REG(RTC_GPIO_OUT_REG, 23, 23, 0), // GPIO32にLowを出力(消灯)
    I_MOVI(R0, 1),      // R0に１を代入
    I_LSHR(R0, R0, R1), // R0をR1の数だけ左シフトしてループ回数を生成
  M_LABEL(4),  // label_4
    I_DELAY(100),       // 待つ
    I_SUBI(R0, R0, 1),  // R0から1引く
    M_BGE(4, 1),        // R0が1以上ならlabel_4ジャンプ
    I_RD_REG(RTC_GPIO_IN_REG, 17, 17), // BtnA状態をR0に取得
    M_BGE(5, 1),        // ボタン押されてなければlabel_5ジャンプ
    I_MOVR(R0, R2),     // 押下フラグをR0にコピー
    M_BGE(2, 1),        // 押しっぱなしならlabel_2ジャンプ
    I_MOVI(R2, 1),      // 押下フラグ オン
    I_ADDI(R1, R1, 1),  // R1に1足す
    M_BX(2),            // label_2ジャンプ
  M_LABEL(5),  // label_5
    I_RD_REG(RTC_GPIO_IN_REG, 15, 15), // BtnC状態をR0に取得
    M_BGE(1, 1),        // ボタン押されてなければlabel_1ジャンプ
    I_MOVR(R0, R2),     // 押下フラグをR0にコピー
    M_BGE(2, 1),        // 押しっぱなしならlabel_2ジャンプ
    I_MOVI(R2, 1),      // 押下フラグ オン
    I_MOVR(R0, R1),     // 消灯ループ回数をR0にコピー
    M_BL(2, 1),         // 1未満ならlabel_2ジャンプ
    I_SUBI(R1, R1, 1),  // R1から1引く
    M_BX(2)             // label_2ジャンプ
    };
    const gpio_num_t lcd_gpios[] = {
      GPIO_NUM_32,
      GPIO_NUM_33
    };
    const gpio_num_t btn_gpios[] = {
      GPIO_NUM_37,
      GPIO_NUM_38,
      GPIO_NUM_39
    };
    ledcDetachPin(TFT_BL);
    for (size_t i = 0; i < sizeof(lcd_gpios) / sizeof(lcd_gpios[0]); ++i) {
      rtc_gpio_set_level(lcd_gpios[i], 1);
      rtc_gpio_set_direction(lcd_gpios[i], RTC_GPIO_MODE_OUTPUT_ONLY);
      rtc_gpio_init(lcd_gpios[i]);
    }
    for (size_t i = 0; i < sizeof(btn_gpios) / sizeof(btn_gpios[0]); ++i) {
      rtc_gpio_set_direction(btn_gpios[i], RTC_GPIO_MODE_INPUT_ONLY);
      rtc_gpio_init(btn_gpios[i]);
    }
    size_t size = sizeof(ulp_bl) / sizeof(ulp_insn_t);
    ulp_process_macros_and_load(0, ulp_bl, &size);
    ulp_run(0);
  }

public:
  bool setup()
  {
    initULP();

    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setFreeFont(&FreeSans9pt7b);
    M5.Lcd.setCursor(30,30);
    M5.Lcd.println("DeepSleeping now...\r\n");
    M5.Lcd.println("Backlight control with ULP\r\n");
    M5.Lcd.println("BtnA : Decrease brightness");
    M5.Lcd.println("BtnB : Reboot");
    M5.Lcd.println("BtnC : Increase brightness");

    M5.Power.setPowerBoostKeepOn(true);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_38, LOW);
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
    esp_deep_sleep_start();

    return false;
  }
  void close()
  {
  }
  bool loop()
  {
  }
};

#endif
