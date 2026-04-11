# GUI Requirements – Premium Coffee Machine Demo (TouchGFX / STM32H7)

## 1. Ziel des Demonstrators

Ziel ist die Entwicklung einer visuell ansprechenden Embedded-GUI für eine Kaffeemaschine als Demonstrator.

Fokus:

- Hochwertige Optik („Premium Look“)
- Flüssige Animationen
- Klare Benutzerführung
- Emotionale Darstellung (z. B. „wie fühlt sich der Kaffee an“)

Kein Fokus:

- Komplexe Maschinenlogik
- Persistente Daten
- Vollständige Funktionalität

------

## 2. Zielhardware (Annahme)

- MCU: STM32H7 (z. B. STM32H750 / H7S7)
- Display: 4.3" TFT (480x272)
- Touch: resistiv oder kapazitiv
- SDRAM vorhanden
- QSPI-Flash für Assets

------

## 3. UX-Flow (Hauptablauf)

```
Splash Screen
   ↓
Coffee Selection Screen
   ↓
Brewing Screen (Animation + Countdown + Emotion)
   ↓
(optional zurück zur Auswahl)
```

------

## 4. Screens im Detail

### 4.1 Splash Screen

**Zweck:**

- Branding
- „Wow“-Effekt beim Start

**Inhalt:**

- Logo (zentriert)
- Hintergrund: dunkel / hochwertig
- MPEG-Animation:
  - sanft aufsteigender Dampf
  - Lichtreflexe

**Dauer:**

- 2–3 Sekunden
- automatischer Übergang

------

### 4.2 Coffee Selection Screen

**Zweck:**

- Auswahl des Getränks

**Layout:**

- Grid mit 4–6 Getränken

**Elemente:**

- DrinkCard (Custom Container)
  - Icon (z. B. Espresso, Cappuccino)
  - Name
  - optional kurze Beschreibung

**Interaktion:**

- Tap → Übergang zu Brewing Screen

**Design:**

- dunkler Hintergrund
- Karten mit leichter Erhöhung (Shadow)
- große Touch-Flächen

------

### 4.3 Brewing Screen (Kernstück)

**Zweck:**

- Emotionale Darstellung der Zubereitung

------

#### 4.3.1 Hauptkomponenten

**A) Tasse (zentral)**

- große Darstellung
- animierter Füllstand

**B) „Gefühl des Kaffees“ (wichtig für Demo)**

Zustände:

- „kräftig“
- „mild“
- „cremig“
- „intensiv“

Darstellung:

- Text + visuelle Unterstützung:
  - Farbe
  - Partikel
  - Bewegung

Beispiel:

- cremig → weich fließende Animation
- stark → dunkler, langsamer Fluss

------

**C) Countdown**

- Format: MM:SS
- große, gut lesbare Schrift
- läuft von z. B. 00:20 herunter

------

**D) Fortschritt**

- Kreis oder Balken
- synchron zur Zeit

------

#### 4.3.2 MPEG-Animation (zentraler Effekt)

**Inhalt:**

- Kaffee fließt in die Tasse
- Dampf steigt auf
- leichte Bewegung der Oberfläche

**Technische Umsetzung:**

Option A (empfohlen):

- Video → Einzelbilder (Frames)
- Speicherung im QSPI-Flash
- Darstellung als Frame-Sequenz

Option B:

- MJPEG-Stream

**Frame-Rate:**

- 15–25 FPS

------

#### 4.3.3 Zustandslogik (Demo)

```
START → „initial pouring“
MID   → „coffee feeling animation“
END   → „ready“
```

------

## 5. Design System

### 5.1 Farben

- Hintergrund: #0F1115 (dunkel)
- Primär: #C8A97E (Kaffee-Gold)
- Sekundär: #6F4E37 (Espresso-Braun)
- Akzent: #EBD5C3 (Milch-Creme)

------

### 5.2 Typografie

- Titel: groß, fett
- Standardtext: gut lesbar
- Countdown: extra groß

------

### 5.3 UI-Komponenten

**1. DrinkCard**

- wiederverwendbar
- Icon + Text

**2. ProgressIndicator**

- Kreis oder Balken

**3. EmotionLabel**

- zeigt „Gefühl des Kaffees“

**4. AnimatedCup**

- zentrale Komponente

------

## 6. Animationen

### 6.1 Grundprinzipien

- weich (ease-in/ease-out)
- keine harten Übergänge
- max. 300 ms für UI-Transitions

------

### 6.2 Wichtige Animationen

- Fade-in Splash
- Card Hover/Touch Feedback
- Cup Fill Animation
- Steam Animation

------

## 7. Assets (KI-generiert)

### 7.1 Kategorien

- Icons (Kaffeearten)
- Hintergrundbilder
- Tassen
- Partikel / Dampf
- Animation Frames (MPEG → Frames)

------

### 7.2 Anforderungen an KI-Assets

- konsistenter Stil
- gleiche Perspektive
- gleiche Lichtquelle
- keine komplexen Details (Performance!)

------

### 7.3 Technische Vorgaben

- Format:
  - PNG (Transparenz)
  - JPEG (Hintergrund)
- Auflösung:
  - exakt Displayauflösung oder kleiner
- Farbtiefe:
  - optimiert (TouchGFX Converter)

------

## 8. MPEG / Video Handling

### 8.1 Empfehlung

Da klassische Video-Decoding auf STM32 limitiert ist:

→ Konvertiere MPEG in:

- Frame-Sequenzen (PNG/JPEG)
- oder MJPEG

------

### 8.2 Pipeline

```
MPEG → FFmpeg → Frames → TouchGFX Assets → Animation
```

------

## 9. Architektur (TouchGFX)

Pattern:

- MVP (Model-View-Presenter)

------

### 9.1 Komponenten

- View: UI
- Presenter: Logik
- Model: Daten (Demo: Mock)

------

### 9.2 Beispielzustände

```cpp
enum struct MachineState
{
    Idle,
    Brewing,
    Finished
};
```

------

## 10. Demo-Logik

- Auswahl eines Getränks startet immer:
  - gleiche Dauer (z. B. 20 Sekunden)
- Emotion wird zufällig gewählt oder fest definiert
- keine echte Hardwaresteuerung

------

## 11. Performance-Ziele

- ≥ 30 FPS UI
- flüssige Animation
- keine Ruckler

------

## 12. Erweiterungen (optional)

- Sound (Kaffeemaschine)
- Touch-Feedback
- zusätzliche Getränke
- Einstellungen

------

## 13. Definition of Done

Die Demo ist fertig, wenn:

- Splash läuft flüssig
- Getränke auswählbar
- Brewing Screen zeigt:
  - Animation
  - Countdown
  - Emotion
- UI wirkt konsistent und hochwertig
- läuft stabil auf Target-Hardware

------

## 14. Besonderheit dieses Demonstrators

Der Fokus liegt nicht auf Funktion,
sondern auf:

→ Emotion
→ Ästhetik
→ Flow
→ „Premium Feeling“