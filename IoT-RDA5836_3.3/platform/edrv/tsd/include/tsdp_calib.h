/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef TSDP_CALIB_H
#define TSDP_CALIB_H

#include "tsd_m.h"

#define CALIBRATION_NOT_SUCCESSFUL 0
#define CALIBRATION_SUCCESSFUL 1

//  This file contains functions that implement calculations 
//  necessary to obtain calibration factors for a touch screen
//  that suffers from multiple distortion effects: namely, 
//  translation, scaling and rotation.
//
//   The following set of equations represent a valid display 
//   point given a corresponding set of touch screen points:
//
//
//                                              /-     -\   //
//              /-    -\     /-             -\  |       |
//              |      |     |              |   |   Xs  |
//              |  Xd  |     | A    B    C  |   |       |
//              |      |  =  |              | * |   Ys  |
//              |  Yd  |     | D    E    F  |   |       |
//              |      |     |              |   |   1   |
//              \-    -/     \-            -/   |       |
//                                              \-     -/
//
// 
//    where:
//
//           (Xd,Yd) represents the desired display point 
//                    coordinates,
//
//           (Xs,Ys) represents the available touch screen
//                    coordinates, and the matrix
//
//           /-   -\\  //
//           |A,B,C|
//           |D,E,F| represents the factors used to translate
//           \-   -/  the available touch screen point values
//                    into the corresponding display 
//                    coordinates.
//
//
//    Note that for practical considerations, the utilitities 
//    within this file do not use the matrix coefficients as
//    defined above, but instead use the following 
//    equivalents, since floating point math is not used:
//
//            A = An/Divider 
//            B = Bn/Divider 
//            C = Cn/Divider 
//            D = Dn/Divider 
//            E = En/Divider 
//            F = Fn/Divider 
//
//
//
//    The functions provided within this file are:
//
//          SetCalibrationMatrix() - calculates the set of factors
//                                    in the above equation, given
//                                    three sets of test points.
//               GetDisplayPoint() - returns the actual display
//                                    coordinates, given a set of
//                                    touch screen coordinates.


typedef struct  
{
    INT32   An;    // A = An/Divider. 
    INT32   Bn;    // B = Bn/Divider.
    INT32   Cn;    // C = Cn/Divider.
    INT32   Dn;    // D = Dn/Divider.
    INT32   En;    // E = En/Divider. 
    INT32   Fn;    // F = Fn/Divider.
    INT32   Divider;
} TSD_MATRIX_T;


// ============================================================================
//  SetCalibrationMatrix
// ----------------------------------------------------------------------------
/// Calling this function with valid input data in the display and screen input
/// arguments causes the calibration factors between the screen and display points 
/// to be calculated, and the output argument - matrixPtr - to be populated.
///
/// This function needs to be called only when new calibration factors are desired.
///               
/// @param displayPtr (input) Pointer to an array of three samples, reference points.
/// @param screenPtr  (input) Pointer to the array of touch screen points 
///                           corresponding to the reference display points.
/// @param matrixPtr  (output)Pointer to calibration factors matrix computed
///                           for the set of points being provided.
///                    
///
/// @return Calibration succesful flag :
///         CALIBRATION_SUCCESSFUL - the calibration matrix was correctly 
///         calculated and its value is in the output argument.
///         CALIBRATION_NOT_SUCCESSFUL- an error was detected and the 
///         function failed to return a valid set of matrix values.
///         The only time this sample code returns
///         CALIBRATION_NOT_SUCCESSFUL is when Divider == 0
///
///                NOTE!    NOTE!    NOTE!
///
/// SetCalibrationMatrix() and GetDisplayPoint() will do fine for you as they are, 
/// provided that your digitizer resolution does not exceed 10 bits (1024 values).  
/// Higher resolutions may cause the integer operations to overflow and return 
/// incorrect values.  If you wish to use these functions with digitizer resolutions 
/// of 12 bits (4096 values) you will either have to a) use 64-bit signed     
/// integer variables and math, or b) judiciously modify the operations to scale 
/// results by a factor of 2 or even 4.  
// ============================================================================
UINT32 SetCalibrationMatrix( TSD_POINT_T* display, 
                             TSD_POINT_T* screen, 
                             TSD_MATRIX_T* matrix);


// ============================================================================
//  GetDisplayPoint
// ----------------------------------------------------------------------------
/// Given a valid set of calibration factors and a point value reported by the 
/// touch screen controler, this function calculates and returns the true 
/// (or closest to true) display point below the spot where the touch screen 
/// was touched.
///               
/// The function simply solves for Xd and Yd by implementing the computations 
/// required by the translation matrix.     
///              
///                                             /-     -\\  //
///             /-    -\     /-            -\   |       |
///             |      |     |              |   |   Xs  |
///             |  Xd  |     | A    B    C  |   |       |
///             |      |  =  |              | * |   Ys  |
///             |  Yd  |     | D    E    F  |   |       |
///             |      |     |              |   |   1   |
///             \-    -/     \-            -/   |       |
///                                             \-     -/
///                           
/// It must be kept brief to avoid consuming CPU cycles.     
///
///
/// @param displayPtr (output) Pointer to the calculated (true) display point.
/// @param screenPtr  (input)  Pointer to the reported touch screen point.
/// @param matrixPtr  (input)  Pointer to calibration factors matrix previously 
///                   calculated from a call to #SetCalibrationMatrix()
///
/// @return Calibration succesful flag :
///         CALIBRATION_SUCCESSFUL - the display point was correctly 
///         calculated and its value is in the output argument.
///         CALIBRATION_NOT_SUCCESSFUL- an error was detected and the 
///         function failed to return a valid point.
// ============================================================================
UINT32 GetDisplayPoint( TSD_POINT_T* display, 
                        TSD_POINT_T* screen, 
                        TSD_MATRIX_T* matrix ) ;

#endif
