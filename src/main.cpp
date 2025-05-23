// AtomS3: https://docs.m5stack.com/ja/core/AtomS3

// -----------------------------------------------------------------------------
#include <WiFi.h>
#include <vector>
#include <LittleFS.h>
#include <M5GFX.h>
#include <M5Unified.h>

#include "LGFX_AtomS3_SPI_ST7735S.hpp"
#include "FsSerial.hpp"
#include "M5AtomS3Face.hpp"

LGFX_AtomS3_SPI_ST7735S ExtDisplay; // インスタンスを作成

face::M5AtomS3Face FACE; // 顔のインスタンスを作成

static int CountMM = 0;
static int CountMMT = 0;

char ssid[] = "Your SSID";          // WiFiルーターに接続する際の SSID (name) を入力してください。
char pass[] = "Your SSID Password"; // WiFiルーターに接続する際の password を入力してください。

/*
// -----------------------------------------------------------------------------
void SaveFsData(String f, String str)
{
  LittleFS.begin();
  File file = LittleFS.open(f, "w");
  if (!file)
  {
    return;
  }
  file.write(reinterpret_cast<const uint8_t *>(str.c_str()), str.length());
  file.close();
  LittleFS.end();
}
// -----------------------------------------------------------------------------
void DeleteFsData(String f)
{
  LittleFS.begin();
  if (LittleFS.exists(f) == true)
  {
    LittleFS.remove(f);
  }
  LittleFS.end();
}
// -----------------------------------------------------------------------------
String OpenFsData(String f)
{
  LittleFS.begin();
  File file = LittleFS.open(f, "r");
  if (!file)
  {
    return "";
  }
  String str = file.readString();
  file.close();
  LittleFS.end();
  return str;
}
// -----------------------------------------------------------------------------
int split(String data, char delimiter, String *dst)
{
  int index = 0;
  int datalength = data.length();

  for (int i = 0; i < datalength; i++)
  {
    char tmp = data.charAt(i);
    if (tmp == delimiter)
    {
      index++;
    }
    else
      dst[index] += tmp;
  }

  return (index + 1);
}
*/
// -----------------------------------------------------------------------------
void ExtPrintln(String str)
{
  ExtDisplay.clear(TFT_BLACK);
  ExtDisplay.setCursor(0, 0);
  ExtDisplay.println(str);
}
// -----------------------------------------------------------------------------

char ntpServer[] = "ntp.nict.jp";
const long gmtOffset_sec = 9 * 3600;
const int daylightOffset_sec = 0;
struct tm timeinfo;
String second, minute, hour, dayOfWeek, dayOfMonth, month, year;

