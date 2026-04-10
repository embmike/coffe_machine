#include "assets.h"

namespace {
constexpr AssetDescriptor kAssets[] = {
  {AssetId::IconEspresso80, "icon_espresso_80.png", 80U, 80U, "Getraenkeauswahl Espresso"},
  {AssetId::IconCoffee80, "icon_coffee_80.png", 80U, 80U, "Getraenkeauswahl Kaffee"},
  {AssetId::IconCappuccino80, "icon_cappuccino_80.png", 80U, 80U, "Getraenkeauswahl Cappuccino"},
  {AssetId::IconSettings32, "icon_settings_32.png", 32U, 32U, "Navigation Einstellungen"},
  {AssetId::IconBack32, "icon_back_32.png", 32U, 32U, "Navigation Zurueck"},
  {AssetId::IconCancel32, "icon_cancel_32.png", 32U, 32U, "Aktion Abbrechen"},
  {AssetId::IconOk32, "icon_ok_32.png", 32U, 32U, "Bestaetigung"},
  {AssetId::IconHeat48, "icon_heat_48.png", 48U, 48U, "Status Heizen"},
  {AssetId::IconBrew48, "icon_brew_48.png", 48U, 48U, "Status Bruehen"},
  {AssetId::IconReady48, "icon_ready_48.png", 48U, 48U, "Status Bereit"},
  {AssetId::IconWarning48, "icon_warning_48.png", 48U, 48U, "Warnhinweis"},
  {AssetId::IconWaterTank48, "icon_water_tank_48.png", 48U, 48U, "Fehler Wasser"},
  {AssetId::IconBeans48, "icon_beans_48.png", 48U, 48U, "Fehler Bohnen"},
  {AssetId::IconDripTray48, "icon_drip_tray_48.png", 48U, 48U, "Fehler Schale"},
  {AssetId::IconService48, "icon_service_48.png", 48U, 48U, "Fehler Service"},
  {AssetId::LogoCoffeeMachine240x80, "logo_coffee_machine_240x80.png", 240U, 80U, "Logo/Splash"},
  {AssetId::IconMachine64, "icon_machine_64.png", 64U, 64U, "Platzhalter Kaffeemaschine"},
};
}

const AssetDescriptor* Assets_GetAll(size_t* count) {
  if (count != nullptr) {
    *count = sizeof(kAssets) / sizeof(kAssets[0]);
  }
  return kAssets;
}

const AssetDescriptor* Assets_Find(AssetId id) {
  for (const auto& asset : kAssets) {
    if (asset.id == id) {
      return &asset;
    }
  }
  return nullptr;
}
