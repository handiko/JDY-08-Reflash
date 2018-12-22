
/*

Copyright (c) 2012-2014 RedBearLab

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#define ADDR_BUF0                   0x0000 // Buffer (512 bytes)
#define ADDR_DMA_DESC_0             0x0200 // DMA descriptors (8 bytes)
#define ADDR_DMA_DESC_1             (ADDR_DMA_DESC_0 + 8)

#define CH_DBG_TO_BUF0              0x01   // Channel 0
#define CH_BUF0_TO_FLASH            0x02   // Channel 1

#define CMD_CHIP_ERASE              0x10
#define CMD_WR_CONFIG               0x19
#define CMD_RD_CONFIG               0x24
#define CMD_READ_STATUS             0x30
#define CMD_RESUME                  0x4C
#define CMD_DEBUG_INSTR_1B          (0x54|1)
#define CMD_DEBUG_INSTR_2B          (0x54|2)
#define CMD_DEBUG_INSTR_3B          (0x54|3)
#define CMD_BURST_WRITE             0x80
#define CMD_GET_CHIP_ID             0x68

#define STATUS_CHIP_ERASE_BUSY_BM   0x80 // New debug interface
#define STATUS_PCON_IDLE_BM         0x40
#define STATUS_CPU_HALTED_BM        0x20
#define STATUS_PM_ACTIVE_BM         0x10
#define STATUS_HALT_STATUS_BM       0x08
#define STATUS_DEBUG_LOCKED_BM      0x04
#define STATUS_OSC_STABLE_BM        0x02
#define STATUS_STACK_OVERFLOW_BM    0x01

#define DUP_DBGDATA                 0x6260  // Debug interface data buffer
#define DUP_FCTL                    0x6270  // Flash controller
#define DUP_FADDRL                  0x6271  // Flash controller addr
#define DUP_FADDRH                  0x6272  // Flash controller addr
#define DUP_FWDATA                  0x6273  // Clash controller data buffer
#define DUP_CLKCONSTA               0x709E  // Sys clock status
#define DUP_CLKCONCMD               0x70C6  // Sys clock configuration
#define DUP_MEMCTR                  0x70C7  // Flash bank xdata mapping
#define DUP_DMA1CFGL                0x70D2  // Low byte, DMA config ch. 1
#define DUP_DMA1CFGH                0x70D3  // Hi byte , DMA config ch. 1
#define DUP_DMA0CFGL                0x70D4  // Low byte, DMA config ch. 0
#define DUP_DMA0CFGH                0x70D5  // Low byte, DMA config ch. 0
#define DUP_DMAARM                  0x70D6  // DMA arming register

#define LOBYTE(w)           ((unsigned char)(w))
#define HIBYTE(w)           ((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))

#define SBEGIN                0x01
#define SDATA                 0x02
#define SRSP                  0x03
#define SEND                  0x04
#define ERRO                 0x05
#define WAITING               0x00
#define RECEIVING             0x01

int DD = 6;
int DC = 5;
int RESET = 4;
int LED = 13;

const unsigned char dma_desc_0[8] =
{
    HIBYTE(DUP_DBGDATA),            // src[15:8]
    LOBYTE(DUP_DBGDATA),            // src[7:0]
    HIBYTE(ADDR_BUF0),              // dest[15:8]
    LOBYTE(ADDR_BUF0),              // dest[7:0]
    0,                              // len[12:8] - filled in later
    0,                              // len[7:0]
    31,                             // trigger: DBG_BW
    0x11                            // increment destination
};

const unsigned char dma_desc_1[8] =
{
    HIBYTE(ADDR_BUF0),              // src[15:8]
    LOBYTE(ADDR_BUF0),              // src[7:0]
    HIBYTE(DUP_FWDATA),             // dest[15:8]
    LOBYTE(DUP_FWDATA),             // dest[7:0]
    0,                              // len[12:8] - filled in later
    0,                              // len[7:0]
    18,                             // trigger: FLASH
    0x42,                           // increment source
};

#pragma inline
void write_debug_byte(unsigned char data)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        digitalWrite(DC, HIGH);
        if(data & 0x80)
        {
          digitalWrite(DD, HIGH);
        }
        else
        {
          digitalWrite(DD, LOW);
        }
        data <<= 1;
        digitalWrite(DC, LOW); // set clock low (DUP capture flank)
    }
}

#pragma inline
unsigned char read_debug_byte(void)
{
    unsigned char i;
    unsigned char data = 0x00;
    for (i = 0; i < 8; i++)
    {
        digitalWrite(DC, HIGH);  // DC high
        data <<= 1;
        if(HIGH == digitalRead(DD))
        {
          data |= 0x01;
        }        
        digitalWrite(DC, LOW);     // DC low
    }
    return data;
}

#pragma inline
unsigned char wait_dup_ready(void)
{
    unsigned int count = 0;
    while ((HIGH == digitalRead(DD)) && count < 16)
    {
        read_debug_byte();
        count++;
    }
    return (count == 16) ? 0 : 1;
}

unsigned char debug_command(unsigned char cmd, unsigned char *cmd_bytes,
                            unsigned short num_cmd_bytes)
{
    unsigned short i;
    unsigned char output = 0;
    pinMode(DD, OUTPUT);
    write_debug_byte(cmd);
    for (i = 0; i < num_cmd_bytes; i++)
    {
        write_debug_byte(cmd_bytes[i]);
    }
    pinMode(DD, INPUT);
    digitalWrite(DD, HIGH);
    wait_dup_ready();
    output = read_debug_byte();
    pinMode(DD, OUTPUT);

    return output;
}

void debug_init(void)
{
    volatile unsigned char i;

    digitalWrite(DD, LOW);
    digitalWrite(DC, LOW);
    digitalWrite(RESET, LOW);
    delay(10);   // Wait
    digitalWrite(DC, HIGH);                    // DC high
    delay(10);   // Wait
    digitalWrite(DC, LOW);                     // DC low
    delay(10);   // Wait
    digitalWrite(DC, HIGH);                    // DC high
    delay(10);   // Wait
    digitalWrite(DC, LOW);                     // DC low
    delay(10);   // Wait
    digitalWrite(RESET, HIGH);              // Release RESET_N
    delay(10);   // Wait
}

unsigned char read_chip_id(void)
{
    unsigned char id = 0;

    pinMode(DD, OUTPUT);
    delay(1);
    write_debug_byte(CMD_GET_CHIP_ID);
    pinMode(DD, INPUT);
    digitalWrite(DD, HIGH);
    delay(1);
    if(wait_dup_ready() == 1)
    {
      id = read_debug_byte(); // ID
      read_debug_byte();      // Revision (discard)
    }
    pinMode(DD, OUTPUT);

    return id;
}

void burst_write_block(unsigned char *src, unsigned short num_bytes)
{
    unsigned short i;

    pinMode(DD, OUTPUT);

    write_debug_byte(CMD_BURST_WRITE | HIBYTE(num_bytes));
    write_debug_byte(LOBYTE(num_bytes));
    for (i = 0; i < num_bytes; i++)
    {
        write_debug_byte(src[i]);
    }

    pinMode(DD, INPUT);
    digitalWrite(DD, HIGH);
    wait_dup_ready();
    read_debug_byte(); 
    pinMode(DD, OUTPUT);
}

void chip_erase(void)
{
    volatile unsigned char status;
    debug_command(CMD_CHIP_ERASE, 0, 0);

    do {
        status = debug_command(CMD_READ_STATUS, 0, 0);
    } while((status & STATUS_CHIP_ERASE_BUSY_BM));
}

void write_xdata_memory_block(unsigned short address,
                              const unsigned char *values,
                              unsigned short num_bytes)
{
    unsigned char instr[3];
    unsigned short i;

    instr[0] = 0x90;
    instr[1] = HIBYTE(address);
    instr[2] = LOBYTE(address);
    debug_command(CMD_DEBUG_INSTR_3B, instr, 3);

    for (i = 0; i < num_bytes; i++)
    {
        instr[0] = 0x74;
        instr[1] = values[i];
        debug_command(CMD_DEBUG_INSTR_2B, instr, 2);

        instr[0] = 0xF0;
        debug_command(CMD_DEBUG_INSTR_1B, instr, 1);

        debug_command(CMD_DEBUG_INSTR_1B, instr, 1);
    }
}

void write_xdata_memory(unsigned short address, unsigned char value)
{
    unsigned char instr[3];

    instr[0] = 0x90;
    instr[1] = HIBYTE(address);
    instr[2] = LOBYTE(address);
    debug_command(CMD_DEBUG_INSTR_3B, instr, 3);

    instr[0] = 0x74;
    instr[1] = value;
    debug_command(CMD_DEBUG_INSTR_2B, instr, 2);

    instr[0] = 0xF0;
    debug_command(CMD_DEBUG_INSTR_1B, instr, 1);
}

unsigned char read_xdata_memory(unsigned short address)
{
    unsigned char instr[3];

    instr[0] = 0x90;
    instr[1] = HIBYTE(address);
    instr[2] = LOBYTE(address);
    debug_command(CMD_DEBUG_INSTR_3B, instr, 3);

    instr[0] = 0xE0;
    return debug_command(CMD_DEBUG_INSTR_1B, instr, 1);
}

void read_flash_memory_block(unsigned char bank,unsigned short flash_addr,
                             unsigned short num_bytes, unsigned char *values)
{
    unsigned char instr[3];
    unsigned short i;
    unsigned short xdata_addr = (0x8000 + flash_addr);

    write_xdata_memory(DUP_MEMCTR, bank);

    instr[0] = 0x90;
    instr[1] = HIBYTE(xdata_addr);
    instr[2] = LOBYTE(xdata_addr);
    debug_command(CMD_DEBUG_INSTR_3B, instr, 3);

    for (i = 0; i < num_bytes; i++)
    {
        instr[0] = 0xE0;
        values[i] = debug_command(CMD_DEBUG_INSTR_1B, instr, 1);

        instr[0] = 0xA3;
        debug_command(CMD_DEBUG_INSTR_1B, instr, 1);
    }
}

/**************************************************************************//**
* @brief    Writes 4-2048 bytes to DUP's flash memory. Parameter \c num_bytes
*           must be a multiple of 4.
* @param    src         Pointer to programmer's source buffer (in XDATA space)
* @param    start_addr  FLASH memory start address [0x0000 - 0x7FFF]
* @param    num_bytes   Number of bytes to transfer [4-1024]
* @return   None.
******************************************************************************/
void write_flash_memory_block(unsigned char *src, unsigned long start_addr,
                              unsigned short num_bytes)
{
    write_xdata_memory_block(ADDR_DMA_DESC_0, dma_desc_0, 8);
    write_xdata_memory_block(ADDR_DMA_DESC_1, dma_desc_1, 8);

    unsigned char len[2] = {HIBYTE(num_bytes), LOBYTE(num_bytes)};
    write_xdata_memory_block((ADDR_DMA_DESC_0+4), len, 2);  // LEN, DBG => ram
    write_xdata_memory_block((ADDR_DMA_DESC_1+4), len, 2);  // LEN, ram => flash

    write_xdata_memory(DUP_DMA0CFGH, HIBYTE(ADDR_DMA_DESC_0));
    write_xdata_memory(DUP_DMA0CFGL, LOBYTE(ADDR_DMA_DESC_0));
    write_xdata_memory(DUP_DMA1CFGH, HIBYTE(ADDR_DMA_DESC_1));
    write_xdata_memory(DUP_DMA1CFGL, LOBYTE(ADDR_DMA_DESC_1));

    write_xdata_memory(DUP_FADDRH, HIBYTE( (start_addr)));//>>2) ));
    write_xdata_memory(DUP_FADDRL, LOBYTE( (start_addr)));//>>2) ));

    write_xdata_memory(DUP_DMAARM, CH_DBG_TO_BUF0);
    burst_write_block(src, num_bytes);

    write_xdata_memory(DUP_DMAARM, CH_BUF0_TO_FLASH);
    write_xdata_memory(DUP_FCTL, 0x0A);//0x06

    while (read_xdata_memory(DUP_FCTL) & 0x80);
}

