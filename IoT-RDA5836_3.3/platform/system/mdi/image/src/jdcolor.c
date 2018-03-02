/*
 * jdcolor.c
 *
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains output colorspace conversion routines.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


/* Private subobject */

typedef struct {
  struct jpeg_color_deconverter pub; /* public fields */

  /* Private state for YCC->RGB conversion */
  int * Cr_r_tab;		/* => table for Cr to R conversion */
  int * Cb_b_tab;		/* => table for Cb to B conversion */
  INT32 * Cr_g_tab;		/* => table for Cr to G conversion */
  INT32 * Cb_g_tab;		/* => table for Cb to G conversion */
} my_color_deconverter;

typedef my_color_deconverter * my_cconvert_ptr;


/**************** YCbCr -> RGB conversion: most common case **************/

/*
 * YCbCr is defined per CCIR 601-1, except that Cb and Cr are
 * normalized to the range 0..MAXJSAMPLE rather than -0.5 .. 0.5.
 * The conversion equations to be implemented are therefore
 *	R = Y                + 1.40200 * Cr
 *	G = Y - 0.34414 * Cb - 0.71414 * Cr
 *	B = Y + 1.77200 * Cb
 * where Cb and Cr represent the incoming values less CENTERJSAMPLE.
 * (These numbers are derived from TIFF 6.0 section 21, dated 3-June-92.)
 *
 * To avoid floating-point arithmetic, we represent the fractional constants
 * as integers scaled up by 2^16 (about 4 digits precision); we have to divide
 * the products by 2^16, with appropriate rounding, to get the correct answer.
 * Notice that Y, being an integral input, does not contribute any fraction
 * so it need not participate in the rounding.
 *
 * For even more speed, we avoid doing any multiplications in the inner loop
 * by precalculating the constants times Cb and Cr for all possible values.
 * For 8-bit JSAMPLEs this is very reasonable (only 256 entries per table);
 * for 12-bit samples it is still acceptable.  It's not very reasonable for
 * 16-bit samples, but if you want lossless storage you shouldn't be changing
 * colorspace anyway.
 * The Cr=>R and Cb=>B values can be rounded to integers in advance; the
 * values for the G calculation are left scaled up, since we must add them
 * together before rounding.
 */

#define SCALEBITS	16	/* speediest right-shift on some machines */
#define ONE_HALF	((INT32) 1 << (SCALEBITS-1))
#define FIX(x)		((INT32) ((x) * (1L<<SCALEBITS) + 0.5))

