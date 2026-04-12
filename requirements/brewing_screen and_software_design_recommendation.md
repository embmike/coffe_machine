# Brewing-Screen und Software-Designempfehlung

Gern 😉 Hier ist die **aktualisierte Fassung** mit dem zusätzlichen Steam-Member `brsc_caffee_steam_animated_image` und den Regeln für Pouring + Steam.

## Designempfehlung

Ich würde es weiter so schneiden:

### 1. Domain

Reine fachliche Typen und Konstanten:

- `CoffeeType`
- `BrewingPhase`
- `CoffeeProfile`
- `BrewingSession`

### 2. Simulation / Service

Eine zentrale Klasse:

- `CoffeeMachineSimulation`

Sie berechnet:

- Restzeit
- Fortschritt
- Phase
- ob Pouring laufen soll
- ob Steam schwach oder stark sein soll

### 3. TouchGFX-Anbindung

Presenter/View:

- Presenter fragt die Simulation periodisch ab
- View zeigt nur an
- generierter Code bleibt unangetastet

## Wichtige Ergänzung für Steam

Steam würde ich **nicht** als eigene fachliche Hauptlogik modellieren, sondern als abgeleiteten Anzeigezustand.

Also nicht zu komplex, sondern so:

- `Start` → Steam schwach oder aus
- `Brewing` → Steam normal
- `Finishing` → Steam stärker
- `Done` → Steam noch kurz sichtbar oder stoppen

Dafür reicht ein zusätzlicher Anzeigezustand.

------

# Aktualisierte Vorgaben für Codex

## Ziel

Implementiere die Simulations- und Presenter-seitige Logik für den `brewing_screen` eines TouchGFX-Demonstrators.

Vorhandene GUI-Member:

- `brsc_caffee_steam_animated_image`
- `brsc_coffee_character_text`
- `brsc_caffee_name_text`
- `brsc_brewing_animated_image`
- `brsc_brewing_count_down`
- `brsc_brewing_progress_bar`
- `brsc_brewing_hero_image`
- `brsc_background_image`

Die Logik soll **außerhalb des generierten TouchGFX-Codes** implementiert werden.

------

## Fachliche Vorgaben

### Getränkeprofile

```cpp
Espresso   -> 25s -> "strong & intense"
Cappuccino -> 45s -> "creamy & smooth"
Latte      -> 65s -> "mild & silky"
Americano  -> 40s -> "clean & aromatic"
```

### Fortschrittsphasen

```text
0–20%   -> Start
20–80%  -> Brewing
80–100% -> Finishing
100%    -> Done
```

### Verhalten je Phase

#### Start

- kaum Bewegung
- Pouring-Animation noch nicht oder nur erstes Frame
- Steam aus oder sehr schwach

#### Brewing

- Pouring-Animation läuft
- Steam läuft normal zyklisch

#### Finishing

- Pouring-Animation stoppen oder letztes Frame halten
- Steam stärker bzw. weiter aktiv

#### Done

- Pouring aus
- Steam optional noch kurz sichtbar
- Fortschritt 100%

------

## Softwaredesign

### Typen

```cpp
enum class CoffeeType
{
    Espresso,
    Cappuccino,
    Latte,
    Americano
};

enum class BrewingPhase
{
    Idle,
    Start,
    Brewing,
    Finishing,
    Done
};

enum class SteamLevel
{
    Off,
    Low,
    Normal,
    Strong
};

struct CoffeeProfile
{
    CoffeeType   type;
    const char*  name;
    const char*  character;
    uint32_t     brew_time_ms;
};

struct BrewingSession
{
    CoffeeType    coffee_type;
    uint32_t      total_time_ms;
    uint32_t      elapsed_time_ms;
    uint32_t      remaining_time_ms;
    uint8_t       progress_percent;
    BrewingPhase  phase;
    SteamLevel    steam_level;
    bool          active;
};
```

------

## Service-Klasse

```cpp
class CoffeeMachineSimulation
{
public:
    void start(CoffeeType type);
    void stop();
    void reset();
    void update(uint32_t delta_ms);

    bool isActive() const;
    bool isFinished() const;

    CoffeeType getCoffeeType() const;
    const CoffeeProfile& getProfile() const;
    BrewingPhase getPhase() const;
    SteamLevel getSteamLevel() const;

    uint32_t getRemainingTimeMs() const;
    uint8_t getProgressPercent() const;
    const char* getCoffeeName() const;
    const char* getCoffeeCharacter() const;

    bool shouldStartPouringAnimation() const;
    bool shouldStopPouringAnimation() const;
    bool shouldShowSteamAnimation() const;

private:
    BrewingSession _session{};
    const CoffeeProfile* _active_profile{ nullptr };
};
```

------

## Logikregeln

### Start eines Getränks

`start(CoffeeType type)`:

- setzt Profil
- initialisiert Session
- `elapsed_time_ms = 0`
- `remaining_time_ms = total_time_ms`
- `progress_percent = 0`
- `phase = BrewingPhase::Start`
- `steam_level = SteamLevel::Low`
- `active = true`

### Update

`update(delta_ms)`:

- erhöht `elapsed_time_ms`
- berechnet `remaining_time_ms`
- berechnet `progress_percent`
- setzt `phase`
- setzt `steam_level`

### Phasenlogik

```cpp
0..19   -> Start
20..79  -> Brewing
80..99  -> Finishing
100     -> Done
```

### Steam-Regeln

```cpp
Start     -> Low
Brewing   -> Normal
Finishing -> Strong
Done      -> Low oder Off
Idle      -> Off
```

