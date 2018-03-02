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

// ================================================================================
/// @file         sxr_mem.h
/// @par Systeme executif nucleus public header file.
// --------------------------------------------------------------------------------
/// @defgroup sx_mem SX Memory Managment
/// @par Heap Memory
/// Heap memory manager is designed to provide big amount of memory with almost
/// no loss. This type of dynamic memory allocation must be used with care. Two
/// kinds of risks are associated to the use of this memory:
///  - Allocation time may vary significantly from one allocation to the other,
///    depending of the currently allocated memory and of the size of the 
///    required memory block.
///  - Successive allocations of small and big memory block may lead to 
///    fragmentation problems.
///  .
/// That is why this memory must not be used at the application level and should
/// be used only by the operating system for unpredictable big memory size 
/// allocation like task stacks for example.
///
/// @par Cluster memory
/// Cluster memory is more real time and embedded software oriented than heap 
/// memory. Different cluster each composed of a number of memory granules with
/// predefined size are made available. This allows a predictable allocation
/// time and avoid fragmentation problem encountered with heap memory.
/// \n
/// There is a price to be paid for this: as granules sizes and clusters are 
/// predefined, a significant amount of memory may be lost.
/// It is up to the user to create the number of clusters with the right sizes,
/// adapted to its application.
/// SX offers a trade off that consists in creating several heap that are used
/// as cluster memory, which means that the allocation will be split in the heap
/// that has been created to match the required size as closely as possible.
/// This option limits both the fragmentation risk and the allocation time 
/// dispersion while avoiding the memory wasted with the predefined size for
/// the memory block of the standard cluster approach.
/// \n
/// When SX is compiled with the switch __SXR_CLUSTER__, the standard cluster
/// approach is held, otherwise a cluster creation leads to a heap creation.
///
/// @{


/*
================================================================================
  History    :
--------------------------------------------------------------------------------
  Jun 13 03  |  ADA  | Creation
  Sep 13 04  |  ADA  | Cluster in Heap.
  Nov 20 04  |  ADA  | Released pointer set to NIL.
  Aug 15 05  |  ADA  | Function sxr_GetClusterInfo.
  Mar 27 06  |  ADA  | Macro _malloc and _free for allocations in user heap.
================================================================================
*/

#ifndef __SXR_MEM_H__
#define __SXR_MEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sxs_type.h"
#include "sxr_cnf.h"


#ifdef __SXR_MEM_VAR__
#define DefExtern
#else
#define DefExtern extern
#endif

#define SXR_STD_MEM  0
#define SXR_INT_MEM  1

#ifndef __SXR_CLUSTER__
#define SXR_CLUSTER_LIKE_MEM 0xFF
#endif

PVOID SRVAPI COS_Malloc (
  UINT32 nSize
);

BOOL SRVAPI COS_Free (
  PVOID pMemBlock
);


#define sxr_NewHeap(a,b)      _sxr_NewHeap(a,b,SXR_STD_MEM)
#define sxr_HMalloc(a)        COS_Malloc(a)	//_sxr_HMalloc(a,SXR_STD_MEM)
#define sxr_HFree(a)         COS_Free(a)
#define sxr_IntMalloc(a)      COS_Malloc(a)	//_sxr_HMalloc(a,SXR_INT_MEM)
//#define _sxr_HMalloc(a,b)      COS_Malloc(a)

// =============================================================================
// _sxr_NewHeap
// -----------------------------------------------------------------------------
/// Create a heap memory area.
/// @param  HeapStart   first address of the heap.
/// @param  Size        in bytes.
/// @param  Idx         Heap instance index.
// =============================================================================
void  _sxr_NewHeap    (void *HeapStart, u32 Size, u8 Idx);
// =============================================================================
// sxr_HMalloc
// -----------------------------------------------------------------------------
/// Heap memory allocation.
/// @param  Size    Requered memory size in bytes.
/// @param  Idx     Heap instance index.
/// @return Pointer onto free memory of required size.
// =============================================================================
void *_sxr_HMalloc    (u32 Size, u8 Idx);
// =============================================================================
// _sxr_UpMemCallerAdd
// -----------------------------------------------------------------------------
/// Update the memory \p CallerAdd field, to be used by custom allocation 
/// function to actually record the user function and not the custom allocation
/// function.
/// @param  Ptr     pointer to allocated memory
/// @param  callAdd new value of \p CallerAdd field.
// =============================================================================
void  _sxr_UpMemCallerAdd(void *Ptr,  u32 callAdd);