const int Cr_r_tab[256]={
0xffffff4d  ,0xffffff4e  ,0xffffff4f  ,0xffffff51  ,0xffffff52  ,0xffffff54  ,0xffffff55  ,0xffffff56  ,
0xffffff58  ,0xffffff59  ,0xffffff5b  ,0xffffff5c  ,0xffffff5d  ,0xffffff5f  ,0xffffff60  ,0xffffff62  ,
0xffffff63  ,0xffffff64  ,0xffffff66  ,0xffffff67  ,0xffffff69  ,0xffffff6a  ,0xffffff6b  ,0xffffff6d  ,
0xffffff6e  ,0xffffff70  ,0xffffff71  ,0xffffff72  ,0xffffff74  ,0xffffff75  ,0xffffff77  ,0xffffff78  ,
0xffffff79  ,0xffffff7b  ,0xffffff7c  ,0xffffff7e  ,0xffffff7f  ,0xffffff80  ,0xffffff82  ,0xffffff83  ,
0xffffff85  ,0xffffff86  ,0xffffff87  ,0xffffff89  ,0xffffff8a  ,0xffffff8c  ,0xffffff8d  ,0xffffff8e  ,
0xffffff90  ,0xffffff91  ,0xffffff93  ,0xffffff94  ,0xffffff95  ,0xffffff97  ,0xffffff98  ,0xffffff9a  ,
0xffffff9b  ,0xffffff9c  ,0xffffff9e  ,0xffffff9f  ,0xffffffa1  ,0xffffffa2  ,0xffffffa3  ,0xffffffa5  ,
0xffffffa6  ,0xffffffa8  ,0xffffffa9  ,0xffffffaa  ,0xffffffac  ,0xffffffad  ,0xffffffaf  ,0xffffffb0  ,
0xffffffb1  ,0xffffffb3  ,0xffffffb4  ,0xffffffb6  ,0xffffffb7  ,0xffffffb8  ,0xffffffba  ,0xffffffbb  ,
0xffffffbd  ,0xffffffbe  ,0xffffffc0  ,0xffffffc1  ,0xffffffc2  ,0xffffffc4  ,0xffffffc5  ,0xffffffc7  ,
0xffffffc8  ,0xffffffc9  ,0xffffffcb  ,0xffffffcc  ,0xffffffce  ,0xffffffcf  ,0xffffffd0  ,0xffffffd2  ,
0xffffffd3  ,0xffffffd5  ,0xffffffd6  ,0xffffffd7  ,0xffffffd9  ,0xffffffda  ,0xffffffdc  ,0xffffffdd  ,
0xffffffde  ,0xffffffe0  ,0xffffffe1  ,0xffffffe3  ,0xffffffe4  ,0xffffffe5  ,0xffffffe7  ,0xffffffe8  ,
0xffffffea  ,0xffffffeb  ,0xffffffec  ,0xffffffee  ,0xffffffef  ,0xfffffff1  ,0xfffffff2  ,0xfffffff3  ,
0xfffffff5  ,0xfffffff6  ,0xfffffff8  ,0xfffffff9  ,0xfffffffa  ,0xfffffffc  ,0xfffffffd  ,0xffffffff  ,
0x0  ,0x1  ,0x3  ,0x4  ,0x6  ,0x7  ,0x8  ,0xa  ,
0xb  ,0xd  ,0xe  ,0xf  ,0x11  ,0x12  ,0x14  ,0x15  ,
0x16  ,0x18  ,0x19  ,0x1b  ,0x1c  ,0x1d  ,0x1f  ,0x20  ,
0x22  ,0x23  ,0x24  ,0x26  ,0x27  ,0x29  ,0x2a  ,0x2b  ,
0x2d  ,0x2e  ,0x30  ,0x31  ,0x32  ,0x34  ,0x35  ,0x37  ,
0x38  ,0x39  ,0x3b  ,0x3c  ,0x3e  ,0x3f  ,0x40  ,0x42  ,
0x43  ,0x45  ,0x46  ,0x48  ,0x49  ,0x4a  ,0x4c  ,0x4d  ,
0x4f  ,0x50  ,0x51  ,0x53  ,0x54  ,0x56  ,0x57  ,0x58  ,
0x5a  ,0x5b  ,0x5d  ,0x5e  ,0x5f  ,0x61  ,0x62  ,0x64  ,
0x65  ,0x66  ,0x68  ,0x69  ,0x6b  ,0x6c  ,0x6d  ,0x6f  ,
0x70  ,0x72  ,0x73  ,0x74  ,0x76  ,0x77  ,0x79  ,0x7a  ,
0x7b  ,0x7d  ,0x7e  ,0x80  ,0x81  ,0x82  ,0x84  ,0x85  ,
0x87  ,0x88  ,0x89  ,0x8b  ,0x8c  ,0x8e  ,0x8f  ,0x90  ,
0x92  ,0x93  ,0x95  ,0x96  ,0x97  ,0x99  ,0x9a  ,0x9c  ,
0x9d  ,0x9e  ,0xa0  ,0xa1  ,0xa3  ,0xa4  ,0xa5  ,0xa7  ,
0xa8  ,0xaa  ,0xab  ,0xac  ,0xae  ,0xaf  ,0xb1  ,0xb2  ,
};
const int Cb_b_tab[256]={
0xffffff1d  ,0xffffff1f  ,0xffffff21  ,0xffffff22  ,0xffffff24  ,0xffffff26  ,0xffffff28  ,0xffffff2a  ,
0xffffff2b  ,0xffffff2d  ,0xffffff2f  ,0xffffff31  ,0xffffff32  ,0xffffff34  ,0xffffff36  ,0xffffff38  ,
0xffffff3a  ,0xffffff3b  ,0xffffff3d  ,0xffffff3f  ,0xffffff41  ,0xffffff42  ,0xffffff44  ,0xffffff46  ,
0xffffff48  ,0xffffff49  ,0xffffff4b  ,0xffffff4d  ,0xffffff4f  ,0xffffff51  ,0xffffff52  ,0xffffff54  ,
0xffffff56  ,0xffffff58  ,0xffffff59  ,0xffffff5b  ,0xffffff5d  ,0xffffff5f  ,0xffffff61  ,0xffffff62  ,
0xffffff64  ,0xffffff66  ,0xffffff68  ,0xffffff69  ,0xffffff6b  ,0xffffff6d  ,0xffffff6f  ,0xffffff70  ,
0xffffff72  ,0xffffff74  ,0xffffff76  ,0xffffff78  ,0xffffff79  ,0xffffff7b  ,0xffffff7d  ,0xffffff7f  ,
0xffffff80  ,0xffffff82  ,0xffffff84  ,0xffffff86  ,0xffffff88  ,0xffffff89  ,0xffffff8b  ,0xffffff8d  ,
0xffffff8f  ,0xffffff90  ,0xffffff92  ,0xffffff94  ,0xffffff96  ,0xffffff97  ,0xffffff99  ,0xffffff9b  ,
0xffffff9d  ,0xffffff9f  ,0xffffffa0  ,0xffffffa2  ,0xffffffa4  ,0xffffffa6  ,0xffffffa7  ,0xffffffa9  ,
0xffffffab  ,0xffffffad  ,0xffffffae  ,0xffffffb0  ,0xffffffb2  ,0xffffffb4  ,0xffffffb6  ,0xffffffb7  ,
0xffffffb9  ,0xffffffbb  ,0xffffffbd  ,0xffffffbe  ,0xffffffc0  ,0xffffffc2  ,0xffffffc4  ,0xffffffc6  ,
0xffffffc7  ,0xffffffc9  ,0xffffffcb  ,0xffffffcd  ,0xffffffce  ,0xffffffd0  ,0xffffffd2  ,0xffffffd4  ,
0xffffffd5  ,0xffffffd7  ,0xffffffd9  ,0xffffffdb  ,0xffffffdd  ,0xffffffde  ,0xffffffe0  ,0xffffffe2  ,
0xffffffe4  ,0xffffffe5  ,0xffffffe7  ,0xffffffe9  ,0xffffffeb  ,0xffffffed  ,0xffffffee  ,0xfffffff0  ,
0xfffffff2  ,0xfffffff4  ,0xfffffff5  ,0xfffffff7  ,0xfffffff9  ,0xfffffffb  ,0xfffffffc  ,0xfffffffe  ,
0x0  ,0x2  ,0x4  ,0x5  ,0x7  ,0x9  ,0xb  ,0xc  ,
0xe  ,0x10  ,0x12  ,0x13  ,0x15  ,0x17  ,0x19  ,0x1b  ,
0x1c  ,0x1e  ,0x20  ,0x22  ,0x23  ,0x25  ,0x27  ,0x29  ,
0x2b  ,0x2c  ,0x2e  ,0x30  ,0x32  ,0x33  ,0x35  ,0x37  ,
0x39  ,0x3a  ,0x3c  ,0x3e  ,0x40  ,0x42  ,0x43  ,0x45  ,
0x47  ,0x49  ,0x4a  ,0x4c  ,0x4e  ,0x50  ,0x52  ,0x53  ,
0x55  ,0x57  ,0x59  ,0x5a  ,0x5c  ,0x5e  ,0x60  ,0x61  ,
0x63  ,0x65  ,0x67  ,0x69  ,0x6a  ,0x6c  ,0x6e  ,0x70  ,
0x71  ,0x73  ,0x75  ,0x77  ,0x78  ,0x7a  ,0x7c  ,0x7e  ,
0x80  ,0x81  ,0x83  ,0x85  ,0x87  ,0x88  ,0x8a  ,0x8c  ,
0x8e  ,0x90  ,0x91  ,0x93  ,0x95  ,0x97  ,0x98  ,0x9a  ,
0x9c  ,0x9e  ,0x9f  ,0xa1  ,0xa3  ,0xa5  ,0xa7  ,0xa8  ,
0xaa  ,0xac  ,0xae  ,0xaf  ,0xb1  ,0xb3  ,0xb5  ,0xb7  ,
0xb8  ,0xba  ,0xbc  ,0xbe  ,0xbf  ,0xc1  ,0xc3  ,0xc5  ,
0xc6  ,0xc8  ,0xca  ,0xcc  ,0xce  ,0xcf  ,0xd1  ,0xd3  ,
0xd5  ,0xd6  ,0xd8  ,0xda  ,0xdc  ,0xde  ,0xdf  ,0xe1  ,
};
const int Cr_g_tab[256]={
0x5b6900  ,0x5ab22e  ,0x59fb5c  ,0x59448a  ,0x588db8  ,0x57d6e6  ,0x572014  ,0x566942  ,
0x55b270  ,0x54fb9e  ,0x5444cc  ,0x538dfa  ,0x52d728  ,0x522056  ,0x516984  ,0x50b2b2  ,
0x4ffbe0  ,0x4f450e  ,0x4e8e3c  ,0x4dd76a  ,0x4d2098  ,0x4c69c6  ,0x4bb2f4  ,0x4afc22  ,
0x4a4550  ,0x498e7e  ,0x48d7ac  ,0x4820da  ,0x476a08  ,0x46b336  ,0x45fc64  ,0x454592  ,
0x448ec0  ,0x43d7ee  ,0x43211c  ,0x426a4a  ,0x41b378  ,0x40fca6  ,0x4045d4  ,0x3f8f02  ,
0x3ed830  ,0x3e215e  ,0x3d6a8c  ,0x3cb3ba  ,0x3bfce8  ,0x3b4616  ,0x3a8f44  ,0x39d872  ,
0x3921a0  ,0x386ace  ,0x37b3fc  ,0x36fd2a  ,0x364658  ,0x358f86  ,0x34d8b4  ,0x3421e2  ,
0x336b10  ,0x32b43e  ,0x31fd6c  ,0x31469a  ,0x308fc8  ,0x2fd8f6  ,0x2f2224  ,0x2e6b52  ,
0x2db480  ,0x2cfdae  ,0x2c46dc  ,0x2b900a  ,0x2ad938  ,0x2a2266  ,0x296b94  ,0x28b4c2  ,
0x27fdf0  ,0x27471e  ,0x26904c  ,0x25d97a  ,0x2522a8  ,0x246bd6  ,0x23b504  ,0x22fe32  ,
0x224760  ,0x21908e  ,0x20d9bc  ,0x2022ea  ,0x1f6c18  ,0x1eb546  ,0x1dfe74  ,0x1d47a2  ,
0x1c90d0  ,0x1bd9fe  ,0x1b232c  ,0x1a6c5a  ,0x19b588  ,0x18feb6  ,0x1847e4  ,0x179112  ,
0x16da40  ,0x16236e  ,0x156c9c  ,0x14b5ca  ,0x13fef8  ,0x134826  ,0x129154  ,0x11da82  ,
0x1123b0  ,0x106cde  ,0xfb60c  ,0xeff3a  ,0xe4868  ,0xd9196  ,0xcdac4  ,0xc23f2  ,
0xb6d20  ,0xab64e  ,0x9ff7c  ,0x948aa  ,0x891d8  ,0x7db06  ,0x72434  ,0x66d62  ,
0x5b690  ,0x4ffbe  ,0x448ec  ,0x3921a  ,0x2db48  ,0x22476  ,0x16da4  ,0xb6d2  ,
0x0  ,0xffff492e  ,0xfffe925c  ,0xfffddb8a  ,0xfffd24b8  ,0xfffc6de6  ,0xfffbb714  ,0xfffb0042  ,
0xfffa4970  ,0xfff9929e  ,0xfff8dbcc  ,0xfff824fa  ,0xfff76e28  ,0xfff6b756  ,0xfff60084  ,0xfff549b2  ,
0xfff492e0  ,0xfff3dc0e  ,0xfff3253c  ,0xfff26e6a  ,0xfff1b798  ,0xfff100c6  ,0xfff049f4  ,0xffef9322  ,
0xffeedc50  ,0xffee257e  ,0xffed6eac  ,0xffecb7da  ,0xffec0108  ,0xffeb4a36  ,0xffea9364  ,0xffe9dc92  ,
0xffe925c0  ,0xffe86eee  ,0xffe7b81c  ,0xffe7014a  ,0xffe64a78  ,0xffe593a6  ,0xffe4dcd4  ,0xffe42602  ,
0xffe36f30  ,0xffe2b85e  ,0xffe2018c  ,0xffe14aba  ,0xffe093e8  ,0xffdfdd16  ,0xffdf2644  ,0xffde6f72  ,
0xffddb8a0  ,0xffdd01ce  ,0xffdc4afc  ,0xffdb942a  ,0xffdadd58  ,0xffda2686  ,0xffd96fb4  ,0xffd8b8e2  ,
0xffd80210  ,0xffd74b3e  ,0xffd6946c  ,0xffd5dd9a  ,0xffd526c8  ,0xffd46ff6  ,0xffd3b924  ,0xffd30252  ,
0xffd24b80  ,0xffd194ae  ,0xffd0dddc  ,0xffd0270a  ,0xffcf7038  ,0xffceb966  ,0xffce0294  ,0xffcd4bc2  ,
0xffcc94f0  ,0xffcbde1e  ,0xffcb274c  ,0xffca707a  ,0xffc9b9a8  ,0xffc902d6  ,0xffc84c04  ,0xffc79532  ,
0xffc6de60  ,0xffc6278e  ,0xffc570bc  ,0xffc4b9ea  ,0xffc40318  ,0xffc34c46  ,0xffc29574  ,0xffc1dea2  ,
0xffc127d0  ,0xffc070fe  ,0xffbfba2c  ,0xffbf035a  ,0xffbe4c88  ,0xffbd95b6  ,0xffbcdee4  ,0xffbc2812  ,
0xffbb7140  ,0xffbaba6e  ,0xffba039c  ,0xffb94cca  ,0xffb895f8  ,0xffb7df26  ,0xffb72854  ,0xffb67182  ,
0xffb5bab0  ,0xffb503de  ,0xffb44d0c  ,0xffb3963a  ,0xffb2df68  ,0xffb22896  ,0xffb171c4  ,0xffb0baf2  ,
0xffb00420  ,0xffaf4d4e  ,0xffae967c  ,0xffaddfaa  ,0xffad28d8  ,0xffac7206  ,0xffabbb34  ,0xffab0462  ,
0xffaa4d90  ,0xffa996be  ,0xffa8dfec  ,0xffa8291a  ,0xffa77248  ,0xffa6bb76  ,0xffa604a4  ,0xffa54dd2  ,
};
const int Cb_g_tab[256]={
0x2c8d00  ,0x2c34e6  ,0x2bdccc  ,0x2b84b2  ,0x2b2c98  ,0x2ad47e  ,0x2a7c64  ,0x2a244a  ,
0x29cc30  ,0x297416  ,0x291bfc  ,0x28c3e2  ,0x286bc8  ,0x2813ae  ,0x27bb94  ,0x27637a  ,
0x270b60  ,0x26b346  ,0x265b2c  ,0x260312  ,0x25aaf8  ,0x2552de  ,0x24fac4  ,0x24a2aa  ,
0x244a90  ,0x23f276  ,0x239a5c  ,0x234242  ,0x22ea28  ,0x22920e  ,0x2239f4  ,0x21e1da  ,
0x2189c0  ,0x2131a6  ,0x20d98c  ,0x208172  ,0x202958  ,0x1fd13e  ,0x1f7924  ,0x1f210a  ,
0x1ec8f0  ,0x1e70d6  ,0x1e18bc  ,0x1dc0a2  ,0x1d6888  ,0x1d106e  ,0x1cb854  ,0x1c603a  ,
0x1c0820  ,0x1bb006  ,0x1b57ec  ,0x1affd2  ,0x1aa7b8  ,0x1a4f9e  ,0x19f784  ,0x199f6a  ,
0x194750  ,0x18ef36  ,0x18971c  ,0x183f02  ,0x17e6e8  ,0x178ece  ,0x1736b4  ,0x16de9a  ,
0x168680  ,0x162e66  ,0x15d64c  ,0x157e32  ,0x152618  ,0x14cdfe  ,0x1475e4  ,0x141dca  ,
0x13c5b0  ,0x136d96  ,0x13157c  ,0x12bd62  ,0x126548  ,0x120d2e  ,0x11b514  ,0x115cfa  ,
0x1104e0  ,0x10acc6  ,0x1054ac  ,0xffc92  ,0xfa478  ,0xf4c5e  ,0xef444  ,0xe9c2a  ,
0xe4410  ,0xdebf6  ,0xd93dc  ,0xd3bc2  ,0xce3a8  ,0xc8b8e  ,0xc3374  ,0xbdb5a  ,
0xb8340  ,0xb2b26  ,0xad30c  ,0xa7af2  ,0xa22d8  ,0x9cabe  ,0x972a4  ,0x91a8a  ,
0x8c270  ,0x86a56  ,0x8123c  ,0x7ba22  ,0x76208  ,0x709ee  ,0x6b1d4  ,0x659ba  ,
0x601a0  ,0x5a986  ,0x5516c  ,0x4f952  ,0x4a138  ,0x4491e  ,0x3f104  ,0x398ea  ,
0x340d0  ,0x2e8b6  ,0x2909c  ,0x23882  ,0x1e068  ,0x1884e  ,0x13034  ,0xd81a  ,
0x8000  ,0x27e6  ,0xffffcfcc  ,0xffff77b2  ,0xffff1f98  ,0xfffec77e  ,0xfffe6f64  ,0xfffe174a  ,
0xfffdbf30  ,0xfffd6716  ,0xfffd0efc  ,0xfffcb6e2  ,0xfffc5ec8  ,0xfffc06ae  ,0xfffbae94  ,0xfffb567a  ,
0xfffafe60  ,0xfffaa646  ,0xfffa4e2c  ,0xfff9f612  ,0xfff99df8  ,0xfff945de  ,0xfff8edc4  ,0xfff895aa  ,
0xfff83d90  ,0xfff7e576  ,0xfff78d5c  ,0xfff73542  ,0xfff6dd28  ,0xfff6850e  ,0xfff62cf4  ,0xfff5d4da  ,
0xfff57cc0  ,0xfff524a6  ,0xfff4cc8c  ,0xfff47472  ,0xfff41c58  ,0xfff3c43e  ,0xfff36c24  ,0xfff3140a  ,
0xfff2bbf0  ,0xfff263d6  ,0xfff20bbc  ,0xfff1b3a2  ,0xfff15b88  ,0xfff1036e  ,0xfff0ab54  ,0xfff0533a  ,
0xffeffb20  ,0xffefa306  ,0xffef4aec  ,0xffeef2d2  ,0xffee9ab8  ,0xffee429e  ,0xffedea84  ,0xffed926a  ,
0xffed3a50  ,0xffece236  ,0xffec8a1c  ,0xffec3202  ,0xffebd9e8  ,0xffeb81ce  ,0xffeb29b4  ,0xffead19a  ,
0xffea7980  ,0xffea2166  ,0xffe9c94c  ,0xffe97132  ,0xffe91918  ,0xffe8c0fe  ,0xffe868e4  ,0xffe810ca  ,
0xffe7b8b0  ,0xffe76096  ,0xffe7087c  ,0xffe6b062  ,0xffe65848  ,0xffe6002e  ,0xffe5a814  ,0xffe54ffa  ,
0xffe4f7e0  ,0xffe49fc6  ,0xffe447ac  ,0xffe3ef92  ,0xffe39778  ,0xffe33f5e  ,0xffe2e744  ,0xffe28f2a  ,
0xffe23710  ,0xffe1def6  ,0xffe186dc  ,0xffe12ec2  ,0xffe0d6a8  ,0xffe07e8e  ,0xffe02674  ,0xffdfce5a  ,
0xffdf7640  ,0xffdf1e26  ,0xffdec60c  ,0xffde6df2  ,0xffde15d8  ,0xffddbdbe  ,0xffdd65a4  ,0xffdd0d8a  ,
0xffdcb570  ,0xffdc5d56  ,0xffdc053c  ,0xffdbad22  ,0xffdb5508  ,0xffdafcee  ,0xffdaa4d4  ,0xffda4cba  ,
0xffd9f4a0  ,0xffd99c86  ,0xffd9446c  ,0xffd8ec52  ,0xffd89438  ,0xffd83c1e  ,0xffd7e404  ,0xffd78bea  ,
0xffd733d0  ,0xffd6dbb6  ,0xffd6839c  ,0xffd62b82  ,0xffd5d368  ,0xffd57b4e  ,0xffd52334  ,0xffd4cb1a  ,
};
/*
 * Initialize tables for YCC->RGB colorspace conversion.
 */

