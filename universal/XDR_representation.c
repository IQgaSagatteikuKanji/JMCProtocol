#include "XDR_representation.h"
#include <assert.h>
#include <stdlib.h>

#define BITS_IN_BYTE 8u

// Actual logic behind (only uint and opaque)
// I decided to copy the code instead of defining a macros even though its possible to do
// I personally liked it more

// Correct me if Im wrong, compiler will just unwrap loops here?

uint32_t translate_uint64_to_XDR(uint64_t *from, char *to){
    uint32_t size_of_type = sizeof(uint64_t);
    for(int i = 0; i < size_of_type; i++){
        to[i] = (char) (*from >> (size_of_type * BITS_IN_BYTE - (i+1) * BITS_IN_BYTE));
    }
    return size_of_type;
}


uint32_t translate_XDR_to_uint64(char *from, uint64_t *to){
    uint32_t size_of_type = sizeof(uint64_t);
    *to = 0u;
    for(int i = 0; i < size_of_type; i++){
        *to = (*to << BITS_IN_BYTE) + from[i];
    }
    return size_of_type;
}


uint32_t translate_uint32_to_XDR(uint32_t *from, char *to){
    uint32_t size_of_type = sizeof(uint32_t);
    for(int i = 0; i < size_of_type; i++){
        to[i] = (char) (*from >> (size_of_type * BITS_IN_BYTE - (i+1) * BITS_IN_BYTE));
    }
    return size_of_type;
}


uint32_t translate_XDR_to_uint32(char *from, uint32_t *to){
    uint32_t size_of_type = sizeof(uint32_t);
    *to = 0u;
    for(int i = 0; i < size_of_type; i++){
        *to = (*to << BITS_IN_BYTE) + from[i];
    }
    return size_of_type;
}


uint32_t translate_uint16_to_XDR(uint16_t *from, char *to){
    uint32_t size_of_type = sizeof(uint16_t);
    for(int i = 0; i < size_of_type; i++){
        to[i] = (char) (*from >> (size_of_type * BITS_IN_BYTE - (i+1) * BITS_IN_BYTE));
    }
    return size_of_type;
}


uint32_t translate_XDR_to_uint16(char *from, uint16_t *to){
    uint32_t size_of_type = sizeof(uint16_t);
    *to = 0u;
    for(int i = 0; i < size_of_type; i++){
        *to = (*to << BITS_IN_BYTE) + from[i];
    }
    return size_of_type;
}


uint32_t translate_uint8_to_XDR(uint8_t *from, char *to){
    *to = *from;
    return sizeof(uint8_t);
}


uint32_t translate_XDR_to_uint8(char *from, uint8_t *to){
    *to = *from;
    return sizeof(uint8_t);
}

#define ZERO_ALIGN_TO 4u 
uint32_t translate_fixed_length_opaque_to_XDR(char *opaque, uint32_t length, char *to){
    for(int i = 0; i < length; i++){
        to[i] = opaque[i];
    }

    uint32_t residual_zeros = 0u;
    if(RESIDUAL_ZEROS_AT_END_OF_OPAQUE){
        residual_zeros = ZERO_ALIGN_TO - length % ZERO_ALIGN_TO;
        for(uint32_t i = 0; i < residual_zeros; i++){
            to[length + i] = 0;
        }
    }

    return length + residual_zeros;
}


uint32_t translate_XDR_to_fixed_length_opaque(char *from, uint32_t length, char *opaque){
    for(int i = 0; i < length; i++){
        opaque[i] = from[i];
    }
    uint32_t residual_zeros = 0;
    if(RESIDUAL_ZEROS_AT_END_OF_OPAQUE){
        residual_zeros = ZERO_ALIGN_TO - length % ZERO_ALIGN_TO;
    }

    return length + residual_zeros;
}


uint32_t translate_variable_length_opaque_to_XDR(char *opaque, uint32_t length, char *to){
    uint32_t space = translate_uint32_to_XDR(&length, to);
    char *shifted_to = to + space;

    space += translate_fixed_length_opaque_to_XDR(opaque, length, shifted_to);
    
    return space;
} 


// I think asserts will only be here due to the fact that it deviates from the contract 
uint32_t translate_XDR_to_variable_length_opaque(char *from, char **opaque, uint32_t *length){
    assert(opaque != NULL);
    assert(*opaque == NULL);
    assert(length != NULL);
    assert(from != NULL);

    uint32_t space = translate_XDR_to_uint32(from, length);

    *opaque = calloc(*length, 1);
    space += translate_XDR_to_fixed_length_opaque(from, *length, *opaque);

    return space;
}


// Full delegation
// boolean
uint32_t translate_boolean_to_XDR(bool *boolean, char *to){
    return translate_int8_to_XDR((int8_t *) boolean, to);
}
uint32_t translate_XDR_to_boolean(char *from, bool *boolean){
    return translate_XDR_to_int8(from, (int8_t *) boolean);
}

// floating points numbers
uint32_t translate_double_to_XDR(double *from, char *to){
    return translate_uint64_to_XDR((uint64_t *)from, to);
}
uint32_t translate_XDR_to_double(char *from, double *to){
    return translate_XDR_to_uint64(from, (uint64_t *) to);
}

uint32_t translate_float_to_XDR(float *from, char *to){
    return translate_uint32_to_XDR((uint32_t *)from, to);
}
uint32_t translate_XDR_to_float(char *from, float *to){
    return translate_XDR_to_uint32(from, (uint32_t *) to);
}

// signed integers
uint32_t translate_int64_to_XDR(int64_t *from, char *to){
    return translate_uint64_to_XDR((uint64_t *)from, to);
}
uint32_t translate_XDR_to_int64(char *from, int64_t *to){
    return translate_XDR_to_uint64(from, (uint64_t *) to);
}

uint32_t translate_int32_to_XDR(int32_t *from, char *to){
    return translate_uint32_to_XDR((uint32_t *)from, to);
}
uint32_t translate_XDR_to_int32(char *from, int32_t *to){
    return translate_XDR_to_uint32(from, (uint32_t *) to);
}

uint32_t translate_int16_to_XDR(int16_t *from, char *to){
    return translate_uint16_to_XDR((uint16_t *)from, to);
}
uint32_t translate_XDR_to_int16(char *from, int16_t *to){
    return translate_XDR_to_uint16(from, (uint16_t *) to);
}

uint32_t translate_int8_to_XDR(int8_t *boolean, char *to){
    return translate_uint8_to_XDR((int8_t *) boolean, to);
}
uint32_t translate_XDR_to_int8(char *from, int8_t *boolean){
    return translate_XDR_to_int8(from, (int8_t *) boolean);
}