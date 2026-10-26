# Contra III: The Alien Wars (USA) — Complete 32-Bank ROM Map & REA Provenance

- **Artifact**: `Contra III - The Alien Wars (USA).sfc` (`1,048,576` bytes, LoROM)
- **Full ROM SHA-256**: `a93ea87fc835c530b5135c5294433d15eef6dbf656144b387e89ac19cf864996`
- **Ghidra Provider**: `Ghidra 12.1.4` (`65816:LE:24:snes`)
- **Segment Inventory Evidence ID (`list_segments`)**: `ev_c4c73e28ddf02563ceeaee8ca06e0284fae013274551f3e9ce2fa2d5369f2fe0`
- **Total Translated Functions**: **2288** across **6** executable code banks
- **Total Unique Decoded Instructions**: **57007** (`0` mid-instruction overlaps)

---

## Bank-by-Bank Summary (`$00` – `$1F`)

| Bank | LoROM Bus Span | Ghidra Base | Kind | Functions | Instructions | Indirects | Output File | REA `read_bytes` Evidence ID | Bank SHA-256 (first 16) |
| :---: | :---: | :---: | :--- | :---: | :---: | :---: | :--- | :--- | :--- |
| `$00` | `$00:8000–FFFF` | `0x8000` | Code + Vectors | 514 | 13007 | 41 | `src/bank_00.c` | `ev_3ff4b25a4e8b3428d08ddd4914a71d771f3bf1230ae0b585d85bbc59caabf7ee` | `74e83bbc3629c01a…` |
| `$01` | `$01:8000–FFFF` | `0x18000` | Executable Code | 484 | 12457 | 42 | `src/bank_01.c` | `ev_77631fe86ccd9423a19ca4cc77c3e2f7d56008cb9ae5727f1fe471783110a903` | `2761a761c9cc16d4…` |
| `$02` | `$02:8000–FFFF` | `0x28000` | Executable Code | 620 | 13757 | 62 | `src/bank_02.c` | `ev_cc61f21c9d6a0347d8ac54c4c67b42619aaebda82fc328213f369d63e99668b2` | `4e8fc9a2814b190f…` |
| `$03` | `$03:8000–FFFF` | `0x38000` | Data + Code | 62 | 3625 | 1 | `src/bank_03.c` | `ev_4019bb2c8b38ff7627ae693be7a1984fc58b897e7845b98b11fb106c6662c3c6` | `5a279c39552b0db2…` |
| `$04` | `$04:8000–FFFF` | `0x48000` | Executable Code | 453 | 10542 | 37 | `src/bank_04.c` | `ev_0bf80c3aa2340cf6eb52429464052a6432760e6913a6b574f19eda6c369c5d04` | `afa06df844004d8f…` |
| `$05` | `$05:8000–FFFF` | `0x58000` | Master Data (`DB=$05`) | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_df5d33da05c0af510034256c365462954b5468ae99f63a6cf915ef86cf393ded` | `14d69166dd9fed0f…` |
| `$06` | `$06:8000–FFFF` | `0x68000` | Stage Data (`DB=$06`) | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_b447225bbd5bda11a56aac8c5be105b9782cc4a5afb2b7122fd77abf5e941c6b` | `e86626f4fdc182eb…` |
| `$07` | `$07:8000–FFFF` | `0x78000` | Mode 7 Data (`DB=$07`) | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_f2fd4d83c752fb4ca562bb08d8220e6572e82733dbe445a23db38a54e8257f64` | `b8a0ef8dfe6b41e5…` |
| `$08` | `$08:8000–FFFF` | `0x88000` | Palette Data (`DB=$08`) | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_086c0bd4c943f7b241069abbfe397b247de7d5dd7e0b6a67dcdf90590a37a459` | `bd58253a82bb5fc7…` |
| `$09` | `$09:8000–FFFF` | `0x98000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_fe4c7313328f36060676c88fc48ff7843f3f8eb9e4c2ce281d15c8e57eb8b023` | `82c0bddad05ebe44…` |
| `$0A` | `$0A:8000–FFFF` | `0xa8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_d7898a8023ac117ed5d7a0d1e29f47e95e85e9eb15517a46603ec4579b0a8975` | `b4dca37f6b61f470…` |
| `$0B` | `$0B:8000–FFFF` | `0xb8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_10bfce9cd846cac8207542e8466cce2f38fb9120300c0995b170ffec694fde42` | `2c1df67f2525015c…` |
| `$0C` | `$0C:8000–FFFF` | `0xc8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_4e0b31ff468b93cb67185ed37b5bca05efc01bb34d5e28ea2d167db2e78078de` | `743281332e6bbe55…` |
| `$0D` | `$0D:8000–FFFF` | `0xd8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_7fe3e39b9abd3f359f4e2cb647803afefd9d7b40658198de0802e9193af13190` | `d51b8bacd010ffee…` |
| `$0E` | `$0E:8000–FFFF` | `0xe8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_f3876cec532c0a5196b233454cbb863f3959ebb2f6cc203670ac22d070c92484` | `01fc097d4f0686c3…` |
| `$0F` | `$0F:8000–FFFF` | `0xf8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_fd472a34924cf3d257aefb0b590dd4ded0c5eccd6d75e073f29a1075e2a4bbb5` | `f366bf06355e2633…` |
| `$10` | `$10:8000–FFFF` | `0x108000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_e7cd8755d44b0e4a9223655b180192407e710682b7211fff4a63874ffd1b7be4` | `697cb953dbce6ed3…` |
| `$11` | `$11:8000–FFFF` | `0x118000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_66ed0444dc62bcd9ca270658fc15fe77cdf3456259ab47392006e2b93e4a949e` | `66aaf6ecc18ccfb1…` |
| `$12` | `$12:8000–FFFF` | `0x128000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_9fde2d897ed409445e3fcc0c4060cf4fa403fc9f5741207d8c4cfbd5d6474ebd` | `c2e9b3c90f6a7163…` |
| `$13` | `$13:8000–FFFF` | `0x138000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_f55163fc25d7db37b17ae38c22101d9e2041c3f833721058dc6a90e2c96ca414` | `11a32f1d48957dbb…` |
| `$14` | `$14:8000–FFFF` | `0x148000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_f36700220fb394a93ddeffb3e2389e0aef7d4657d6b91e0ae441c5bee16529fe` | `2bb2b8c7ced17dfd…` |
| `$15` | `$15:8000–FFFF` | `0x158000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_3a40493d3e5adc77a63f0d067ffc89a7b851041a88244395e331d135d88d8081` | `421a4ee8b062a1a7…` |
| `$16` | `$16:8000–FFFF` | `0x168000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_60a3a6058486ca66ac6ccddac78cb7857ee90acacb46b7b53711272b4095d4f9` | `8627b73ca451b3af…` |
| `$17` | `$17:8000–FFFF` | `0x178000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_8323da99427c88982e38dbf963cf56e816a64629a410b2cabf8027e67ed2d159` | `93c3b1cbf1d0fb00…` |
| `$18` | `$18:8000–FFFF` | `0x188000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_47f437c74d399eea0fb5c62ea70620bfb06fa372e0b923f2c11ca569fb048aaf` | `af78e3e4fa0c435c…` |
| `$19` | `$19:8000–FFFF` | `0x198000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_f754816418709cc589b17247258ab595e936c100a5659f9e329e4cd05f2bc8f7` | `56e9bc0c31588127…` |
| `$1A` | `$1A:8000–FFFF` | `0x1a8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_963838b2d8de594bc829c8f208706ee0d768d36679ee8f0a0cba36645d73b1c3` | `97fe9533fc3197d6…` |
| `$1B` | `$1B:8000–FFFF` | `0x1b8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_8831df84aae84f6e38b1fd94ff41afe60eab4e4af29ab2dd7c5e3d87217151d7` | `1a8742d423cf93eb…` |
| `$1C` | `$1C:8000–FFFF` | `0x1c8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_09a5bad1f0f1ec1ce540271a4203f9fe41b167a83454028ddb0d56b99da1ed35` | `58999d56816f82a2…` |
| `$1D` | `$1D:8000–FFFF` | `0x1d8000` | Data + Code | 155 | 3619 | 7 | `src/bank_1d.c` | `ev_2bc300b36129b7289068f779d03db2a8dccd10d3a4e2792edefb3f492f074d85` | `cdf795a1cf5bcc8f…` |
| `$1E` | `$1E:8000–FFFF` | `0x1e8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_ade9c0c685f2649c2369a5d0b6c5b5605184566503f1101246fc53769fde99fe` | `dbf26f87349549ec…` |
| `$1F` | `$1F:8000–FFFF` | `0x1f8000` | LZSS / Audio Assets | 0 | 0 | 0 | `src/bank_data_manifest.c` | `ev_5181e2fcd60bc02e3a64e18209d730faabd4e16cd4689a9b8d0cfbab51b85e6a` | `60bf4c51c642cdc2…` |