#define __SXR_RELEASED_PTR_TO_NIL__
// =============================================================================
// sxr_HFree
// -----------------------------------------------------------------------------
/// Free memory allocated in heap.
/// @param  Ptr on memory.
// =============================================================================
#ifdef __SXR_RELEASED_PTR_TO_NIL__
//void _sxr_HFree (void **Ptr);
//#define sxr_HFree(a)  COS_Free((void **)&(a))	//_sxr_HFree((void **)&(a))
#else
//void  sxr_HFree       (void *MemoryPtr);
//#define _sxr_HFree(a)  COS_Free(a)	//sxr_HFree(a)
#endif

#define sxr_UpMemCallerAdd(a, b)   \
do{\
   _sxr_UpMemCallerAdd((void *)(a),  b);\
}while(0)


// =============================================================================
// sxr_GetMemSize
// -----------------------------------------------------------------------------
/// Get the allocated size of the memory which start at the specified address.
/// @param  Address Start address of the allocated memory.
/// @return Allocated size.
// =============================================================================
u16   sxr_GetMemSize  (void *Address);

// =============================================================================
// sxr_CheckHeap
// -----------------------------------------------------------------------------
/// Check Heaps load and consistency.
// =============================================================================
void  sxr_CheckHeap   (void);

void sxr_InitCluster   (void);

// =============================================================================
// sxr_NewCluster
// -----------------------------------------------------------------------------
/// This function is used to create a new cluster. The cluster will be composed
/// of NbUnit of Size bytes.
/// @param  Size of units
/// @param  NbUnit number of units
// =============================================================================
void  sxr_NewCluster  (u16 Size, u8 NbUnit);
#ifdef __SXR_CLUSTER__
// =============================================================================
// sxr_Malloc
// -----------------------------------------------------------------------------
/// Memory allocation from cluster.
/// @param  Memory Size
/// @return Pointer onto memory block.
// =============================================================================
void *sxr_Malloc      (u16 Size);
#else
// =============================================================================
// sxr_Malloc
// -----------------------------------------------------------------------------
/// Memory allocation from cluster.
/// @param  a dMemory Size
/// @return Pointer onto memory block.
// =============================================================================
#define sxr_Malloc(a) COS_Malloc(a)	//_sxr_HMalloc(a,SXR_CLUSTER_LIKE_MEM)
#endif


// =============================================================================
// sxr_Link
// -----------------------------------------------------------------------------
/// Link a piece of memory already allocated.
/// @param  Ptr Pointer onto cluster
// =============================================================================
void  sxr_Link        (void *Ptr);

extern BOOL COS_FreeSetPtrNull(PVOID *ptr);
#ifdef __SXR_CLUSTER__
// =============================================================================
// sxr_Free
// -----------------------------------------------------------------------------
/// Free memory.
/// @param  Ptr Pointer onto memory block to be released.
// =============================================================================
#ifdef __SXR_RELEASED_PTR_TO_NIL__
//void _sxr_Free (void **Ptr);
#define sxr_Free(a)   COS_FreeSetPtrNull((void *)&(a))	//_sxr_Free((void *)&(a))
#else
void  sxr_Free(a)        COS_Free(a);//        (void *Ptr);
#endif
#else
// =============================================================================
// sxr_Free
// -----------------------------------------------------------------------------
/// Free memory.
/// @param  a Pointer onto memory block to be released.
// =============================================================================
#ifdef __SXR_RELEASED_PTR_TO_NIL__
#define sxr_Free(a)   COS_FreeSetPtrNull((void *)&(a))	//_sxr_HFree((void *)&(a))
#else
#define sxr_Free(a)  COS_Free(a)	// sxr_HFree(a)
#endif
#endif

// =============================================================================
// sxr_GetLink
// -----------------------------------------------------------------------------
/// Get Link counter
/// @param  Ptr on allocated memory.
/// @return Number of link.
// =============================================================================
u8  sxr_GetLink (void *Ptr);

// =============================================================================
// sxr_GetAbsoluteRemainingSize
// -----------------------------------------------------------------------------
/// Return the number of bytes remaining in the identified heap.
/// If the Heap is identified thanks to its index, the parameter Idx is useless.
/// When Idx == SXR_CLUSTER_LIKE_MEM the function will return the size remaining
/// in the "cluster heap" targeting the providing size.
/// @param Size allows to identify the target "cluster heap"
/// @param Idx  heap index or SXR_CLUSTER_LIKE_MEM.
/// @return     number of bytes available in the identified Heap.
// =============================================================================
u32 sxr_GetAbsoluteRemainingSize (u32 Size, u8 Idx);

