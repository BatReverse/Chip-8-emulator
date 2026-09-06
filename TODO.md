# TODO - CHIP-8

## 1. Périphériques (SDL2)
- [ ] Ajouter SDL2 au `CMakeLists.txt` (find_package + link)
- [ ] Créer une fenêtre SDL2 et une boucle principale (init/quit)
- [ ] Affichage : rendre `chip->gfx[64*32]` à l'écran (texture ou pixels scalés)
- [ ] Clavier : mapper les touches SDL vers `chip->keypad[16]` (mapping CHIP-8 standard)
- [ ] Timers : décrémenter `DT` et `ST` à 60Hz
- [ ] Son : jouer un bip tant que `ST > 0`
- [ ] Boucle d'émulation : intégrer fetch/decode + rendu + gestion des events SDL (quit, input)

## 2. Finir l'implémentation des instructions ASM
- [ ] `DRW Vx, Vy, nibble` (0xDXYN) — actuellement vide (`//todo` dans `Chip.c`)
- [ ] `SKP Vx` (0xEX9E) — skip si touche appuyée
- [ ] `SKNP Vx` (0xEXA1) — skip si touche relâchée
- [ ] `LD Vx, DT` (0xFX07)
- [ ] `LD Vx, K` (0xFX0A) — attente d'une touche
- [ ] `LD DT, Vx` (0xFX15)
- [ ] `LD ST, Vx` (0xFX18)
- [ ] `ADD I, Vx` (0xFX1E)
- [ ] `LD F, Vx` (0xFX29) — pointeur vers le sprite de fonte
- [ ] `LD B, Vx` (0xFX33) — BCD
- [ ] `LD [I], Vx` (0xFX55)
- [ ] `LD Vx, [I]` (0xFX65)
- [ ] Vérifier/compléter `LD Vx, Vy` avec shift (0x8XY6 / 0x8XYE) selon quirks souhaités
- [ ] Ajouter un jeu de sprites de fonte (0-F) chargé en mémoire au boot

## 3. Implémenter le fetch/decode complet
- [ ] Compléter le `switch` dans `fetch_decodeChip` pour tous les opcodes 0x3 à 0xF (actuellement seuls 0x0, 0x1, 0x2 sont traités)
- [ ] Ajouter les sous-switch nécessaires (ex: 0x8XY_, 0xEX__, 0xFX__ selon le dernier octet)
- [ ] Brancher toutes les fonctions déjà écrites (SE, SNE, LD, ADD, OR, AND, XOR, SUB, SHR, SUBN, SHL, LD_I, RND, DRW) dans le fetch/decode
- [ ] Gérer les opcodes inconnus (log/erreur)
