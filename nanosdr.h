/*
 * Copyright (c) 2016-2017, TAKAHASHI Tomohiro (TTRFTECH) edy555@gmail.com
 * All rights reserved.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * The software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

extern void I2CWrite(int addr, uint8_t d0, uint8_t d1);

/*
 * tlv320aic3204.c
 */

typedef struct {
  int target_level;
  int gain_hysteresis;
  int attack;
  int attack_scale;
  int decay;
  int decay_scale;
} tlv320aic3204_agc_config_t;

extern void tlv320aic3204_init(void);
extern void tlv320aic3204_set_gain(int gain);
extern void tlv320aic3204_set_digital_gain(int gain);
extern void tlv320aic3204_set_volume(int gain);
extern void tlv320aic3204_agc_config(tlv320aic3204_agc_config_t *conf);

extern void tlv320aic3204_config_adc_filter(int enable);
extern void tlv320aic3204_set_impedance(int imp);

/*
 * dsp.c
 */

// 5ms @ 48kHz
#define AUDIO_BUFFER_LEN 480

extern int16_t rx_buffer[AUDIO_BUFFER_LEN * 2];
extern int16_t tx_buffer[AUDIO_BUFFER_LEN * 2];

extern int16_t buffer[2][AUDIO_BUFFER_LEN];
extern int16_t buffer2[2][AUDIO_BUFFER_LEN];

typedef void (*signal_process_func_t)(int16_t *src, int16_t *dst, size_t len);

extern signal_process_func_t signal_process;

void am_demod(int16_t *src, int16_t *dst, size_t len);
void lsb_demod(int16_t *src, int16_t *dst, size_t len);
void usb_demod(int16_t *src, int16_t *dst, size_t len);
void fm_demod(int16_t *src, int16_t *dst, size_t len);

void set_agc_mode(int agcmode);

#define AM_FREQ_OFFSET 10000
#define SSB_FREQ_OFFSET 1300

extern int32_t center_frequency;
extern int16_t mode_freq_offset;

typedef struct {
  int16_t *buffer;
  int16_t stride;
  int16_t count;
  int16_t coeff;

  int32_t accumlate;
  int16_t offset;
} dcrejection_t;

/*
 * font: Font5x7.c numfont32x24.c numfont20x24.c icons.c
 */

extern const uint16_t x5x7_bits [];
extern const uint32_t numfont20x24[][24];
extern const uint32_t numfont32x24[][24];
extern const uint32_t icons48x20[][20*2];

#define S_PI    "\034"
#define S_MICRO "\035"
#define S_OHM   "\036"
#define S_DEGREE "\037"
#define S_RARROW "\033"


/*
 * ili9341.c
 */

#define RGB565(b,g,r)     ( (((r)<<8)&0xf800) | (((g)<<3)&0x07e0) | (((b)>>3)&0x001f) )

typedef struct {
	uint16_t width;
	uint16_t height;
	uint16_t scaley;
	uint16_t slide;
	uint16_t stride;
	const uint32_t *bitmap;
} font_t;

extern const font_t NF20x24;
extern const font_t NF32x24;
extern const font_t NF32x48;
extern const font_t ICON48x20;

extern uint16_t spi_buffer[];

void ili9341_init(void);
void ili9341_test(int mode);
void ili9341_bulk(int x, int y, int w, int h);
void ili9341_fill(int x, int y, int w, int h, int color);
void ili9341_draw_bitmap(int x, int y, int w, int h, uint16_t *bitmap);
void ili9341_drawchar_5x7(uint8_t ch, int x, int y, uint16_t fg, uint16_t bg);
void ili9341_drawstring_5x7(const char *str, int x, int y, uint16_t fg, uint16_t bg);
void ili9341_drawfont(uint8_t ch, const font_t *font, int x, int y, uint16_t fg, uint16_t bg);
void ili9341_drawfont_string(const char *str, const font_t *font, int x, int y, uint16_t fg, uint16_t bg);


/*
 * display.c
 */

void disp_init(void);
void disp_process(void);
void disp_fetch_samples(void);
void disp_update(void);

void set_window_function(int wf_type);


/*
 * ui.c
 */

extern void ui_init(void);
extern void ui_process(void);

typedef enum {
	MOD_LSB,
	MOD_USB,
	MOD_AM,
	MOD_FM,
	MOD_MAX
} modulation_t;

extern void set_tune(int hz);
extern void set_modulation(modulation_t mod);


typedef struct {
    uint32_t freq;
	modulation_t modulation;
	int8_t rfgain;
    int8_t dgain;
} setting_t;

typedef struct {
    enum { CHANNEL, FREQ, VOLUME, MOD, AGC, RFGAIN, SPDISP, WFDISP, MODE_MAX } mode;
	int8_t volume;
	int8_t channel;

    uint32_t freq;
	modulation_t modulation;
	int8_t rfgain;
    int8_t dgain;

	enum { AGC_MANUAL, AGC_SLOW, AGC_MID, AGC_FAST } agcmode;
	int8_t digit; /* 0~5 */
    int freq_offset;
    enum { SPDISP_CAP, SPDISP_CAP2, SPDISP_IF, SPDISP_AUD, SPDISP_MODE_MAX } spdispmode;
    enum { WATERFALL, WAVEFORM, WFDISP_MODE_MAX } wfdispmode;
} uistat_t;

extern uistat_t uistat;


/*EOF*/
