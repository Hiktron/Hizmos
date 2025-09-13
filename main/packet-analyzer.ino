// إعدادات الجراف
#define GRAPH_WIDTH     128
#define GRAPH_HEIGHT    44
#define GRAPH_TOP       10
#define MAX_POINTS      128
#define SPIKE_THRESHOLD 30

// تعريف struct قبل استخدامه
struct SnifferGraph {
  uint8_t graphData[MAX_POINTS];
  uint8_t currentChannel = 1;
  volatile uint16_t packetCounter = 0;
  unsigned long lastChannelSwitch = 0;
  unsigned long lastUpdate = 0;
};

// تعريف كائن الحالة
SnifferGraph sniffer;

// رد نداء للباكتات المستلمة
void IRAM_ATTR snifferCallback(void *buf, wifi_promiscuous_pkt_type_t type) {
  if (type == WIFI_PKT_MGMT || type == WIFI_PKT_DATA || type == WIFI_PKT_CTRL) {
    sniffer.packetCounter++;
  }
}

void initDisplay() {
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_5x8_tr);
  u8g2.drawStr(0, 10, "Starting analyzer...");
  u8g2.sendBuffer();
}

void initSniffer(struct SnifferGraph &g) {
  WiFi.disconnect(true, true);
  esp_wifi_stop();
  delay(200);
  esp_wifi_deinit();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();

  esp_wifi_set_channel(g.currentChannel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous_rx_cb(snifferCallback);
  esp_wifi_set_promiscuous(true);
}

void switchChannel(struct SnifferGraph &g) {
  g.currentChannel++;
  if (g.currentChannel > 13) g.currentChannel = 1;
  esp_wifi_set_channel(g.currentChannel, WIFI_SECOND_CHAN_NONE);
}

void updateGraphData(struct SnifferGraph &g, uint8_t value) {
  for (int i = 0; i < MAX_POINTS - 1; i++) {
    g.graphData[i] = g.graphData[i + 1];
  }
  g.graphData[MAX_POINTS - 1] = value;
}

void drawGraph(struct SnifferGraph &g, uint16_t pktCount) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_5x8_tr);

  char line1[16];
  char line2[16];
  sprintf(line1, "Ch: %d", g.currentChannel);
  sprintf(line2, "Pkts/s: %d", pktCount * 5);

  u8g2.drawStr(0, 8, line1);
  u8g2.drawStr(60, 8, line2);

  for (int x = 1; x < GRAPH_WIDTH; x++) {
    int y1 = GRAPH_TOP + GRAPH_HEIGHT - g.graphData[x - 1];
    int y2 = GRAPH_TOP + GRAPH_HEIGHT - g.graphData[x];
    u8g2.drawLine(x - 1, y1, x, y2);
  }

  if (pktCount >= SPIKE_THRESHOLD) {
    u8g2.drawVLine(GRAPH_WIDTH / 2, GRAPH_TOP, GRAPH_HEIGHT);
  }

  u8g2.sendBuffer();
}

void setupSnifferGraph() {
  initDisplay();
  initSniffer(sniffer);
}

void updateSnifferGraph() {
  unsigned long now = millis();

  if (now - sniffer.lastChannelSwitch >= 1000) {
    sniffer.lastChannelSwitch = now;
    switchChannel(sniffer);
  }

  if (now - sniffer.lastUpdate >= 200) {
    sniffer.lastUpdate = now;

    uint16_t pktCount = sniffer.packetCounter;
    uint8_t scaled = pktCount * 2;
    uint8_t value = min(scaled, (uint8_t)GRAPH_HEIGHT);

    updateGraphData(sniffer, value);
    drawGraph(sniffer, pktCount);

    sniffer.packetCounter = 0;
  }
}