LOCAL(void)
build_ycc_rgb_table (j_decompress_ptr cinfo)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;

//  FILE*  fp=fopen("data.dat","wb");

  cconvert->Cr_r_tab = (int *)Cr_r_tab;
    
  cconvert->Cb_b_tab = (int *)Cb_b_tab;
   
  cconvert->Cr_g_tab = (INT32 *)Cr_g_tab;
  
  cconvert->Cb_g_tab = (INT32 *)Cb_g_tab;
}


/*
 * Convert some rows of samples to the output colorspace.
 *
 * Note that we change from noninterleaved, one-plane-per-component format
 * to interleaved-pixel format.  The output buffer is therefore three times
 * as wide as the input buffer.
 * A starting row offset is provided only for the input buffer.  The caller
 * can easily adjust the passed output_buf value to accommodate any row
 * offset required on that side.
 */

METHODDEF(void)
ycc_rgb_convert (j_decompress_ptr cinfo,
		 JSAMPIMAGE input_buf, JDIMENSION input_row,
		 JSAMPARRAY output_buf, int num_rows)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;
  register int y, cb, cr;
  register JSAMPROW outptr;
  register JSAMPROW inptr0, inptr1, inptr2;
  register JDIMENSION col;
  JDIMENSION num_cols = cinfo->output_width;
  /* copy these pointers into registers if possible */
  const JSAMPLE * range_limit = cinfo->sample_range_limit;
  register int * Crrtab = cconvert->Cr_r_tab;
  register int * Cbbtab = cconvert->Cb_b_tab;
  register INT32 * Crgtab = cconvert->Cr_g_tab;
  register INT32 * Cbgtab = cconvert->Cb_g_tab;
  SHIFT_TEMPS

  while (--num_rows >= 0) {
    inptr0 = input_buf[0][input_row];
    inptr1 = input_buf[1][input_row];
    inptr2 = input_buf[2][input_row];
    input_row++;
    outptr = *output_buf++;
    for (col = 0; col < num_cols; col++) {
      y  = GETJSAMPLE(inptr0[col]);
      cb = GETJSAMPLE(inptr1[col]);
      cr = GETJSAMPLE(inptr2[col]);
      /* Range-limiting is essential due to noise introduced by DCT losses. */
      outptr[RGB_RED] =   range_limit[y + Crrtab[cr]];
      outptr[RGB_GREEN] = range_limit[y +
			      ((int) RIGHT_SHIFT(Cbgtab[cb] + Crgtab[cr],
						 SCALEBITS))];
      outptr[RGB_BLUE] =  range_limit[y + Cbbtab[cb]];
      outptr += RGB_PIXELSIZE;
    }
  }
}


