#ifndef AUDIO_H
#define AUDIO_H
#include <stdint.h>

/**
 * @defgroup AUDIO Audio library
 *
 * @brief Low-level audio functions.
 *
 * mic+ph and line_in can be used as audio input and mic+ph as output.
 *
 * An example of using this library to play audio from line_in to mic+Ph:
 * @code
 * #include <libpynq.h>
 * int main (void)
 * {
 *  pynq_init();
 *  audio_init();
 *  audio_select_input(MIC);
 *  while(1) {
 *    audio_bypass(64*1024, 32*1024, 50, 0);
 *  }
 *  deselect();
 *  pynq_destroy();
 *  return EXIT_SUCCES;
 * }
 * @endcode
 *
 * @{
 */

#define LINE_IN 0
#define MIC 1

// Slave address for the ADAU audio controller 8
#define IIC_SLAVE_ADDR 0x3b

// I2C Serial Clock frequency in Hertz
#define IIC_SCLK_RATE 400000

// I2S Register
#define I2S_DATA_RX_L_REG 0x00
#define I2S_DATA_RX_R_REG 0x04
#define I2S_DATA_TX_L_REG 0x08
#define I2S_DATA_TX_R_REG 0x0C
#define I2S_STATUS_REG 0x10

// Audio registers
enum audio_adau1761_regs {
  R0_CLOCK_CONTROL = 0x00,
  R1_PLL_CONTROL = 0x02,
  R2_DIGITAL_MIC_JACK_DETECTION_CONTROL = 0x08,
  R3_RECORD_POWER_MANAGEMENT = 0x09,
  R4_RECORD_MIXER_LEFT_CONTROL_0 = 0x0A,
  R5_RECORD_MIXER_LEFT_CONTROL_1 = 0x0B,
  R6_RECORD_MIXER_RIGHT_CONTROL_0 = 0x0C,
  R7_RECORD_MIXER_RIGHT_CONTROL_1 = 0x0D,
  R8_LEFT_DIFFERENTIAL_INPUT_VOLUME_CONTROL = 0x0E,
  R9_RIGHT_DIFFERENTIAL_INPUT_VOLUME_CONTROL = 0x0F,
  R10_RECORD_MICROPHONE_BIAS_CONTROL = 0x10,
  R11_ALC_CONTROL_0 = 0x11,
  R12_ALC_CONTROL_1 = 0x12,
  R13_ALC_CONTROL_2 = 0x13,
  R14_ALC_CONTROL_3 = 0x14,
  R15_SERIAL_PORT_CONTROL_0 = 0x15,
  R16_SERIAL_PORT_CONTROL_1 = 0x16,
  R17_CONVERTER_CONTROL_0 = 0x17,
  R18_CONVERTER_CONTROL_1 = 0x18,
  R19_ADC_CONTROL = 0x19,
  R20_LEFT_INPUT_DIGITAL_VOLUME = 0x1A,
  R21_RIGHT_INPUT_DIGITAL_VOLUME = 0x1B,
  R22_PLAYBACK_MIXER_LEFT_CONTROL_0 = 0x1C,
  R23_PLAYBACK_MIXER_LEFT_CONTROL_1 = 0x1D,
  R24_PLAYBACK_MIXER_RIGHT_CONTROL_0 = 0x1E,
  R25_PLAYBACK_MIXER_RIGHT_CONTROL_1 = 0x1F,
  R26_PLAYBACK_LR_MIXER_LEFT_LINE_OUTPUT_CONTROL = 0x20,
  R27_PLAYBACK_LR_MIXER_RIGHT_LINE_OUTPUT_CONTROL = 0x21,
  R28_PLAYBACK_LR_MIXER_MONO_OUTPUT_CONTROL = 0x22,
  R29_PLAYBACK_HEADPHONE_LEFT_VOLUME_CONTROL = 0x23,
  R30_PLAYBACK_HEADPHONE_RIGHT_VOLUME_CONTROL = 0x24,
  R31_PLAYBACK_LINE_OUTPUT_LEFT_VOLUME_CONTROL = 0x25,
  R32_PLAYBACK_LINE_OUTPUT_RIGHT_VOLUME_CONTROL = 0x26,
  R33_PLAYBACK_MONO_OUTPUT_CONTROL = 0x27,
  R34_PLAYBACK_POP_CLICK_SUPPRESSION = 0x28,
  R35_PLAYBACK_POWER_MANAGEMENT = 0x29,
  R36_DAC_CONTROL_0 = 0x2A,
  R37_DAC_CONTROL_1 = 0x2B,
  R38_DAC_CONTROL_2 = 0x2C,
  R39_SERIAL_PORT_PAD_CONTROL = 0x2D,
  R40_CONTROL_PORT_PAD_CONTROL_0 = 0x2F,
  R41_CONTROL_PORT_PAD_CONTROL_1 = 0x30,
  R42_JACK_DETECT_PIN_CONTROL = 0x31,
  R67_DEJITTER_CONTROL = 0x36,
  R58_SERIAL_INPUT_ROUTE_CONTROL = 0xF2,
  R59_SERIAL_OUTPUT_ROUTE_CONTROL = 0xF3,
  R61_DSP_ENABLE = 0xF5,
  R62_DSP_RUN = 0xF6,
  R63_DSP_SLEW_MODES = 0xF7,
  R64_SERIAL_PORT_SAMPLING_RATE = 0xF8,
  R65_CLOCK_ENABLE_0 = 0xF9,
  R66_CLOCK_ENABLE_1 = 0xFA
};

