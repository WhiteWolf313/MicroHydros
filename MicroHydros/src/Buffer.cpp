#include "Buffer.h"

// --- Offline-buffert ---
static Matning ringen[BUFFER_KAPACITET];
static size_t  huvud = 0;      // nasta skrivposition
static size_t  antal = 0;      // antal lagrade varden
static bool    tappat = false; // har vi nagon gang skrivit over ett oskickat varde?

void bufferPush(const Matning& m) {
    ringen[huvud] = m;
    huvud = (huvud + 1) % BUFFER_KAPACITET;

    if (antal < BUFFER_KAPACITET) {
        antal++;
    } else {
        // Bufferten var full: det aldsta vardet skrevs over.
        // Vi valjer medvetet att kasta det ALDSTA vardet och behalla det nyaste,
        // eftersom farska varden ar viktigare for att bedoma odlingsmiljon nu.
        tappat = true;
    }
}

bool bufferPeek(Matning& m) {
    if (antal == 0) return false;
    size_t svans = (huvud + BUFFER_KAPACITET - antal) % BUFFER_KAPACITET;
    m = ringen[svans];
    return true;
}

void bufferPop() {
    if (antal > 0) antal--;
}

size_t bufferAntal()      { return antal; }
size_t bufferKapacitet()  { return BUFFER_KAPACITET; }
bool   bufferHarTappatVarden() { return tappat; }

// --- Historik for dashboarden ---
static Matning historiken[HISTORIK_LANGD];
static size_t  histHuvud = 0;
static size_t  histAntal = 0;

void historikPush(const Matning& m) {
    historiken[histHuvud] = m;
    histHuvud = (histHuvud + 1) % HISTORIK_LANGD;
    if (histAntal < HISTORIK_LANGD) histAntal++;
}

size_t historikAntal() { return histAntal; }

bool historikGet(size_t index, Matning& m) {
    if (index >= histAntal) return false;
    size_t start = (histHuvud + HISTORIK_LANGD - histAntal) % HISTORIK_LANGD;
    m = historiken[(start + index) % HISTORIK_LANGD];
    return true;
}