/**************** Cases other than YCbCr -> RGB **************/


/*
 * Color conversion for no colorspace change: just copy the data,
 * converting from separate-planes to interleaved representation.
 */

METHODDEF(void)
null_convert (j_decompress_ptr cinfo,
	      JSAMPIMAGE input_buf, JDIMENSION input_row,
	      JSAMPARRAY output_buf, int num_rows)
{
  register JSAMPROW inptr, outptr;
  register JDIMENSION count;
  register int num_components = cinfo->num_components;
  JDIMENSION num_cols = cinfo->output_width;
  int ci;

  while (--num_rows >= 0) {
    for (ci = 0; ci < num_components; ci++) {
      inptr = input_buf[ci][input_row];
      outptr = output_buf[0] + ci;
      for (count = num_cols; count > 0; count--) {
	*outptr = *inptr++;	/* needn't bother with GETJSAMPLE() here */
	outptr += num_components;
      }
    }
    input_row++;
    output_buf++;
  }
}


/*
 * Color conversion for grayscale: just copy the data.
 * This also works for YCbCr -> grayscale conversion, in which
 * we just copy the Y (luminance) component and ignore chrominance.
 */

METHODDEF(void)
grayscale_convert (j_decompress_ptr cinfo,
		   JSAMPIMAGE input_buf, JDIMENSION input_row,
		   JSAMPARRAY output_buf, int num_rows)
{
  jcopy_sample_rows(input_buf[0], (int) input_row, output_buf, 0,
		    num_rows, cinfo->output_width);
}


