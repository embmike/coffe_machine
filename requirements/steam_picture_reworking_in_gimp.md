

# Dampfbilder in Gimp bearbeiten

------

## ✅ **SCHRITT 0 – Bild laden**

- GIMP öffnen
- Dein Dampf-Bild laden

👉 Wichtig:

- Hintergrund ist **schwarz**
- Dampf ist **hell**

------

## ✅ **SCHRITT 1 – Bild sichtbar machen (wichtig!)**

👉 Du hattest das Problem: „ich sehe nichts“

### Lösung:

**Ebene → Neue Ebene**

- Füllung: **weiß**
- OK

Dann:

👉 neue Ebene **unter das Bild ziehen**

🎯 Ergebnis:
→ Du siehst den Dampf wieder klar

------

## ✅ **SCHRITT 2 – Noise reduzieren (GANZ WICHTIG)**

👉 Menü:

**Filter → Weichzeichnen → Gaußscher Weichzeichner**

👉 Einstellungen:

- Radius: **2.0 – 3.0**

🎯 Ergebnis:

- Pixelmüll verschwindet
- Kanten werden weich

------

## ✅ **SCHRITT 3 – Kontrast optimieren**

👉 Menü:

**Farben → Kurven**

👉 Mach eine leichte S-Kurve:

- unten minimal runter (Schwarz stärker)
- oben leicht hoch (Dampf heller)

🎯 Ziel:

- Hintergrund = wirklich schwarz
- Dampf = klarer

------

## ✅ **SCHRITT 4 – Transparenz aktivieren**

👉 Menü:

**Ebene → Transparenz → Alphakanal hinzufügen**

------

## ✅ **SCHRITT 5 – MAGISCHER SCHRITT**

👉 Menü:

**Farben → Farbe nach Alpha**

👉 Farbe auswählen:

👉 **Schwarz (#000000)**

👉 OK

🎯 Ergebnis:

- Schwarz wird transparent
- Dampf bleibt erhalten

------

## ✅ **SCHRITT 6 – Kontrolle**

👉 Hintergrund ausblenden (weiße Ebene löschen)

Jetzt solltest du sehen:

- grau-weißes Schachbrett = transparent
- Dampf = weich sichtbar

------

## ✅ **SCHRITT 7 – Feinschliff (optional, aber gut)**

Wenn noch „krisselig“:

👉 Menü:

**Filter → Weichzeichnen → Gauß**

- Radius: **0.5 – 1.0**

------

## ✅ **SCHRITT 8 – Export**

👉 Datei → Exportieren als
👉 **PNG**

👉 Wichtig:

- „Transparenz speichern“ aktiv lassen

------

# 💡 WICHTIGE REGEL (merken!)

👉 IMMER:

```text
erst weichzeichnen + optimieren
DANN Alpha erzeugen
```

👉 NIE:

```text
erst Alpha → dann bearbeiten
```

------

# 🧠 Warum das jetzt funktioniert

Du hast vorher:

- Noise + harte Kanten gehabt
  → GIMP macht daraus kaputte Alpha

Jetzt:

- weiche Übergänge
  → perfekte Transparenz

------

# 🎯 Ergebnis

Du bekommst:

✔ weichen Dampf
✔ sauberen Alpha-Kanal
✔ keine Artefakte
✔ perfekt für TouchGFX

