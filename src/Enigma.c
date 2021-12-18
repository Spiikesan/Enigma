#include "Enigma.h"

#include <ctype.h>
#include <string.h>

#include <stdio.h>

/**
 * @brief Set the wheel data object
 * 
 * @param p_wheel 
 * @param p_values 
 */
static void set_wheel_data(t_enigmaWheel p_wheel, t_enigmaWheel p_reversedWheel, const char *p_values)
{
    for (unsigned int i = 0; i < ENIGMA_WHEEL_CELLS_COUNT; i++)
    {
        p_wheel[i] = EW_SET_VALUE(tolower(p_values[i]) - 'a', isupper(p_values[i]));
    }
    if (p_reversedWheel)
        for (unsigned int i = 0; i < ENIGMA_WHEEL_CELLS_COUNT; i++)
        {
            p_reversedWheel[EW_GET_VALUE(p_wheel[i])] = i;
        }
}

/**
 * @brief Create the substitutions in the wheel data with the given values
 *        Values must be given in lower case, or uppercase for to set notch position(s)
 * @param p_wheel The wheel to encode
 * @param p_values The substitutions to set. Caps means turnover (notch) position
 */
void build_wheel(t_enigmaParameters *p_engine, unsigned int p_wheelIndex, const char *p_values)
{
    if (p_wheelIndex < ENIGMA_WHEELS_COUNT)
    {
        set_wheel_data(p_engine->wheels[p_wheelIndex], p_engine->wheelsReversed[p_wheelIndex], p_values);
    }
}

/**
 * @brief Create the substitutions in the wheel data with the given values
 *        Values must be given in lower case, or uppercase for to set notch position(s)
 * @param p_wheel The wheel to encode
 * @param p_values The substitutions to set. Caps means turnover (notch) position
 */
void build_reciprocal(t_enigmaParameters *p_engine, const char *p_values)
{
    set_wheel_data(p_engine->reciprocal, NULL, p_values);
}

/**
 * @brief Change the wheel position
 * 
 * @param p_engine 
 * @param p_wheelIndex 
 * @param p_startOffset 
 */
void set_wheel_position(t_enigmaParameters *p_engine, unsigned int p_wheelIndex, unsigned int p_startOffset)
{
    if (p_wheelIndex < ENIGMA_WHEELS_COUNT && p_startOffset < ENIGMA_WHEEL_CELLS_COUNT)
    {
        p_engine->wheelsPosition[p_wheelIndex] = p_startOffset;
    }
}

/**
 * @brief Set 3 wheels position (convenience function)
 * 
 * @param p_engine 
 * @param p_wheelOff1 
 * @param p_wheelOff2 
 * @param p_wheelOff3 
 */
void set_3_wheels_position(t_enigmaParameters *p_engine, unsigned int p_wheelOff1, unsigned int p_wheelOff2, unsigned int p_wheelOff3)
{
    if (p_wheelOff1 < ENIGMA_WHEEL_CELLS_COUNT && p_wheelOff2 < ENIGMA_WHEEL_CELLS_COUNT && p_wheelOff3 < ENIGMA_WHEEL_CELLS_COUNT)
    {
        p_engine->wheelsPosition[0] = p_wheelOff1;
        p_engine->wheelsPosition[1] = p_wheelOff2;
        p_engine->wheelsPosition[2] = p_wheelOff3;
    }
}

/**
 * @brief Change the wheel order for n wheels
 * 
 * @param p_engine 
 * @param p_wheelIndex 
 * @param p_startOffset 
 */
void set_wheels_order(t_enigmaParameters *p_engine, unsigned int p_wheelOrder[ENIGMA_WHEELS_COUNT])
{
    memcpy(p_engine->wheelsOrder, p_wheelOrder, sizeof(unsigned int) * ENIGMA_WHEELS_COUNT);
}

/**
 * @brief Set 3 wheels order (convenience function)
 * 
 * @param p_engine 
 * @param p_wheelOrd1 
 * @param p_wheelOrd2 
 * @param p_wheelOrd3 
 */
void set_3_wheels_order(t_enigmaParameters *p_engine, unsigned int p_wheelOrd1, unsigned int p_wheelOrd2, unsigned int p_wheelOrd3)
{
    if (p_wheelOrd1 <= ENIGMA_WHEELS_COUNT && p_wheelOrd2 <= ENIGMA_WHEELS_COUNT && p_wheelOrd3 <= ENIGMA_WHEELS_COUNT)
    {
        p_engine->wheelsOrder[0] = p_wheelOrd1;
        p_engine->wheelsOrder[1] = p_wheelOrd2;
        p_engine->wheelsOrder[2] = p_wheelOrd3;
        p_engine->wheelsOrder[3] = 0;
    }
}

/**
 * @brief Set the default configuration for the engine.
 * 
 * @param p_engine 
 */
