/*
 * Wave format tags header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_WAVE_FORMAT_TAGS_H__
#define __FORK_WAVE_FORMAT_TAGS_H__


namespace Fork
{

namespace Audio
{


/**
Wave format encoding tags.
\see http://www.onicos.com/staff/iz/formats/wav.html
*/
struct WaveFormatTags
{
    enum : unsigned short
    {
        __Unknown__                 = 0x0000, //!< Unknown Format
        PCM                         = 0x0001, //!< PCM
        ADPCM                       = 0x0002, //!< Microsoft ADPCM Format
        IEEE_FLOAT                  = 0x0003, //!< IEEE Float
        VSELP                       = 0x0004, //!< Compaq Computer's VSELP
        IBM_CSVD                    = 0x0005, //!< IBM CVSD
        ALAW                        = 0x0006, //!< ALAW
        MULAW                       = 0x0007, //!< MULAW
        OKI_ADPCM                   = 0x0010, //!< OKI ADPCM
        DVI_ADPCM                   = 0x0011, //!< Intel's DVI ADPCM
        MediaSpace_ADPCM            = 0x0012, //!< Videologic's MediaSpace ADPCM
        Sierra_ADPCM                = 0x0013, //!< Sierra ADPCM
        G723_ADPCM                  = 0x0014, //!< G.723 ADPCM
        DIGISTD                     = 0x0015, //!< DSP Solution's DIGISTD
        DIGIFIX                     = 0x0016, //!< DSP Solution's DIGIFIX
        Dialogic_OKI_ADPCM          = 0x0017, //!< Dialogic OKI ADPCM
        MediaVision_ADPCM           = 0x0018, //!< MediaVision ADPCM
        CU_CODEC                    = 0x0019, //!< HP CU
        Yamaha_ADPCM                = 0x0020, //!< Yamaha ADPCM
        Sonarc                      = 0x0021, //!< Speech Compression's Sonarc
        TrueSpeech                  = 0x0022, //!< DSP Group's True Speech
        EchoSC1                     = 0x0023, //!< Echo Speech's EchoSC1
        AudioFile_AF36              = 0x0024, //!< AudioFile AF36
        APTX                        = 0x0025, //!< APTX
        AudioFile_AF10              = 0x0026, //!< AudioFile AF10
        Prosody_1612                = 0x0027, //!< Prosody 1612
        LRC                         = 0x0028, //!< LRC
        AC2                         = 0x0030, //!< Dolby AC2
        GSM610                      = 0x0031, //!< GSM610
        MSNAudio                    = 0x0032, //!< MSNAudio
        Antex_ADPCME                = 0x0033, //!< Antex ADPCME
        Control_Res_VQLPC           = 0x0034, //!< Control Res VQLPC
        Digireal                    = 0x0035, //!< Digireal
        DigiADPCM                   = 0x0036, //!< DigiADPCM
        Control_Res_CR10            = 0x0037, //!< Control Res CR10
        VBXADPCM                    = 0x0038, //!< NMS VBXADPCM
        Roland_RDAC                 = 0x0039, //!< Roland RDAC
        ECHOSC3                     = 0x003A, //!< EchoSC3
        Rockwell_ADPCM              = 0x003B, //!< Rockwell ADPCM
        Rockwell_Digit_LK           = 0x003C, //!< Rockwell Digit LK
        Xebec                       = 0x003D, //!< Xebec
        G721_ADPCM                  = 0x0040, //!< Antex Electronics G.721
        G728_CELP                   = 0x0041, //!< G.728 CELP
        MSG723                      = 0x0042, //!< MSG723
        MPEG                        = 0x0050, //!< MPEG Layer 1,2
        RT24                        = 0x0051, //!< RT24
        PAC                         = 0x0051, //!< PAC
        MPEGLAYER3                  = 0x0055, //!< MPEG Layer 3
        Cirrus                      = 0x0059, //!< Cirrus
        ESPCM                       = 0x0061, //!< ESPCM
        Voxware                     = 0x0062, //!< Voxware (obsolete)
        Canopus_Atrac               = 0x0063, //!< Canopus Atrac
        G726_ADPCM                  = 0x0064, //!< G.726 ADPCM
        G722_ADPCM                  = 0x0065, //!< G.722 ADPCM
        DSAT                        = 0x0066, //!< DSAT
        DSAT_DISPLAY                = 0x0067, //!< DSAT Display
        Voxware_Byte_Aligned        = 0x0069, //!< Voxware Byte Aligned (obsolete)
        Voxware_AC8                 = 0x0070, //!< Voxware AC8 (obsolete)
        Voxware_AC10                = 0x0071, //!< Voxware AC10 (obsolete)
        Voxware_AC16                = 0x0072, //!< Voxware AC16 (obsolete)
        Voxware_AC20                = 0x0073, //!< Voxware AC20 (obsolete)
        Voxware_RT24                = 0x0074, //!< Voxware MetaVoice (obsolete)
        Voxware_RT29                = 0x0075, //!< Voxware MetaSound (obsolete)
        Voxware_RT29HW              = 0x0076, //!< Voxware RT29HW (obsolete)
        Voxware_VR12                = 0x0077, //!< Voxware VR12 (obsolete)
        Voxware_VR18                = 0x0078, //!< Voxware VR18 (obsolete)
        Voxware_TQ40                = 0x0079, //!< Voxware TQ40 (obsolete)
        Softsound                   = 0x0080, //!< Softsound
        Voxware_TQ60                = 0x0081, //!< Voxware TQ60 (obsolete)
        MSRT24                      = 0x0082, //!< MSRT24
        G729A                       = 0x0083, //!< G.729A
        MVI_MV12                    = 0x0084, //!< MVI MV12
        DF_G726                     = 0x0085, //!< DF G.726
        DF_GSM610                   = 0x0086, //!< DF GSM610
        ISIAudio                    = 0x0088, //!< ISIAudio
        Onlive                      = 0x0089, //!< Onlive
        SBC24                       = 0x0091, //!< SBC24
        Dolby_AC3_SPDIF             = 0x0092, //!< Dolby AC3 SPDIF
        ZyXEL_ADPCM                 = 0x0097, //!< ZyXEL ADPCM
        Philips_LPCBB               = 0x0098, //!< Philips LPCBB
        Packed                      = 0x0099, //!< Packed
        Rhetorex_ADPCM              = 0x0100, //!< Rhetorex ADPCM
        IRAT                        = 0x0101, //!< BeCubed Software's IRAT
        Vivo_G723                   = 0x0111, //!< Vivo G.723
        Vivo_Siren                  = 0x0112, //!< Vivo Siren
        Digital_G723                = 0x0123, //!< Digital G.723
        Creative_ADPCM              = 0x0200, //!< Creative ADPCM
        Creative_FastSpeech8        = 0x0202, //!< Creative FastSpeech8
        Creative_FastSpeech10       = 0x0203, //!< Creative FastSpeech10
        Quarterdeck                 = 0x0220, //!< Quarterdeck
        FM_TOWNS_SND                = 0x0300, //!< FM Towns Snd
        BTV_DIGITAL                 = 0x0400, //!< BTV Digital
        VME_VMPCM                   = 0x0680, //!< VME VMPCM
        OLIGSM                      = 0x1000, //!< OLIGSM
        OLIADPCM                    = 0x1001, //!< OLIADPCM
        OLICELP                     = 0x1002, //!< OLICELP
        OLISBC                      = 0x1003, //!< OLISBC
        OLIOPR                      = 0x1004, //!< OLIOPR
        LH_CODEC                    = 0x1100, //!< LH Codec
        Norris                      = 0x1400, //!< Norris
        Soundspace_MusicCompression = 0x1500, //!< Soundspace Music Compression
        DVM                         = 0x2000, //!< DVM
        Extensible                  = 0xfffe, //!< SubFormat
        Development                 = 0xffff, //!< Development
    };
};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================