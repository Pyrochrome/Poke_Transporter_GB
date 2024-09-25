#include <tonc.h>
#include "mystery_gift_injector.h"
#include "flash_mem.h"
#include "mystery_gift_builder.h"
#include "rom_data.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/save.h"
#include "pokemon_data.h"

// This will need to be modified for the JP releases
static u8 em_wonder_card[0x14E] = {
    0x08, 0x6E, 0x00, 0x00, 0xBA, 0xB4, 0xBE, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0xCA, 0xCC, 0xC9, 0xC0, 0xBF, 0xCD, 0xCD, 0xC9, 0xCC, 0x00, 0xC0, 0xBF, 0xC8, 0xC8, 0xBF, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0x00, 0xBD, 0xD9, 0xE6, 0xE8, 0xDD, 0xDA, 0xDD, 0xD7, 0xD5, 0xE8, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xDD, 0xE7, 0xDD, 0xE8, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xDC, 0xE3, 0xE9, 0xE7, 0xD9, 0x00, 0xE7, 0xE3, 0xE9, 0xE8, 0xDC, 0xD9, 0xD5, 0xE7, 0xE8, 0x00, 0xE3, 0xDA, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xBD, 0xBF, 0xC8, 0xCE, 0xBF, 0xCC, 0x00, 0xDD, 0xE2, 0x00, 0xCD, 0xE3, 0xE3, 0xE8, 0xE3, 0xE4, 0xE3, 0xE0, 0xDD, 0xE7, 0x00, 0xBD, 0xDD, 0xE8, 0xED, 0x00, 0xE8, 0xE3, 0x00, 0xE6, 0xD9, 0xD7, 0xDD, 0xD9, 0xEA, 0xD9, 0x00, 0x00, 0x00, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0xE3, 0x00, 0xE2, 0xE3, 0xE8, 0x00, 0xE8, 0xE3, 0xE7, 0xE7, 0x00, 0xE8, 0xDC, 0xDD, 0xE7, 0x00, 0xBF, 0xEC, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0x00, 0xBD, 0xD5, 0xE6, 0xD8, 0x00, 0xD6, 0xD9, 0xDA, 0xE3, 0xE6, 0xD9, 0x00, 0x00, 0x00, 0xE6, 0xD9, 0xD7, 0xD9, 0xDD, 0xEA, 0xDD, 0xE2, 0xDB, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// static u8 rs_wonder_card[0x14E] = {
//    0xCE, 0x7C, 0x00, 0x00, 0xBA, 0xB4, 0xBE, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0xCA, 0xCC, 0xC9, 0xC0, 0xBF, 0xCD, 0xCD, 0xC9, 0xCC, 0x00, 0xC0, 0xBF, 0xC8, 0xC8, 0xBF, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0x00, 0xBD, 0xD9, 0xE6, 0xE8, 0xDD, 0xDA, 0xDD, 0xD7, 0xD5, 0xE8, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xDD, 0xE7, 0xDD, 0xE8, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xDC, 0xE3, 0xE9, 0xE7, 0xD9, 0x00, 0xE7, 0xE3, 0xE9, 0xE8, 0xDC, 0xD9, 0xD5, 0xE7, 0xE8, 0x00, 0xE3, 0xDA, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xBD, 0xBF, 0xC8, 0xCE, 0xBF, 0xCC, 0x00, 0xDD, 0xE2, 0x00, 0xC7, 0xE3, 0xE7, 0xE7, 0xD8, 0xD9, 0xD9, 0xE4, 0x00, 0xBD, 0xDD, 0xE8, 0xED, 0x00, 0xE8, 0xE3, 0x00, 0xE6, 0xD9, 0xD7, 0xDD, 0xD9, 0xEA, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0xE3, 0x00, 0xE2, 0xE3, 0xE8, 0x00, 0xE8, 0xE3, 0xE7, 0xE7, 0x00, 0xE8, 0xDC, 0xDD, 0xE7, 0x00, 0xBF, 0xEC, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0x00, 0xBD, 0xD5, 0xE6, 0xD8, 0x00, 0xD6, 0xD9, 0xDA, 0xE3, 0xE6, 0xD9, 0x00, 0x00, 0x00, 0xE6, 0xD9, 0xD7, 0xD9, 0xDD, 0xEA, 0xDD, 0xE2, 0xDB, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // checksum
static u8 frlg_wonder_card[0x14E] = {
    0x67, 0x18, 0x00, 0x00, 0xBA, 0xB4, 0xBE, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0xCA, 0xCC, 0xC9, 0xC0, 0xBF, 0xCD, 0xCD, 0xC9, 0xCC, 0x00, 0xC0, 0xBF, 0xC8, 0xC8, 0xBF, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0x00, 0xBD, 0xD9, 0xE6, 0xE8, 0xDD, 0xDA, 0xDD, 0xD7, 0xD5, 0xE8, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xDD, 0xE7, 0xDD, 0xE8, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xDC, 0xE3, 0xE9, 0xE7, 0xD9, 0x00, 0xE7, 0xE3, 0xE9, 0xE8, 0xDC, 0x00, 0xE3, 0xDA, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0x00, 0x00, 0x00, 0x00, 0xBD, 0xBF, 0xC8, 0xCE, 0xBF, 0xCC, 0x00, 0xE3, 0xE2, 0x00, 0xCD, 0xD9, 0xEA, 0xD9, 0xE2, 0x00, 0xC3, 0xE7, 0xE0, 0xD5, 0xE2, 0xD8, 0x00, 0xE8, 0xE3, 0x00, 0xE6, 0xD9, 0xD7, 0xDD, 0xD9, 0xEA, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0xE3, 0x00, 0xE2, 0xE3, 0xE8, 0x00, 0xE8, 0xE3, 0xE7, 0xE7, 0x00, 0xE8, 0xDC, 0xDD, 0xE7, 0x00, 0xBF, 0xEC, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0x00, 0xBD, 0xD5, 0xE6, 0xD8, 0x00, 0xD6, 0xD9, 0xDA, 0xE3, 0xE6, 0xD9, 0x00, 0x00, 0x00, 0xE6, 0xD9, 0xD7, 0xD9, 0xDD, 0xEA, 0xDD, 0xE2, 0xDB, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // checksum

bool inject_mystery(Pokemon_Party &incoming_box_data)
{
    mystery_gift_script script;
    if (ENABLE_OLD_EVENT)
    {
        // script.build_script_old(incoming_box_data);
    }
    else
    {
        script.build_script(incoming_box_data);
    }
    u32 checksum = 0;
    if (curr_rom.is_ruby_sapphire())
    {
        checksum = script.calc_checksum32();
    }
    else
    {
        checksum = script.calc_crc16();
    }

    // Add in Wonder Card
    copy_save_to_ram(memory_section_array[4], &global_memory_buffer[0], 0x1000);
    switch (curr_rom.gamecode)
    {
    case RUBY_ID:
    case SAPPHIRE_ID:
        // Wonder Card doesn't exist
        break;
    case FIRERED_ID:
    case LEAFGREEN_ID:
        for (int i = 0; i < 0x14E; i++)
        {
            global_memory_buffer[curr_rom.offset_wondercard + i] = frlg_wonder_card[i];
        }
        break;
    case EMERALD_ID:
    default:
        for (int i = 0; i < 0x14E; i++)
        {
            global_memory_buffer[curr_rom.offset_wondercard + i] = em_wonder_card[i];
        }
        break;
    }

    // Set checksum and padding
    global_memory_buffer[curr_rom.offset_script] = checksum >> 0;
    global_memory_buffer[curr_rom.offset_script + 1] = checksum >> 8;
    global_memory_buffer[curr_rom.offset_script + 2] = checksum >> 16;
    global_memory_buffer[curr_rom.offset_script + 3] = checksum >> 24;

    // Add in Mystery Script data
    for (int i = 0; i < MG_SCRIPT_SIZE; i++)
    {
        global_memory_buffer[curr_rom.offset_script + 4 + i] = script.get_script_value_at(i);
    }

    update_memory_buffer_checksum(false);
    erase_sector(memory_section_array[4]);
    copy_ram_to_save(&global_memory_buffer[0], memory_section_array[4], 0x1000);

    // Add in Pokemon and Dex data
    copy_save_to_ram(0x1E000, &global_memory_buffer[0], 0x1000);
    int curr_index = 0;
    int dex_nums[MAX_PKMN_IN_BOX] = {};
    if (ENABLE_OLD_EVENT)
    {
        for (int i = 0; i < MAX_PKMN_IN_BOX; i++) // Add in the Pokemon data
        {
            Pokemon curr_pkmn = incoming_box_data.get_converted_pkmn(i);
            if (curr_pkmn.get_validity())
            {

                for (int curr_byte = 0; curr_byte < POKEMON_SIZE; curr_byte++)
                {
                    global_memory_buffer[curr_index] = curr_pkmn.get_gen_3_data(curr_byte);
                    curr_index++;
                }
                dex_nums[i] = curr_pkmn.get_dex_number();
            }
            else
            {
                curr_index += POKEMON_SIZE;
            }
        }
        for (int i = 0; i < MAX_PKMN_IN_BOX; i++) // Add in the dex numbers
        {
            global_memory_buffer[curr_index] = dex_nums[i];
            curr_index++;
        }
    }
    else
    {
        for (int i = 0; i < 0x1000; i++) // Copy over the save data section
        {
            global_memory_buffer[curr_index] = script.get_section30_value_at(i);
            curr_index++;
        }
    }

    update_memory_buffer_checksum(false);
    erase_sector(0x1E000);
    copy_ram_to_save(&global_memory_buffer[0], 0x1E000, 0x1000);

    // Set flags
    int memory_section = 1 + ((curr_rom.offset_flags + (curr_rom.unused_flag_start / 8)) / 0xF80); // This sets the correct memory section, since flags stretch between section 1 and 2.
    copy_save_to_ram(memory_section_array[memory_section], &global_memory_buffer[0], 0x1000);
    global_memory_buffer[(curr_rom.offset_flags + (curr_rom.all_collected_flag / 8)) % 0xF80] &= ~(1 << (curr_rom.all_collected_flag % 8)); // Set "collected all" flag to 0

    for (int i = 0; i < MAX_PKMN_IN_BOX; i++)
    {
        int curr_flag;
        curr_flag = curr_rom.pkmn_collected_flag_start + i;
        global_memory_buffer[(curr_rom.offset_flags + (curr_flag / 8)) % 0xF80] &= ~(1 << (curr_flag % 8)); // Reset the flag
        if (incoming_box_data.get_simple_pkmn(i).is_valid)
        {
            global_memory_buffer[(curr_rom.offset_flags + (curr_flag / 8)) % 0xF80] |= (1 << (curr_flag % 8)); // Set flag accordingly
        }
    }

    update_memory_buffer_checksum(false);
    erase_sector(memory_section_array[memory_section]);
    copy_ram_to_save(&global_memory_buffer[0], memory_section_array[memory_section], 0x1000);

    // Save custom save data
    write_custom_save_data();
    return true;
}
