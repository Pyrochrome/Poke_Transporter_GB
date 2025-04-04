## Modifications to Transfered Pokémon
- Nature is set to the modulo of the Pokémon's EXP, just like how PokéBank handles it
- Abilities, Characteristics, and Size are randomized
- Gender, Shininess, and Unown forms are retained from previous generations
- OT name and ID are retained, but SID is set to 00000 (unless the Pokémon is shiny)
- All Pokémon will have Square sparkles in Sword and Shield, both due to the SID and due to the Fateful Encounter flag.
- Nicknames are retained, however some special characters have been replaced
- Language is set to match whatever the language is in Ruby, Sapphire, FireRed, LeafGreen, or Emerald.
- Korean Pokémon are set to Japanese and given their default Japanese names, due to Generation 3 not supporting the Korean language
- Items are not transfered, but Pokémon that are new in the DreamDex will be given a Rare Candy
- Level and EXP are retained, but any level greater than 100 are set to 100
- Illegal moves are removed, even if they were legal in generation 1 and 2. If a Pokémon has no legal moves, it will be given its earliest move instead
- Stat Experience is erased and EVs are set to zero
- PP Ups are retained
- Pokérus is retained
- the OT gender will be set to Female if the player is playing as Kris in Pokémon Crystal
- The transfered Pokémon will be caught in a Pokéball
- IVs are randomly generated based on Gen 3's pseudo-random generation
- Met level is set to the Pokémon's current level, and met location is set to "Fateful Encounter". This value is invisible to the player, and when transfered to Diamond, Pearl, Platinum, HeartGold, or SoulSilver this will be replaced by "Met at Pal Park"
- Game of Origin is set to FireRed/LeafGreen for Pokémon transfered from Red, Blue, and Yellow. Pokémon from Gold, Silver, and Crystal will have their Game of Origin set to HeartGold/SoulSilver. This will display as "A far off land" in Ruby, Sapphire, FireRed, LeafGreen, and Emerald- much like how a Game of Origin of FireRed/LeafGreen displays in Ruby and Sapphire
- Mew will be given a Fateful Encounter flag, so that it obeys the player.
