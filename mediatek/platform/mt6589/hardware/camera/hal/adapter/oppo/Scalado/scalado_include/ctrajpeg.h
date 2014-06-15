/*  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
 .                                                                             .
 .    (c) 2000-2009 Scalado AB. All rights reserved.                           .
 .                                                                             .
 .    Technologies used in this source code are patented or patent pending     .
 .    by Scalado AB Swedish Org. Number, 556594-6885.                          .
 .                                                                             .
 .    All Intellectual Property Rights related to this source code,            .
 .    belongs to Scalado AB.                                                   .
 .                                                                             .
 .    This source code is furnished under license agreement and may be used    .
 .    or copied only in accordance with terms of such license agreement.       .
 .                                                                             .
 .    Except as permitted by such license agreement, no part of this source    .
 .    code may be reproduced, stored in a retrieval system, or transmitted,    .
 .    in any form or by any means, electronic, mechanical, recording, or       .
 .    otherwise, without the prior written permission of Scalado.              .
 .                                                                             .
 .    Scalado assumes no responsibility or liability for any errors or         .
 .    inaccuracies in this source code or any consequential, incidental or     .
 .    indirect damage arising out of the use of this source code.              .
 .                                                                             .
 .    Scalado and the Scalado logo are either registered trademarks or         .
 .    trademarks of Scalado AB in Sweden and/or other countries.               .
 .                                                                             .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

#ifndef CTRAJPEG_H
#define CTRAJPEG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cidecoderfactory.h"
#include "ctdecoderfactory.h"
#include "ctinterfaceid.h"
#include "ctencoder.h"

/**
    Interface used to create, or directly reference, the rajpeg decoder
    factory interface.

*/
extern const CIDecoderFactory rajpegFactoryInterface;


/**
    Interface id used to retrieve a rajpeg_encoder type from the ctencoder
    using interfaceContext.

    @sa ctdecoder_interfaceContext
*/
CTINTERFACEID_DECLARE( RAJPEG_ENCODER_IID );


/**
    @brief
    Creates a factory capable of producing rajpeg decoders.

    @param[out] ppFactory
    Pointer to a reference to the factory.

    @return
    @c CERR_OK or a specific error code on failure.

    @since 3.8.2 R01
*/
CRESULT ctrajpeg_createDecoderFactory( CTDecoderFactory **ppFactory );

/**
    @brief
    Creates a factory capable of producing rajpeg decoders with JPS image properties.

    @param[out] ppFactory
    Pointer to a reference to the factory.

    @return
    @c CERR_OK or a specific error code on failure.

    @since 3.10
*/
CRESULT ctrajpeg_createJpsDecoderFactory( CTDecoderFactory **ppFactory );

/**
    @brief Creates a rajpeg encoder.

    @param pStream Output stream that encoded jpeg data will be written to.
    @param[out] ppEncoder Pointer to the encoder to be created by this call.

    @return
    @c CERR_OK or a specific error code on failure.

    @since 3.8.2 R01
*/
CRESULT ctrajpeg_createEncoder( CTStream *pStream, CTEncoder **ppEncoder );


#ifdef __cplusplus
}           // extern "C"
#endif      // __cplusplus
#endif      // CTRAJPEG_H
