# Definition

Es sollen Drohnen über das 2D Gitter (Karte) fliegen können
Es soll drei Stationen geben woraus jeweils mehrere Drohnen rausfliegen
Es gibt drei markierte Punkte auf der Karte
Ziel der Drohnen ist es, alle drei markierten Punkte zu finden und an die Stationen zurück zu kommunizieren
Es soll neben dem Fenster, wo man sieht wie die Drohnen rumfliegen, ein weiteres Fenster geben: Dort soll man sehen wie die aktuelle erkundete Karte aussieht und man soll die erkundete Karte einer einzelnen Drohne / Station sehen

Zukunft
Drohnen bekommen eine Lebensdauer (Akku)
Es soll verteilt Jammer geben, die Drohnen zerstören können
Hindernisse auf der Karte, die Drohnen ausweichen können (z.B. Höhen, die die Kommunikation beeinflussen)

# Ressourcen

raylib.com/cheatsheet/cheatsheet.html

natureofcode.com/autonomous-agents/

gameprogrammingpatterns.com/spatial-partition.html

en.algorithmica.org/hpc/

Talk: Mike Acton – „Data-Oriented Design and C++“ (CppCon 2014) (auf YouTube)
    Warum: Der wohl berühmteste Vortrag in der Spiele-/Simulationsentwicklung. Erklärt
    schonungslos, warum klassisches OOP (vector<Drone>) für die CPU langsam ist und wie man
    Daten in SoA (Struct of Arrays) statt AoS (Array of Structs) organisiert.

Buch (Online kostenlos): Data-Oriented Design https://www.dataorienteddesign.com/dodbook/ von
Richard Fabian
    Warum: Geht tief darauf ein, wie man Zustände (z. B. Drohne sucht vs. Drohne kehrt
    zurück) ohne if-Verzweigungen (Branch Mispredictions) im heißen Loop verarbeitet.


Paper: „What Every Programmer Should Know About Memory“ von Ulrich Drepper
    Warum: Der absolute Klassiker, wenn du wirklich verstehen willst, wie L1/L2/L3-Caches,
    Cache-Lines (64 Bytes) und Hardware-Prefetcher unter Linux funktionieren.

Video/Paper: Ten Minute Physics – Tutorial 11: Finding Neighbors (Spatial Hashing)
    https://matthias-research.github.io/pages/tenMinutePhysics/ von Matthias Müller
        Warum: In Game Programming Patterns wird Spatial Partitioning oft mit verketteten Listen
        (Pointer-Chasing = schlecht für den Cache!) erklärt. Matthias Müller (Physik-Engine-Legende
        bei NVIDIA) zeigt hier, wie man ein Spatial Grid mit Prefix Sums (Counting Sort) in einem
        einzigen flachen Array in O(N) komplett ohne Pointer und ohne Speicher-Allokationen baut.

Buch (Online kostenlos): Performance Analysis and Tuning on Modern CPUs https://easyperf.
net/perf_book/ von Denis Bakhvalov
    Warum: Perfekte Ergänzung zu Algorithmica. Zeigt dir, wie du unter Linux mit perf (perf
    ￹stat, perf record) und Tools wie Hotspot (GUI für perf) misst, ob deine Simulation gerade
    an Cache-Misses, Branch-Misses oder CPU-Zyklen hängt.

Referenz: Intel Intrinsics Guide https://www.intel.com/content/www/us/en/docs/intrinsics-
guide/index.html & Compiler Explorer (godbolt.org) https://godbolt.org/
    Warum: Unverzichtbar, wenn du die Kapitel aus Algorithmica anwendest, um zu prüfen, ob g++
    ￹-O3 -march=native deine Schleifen automatisch vektorisiert oder ob du mit Intrinsics
    nachhelfen willst.

Paper: „A Frontier-Based Approach for Autonomous Exploration“ von Brian Yamauchi (1997)
    Warum: Nur 4 Seiten lang, sehr leicht zu lesen und bildet bis heute die Grundlage dafür,
    wie autonome Roboter/Drohnen ein unbekanntes 2D-Gitter systematisch erkunden, ohne einfach
    nur im Zufallsmodus herumzuirren.