void getTimeFromNTP()
{
  // sntp_set_time_sync_notification_cb( timeavailable );
  //  NTPサーバと時刻を同期
  configTime(gmtOffset_sec, daylightOffset_sec, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  int retry = 20;
  while (!getLocalTime(&timeinfo))
  {
    Serial.print("!");
    delay(500);
    if (retry-- <= 0)
    {
      break;
    }
  }
}
bool getTime(String *year, String *month, String *day, String *hour, String *min, String *sec, String *wday)
{
  Serial.println("getTime000");
  char buf[10];
  bool ret = getLocalTime(&timeinfo);
  *year = String(timeinfo.tm_year + 1900);
  sprintf(buf, "%02d", timeinfo.tm_mon + 1);
  *month = String(buf);
  sprintf(buf, "%02d", timeinfo.tm_mday);
  *day = String(buf);
  sprintf(buf, "%02d", timeinfo.tm_hour);
  *hour = String(buf);
  sprintf(buf, "%02d", timeinfo.tm_min);
  *min = String(buf);
  sprintf(buf, "%02d", timeinfo.tm_sec);
  *sec = String(buf);
  strftime(buf, 10, "%A", &timeinfo);
  *wday = String(buf);
  Serial.println("getTime999");
  return true;
}

// -----------------------------------------------------------------------------
bool wifi_connect()
{
  bool ret = false;

  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi.disconnect(true, true);
    delay(500);
  }

  WiFi.begin(ssid, pass);
  int retry = 20;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("*");
    delay(500);
    if (retry-- <= 0)
    {
      break;
    }
  }
  ret = (WiFi.status() == WL_CONNECTED);

  if (ret)
  {
    ExtDisplay.println("WiFi connected");
    getTimeFromNTP();
    // WiFi.disconnect(true, true);
    CountMMT = millis() + 1000;
  }
  else
  {
    ExtDisplay.println("WiFi failed");
  }

  return ret;
}

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void GetSerialCMD()
{
  if (Serial.available() < 8)
    return;
  BrySerial srl;

  char header[4];
  for (int i = 0; i < 4; ++i)
  {
    header[i] = (char)Serial.read();
  }
  uint32_t rsize = 0;
  const size_t maxSize = 1024; // 最大100KB
  for (int i = 0; i < 4; ++i)
  {
    rsize |= Serial.read() << (i * 8);
  }

  int err = 0;
  int cnt = 0;
  bool aaa = false;
  std::vector<uint8_t> dataBuffer;
  if ((rsize > 0) && (rsize < 1024))
  {
    while (dataBuffer.size() < rsize)
    {
      if (Serial.available())
      {
        dataBuffer.push_back(Serial.read());
        cnt++;
      }
      else
      {
        err++;
        if (err > 1000)
        {
          err = -1;
          break;
        }
        delay(10);
      }
    }
  }
  if (srl.compHeader(header, (char *)"m5sk") == true)
  {
    char str[] = "M5Stack AtomS3";
    srl.SendBin("m5sk", (uint8_t *)str, strlen(str));
    ExtPrintln("Hello!");
    aaa = true;
  }

  else if (srl.compHeader(header, (char *)"text") == true)
  {

    ExtPrintln(String((char *)dataBuffer.data()));
    srl.SendText("return:" + String((char *)dataBuffer.data()));
    aaa = true;
  }
  else if (srl.compHeader(header, (char *)"gskn") == true)
  {
    byte head[8];
    if (srl.CreateHeader(head, "gskn", 4) == true)
    {
      int col = FACE.GetSkinColor();
      srl.SendBin("gskn", (uint8_t *)&col, 4);
    }
    aaa = true;
  }
  else if (srl.compHeader(header, (char *)"sskn") == true)
  {
    int sk = 0;
    for (int i = 0; i < 4; ++i)
    {
      sk |= dataBuffer[i] << (i * 8);
    }
    // SkinColor = sk;
    // DrawNormal();
    FACE.SetSkinColor(sk);
  }

  else
  {
    ExtPrintln("header:" + String(header) + " size:" + String(rsize) + " cnt:" + String(cnt));
  }
  if (aaa)
  {
    FACE.DrawBigEye();
    CountMM = millis() + random(100, 1000);
  }
}
// -----------------------------------------------------------------------------
void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  Serial.begin(115200);
  Serial.setTimeout(2000);

  ExtDisplay.init();             // 外部ディスプレイを初期化
  ExtDisplay.setBrightness(250); // バックライトの明るさ(0-255)

  ExtDisplay.clear(TFT_BLACK);
  ExtDisplay.setTextColor(TFT_WHITE);
  ExtDisplay.setTextSize(1);
  ExtDisplay.setFont(&fonts::lgfxJapanGothic_16);
  ExtDisplay.println("こんにちは、世界！");
  wifi_connect();
  FACE.Begin(&M5);

  FACE.DrawNormal();
  CountMM = millis() + random(500, 2000);

  // SaveFsData("/test.txt", "Hello, world!");
  // String str = OpenFsData("/test.txt");
  // ExtDisplay.println(str);
}
// -----------------------------------------------------------------------------
void loop()
{
  M5.update();
  long now = millis();
  // Serial.println(String(now));
  if (M5.BtnA.wasClicked())
  {
    Serial.println("Button A was clicked");
    FACE.DrawBigEye();
    CountMM = millis() + random(100, 1000);
  }
  else if (now > CountMM)
  {
    FACE.blink();
    CountMM = millis() + random(500, 3500);
  }

  // シリアル通信
  GetSerialCMD();

  // if ((WiFi.status() == WL_CONNECTED) && (now > CountMMT))
  if ((now > CountMMT))
  {
    if (getTime(&year, &month, &dayOfMonth, &hour, &minute, &second, &dayOfWeek))
    {
      ExtDisplay.startWrite();
      ExtDisplay.clear(TFT_BLACK);
      ExtDisplay.setCursor(0, 50);
      ExtDisplay.print(year + "/" + month + "/" + dayOfMonth);
      ExtDisplay.setCursor(0, 62);
      ExtDisplay.print(hour + ":" + minute + ":" + second);
      ExtDisplay.setCursor(0, 74);
      ExtDisplay.print(dayOfWeek);
      ExtDisplay.endWrite();
      CountMMT = millis() + 1000;
    }
  }
}