/*
 * Convert grayscale to RGB: just duplicate the graylevel three times.
 * This is provided to support applications that don't want to cope
 * with grayscale as a separate case.
 */

METHODDEF(void)
gray_rgb_convert (j_decompress_ptr cinfo,
		  JSAMPIMAGE input_buf, JDIMENSION input_row,
		  JSAMPARRAY output_buf, int num_rows)
{
  register JSAMPROW inptr, outptr;
  register JDIMENSION col;
  JDIMENSION num_cols = cinfo->output_width;

  while (--num_rows >= 0) {
    inptr = input_buf[0][input_row++];
    outptr = *output_buf++;
    for (col = 0; col < num_cols; col++) {
      /* We can dispense with GETJSAMPLE() here */
      outptr[RGB_RED] = outptr[RGB_GREEN] = outptr[RGB_BLUE] = inptr[col];
      outptr += RGB_PIXELSIZE;
    }
  }
}


/*
 * Adobe-style YCCK->CMYK conversion.
 * We convert YCbCr to R=1-C, G=1-M, and B=1-Y using the same
 * conversion as above, while passing K (black) unchanged.
 * We assume build_ycc_rgb_table has been called.
 */

METHODDEF(void)
ycck_cmyk_convert (j_decompress_ptr cinfo,
		   JSAMPIMAGE input_buf, JDIMENSION input_row,
		   JSAMPARRAY output_buf, int num_rows)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;
  register int y, cb, cr;
  register JSAMPROW outptr;
  register JSAMPROW inptr0, inptr1, inptr2, inptr3;
  register JDIMENSION col;
  JDIMENSION num_cols = cinfo->output_width;
  /* copy these pointers into registers if possible */
  const JSAMPLE * range_limit = cinfo->sample_range_limit;
  register int * Crrtab = cconvert->Cr_r_tab;
  register int * Cbbtab = cconvert->Cb_b_tab;
  register INT32 * Crgtab = cconvert->Cr_g_tab;
  register INT32 * Cbgtab = cconvert->Cb_g_tab;
  SHIFT_TEMPS

  while (--num_rows >= 0) {
    inptr0 = input_buf[0][input_row];
    inptr1 = input_buf[1][input_row];
    inptr2 = input_buf[2][input_row];
    inptr3 = input_buf[3][input_row];
    input_row++;
    outptr = *output_buf++;
    for (col = 0; col < num_cols; col++) {
      y  = GETJSAMPLE(inptr0[col]);
      cb = GETJSAMPLE(inptr1[col]);
      cr = GETJSAMPLE(inptr2[col]);
      /* Range-limiting is essential due to noise introduced by DCT losses. */
      outptr[0] = range_limit[MAXJSAMPLE - (y + Crrtab[cr])];	/* red */
      outptr[1] = range_limit[MAXJSAMPLE - (y +			/* green */
			      ((int) RIGHT_SHIFT(Cbgtab[cb] + Crgtab[cr],
						 SCALEBITS)))];
      outptr[2] = range_limit[MAXJSAMPLE - (y + Cbbtab[cb])];	/* blue */
      /* K passes through unchanged */
      outptr[3] = inptr3[col];	/* don't need GETJSAMPLE here */
      outptr += 4;
    }
  }
}


