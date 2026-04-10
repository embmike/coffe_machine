#ifndef ASSETS_H
#define ASSETS_H

#include <stddef.h>
#include <stdint.h>

enum class AssetId : uint8_t {
  IconEspresso80 = 0,
  IconCoffee80,
  IconCappuccino80,
  IconSettings32,
  IconBack32,
  IconCancel32,
  IconOk32,
  IconHeat48,
  IconBrew48,
  IconReady48,
  IconWarning48,
  IconWaterTank48,
  IconBeans48,
  IconDripTray48,
  IconService48,
  LogoCoffeeMachine240x80,
  IconMachine64,
};

struct AssetDescriptor {
  AssetId id;
  const char* filename;
  uint16_t width;
  uint16_t height;
  const char* purpose;
};

const AssetDescriptor* Assets_GetAll(size_t* count);
const AssetDescriptor* Assets_Find(AssetId id);

#endif
