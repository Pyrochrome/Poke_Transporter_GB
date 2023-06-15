#include <tonc.h>
#include <string>
#include "gameboy_colour.h"
#include "pokemon.h"
#include "output.h"
#include "LinkGPIO.h"


const int MODE = 1; // mode=0 will transfer pokemon data from pokemon.h
                    // mode=1 will copy pokemon party data being received

LinkGPIO* linkGPIO = new LinkGPIO();

uint8_t shift = 0;
uint8_t in_data = 0;
uint8_t out_data = 0;
uint frame = 0;

connection_state_t connection_state = NOT_CONNECTED;
trade_centre_state_gen_II_t trade_centre_state_gen_II = INIT;
int counter = 0;

int trade_pokemon = -1;

unsigned long last_bit;

char hexArr[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char output[2];

std::string outHexStr(vu8 inputNum){
  std::string out = "XX";
  out[0] = (inputNum) & 0xF;
  out[1] = (inputNum>>4) & 0xF;
	return out;
}

std::string out_array[10];

void print(std::string str){
  for (int i = 10; i > 0; i--){
    out_array[i] = out_array[i - 1];
  }
  out_array[0] = std::to_string(frame) + ": " + str + "\n";
    tte_erase_screen();
    tte_set_pos(0, 0);
  for (int j = 0; j < 10; j++){
    tte_write(out_array[j].c_str());
  }
}

void setup()
{

  // pinMode(SCLK_, INPUT);
  // pinMode(MISO_, INPUT);
  // pinMode(MOSI_, OUTPUT);

  linkGPIO->reset();

  linkGPIO->setMode(LinkGPIO::Pin::SC, LinkGPIO::Direction::OUTPUT);
  linkGPIO->setMode(LinkGPIO::Pin::SO, LinkGPIO::Direction::OUTPUT);
  linkGPIO->setMode(LinkGPIO::Pin::SI, LinkGPIO::Direction::INPUT);

  /*
  LINK_SPI_SET_LOW(REG_RCNT, 4);
  LINK_SPI_SET_LOW(REG_RCNT, 6);
  LINK_SPI_SET_HIGH(REG_RCNT, 7);

  LINK_SPI_SET_LOW(REG_RCNT, LINK_SPI_BIT_GENERAL_PURPOSE_LOW);
  LINK_SPI_SET_HIGH(REG_RCNT, LINK_SPI_BIT_GENERAL_PURPOSE_HIGH);
  */

  tte_erase_screen();
  tte_set_pos(0, 0);
  tte_write("FEED ME POKEMON, I HUNGER!\n");

  // digitalWrite(MOSI_, LOW);
  linkGPIO->writePin(LinkGPIO::Pin::SO, 0);
  out_data <<= 1;
}

byte handleIncomingByte(byte in)
{
  byte send = 0x00;

  switch (connection_state)
  {
  case NOT_CONNECTED:
    if (in == PKMN_MASTER)
      send = PKMN_SLAVE;
    else if (in == PKMN_BLANK)
      send = PKMN_BLANK;
    else if (in == PKMN_CONNECTED_II)
    {
      send = PKMN_CONNECTED_II;
      connection_state = CONNECTED;
    }
    break;

  case CONNECTED:
    if (in == PKMN_CONNECTED_II) // acknowledge connection
      send = PKMN_CONNECTED_II;
    else if (in == GEN_II_CABLE_TRADE_CENTER)
    { // acknowledge trade center selection
      connection_state = TRADE_CENTRE;
      send = GEN_II_CABLE_TRADE_CENTER;
    }
    else if (in == GEN_II_CABLE_CLUB_COLOSSEUM)
    { // acknowledge colosseum selection
      connection_state = COLOSSEUM;
      send = GEN_II_CABLE_CLUB_COLOSSEUM;
    }
    else
    {
      send = in;
    }
    break;

  case TRADE_CENTRE:
    if (trade_centre_state_gen_II == INIT && in == 0x00)
    {
      trade_centre_state_gen_II = READY_TO_GO;
      send = 0x00;
    }
    else if (trade_centre_state_gen_II == READY_TO_GO && in == 0xFD)
    {
      trade_centre_state_gen_II = SEEN_FIRST_WAIT;
      send = 0xFD;
    }
    else if (trade_centre_state_gen_II == SEEN_FIRST_WAIT && in != 0xFD)
    {
      // random data of slave is ignored.
      send = in;
      trade_centre_state_gen_II = SENDING_RANDOM_DATA;
    }
    else if (trade_centre_state_gen_II == SENDING_RANDOM_DATA && in == 0xFD)
    {
      trade_centre_state_gen_II = WAITING_TO_SEND_DATA;
      send = 0xFD;
    }
    else if (trade_centre_state_gen_II == WAITING_TO_SEND_DATA && in != 0xFD)
    {
      counter = 0;
      // send first byte
      switch (MODE)
      {
      case 0:
        send = DATA_BLOCK_GEN_II[counter];
        INPUT_BLOCK_GEN_II[counter] = in;
        break;
      case 1:
        send = in;
        break;
      default:
        send = in;
        break;
      }
      counter++;
      trade_centre_state_gen_II = SENDING_DATA;
    }
    else if (trade_centre_state_gen_II == SENDING_DATA)
    {
      switch (MODE)
      {
      case 0:
        send = DATA_BLOCK_GEN_II[counter];
        INPUT_BLOCK_GEN_II[counter] = in;
        break;
      case 1:
        send = in;
        break;
      default:
        send = in;
        break;
      }
      counter++;
      if (counter == PLAYER_LENGTH_GEN_II)
      {
        trade_centre_state_gen_II = SENDING_PATCH_DATA;
      }
    }
    else if (trade_centre_state_gen_II == SENDING_PATCH_DATA && in == 0xFD)
    {
      counter = 0;
      send = 0xFD;
    }
    else if (trade_centre_state_gen_II == SENDING_PATCH_DATA && in != 0xFD)
    {
      send = in;
      trade_centre_state_gen_II = MIMIC;
    }
    else if (trade_centre_state_gen_II == MIMIC)
    {
      send = in;
    }
    else if (trade_centre_state_gen_II == TRADE_PENDING && (in & 0x60) == 0x60)
    {
      if (in == 0x6f)
      {
        trade_centre_state_gen_II = READY_TO_GO;
        send = 0x6f;
      }
      else
      {
        send = 0x60; // first pokemon
        trade_pokemon = in - 0x60;
      }
    }
    else if (trade_centre_state_gen_II == TRADE_PENDING && in == 0x00)
    {
      send = 0;
      trade_centre_state_gen_II = TRADE_CONFIRMATION;
    }
    else if (trade_centre_state_gen_II == TRADE_CONFIRMATION && (in & 0x60) == 0x60)
    {
      send = in;
      if (in == 0x61)
      {
        trade_pokemon = -1;
        trade_centre_state_gen_II = TRADE_PENDING;
      }
      else
      {
        trade_centre_state_gen_II = DONE;
      }
    }
    else if (trade_centre_state_gen_II == DONE && in == 0x00)
    {
      send = 0;
      trade_centre_state_gen_II = INIT;
    }
    else
    {
      send = in;
    }
    break;

  default:
    send = in;
    break;
  }

  return send;
}

void transferBit(void)
{        
  in_data |=  (linkGPIO->readPin(LinkGPIO::Pin::SI)) << (7 - shift);  /*digitalRead(MISO_)*/
  if (++shift > 7)
  {
    shift = 0;
    out_data = handleIncomingByte(in_data); //in_data
    print(
      std::to_string(trade_centre_state_gen_II) + " " + 
      std::to_string(connection_state) + " " +
      std::to_string(in_data) + " " +
      std::to_string(out_data) + "\n");
    in_data = 0;
  }

  while (!linkGPIO->readPin(LinkGPIO::Pin::SC)){} // no activity when clock is low //!digitalRead(SCLK_)
  //digitalWrite(MOSI_, out_data & 0x80 ? HIGH : LOW);

  linkGPIO->writePin(LinkGPIO::Pin::SO, (out_data >> 7));
  out_data <<= 1;
}

void loop()
{
  while (true)
  {
    last_bit = frame;
    while (linkGPIO->readPin(LinkGPIO::Pin::SC)) /*digitalRead(SCLK_)*/
    {
      if (frame - last_bit > 999999)
      { // show when the clock is inactive
        print("idle\n");
        last_bit = frame;
        shift = 0;
        in_data = 0;
      }
      frame++;
    }
    frame++;
    transferBit();
  }
}