# Aktueller Plan

Phase 1: Bewegung, Stationen & Ziele (Dein Fokus JETZT)

Aktuell stehen deine Drohnen still. Dein nächster Meilenstein ist, dass 3 Stationen und 3 Ziele
auf der Karte existieren und die Drohnen aus den Stationen losfliegen.

Deine einzige Ressource für jetzt:
👉 Nature of Code – Autonomous Agents https://natureofcode.com/autonomous-agents/ (nur die
Grundlagen: Position, Geschwindigkeit velocity, Wander-Verhalten und das Abprallen/Umkehren am
Kartenrand).
Optional nebenbei beim Essen anschauen:
👉 Mike Acton (CppCon 2014) auf YouTube – nur für das Mindset, ob du in Zeile 26 ein struct
Drone (Array of Structs) oder lieber getrennte Arrays für posX, posY, velX, velY (Struct of
Arrays) nutzen willst.
Deine nächsten Mini-Schritte im Code:
    1. Positioniere 3 Stationen (z. B. blaue Quadrate) und 3 Zielpunkte (z. B. grüne Kreise).
    Lass die Drohnen beim Start an den Positionen der 3 Stationen spawnen statt zufällig auf
    der Karte.
    Gib jeder Drohne eine Geschwindigkeit (velX, velY, am besten als float statt int, damit
    sie in beliebige Winkel fliegen können) und addiere sie in jedem Frame auf posX / posY.
    
──────
Phase 2: Funkreichweite & Kommunikation (Sobald die Drohnen fliegen)

Wenn sich die Drohnen bewegen, sollen sie prüfen, ob andere Drohnen oder Stationen in
Funkreichweite sind, und eine Linie zwischen sich zeichnen.

Schritt 1 (Erstmal simpel bauen!): Schreib ganz bewusst zuerst die „langsame“ doppelte for-
Schleife (jede Drohne prüft den Abstand zu jeder anderen). Low-Level-Tipp: Nutze den
quadrierten Abstand (dxdx + dydy <= radius*radius), um die extrem teure Wurzelberechnung
(sqrt) der CPU zu sparen!
Schritt 2 (Wenn es bei tausenden Drohnen ruckelt):
👉 Jetzt erst liest du Game Programming Patterns – Spatial Partition
https://gameprogrammingpatterns.com/spatial-partition.html und schaust dir Ten Minute Physics
(Tutorial 11) an, um die Doppelschleife durch ein schnelles Such-Gitter zu ersetzen.
──────
Phase 3: Erkundete Karte & Zweites Panel (Sobald Kommunikation klappt)

Jetzt merkt sich jede Drohne, wo sie schon war, gibt dieses Wissen bei Funkkontakt weiter, und
du zeichnest daneben die erkundete Karte.

Deine Ressource für Phase 3:
👉 raylib Cheatsheet https://www.raylib.com/cheatsheet/cheatsheet.html (Abschnitt Textures:
GenImageColor, LoadTextureFromImage, UpdateTexture). Statt für jede erkundete Gitterzelle
einzeln DrawRectangle aufzurufen (was die GPU bei tausenden Zellen ausbremst), schreibst du die
erkundeten Zellen in ein Pixel-Array im RAM und schickst es einmal pro Frame per UpdateTexture
an die Grafikkarte.
──────
Phase 4: Das HPC-Labor (Ganz zum Schluss)

Erst wenn das alles funktioniert und die Simulation zu Ende geht, sobald alle 3 Ziele an die
Stationen gefunkt wurden, drehst du das Gitter und die Drohnenanzahl ans Limit.

Erst dann öffnest du Algorithmica HPC https://en.algorithmica.org/hpc/ (für Bitsets & SIMD
beim Karten-Austausch) sowie Drepper / Bakhvalov (für Cache-Profiling unter Linux).

Kurz gesagt: Leg alle Links beiseite bis auf das raylib Cheatsheet
https://www.raylib.com/cheatsheet/cheatsheet.html und Nature of Code
https://natureofcode.com/autonomous-agents/, und bring als Nächstes die 3 Stationen aufs Gitter
und die Drohnen in Bewegung!