void default_params(t_enigmaParameters *p_engine)
{
    memset(p_engine, 0, sizeof(*p_engine));

    /* 1930, 1938 and 1939 default encodings. */
    /* Encoding values for    abcdefghijklmnopqrstuvwxyz */
    build_wheel(p_engine, 0, "ekmflgdqvzntowyhxUspaibrcj");   /* I    */
    build_wheel(p_engine, 1, "ajdksIruxblhwtmcqgznpyfvoe");   /* II   */
    build_wheel(p_engine, 2, "bdfhjlcprtxvznyeiwgakmUsqo");   /* III  */
    build_wheel(p_engine, 3, "esovpzjayqUirhxlnftgkdcmwb");   /* IV   */
    build_wheel(p_engine, 4, "Vzbrgityupsdnhlxawmjqofeck");   /* V    */
    build_wheel(p_engine, 5, "JpgvoumfyqbenHzrdkasxlictw");   /* VI   */
    build_wheel(p_engine, 6, "NzjhgrcxmyswbOufaivlpekqdt");   /* VII  */
    build_wheel(p_engine, 7, "FkqhtlxocbjspDzramewniuygv");   /* VIII */
    build_reciprocal(p_engine, "ejmzalyxvbwfcrquontspikhgd"); /* R */

    set_wheels_order(p_engine, (unsigned int []){ 5, 1, 7, 6, 2, 4, 3, 8 });
    set_wheel_position(p_engine, 0, 5 );
    set_wheel_position(p_engine, 1, 17 );
    set_wheel_position(p_engine, 2, 23 );
    set_wheel_position(p_engine, 3, 13 );
    set_wheel_position(p_engine, 4, 19 );
    set_wheel_position(p_engine, 5, 7 );
    set_wheel_position(p_engine, 6, 2 );
    set_wheel_position(p_engine, 7, 11 );
}

static void rotate_wheels(t_enigmaParameters *p_engine)
{
    int wh = 0;
    int next = 1;

    while (next && p_engine->wheelsOrder[wh] != 0 && wh < ENIGMA_WHEELS_COUNT)
    {
        p_engine->wheelsPosition[wh] = (p_engine->wheelsPosition[wh] + 1) % ENIGMA_WHEEL_CELLS_COUNT;
        next = EW_GET_NOTCH(p_engine->wheels[p_engine->wheelsOrder[wh] - 1][p_engine->wheelsPosition[wh]]);
        wh++;
    }
}

/**
 * @brief Get the next character corresponding to the current state of the Enigma engine
 * 
 * Steps :
 *  - Rotating wheels
 *  - Getting the value of the current data
 *  - Passing through plugboard (TODO)
 *  - Passing through rotor 1
 *  - Passing through rotor n
 *  - Passing through reciprocal
 *  - Passing through rotor n
 *  - Passing through rotor 1
 *  - Return the value.
 * @param p_engine 
 * @param p_data 
 * @return t_wheelCell 
 */
static t_wheelCell get_next_data(t_enigmaParameters *p_engine, t_wheelCell p_data)
{
    int data = tolower(p_data) - 'a';
    int wh = 0;

    //printf("Encoding %c\n", p_data);
    rotate_wheels(p_engine);
    while (p_engine->wheelsOrder[wh] != 0 && wh < ENIGMA_WHEELS_COUNT)
    {
        //printf("[wheel %d] data = %d, ", wh, data);
        data = (data + p_engine->wheelsPosition[wh]) % ENIGMA_WHEEL_CELLS_COUNT;
        //printf("shift (%d) = %d, ", p_engine->wheelsPosition[wh], data);
        data = EW_GET_VALUE(p_engine->wheels[p_engine->wheelsOrder[wh] - 1][data]);
        //printf("pass = %d\n", data);
        ++wh;
    }
    --wh;
    //printf("[reciproc] data = %d, ", data);
    data = p_engine->reciprocal[data];
    //printf("after = %d\n", data);
    while (wh >= 0)
    {
        //printf("[wheel %d] data = %d, ", wh, data);
        data = EW_GET_VALUE(p_engine->wheelsReversed[p_engine->wheelsOrder[wh] - 1][data]);
        //printf("pass = %d, ", data);
        data = (data - p_engine->wheelsPosition[wh]);
        if (data < 0) data += ENIGMA_WHEEL_CELLS_COUNT;
        //printf("shift (%d) = %d\n", p_engine->wheelsPosition[wh], data);
        --wh;
    }
    //printf("Encoded to %c\n", data + 'a');
    return data + 'a';
}

/**
 * @brief Encode the data given in parameter and place output into the dataOut
 * 
 * @param p_engine Parameterized data.
 * @param p_dataIn Data to encode
 * @param p_dataOut Buffer where to store the encoded data
 */
void encode_data(t_enigmaParameters *p_engine, const char *p_dataIn, char *p_dataOut)
{
    for (int i = 0; p_dataIn[i] != 0; i++)
    {
        if (isalpha(p_dataIn[i]))
            p_dataOut[i] = get_next_data(p_engine, p_dataIn[i]);
        else
            p_dataOut[i] = p_dataIn[i];
    }
}