void RunDUP(void)
{
  volatile unsigned char i;

  digitalWrite(DD, LOW);
  digitalWrite(DC, LOW);
  digitalWrite(RESET, LOW);
  delay(10);   // Wait

  digitalWrite(RESET, HIGH);
  delay(10);   // Wait
}

void ProgrammerInit(void)
{
  pinMode(DD, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(DD, LOW);
  digitalWrite(DC, LOW);
  digitalWrite(RESET, HIGH);
  digitalWrite(LED, LOW);
}

void setup() 
{  
  ProgrammerInit();  
  Serial.begin(115200);
  while(!Serial);
}

void loop() 
{
  unsigned char chip_id = 0;
  unsigned char debug_config = 0;
  unsigned char Continue = 0;
  unsigned char Verify = 0;
  
  while(!Continue)     // Wait for starting
  {  
    
    if(Serial.available()==2)
    {      
      if(Serial.read() == SBEGIN)
      {
        Verify = Serial.read();
        Continue = 1;
      }
      else
      {
        Serial.read(); // Clear RX buffer
      }
    }
  }

  debug_init();
  chip_id = read_chip_id();
  if(chip_id == 0) 
  {
    Serial.write(ERRO);  
    return; // No chip detected, run loop again.
  }
  
  RunDUP();
  debug_init();
  
  chip_erase();
  RunDUP();
  debug_init();
  
  write_xdata_memory(DUP_CLKCONCMD, 0x80);
  while (read_xdata_memory(DUP_CLKCONSTA) != 0x80);//0x80)
  
  debug_config = 0x22;
  debug_command(CMD_WR_CONFIG, &debug_config, 1);
  
  Serial.write(SRSP);    // Request data blocks
  digitalWrite(LED, HIGH);  
  unsigned char Done = 0;
  unsigned char State = WAITING;
  unsigned char  rxBuf[514]; 
  unsigned int BufIndex = 0;
  unsigned int addr = 0x0000;
  while(!Done)
  {
    while(Serial.available())
    {
      unsigned char ch;    
      ch = Serial.read();        
      switch (State)
      {
        case WAITING:
        {
          if(SDATA == ch)  // Incoming bytes are data
          {
            State = RECEIVING;
          }
          else if(SEND == ch)   // End receiving firmware
          {
            Done = 1;           // Exit while(1) in main function
          }
          break;
        }      

        case RECEIVING:
        {          
          rxBuf[BufIndex] = ch;
          BufIndex++;            
          if (BufIndex == 514) // If received one block, write it to flash
          {
            BufIndex = 0;              
            uint16_t CheckSum = 0x0000;
            for(unsigned int i=0; i<512; i++)
            {
              CheckSum += rxBuf[i];
            }
            uint16_t CheckSum_t = rxBuf[512]<<8 | rxBuf[513];
            if(CheckSum_t != CheckSum)
            {
              State = WAITING;
              Serial.write(ERRO);                    
              chip_erase();
              return;
            } 
            write_flash_memory_block(rxBuf, addr, 512); // src, address, count                    
            if(Verify)
            {
              unsigned char bank = addr / (512 * 16);
              unsigned int  offset = (addr % (512 * 16)) * 4;
              unsigned char read_data[512];
              read_flash_memory_block(bank, offset, 512, read_data); // Bank, address, count, dest.            
              for(unsigned int i = 0; i < 512; i++) 
              {
                if(read_data[i] != rxBuf[i]) 
                {
                  // Fail
                  State = WAITING;
                  Serial.write(ERRO);                    
                  chip_erase();
                  return;
                }
              }
            }
            addr += (unsigned int)128;              
            State = WAITING;
            Serial.write(SRSP);
          }
          break;
        }      
        default:
          break;
      }
    }
  }
  
  digitalWrite(LED, LOW);
  RunDUP();
}