---

## Detailed Bank Roles

- **Bank `$00`** (Code + Vectors -> `src/bank_00.c`): Core engine, boot (`$8000`), NMI (`$819B`), IRQ (`$8BE2`), LZSS (`$94A9`), SPC700, dispatchers.
- **Bank `$01`** (Executable Code -> `src/bank_01.c`): Player state machine, weapons, projectiles, side-scrolling stage enemies.
- **Bank `$02`** (Executable Code -> `src/bank_02.c`): Stage bosses, Mode 7 overhead stages (Stages 2 & 5), multi-part entities.
- **Bank `$03`** (Data + Code -> `src/bank_03.c`): Mode 7 map/LUT data (`$8000–$E17F`) + Mode 7 overhead stage mechanics (`$E180–$FE4B`).
- **Bank `$04`** (Executable Code -> `src/bank_04.c`): Cutscenes, intro/ending, stage events, mid-bosses & final bosses.
- **Bank `$05`** (Master Data (`DB=$05`) -> `src/bank_data_manifest.c`): 24-bit dispatch tables (`$A0B4`, `$A48A`), asset load scripts (`$F537`, `$F55B`), global LUTs.
- **Bank `$06`** (Stage Data (`DB=$06`) -> `src/bank_data_manifest.c`): Stage layout/collision tables + Konami LZSS compressed graphics.
- **Bank `$07`** (Mode 7 Data (`DB=$07`) -> `src/bank_data_manifest.c`): Mode 7 overhead stage tilemaps & tables + LZSS compressed assets.
- **Bank `$08`** (Palette Data (`DB=$08`) -> `src/bank_data_manifest.c`): Master CGRAM palettes, fade tables (`$00:9AE6`) & LZSS compressed assets.
- **Bank `$09`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$0A`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$0B`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$0C`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$0D`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$0E`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$0F`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$10`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$11`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$12`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$13`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$14`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$15`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$16`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$17`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$18`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$19`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$1A`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$1B`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$1C`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$1D`** (Data + Code -> `src/bank_1d.c`): Sprite assembly tables (`$8000–$AB9F`) + shared trig/physics (`$ABA0`) & late-boss AI (`$ABA0–$CCDF`).
- **Bank `$1E`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
- **Bank `$1F`** (LZSS / Audio Assets -> `src/bank_data_manifest.c`): Konami LZSS compressed graphics, tilemaps, or SPC700 driver/BRR sample streams consumed via Bank `$05` asset scripts.