/*
 * Empty method for start_pass.
 */

METHODDEF(void)
start_pass_dcolor (j_decompress_ptr cinfo)
{
  /* no work needed */
}


/*
 * Module initialization routine for output colorspace conversion.
 */

GLOBAL(void)
jinit_color_deconverter (j_decompress_ptr cinfo)
{
  my_cconvert_ptr cconvert;
  int ci;

  cconvert = (my_cconvert_ptr)
    (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_color_deconverter));
  cinfo->cconvert = (struct jpeg_color_deconverter *) cconvert;
  cconvert->pub.start_pass = start_pass_dcolor;

  /* Make sure num_components agrees with jpeg_color_space */
  switch (cinfo->jpeg_color_space) {
  case JCS_GRAYSCALE:
    if (cinfo->num_components != 1)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    break;

  case JCS_RGB:
  case JCS_YCbCr:
    if (cinfo->num_components != 3)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    break;

  case JCS_CMYK:
  case JCS_YCCK:
    if (cinfo->num_components != 4)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    break;

  default:			/* JCS_UNKNOWN can be anything */
    if (cinfo->num_components < 1)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    break;
  }

  /* Set out_color_components and conversion method based on requested space.
   * Also clear the component_needed flags for any unused components,
   * so that earlier pipeline stages can avoid useless computation.
   */

  switch (cinfo->out_color_space) {
  case JCS_GRAYSCALE:
    cinfo->out_color_components = 1;
    if (cinfo->jpeg_color_space == JCS_GRAYSCALE ||
	cinfo->jpeg_color_space == JCS_YCbCr) {
      cconvert->pub.color_convert = grayscale_convert;
      /* For color->grayscale conversion, only the Y (0) component is needed */
      for (ci = 1; ci < cinfo->num_components; ci++)
	cinfo->comp_info[ci].component_needed = FALSE;
    } else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  case JCS_RGB:
    cinfo->out_color_components = RGB_PIXELSIZE;
    if (cinfo->jpeg_color_space == JCS_YCbCr) {
      cconvert->pub.color_convert = ycc_rgb_convert;
      build_ycc_rgb_table(cinfo);
    } else if (cinfo->jpeg_color_space == JCS_GRAYSCALE) {
      cconvert->pub.color_convert = gray_rgb_convert;
    } else if (cinfo->jpeg_color_space == JCS_RGB && RGB_PIXELSIZE == 3) {
      cconvert->pub.color_convert = null_convert;
    } else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  case JCS_CMYK:
    cinfo->out_color_components = 4;
    if (cinfo->jpeg_color_space == JCS_YCCK) {
      cconvert->pub.color_convert = ycck_cmyk_convert;
      build_ycc_rgb_table(cinfo);
    } else if (cinfo->jpeg_color_space == JCS_CMYK) {
      cconvert->pub.color_convert = null_convert;
    } else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  default:
    /* Permit null conversion to same output space */
    if (cinfo->out_color_space == cinfo->jpeg_color_space) {
      cinfo->out_color_components = cinfo->num_components;
      cconvert->pub.color_convert = null_convert;
    } else			/* unsupported non-null conversion */
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;
  }

  if (cinfo->quantize_colors)
    cinfo->output_components = 1; /* single colormapped output component */
  else
    cinfo->output_components = cinfo->out_color_components;
}
