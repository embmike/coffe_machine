# Kaffeedampf

Am besten **entkoppelt** vom Downcounter.

## Empfehlung

Für den **Brühprozess**:

- die 10 Bilder direkt am Fortschritt bzw. Downcounter ausrichten

Für den **Kaffeedampf**:

- die 4 Bilder **zyklisch als eigene kleine Animation** laufen lassen

## Warum?

Der Dampf ist optisch etwas anderes als der Brühstrahl:

- **Brühprozess** hat einen klaren fachlichen Bezug zur Zeit
- **Dampf** ist ein atmosphärischer Effekt, eher organisch und kontinuierlich

Wenn du den Dampf hart an den Downcounter koppelst, wirkt er schnell:

- mechanisch
- sprunghaft
- unnatürlich

## Gute Lösung für dein Demo

### 1. Brühbilder

Die 10 Pouring-Frames an den Fortschritt koppeln, zum Beispiel:

- 0–10% → Frame 1
- 10–20% → Frame 2
- ...
- 90–100% → Frame 10

oder etwas schöner:

- `frame_index = progress_percent / 10`

### 2. Dampfbilder

Die 4 Steam-Bilder unabhängig davon in einer kleinen Schleife wechseln lassen, z. B.:

- alle **150 bis 250 ms** nächstes Bild
- zyklisch:
  - 1 → 2 → 3 → 4 → 3 → 2 → 1 ...

Das wirkt meist natürlicher als:

- 1 → 2 → 3 → 4 → 1 → 2 ...

## Meine konkrete Empfehlung

Für deinen Demonstrator würde ich es so machen:

### Startphase 0–20%

- Steam entweder aus
- oder sehr langsam:
  - nur Bild 1 und 2
  - Wechsel alle 300–400 ms

### Hauptbrühphase 20–80%

- Steam normal:
  - 1 → 2 → 3 → 4 → 3 → 2
  - Wechsel alle 180–220 ms

### Endphase 80–100%

- Steam etwas stärker:
  - gleiche Sequenz
  - Wechsel etwas schneller, z. B. 120–160 ms
  - optional höhere Sichtbarkeit

So bekommt der Dampf Bezug zur Brühphase, ohne am Countdown zu kleben.

## Designprinzip

Also nicht:

```text
Downcounter -> Steam Frame direkt
```

sondern:

```text
Downcounter -> Brühphase
Brühphase -> Steam-Geschwindigkeit / Sichtbarkeit
eigener Steam-Timer -> aktuelles Steam-Bild
```

## Softwaredesign dazu

Ich würde in der Simulationslogik zwei Dinge trennen:

### A. Brewing progress

- fachlich
- bestimmt Pouring-Frame

### B. Steam animation state

- visuell
- bestimmt Steam-Frame

Zum Beispiel:

```cpp
struct SteamAnimationState
{
    uint8_t current_frame;
    uint32_t elapsed_ms_since_frame_change;
    uint32_t frame_interval_ms;
    bool active;
};
```

Dann in `update(delta_ms)`:

- Brühphase berechnen
- daraus `frame_interval_ms` und `active` setzen
- Steam-Timer weiterlaufen lassen
- bei Ablauf auf nächstes Dampfbild schalten

## Noch besser als einfache Schleife

Für natürlichen Eindruck:

```cpp
1, 2, 3, 4, 3, 2
```

statt

```cpp
1, 2, 3, 4
```

Das vermeidet einen harten Sprung von 4 zurück auf 1.

## Mein Fazit

**Der Dampf sollte nicht direkt am Downcounter hängen, sondern über die Brühphase beeinflusst und über einen eigenen kleinen Animationstimer gesteuert werden.**

Das ist für deinen Demonstrator:

- einfach genug
- gut wartbar
- visuell deutlich schöner

