#include <functional>
#include <Arduino.h>
#include <MiLightRadio.h>
#include <MiLightRadioFactory.h>
#include <MiLightButtons.h>
#include <MiLightRemoteConfig.h>
#include <Settings.h>

#ifndef _MILIGHTCLIENT_H
#define _MILIGHTCLIENT_H

//#define DEBUG_PRINTF

#define MILIGHT_DEFAULT_RESEND_COUNT 10
//Used to determine close to white
#define RGB_WHITE_BOUNDARY 40

class MiLightClient {
public:
  MiLightClient(MiLightRadioFactory* radioFactory);

  ~MiLightClient() {
    delete[] radios;
  }

  typedef std::function<void(uint8_t* packet, const MiLightRemoteConfig& config)> PacketSentHandler;

  void begin();
  void prepare(const MiLightRemoteConfig* remoteConfig, const uint16_t deviceId = -1, const uint8_t groupId = -1);
  void prepare(const MiLightRemoteType type, const uint16_t deviceId = -1, const uint8_t groupId = -1);

  void setResendCount(const unsigned int resendCount);
  bool available();
  size_t read(uint8_t packet[]);
  void write(uint8_t packet[]);

  void setHeld(bool held);

  // Common methods
  void updateStatus(MiLightStatus status);
  void updateStatus(MiLightStatus status, uint8_t groupId);
  void pair();
  void unpair();
  void command(uint8_t command, uint8_t arg);
  void updateMode(uint8_t mode);
  void nextMode();
  void previousMode();
  void modeSpeedDown();
  void modeSpeedUp();

  // RGBW methods
  void updateHue(const uint16_t hue);
  void updateBrightness(const uint8_t brightness);
  void updateColorWhite();
  void updateColorRaw(const uint8_t color);
  void enableNightMode();

  // CCT methods
  void updateTemperature(const uint8_t colorTemperature);
  void decreaseTemperature();
  void increaseTemperature();
  void increaseBrightness();
  void decreaseBrightness();

  void updateSaturation(const uint8_t saturation);

  void update(const JsonObject& object);
  void handleCommand(const String& command);
  void handleEffect(const String& effect);

  void onPacketSent(PacketSentHandler handler);

  size_t getNumRadios() const;
  MiLightRadio* switchRadio(size_t radioIx);
  MiLightRemoteConfig& currentRemoteConfig() const;

protected:

  MiLightRadio** radios;
  MiLightRadio* currentRadio;
  const MiLightRemoteConfig* currentRemote;
  const size_t numRadios;
  unsigned int resendCount;
  PacketSentHandler packetSentHandler;

  MiLightRadio* switchRadio(const MiLightRemoteConfig* remoteConfig);
  uint8_t parseStatus(const JsonObject& object);

  void flushPacket();
};

#endif
