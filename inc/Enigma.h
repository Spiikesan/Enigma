#if !defined(ENIGMA_H_)
#define ENIGMA_H_

/**
 * CONSTANTS
 */

#define ENIGMA_WHEELS_COUNT (8)
#define ENIGMA_WHEEL_CELLS_COUNT (26)
/**
 * TYPES
 */

typedef unsigned char t_wheelCell;

#define WC_NOTCH_OFFSET ((sizeof(t_wheelCell) * 8) - 1)
#define WC_NOTCH_MASK (1 << WC_NOTCH_OFFSET)
#define WC_VALUE_MASK (((t_wheelCell)-1) ^ WC_NOTCH_MASK)

typedef t_wheelCell t_enigmaWheel[ENIGMA_WHEEL_CELLS_COUNT];

typedef struct {
    t_enigmaWheel reciprocal;
    t_enigmaWheel wheels[ENIGMA_WHEELS_COUNT];
    t_enigmaWheel wheelsReversed[ENIGMA_WHEELS_COUNT];
    unsigned int wheelsOrder[ENIGMA_WHEELS_COUNT];
    unsigned int wheelsPosition[ENIGMA_WHEELS_COUNT];
} t_enigmaParameters;

/**
 * FUNCTIONS
 */

/***
 * @brief Create an Enigma wheel cell (limited to 7 bits data) and sets the notch marker
 * if the notch parameter isn't zero
 * @param value The value to encode (7 bits max)
 * @param notch boolean value if notch must be placed on this wheel cell
 */

#define EW_SET_VALUE(value, notch) (((value)&0x7F) | (((notch) != 0) << WC_NOTCH_OFFSET))

/**
 * @brief Get the value from the wheel cell
 * @param wheelCell
 */
#define EW_GET_VALUE(wheelCell) ((wheelCell)&WC_VALUE_MASK)
#define EW_GET_NOTCH(wheelCell) (((wheelCell)&WC_NOTCH_MASK) == WC_NOTCH_MASK)

/**
 * @brief Set default parameters for the Enigma engine
 * 
 * @param p_parameters Pointer to the Enigma engine structure.
 */
void default_params(t_enigmaParameters *p_parameters);

/**
 * @brief Create the substitutions in the wheel data with the given values
 *        Values must be given in lower case, or uppercase for to set notch position(s)
 * @param p_wheel The wheel to encode
 * @param p_values The substitutions to set. Caps means turnover (notch) position
 */
void build_wheel(t_enigmaParameters *p_parameters, unsigned int p_wheelIndex, const char *p_values);

/**
 * @brief Create the substitutions in the wheel data with the given values
 *        Values must be given in lower case, or uppercase for to set notch position(s)
 * @param p_wheel The wheel to encode
 * @param p_values The substitutions to set. Caps means turnover (notch) position
 */
void build_reciprocal(t_enigmaParameters *p_engine, const char *p_values);

/**
 * @brief change the wheel position
 * 
 * @param p_engine 
 * @param p_wheelIndex 
 * @param p_startOffset 
 */
void set_wheel_position(t_enigmaParameters *p_engine, unsigned int p_wheelIndex, unsigned int p_startOffset);

/**
 * @brief Set 3 wheels position (convenience function)
 * 
 * @param p_engine 
 * @param p_wheelOff1 
 * @param p_wheelOff2 
 * @param p_wheelOff3 
 */
void set_3_wheels_position(t_enigmaParameters *p_engine, unsigned int p_wheelOff1, unsigned int p_wheelOff2, unsigned int p_wheelOff3);

/**
 * @brief Change the wheel order for n wheels
 * 
 * @param p_engine 
 * @param p_wheelIndex 
 * @param p_startOffset 
 */
void set_wheels_order(t_enigmaParameters *p_engine, unsigned int p_wheelOrder[ENIGMA_WHEELS_COUNT]);

/**
 * @brief Encode the data given in parameter and place output into the dataOut
 * 
 * @param p_engine Parameterized data.
 * @param p_dataIn Data to encode
 * @param p_dataOut Buffer where to store the encoded data
 */
void encode_data(t_enigmaParameters *p_engine, const char *p_dataIn, char *p_dataOut);

#endif // ENIGMA_H_