// =============================================================================
// sxr_GetRelativeRemainingSize
// -----------------------------------------------------------------------------
/// Return the ratio of bytes remaining in the identified memory the total
/// number of bytes dedicated to the heap.
/// If the Heap is identified thanks to its index, the parameter Size is useless.
/// When Idx == SXR_CLUSTER_LIKE_MEM the function will return the size remaining
/// in the "cluster heap" targeting the providing size.
/// @param Size allows to identify the target "cluster heap"
/// @param Idx  heap index or SXR_CLUSTER_LIKE_MEM.
/// @return     percentage of bytes available in the identified Heap.
// =============================================================================
u32 sxr_GetRelativeRemainingSize (u32 Size, u8 Idx);

#define sxr_GetAbsoluteHeapRemainingSize(Idx)     sxr_GetAbsoluteRemainingSize(0, Idx)
#define sxr_GetRelativeHeapRemainingSize(Idx)     sxr_GetRelativeRemainingSize(0, Idx)

#define sxr_GetAbsoluteClusterRemainingSize(Size) sxr_GetAbsoluteRemainingSize(Size, SXR_CLUSTER_LIKE_MEM)
#define sxr_GetRelativeClusterRemainingSize(Size) sxr_GetRelativeRemainingSize(Size, SXR_CLUSTER_LIKE_MEM)

// =============================================================================
// sxr_GetHeapEnd
// -----------------------------------------------------------------------------
/// Return Heap end address.
/// @param  Idx Heap index.
/// @return Address on heap end.
// =============================================================================
void *sxr_GetHeapEnd  (u8 Idx);

// =============================================================================
// sxr_CheckCluster
// -----------------------------------------------------------------------------
/// Check clusters load and consistency.
// =============================================================================
void  sxr_CheckCluster (void);

// =============================================================================
// sxr_CheckClusterRmt
// -----------------------------------------------------------------------------
/// Check clusters load and consistency and send Info to Remote.
// =============================================================================
void  sxr_CheckClusterRmt (void);

// =============================================================================
// sxr_CheckMemTracker
// -----------------------------------------------------------------------------
/// Print the last memory allocation activity.
// =============================================================================
void  sxr_CheckMemTracker (void);

typedef struct
{
    u16 Size;            // Cluster size.
    u16 NbCluster;       // Number of clusters.
    u16 NbAllocation;    // Number of blocks currently allocated.
} sxr_ClusterInfo_t;

// =============================================================================
// sxr_GetClusterInfo
// -----------------------------------------------------------------------------
/// Provide information about the specified cluster.
/// @param  ClusterIdx  Cluster index
/// @param  ClusterInfo Pointer to sxr_ClusterInfo_t
// =============================================================================
void sxr_GetClusterInfo (u8 ClusterIdx, sxr_ClusterInfo_t *ClusterInfo);

#if defined (__SXR_DEBUG__) || defined (__SXR_MEM_CHECK__)
// =============================================================================
// sxr_CheckUpdateHeap
// -----------------------------------------------------------------------------
/// Check heaps load and consistency in debug mode.
/// @param  Block
/// @param  Idx Heap index.
// =============================================================================
void sxr_CheckUpdateHeap (void *Block, u8 Idx); // (sxr_HMemHead_t *PHeap, u8 Idx);

// =============================================================================
// sxr_CheckUpdateCluster
// -----------------------------------------------------------------------------
/// Check clusters load and consistency in debug mode.
/// @param  Ptr
/// @param  ClusterIdx
// =============================================================================
void sxr_CheckUpdateCluster (u8 *Ptr, u8 ClusterIdx);
#else
#define sxr_CheckUpdateHeap(a,b)
#define sxr_CheckUpdateCluster(a,b)
#endif

// =============================================================================
// sxr_MemoryAlertDispatchRegistration
// -----------------------------------------------------------------------------
/// Register a call back for the memory level notification.
/// @param  CallBack
// =============================================================================
void sxr_MemoryAlertDispatchRegistration (void (*CallBack)(u32));

#define SXR_MEMORY_LEVEL_OK  0
#define SXR_MEMORY_LEVEL_LOW 1


#define _malloc(Size) _sxr_HMalloc(Size, PAL_NB_RAM_AREA)
#define _free(p)      sxr_Free(p)

#undef DefExtern

#ifdef __cplusplus
}
#endif

#endif

/// @} // <-- End of sx_mem group