/**
 * @brief Initializes the audio register. Sets the sampling frequency.
 * defines several values such as audio record volume and playback volume.
 * output is always played over mic+ph aux output.
 */
extern void audio_init(void);

/**
 * @brief selects the audio input channel.
 * @param input defines the input. Can be 0 LINE_IN or 1 MIC
 * @warning Fails with program exit when input is not valid.
 */
extern void audio_select_input(int input);

// Original ADAU1761 code

extern void write_audio_reg(unsigned char u8RegAddr, unsigned char u8Data,
                            int iic_fd);

extern void config_audio_pll(void);

extern void config_audio_codec(void);

/**
 * @brief Function to select LINE_IN as input.
 */
extern void select_line_in(void);

/**
 * @brief Function to select MIC as input.
 */
extern void select_mic(void);

/**
 * @brief Function to deselect input, either LINE_IN, or MIC.
 */
extern void deselect(void);

/**
 * @brief Record and play the audio without storing in DRAM.
 *
 * @param   audio_mmap_size is the address range of the audio codec.
 * @param   nsamples is the number of samples to read and output.
 * @param   uio_index is the uio index in /dev list.
 */
extern void audio_bypass(unsigned int audio_mmap_size, unsigned int nsamples,
                         unsigned int volume, int uio_index);

/**
 * @brief Function to support audio recording without the audio codec
 * controller.
 *
 * Notice that the buffer has to be twice the size of the number of samples,
 * because both left and right channels are sampled.
 *
 * @param   audio_mmap_size is the address range of the audio codec.
 * @param   BufAddr is the buffer address.
 * @param   nsamples is the number of samples.
 * @param   uio_index is the uio index in /dev list.
 */
extern void audio_record(unsigned int audio_mmap_size, unsigned int *BufAddr,
                         unsigned int nsamples, int uio_index);

/*
 * @brief Function to support audio playing without the audio codec controller.
 *
 * Notice that the buffer has to be twice the size of the number of samples,
 * because both left and right channels are sampled.
 * Consecutive indexes are played synchronisly on left and right output.
 *
 * @param   audio_mmap_size is the address range of the audio codec.
 * @param   BufAddr is the buffer address.
 * @param   nsamples is the number of samples.
 * @param   uio_index is the uio index in /dev list.
 * @param   volume is the volume of the output.
 */
extern void audio_play(unsigned int audio_mmap_size, unsigned int *BufAddr,
                       unsigned int nsamples, unsigned int volume,
                       int uio_index);

/**
 * @brief Function to play one audio fragment for multiple repititions.
 * @param   audio_mmap_size is the address range of the audio codec.
 * @param   BufAddr is the buffer address.
 * @param   nsamples is the number of samples.
 * @param   volume is the volume of the output.
 * @param   repetitions is the number of repitions.
 */

extern void audio_repeat_play(unsigned int audio_mmap_size,
                              unsigned int *BufAddr, unsigned int nsamples,
                              unsigned int volume, unsigned int repetitions);

/*
 * @brief Function to generate a specific tone on the audio output.
 * @param   frequency is the frequency in Hz to be played.
 * @param   time_ms is the time the frequency should be played in ms.
 * @param   volume is the volume of the output.
 */
extern void audio_generate_tone(unsigned int frequency, uint32_t time_ms,
                                unsigned int volume);

int32_t *audio_record_response(unsigned int frequency, uint32_t nperiods, unsigned
int volume, uint32_t *nsamples);

void audio_record_response_start(void);
/**
 * @}
 */

#endif
