void checksysdevices() {
  static const unsigned char image_micro_sd_bits[] U8X8_PROGMEM = {
    0xfc,0x03,0x06,0x04,0x02,0x04,0x02,0x04,0xe2,0x0c,0x12,0x09,
    0xf2,0x11,0x02,0x10,0x92,0x08,0x52,0x09,0x22,0x11,0x02,0x10,
    0xe2,0x11,0x1a,0x16,0xec,0x0d,0x00,0x00
  };
  static const unsigned char image_satellite_dish_bits[] U8X8_PROGMEM = {
    0x00,0x00,0x00,0x1e,0x0c,0x20,0x34,0x4c,0xc6,0x50,0x15,0x57,
    0x49,0x45,0x09,0x07,0x91,0x08,0x51,0x12,0x22,0x10,0xc2,0x24,
    0x04,0x23,0x0c,0x3c,0x30,0x08,0xc0,0x07
  };

  // حالات الأجهزة
  String radio1_state = "error";
  String radio2_state = "error";
  String sd_state     = "error";

  // ----- Init radio1 -----
  if (radio1.begin(&RADIO_SPI)) {
    radio1_state = "work";
    
    
  }

  // ----- Init radio2 -----
  if (radio2.begin(&RADIO_SPI)) {
    radio2_state = "work";
   
    
  }

  // ----- Init SD Card -----
  if (SD.begin(SD_CS, SD_SPI)) {
    sd_state = "work";
    setColor(0,255,0);
    
  }

  // ----- عرض النتائج -----
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);

  // أيقونات
  u8g2.drawXBMP(10, 20, 14, 16, image_micro_sd_bits);          // SD Icon
  u8g2.drawXBMP(47, 20, 15, 16, image_satellite_dish_bits);    // Radio1 Icon
  u8g2.drawXBMP(91, 19, 15, 16, image_satellite_dish_bits);    // Radio2 Icon

  // تسميات
  u8g2.setFont(u8g2_font_haxrcorp4089_tr);
  u8g2.drawStr(13, 10, "sd");
  u8g2.drawStr(43, 10, "radio1");
  u8g2.drawStr(87, 10, "radio2");

  // الحالات
  u8g2.setFont(u8g2_font_6x10_tr);  // وسط بين الصغر والوضوح

  u8g2.drawStr(5,  53, sd_state.c_str());
  u8g2.drawStr(42, 53, radio1_state.c_str());
  u8g2.drawStr(86, 53, radio2_state.c_str());

  u8g2.sendBuffer();
}




   





    

   