### Pouring-Regeln

```cpp
Start     -> optional aus oder erstes Frame
Brewing   -> Animation läuft
Finishing -> stoppen oder letztes Frame halten
Done      -> aus
```

------

## TouchGFX-Anbindung

## Presenter

Der Presenter soll:

- `simulation.start(type)` aufrufen
- periodisch `simulation.update(100)` aufrufen
- darstellungsnahe Werte an die View geben

Beispiel:

```cpp
void BrewingScreenPresenter::tick()
{
    _simulation.update(100);

    view.setCoffeeName(_simulation.getCoffeeName());
    view.setCoffeeCharacter(_simulation.getCoffeeCharacter());
    view.setProgress(_simulation.getProgressPercent());
    view.setCountdown(_simulation.getRemainingTimeMs());
    view.updateBrewingPhase(_simulation.getPhase(), _simulation.getSteamLevel());
}
```

------

## View-seitige Anforderungen

Die View soll Methoden bekommen wie:

```cpp
void setCoffeeName(const char* text);
void setCoffeeCharacter(const char* text);
void setProgress(uint8_t percent);
void setCountdown(uint32_t remaining_ms);
void updateBrewingPhase(BrewingPhase phase, SteamLevel steam_level);
```

------

## Vorgaben für `brsc_brewing_animated_image`

### Designer

- 10 Pouring-Frames in korrekter Reihenfolge
- gleiche Größe
- gleiche Komposition
- Loop im Designer deaktiviert
- Start/Stop per Code

### View-Verhalten

- `Start`: erstes Frame oder stop
- `Brewing`: `startAnimation(false, false, false)` oder passende TouchGFX-Methode für einmaliges/gezieltes Starten
- `Finishing`: stoppen oder letztes Frame halten
- `Done`: unsichtbar oder gestoppt

Codex soll die für deine TouchGFX-Version passende API nutzen.

------

## Vorgaben für `brsc_caffee_steam_animated_image`

### Designer

- 4 Steam-Frames
- gleiche Größe
- gleiche Position
- direkt oberhalb der Tasse
- zyklisch

### Einfache Variante

Steam als `AnimatedImage`:

- `Low`: langsam oder mit geringer Sichtbarkeit
- `Normal`: zyklisch normal
- `Strong`: schneller oder mit höherer Sichtbarkeit

### Alternative

Falls `AnimatedImage` unpraktisch ist:

- per Code zwischen 4 Einzelbildern umschalten

### Empfehlung

Für den Demonstrator:
**Steam als `AnimatedImage`** ist die einfachste und sauberste Lösung.

------

## UI-Verhalten pro Kaffee

### Espresso

- Name: `Espresso`
- Charakter: `strong & intense`
- Dauer: 25s

### Cappuccino

- Name: `Cappuccino`
- Charakter: `creamy & smooth`
- Dauer: 45s

### Latte

- Name: `Latte`
- Charakter: `mild & silky`
- Dauer: 65s

### Americano

- Name: `Americano`
- Charakter: `clean & aromatic`
- Dauer: 40s

------

## Countdown-Format

Codex soll eine Hilfsfunktion implementieren:

```cpp
void format_countdown(uint32_t remaining_ms, char* buffer, size_t buffer_size);
```

Verhalten:

- `25000 ms` -> `00:25`
- `9000 ms` -> `00:09`

------

## Minimaler technischer Ablauf

### Beim Eintritt in den Brewing-Screen

- Kaffeeprofil setzen
- Name und Charakter anzeigen
- Hero-Image setzen
- ProgressBar auf 0
- Countdown initialisieren
- Steam optional auf Low starten

### Während `tick()`

- Simulation updaten
- Countdown aktualisieren
- ProgressBar aktualisieren
- Phase auswerten
- Pouring/Steam entsprechend starten oder stoppen

### Bei `Done`

- ProgressBar = 100
- Countdown = 00:00
- Pouring stoppen
- Steam optional noch kurz sichtbar lassen
- danach Rückkehr zur Auswahl oder Done-Zustand

------

## Definition of Done

Die Aufgabe ist fertig, wenn:

- alle 4 Kaffeeprofile vorhanden sind
- `CoffeeMachineSimulation` implementiert ist
- Fortschritt und Countdown korrekt laufen
- Pouring nur in der Hauptphase läuft
- Steam phasenabhängig gesteuert wird
- keine fachliche Logik im generierten TouchGFX-Code steht
- `brewing_screen` seine Member korrekt aktualisiert

------

## Meine klare Designempfehlung

**Ja zu einer Klasse `CoffeeMachineSimulation`, aber nein zu einer riesigen Alles-in-einem-Klasse.**
Die Klasse soll nur die Simulationslogik kapseln. Texte, Anzeigeelemente und TouchGFX-spezifische Steuerung gehören in Presenter/View.

## Praktische Minimalstruktur

```text
/Application
  /Domain
    coffee_type.hpp
    brewing_phase.hpp
    steam_level.hpp
    coffee_profile.hpp
    brewing_session.hpp

  /Services
    coffee_machine_simulation.hpp
    coffee_machine_simulation.cpp
    countdown_formatter.hpp
    countdown_formatter.cpp

  /UiAdapter
    brewing_screen_presenter_adapter.hpp
    brewing_screen_presenter_adapter.cpp
```

Wenn du möchtest, formuliere ich dir daraus im nächsten Schritt direkt eine **fertige `coding_task_for_codex.md` auf Deutsch**.