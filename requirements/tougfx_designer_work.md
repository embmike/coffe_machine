## 4. Anleitung: Was du im TouchGFX Designer tun musst

Deine GUI-Spezifikation ist dafür die funktionale Grundlage: Splash → Auswahl → Brewing mit bildbasierter Animation statt echtem MPEG-Playback. 

### Schritt 1: Projekt anlegen

- Neues TouchGFX-Projekt für dein STM32-Board anlegen.
- Display auf **480x272** einstellen.
- Simulator mit erzeugen.

### Schritt 2: Assets ins Projekt bringen

Im Designer:

- **Images** öffnen
- alle JPEGs importieren
- die optionalen Steam-PNGs importieren
- prüfen, dass die Namen klar und konsistent sind

Empfohlene Asset-Gruppen:

- Splash
- Drinks
- Brewing
- Animation
- Overlay

### Schritt 3: Screens anlegen

Du brauchst drei Screens:

- `SplashScreen`
- `SelectionScreen`
- `BrewingScreen`

### Schritt 4: SplashScreen bauen

Elemente:

- `Image` für `splash_hintergrund_480x272.jpg`
- optional Logo als separates Bild oder Textarea
- optional leichter Fade-In

Verhalten:

- beim Start anzeigen
- nach ca. 2–3 Sekunden automatisch zu `SelectionScreen`

### Schritt 5: SelectionScreen bauen

Layout für 480x272:

Oben:

- Titel, z. B. „Kaffee wählen“

Mitte:

- vier DrinkCards in 2x2-Anordnung

Je DrinkCard:

- `Image` für Getränkebild
- `TextArea` für Getränkename
- `Button` oder `TouchArea` für Auswahl

Empfohlene Anordnung:

- links oben Espresso
- rechts oben Cappuccino
- links unten Latte
- rechts unten Americano

### Schritt 6: BrewingScreen bauen

Elemente:

- Hintergrundbild oder dunkler Screen-Hintergrund
- großes `Image` für `brewing_hero_tasse_220x160.jpg`
- `AnimatedImage` für `pouring_frame_01...10`
- optional zweites `AnimatedImage` oder `Image`-Wechsel für Steam-Overlay
- `TextArea` für Getränkename
- `TextArea` für Kaffeecharakter
- `TextArea` für Countdown `MM:SS`
- `ProgressBar` oder eigener Balken für Fortschritt

### Schritt 7: Pouring-Animation einrichten

Im Designer:

- `AnimatedImage` auf den Brewing-Screen ziehen
- die 10 Pouring-Frames in der richtigen Reihenfolge zuweisen
- Loop zunächst deaktivieren
- Start manuell über Code/Presenter anstoßen

Wichtig:

- alle Frames müssen **gleiche Größe und gleiche Komposition** haben
- sonst „zittert“ die Animation

### Schritt 8: Steam einrichten

Einfachste Variante:

- 4 Steam-PNGs
- zyklisch über einem festen Bereich oberhalb der Tasse wechseln

TouchGFX-seitig:

- entweder als `AnimatedImage`
- oder per Code zwischen Einzelbildern umschalten

### Schritt 9: Texte vorbereiten

Typische Textfelder:

- Splash: Markenname oder Demo-Titel
- Selection: Getränkenamen
- Brewing:
  - Getränk
  - Mood, z. B. „Cremig“
  - Countdown, z. B. `00:20`

### Schritt 10: Navigation definieren

Du brauchst mindestens diese Übergänge:

- Splash → Selection nach Timeout
- Selection → Brewing nach Getränkewahl
- Brewing → Selection nach Abschluss

### Schritt 11: Custom Containers

Für saubere Struktur würde ich im Designer mindestens einen Custom Container anlegen:

- `DrinkCard`

Inhalt:

- Bild
- Label
- Touch-/Click-Fläche

Dadurch wird die Selection-Seite später viel sauberer.

### Schritt 12: Code-Generierung

Wenn die Screens stehen:

- Code generieren
- danach die Logik **außerhalb** des generierten Codes ergänzen

Das passt auch zu deinen Coding Guidelines: klare Struktur, keine unnötige Vermischung, moderne C++-Typen, Doxygen, verständliche APIs. 

## 5. Meine klare Empfehlung für die praktische Umsetzung

Arbeite in genau dieser Reihenfolge:

1. zuerst alle Einzel-Assets sauber erzeugen
2. dann im TouchGFX Designer nur die Screen-Struktur aufbauen
3. dann Animation und Navigation testen
4. danach erst Codex an die App-Logik lassen

So verhinderst du, dass Codex gegen eine noch unfertige GUI-Struktur arbeiten muss.

Als nächsten Schritt erstelle ich dir am sinnvollsten direkt eine **deutsche Checkliste „TouchGFX Designer Schritt für Schritt“ als Markdown-Datei**, und danach leite ich daraus die `coding_task_for_codex.md` ab.