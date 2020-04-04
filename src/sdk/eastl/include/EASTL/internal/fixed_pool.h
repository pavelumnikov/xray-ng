/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements the following
//     aligned_buffer
//     fixed_pool_base
//     fixed_pool
//     fixed_pool_with_overflow
//     fixed_hashtable_allocator
//     fixed_vector_allocator
//     fixed_swap
//
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_INTERNAL_FIXED_POOL_H
#define EASTL_INTERNAL_FIXED_POOL_H


#include <EASTL/EABase/eabase.h>
#if defined(EA_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <EASTL/internal/config.h>
#include <EASTL/functional.h>
#include <EASTL/memory.h>
#include <EASTL/allocator.h>
#include <EASTL/type_traits.h>

#ifdef _MSC_VER
	#pragma warning(push, 0)
	#include <new>
	#pragma warning(pop)
#else
	#include <new>
#endif

#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable: 4275) // non dll-interface class used as base for DLL-interface classkey 'identifier'
#endif


namespace eastl
{

	/// EASTL_FIXED_POOL_DEFAULT_NAME
	///
	/// Defines a default allocator name in the absence of a user-provided name.
	///
	#ifndef EASTL_FIXED_POOL_DEFAULT_NAME
		#define EASTL_FIXED_POOL_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixed_pool" // Unless the user overrides something, this is "EASTL fixed_pool".
	#endif



	///////////////////////////////////////////////////////////////////////////
	// aligned_buffer
	///////////////////////////////////////////////////////////////////////////

	/// aligned_buffer
	///
	/// This is useful for creating a buffer of the same size and alignment 
	/// of a given struct or class. This is useful for creating memory pools
	/// that support both size and alignment requirements of stored objects
	/// but without wasting space in over-allocating. 
	///
	/// Note that we implement this via struct specializations, as some 
	/// compilers such as VC++ do not support specification of alignments
	/// in any way other than via an integral constant.
	///
	/// Example usage:
	///    struct Widget{ }; // This class has a given size and alignment.
	///
	///    Declare a char buffer of equal size and alignment to Widget.
	///    aligned_buffer<sizeof(Widget), EASTL_ALIGN_OF(Widget)> mWidgetBuffer; 
	///
	///    Declare an array this time.
	///    aligned_buffer<sizeof(Widget), EASTL_ALIGN_OF(Widget)> mWidgetArray[15]; 
	///
	typedef char EASTL_MAY_ALIAS aligned_buffer_char; 

	template <size_t size, size_t alignment>
	struct aligned_buffer { aligned_buffer_char buffer[size]; };

	template<size_t size>
	struct aligned_buffer<size, 2>    { EA_PREFIX_ALIGN(2) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(2); };

	template<size_t size>
	struct aligned_buffer<size, 4> 
	{ 
		EA_PREFIX_ALIGN(4) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(4);
	};

	template<size_t size>
	struct aligned_buffer<size, 8>
	{ 
		EA_PREFIX_ALIGN(8) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(8);
	};

	template<size_t size>
	struct aligned_buffer<size, 16> 
	{ 
		EA_PREFIX_ALIGN(16) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(16); 
	};

	template<size_t size>
	struct aligned_buffer<size, 32>
	{ 
		EA_PREFIX_ALIGN(32) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(32); 
	};

	template<size_t size>
	struct aligned_buffer<size, 64>
	{
		EA_PREFIX_ALIGN(64) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(64); 
	};

	template<size_t size>
	struct aligned_buffer<size, 128>  { EA_PREFIX_ALIGN(128) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(128); };

	template<size_t size>
	struct aligned_buffer<size, 256>
	{ 
		EA_PREFIX_ALIGN(256) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(256); 
	};

	template<size_t size>
	struct aligned_buffer<size, 512> 
	{
		EA_PREFIX_ALIGN(512) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(512);
	};

	template<size_t size>
	struct aligned_buffer<size, 1024> 
	{ 
		EA_PREFIX_ALIGN(1024) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(1024);
	};

	template<size_t size>
	struct aligned_buffer<size, 2048>
	{
		EA_PREFIX_ALIGN(2048) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(2048);
	};

	template<size_t size>
	struct aligned_buffer<size, 4096>
	{
		EA_PREFIX_ALIGN(4096) aligned_buffer_char buffer[size] EA_POSTFIX_ALIGN(4096);
	};




	///////////////////////////////////////////////////////////////////////////
	// fixed_pool_base
	///////////////////////////////////////////////////////////////////////////

	/// fixed_pool_base
	///
	/// This is a base class for the implementation of fixed-size pools.
	/// In particular, the fixed_pool and fixed_pool_with_overflow classes
	/// are based on fixed_pool_base.
	///
	struct fixed_pool_base
	{
	public:
		/// fixed_pool_base
		///
		fixed_pool_base(void* pMemory = NULL)
			: mpHead((Link*)pMemory)
			, mpNext((Link*)pMemory)
			, mpCapacity((Link*)pMemory)
			, mnNodeSize(0) // This is normally set in the init function.
		{
			#if EASTL_FIXED_SIZE_TRACKING_ENABLED
				mnCurrentSize = 0;
				mnPeakSize    = 0;
			#endif
		}


		/// fixed_pool_base
		///
		// Disabled because the default is sufficient. While it normally makes no sense to deep copy
		// this data, our usage of this class is such that this is OK and wanted.
		//
		// fixed_pool_base(const fixed_pool_base& x)
		// {
		// }


		/// operator=
		///
		fixed_pool_base& operator=(const fixed_pool_base&)
		{
			// By design we do nothing. We don't attempt to deep-copy member data. 
			return *this;
		}


		/// init
		///
		/// Initializes a fixed_pool with a given set of parameters.
		/// You cannot call this function twice else the resulting 
		/// behaviour will be undefined. You can only call this function
		/// after constructing the fixed_pool with the default constructor.
		///
		EASTL_API void init(void* pMemory, size_t memorySize, size_t nodeSize,
							size_t alignment, size_t alignmentOffset = 0);


		/// peak_size
		///
		/// Returns the maximum number of outstanding allocations there have been
		/// at any one time. This represents a high water mark for the allocation count.
		///
		size_t peak_size() const
		{
			#if EASTL_FIXED_SIZE_TRACKING_ENABLED
				return mnPeakSize;
			#else
				return 0;
			#endif
		}


		/// can_allocate
		///
		/// Returns true if there are any free links.
		///
		bool can_allocate() const
		{
			return (mpHead != NULL) || (mpNext != mpCapacity);
		}

	public:
		/// Link
		/// Implements a singly-linked list.
		struct Link
		{
			Link* mpNext;
		};

		Link*   mpHead;
		Link*   mpNext;
		Link*   mpCapacity;
		size_t  mnNodeSize;

		#if EASTL_FIXED_SIZE_TRACKING_ENABLED
			uint32_t mnCurrentSize; /// Current number of allocated nodes.
			uint32_t mnPeakSize;    /// Max number of allocated nodes at any one time.
		#endif

	}; // fixed_pool_base





	///////////////////////////////////////////////////////////////////////////
	// fixed_pool
	///////////////////////////////////////////////////////////////////////////

	/// fixed_pool
	///
	/// Implements a simple fixed pool allocator for use by fixed-size containers. 
	/// This is not a generic eastl allocator which can be plugged into an arbitrary
	/// eastl container, as it simplifies some functions are arguments for the 
	/// purpose of efficiency.
	/// 
	class EASTL_API fixed_pool : public fixed_pool_base
	{
	public:
		/// fixed_pool
		///
		/// Default constructor. User usually will want to call init() after  
		/// constructing via this constructor. The pMemory argument is for the 
		/// purposes of temporarily storing a pointer to the buffer to be used.
		/// Even though init may have a pMemory argument, this arg is useful 
		/// for temporary storage, as per copy construction.
		///
		fixed_pool(void* pMemory = NULL)
			: fixed_pool_base(pMemory)
		{
		}


		/// fixed_pool
		///
		/// Constructs a fixed_pool with a given set of parameters.
		///
		fixed_pool(void* pMemory, size_t memorySize, size_t nodeSize, 
					size_t alignment, size_t alignmentOffset = 0)
		{
			init(pMemory, memorySize, nodeSize, alignment, alignmentOffset);
		}


		/// fixed_pool
		///
		// Disabled because the default is sufficient. While it normally makes no sense to deep copy
		// this data, our usage of this class is such that this is OK and wanted.
		//
		// fixed_pool(const fixed_pool& x)
		// {
		// }


		/// operator=
		///
		fixed_pool& operator=(const fixed_pool&)
		{
			// By design we do nothing. We don't attempt to deep-copy member data. 
			return *this;
		}


		/// allocate
		///
		/// Allocates a new object of the size specified upon class initialization.
		/// Returns NULL if there is no more memory. 
		///
		void* allocate()
		{
			Link* pLink = mpHead;

			if(pLink) // If we have space...
			{
				#if EASTL_FIXED_SIZE_TRACKING_ENABLED
					if(++mnCurrentSize > mnPeakSize)
						mnPeakSize = mnCurrentSize;
				#endif

				mpHead = pLink->mpNext;
				return pLink;
			}
			else
			{
				// If there's no free node in the free list, just
				// allocate another from the reserved memory area

				if(mpNext != mpCapacity)
				{
					pLink = mpNext;
					
					mpNext = reinterpret_cast<Link*>(reinterpret_cast<char*>(mpNext) + mnNodeSize);

					#if EASTL_FIXED_SIZE_TRACKING_ENABLED
						if(++mnCurrentSize > mnPeakSize)
							mnPeakSize = mnCurrentSize;
					#endif

					return pLink;
				}

				return NULL;
			}
		}

		void* allocate(size_t /*alignment*/, size_t /*offset*/)
		{
			return allocate();
		}
		
		/// deallocate
		///
		/// Frees the given object which was allocated by allocate(). 
		/// If the given node was not allocated by allocate() then the behaviour 
		/// is undefined.
		///
		void deallocate(void* p)
		{
			#if EASTL_FIXED_SIZE_TRACKING_ENABLED
				--mnCurrentSize;
			#endif

			((Link*)p)->mpNext = mpHead;
			mpHead = ((Link*)p);
		}


		using fixed_pool_base::can_allocate;


		const char* get_name() const
		{
			return EASTL_FIXED_POOL_DEFAULT_NAME;
		}


		void set_name(const char*)
		{
			// Nothing to do. We don't allocate memory.
		}

	}; // fixed_pool


	///////////////////////////////////////////////////////////////////////////
	// fixed_node_allocator
	///////////////////////////////////////////////////////////////////////////

	/// fixed_node_allocator
	///
	/// Note: This class was previously named fixed_node_pool, but was changed because this name
	///       was inconsistent with the other allocators here which ended with _allocator.
	///
	/// Implements a fixed_pool with a given node count, alignment, and alignment offset.
	/// fixed_node_allocator is like fixed_pool except it is templated on the node type instead
	/// of being a generic allocator. All it does is pass allocations through to
	/// the fixed_pool base. This functionality is separate from fixed_pool because there
	/// are other uses for fixed_pool.
	///
	/// We template on kNodeSize instead of node_type because the former allows for the
	/// two different node_types of the same size to use the same template implementation.
	///
	/// Template parameters:
	///     nodeSize               The size of the object to allocate.
	///     nodeCount              The number of objects the pool contains.
	///     nodeAlignment          The alignment of the objects to allocate.
	///     nodeAlignmentOffset    The alignment offset of the objects to allocate.
	///     bEnableOverflow        Whether or not we should use the overflow heap if our object pool is exhausted.
	///     OverflowAllocator      Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
	///
	template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset>
	class fixed_node_allocator
	{
	public:
		typedef fixed_pool pool_type;
		typedef fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset> this_type;

		enum
		{
			kNodeSize            = nodeSize,
			kNodeCount           = nodeCount,
			kNodesSize           = nodeCount * nodeSize, // Note that the kBufferSize calculation assumes that the compiler sets sizeof(T) to be a multiple alignof(T), and so sizeof(T) is always >= alignof(T).
			kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize-1 : 0) + nodeAlignmentOffset,
			kNodeAlignment       = nodeAlignment,
			kNodeAlignmentOffset = nodeAlignmentOffset
		};

	public:
		pool_type mPool;

	public:
		fixed_node_allocator(void* pNodeBuffer)
			: mPool(pNodeBuffer, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset)
		{
		}


		/// fixed_node_allocator
		///
		/// Note that we are copying x.mpHead to our own fixed_pool. This at first may seem 
		/// broken, as fixed pools cannot take over ownership of other fixed pools' memory.
		/// However, we declare that this copy ctor can only ever be safely called when 
		/// the user has intentionally pre-seeded the source with the destination pointer.
		/// This is somewhat playing with fire, but it allows us to get around chicken-and-egg
		/// problems with containers being their own allocators, without incurring any memory
		/// costs or extra code costs. There's another reason for this: we very strongly want
		/// to avoid full copying of instances of fixed_pool around, especially via the stack.
		/// Larger pools won't even be able to fit on many machine's stacks. So this solution
		/// is also a mechanism to prevent that situation from existing and being used. 
		/// Perhaps some day we'll find a more elegant yet costless way around this. 
		///
		fixed_node_allocator(const this_type& x)            // No need to copy the overflow allocator, because bEnableOverflow is false in this specialization.
			: mPool(x.mPool.mpNext, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset)
		{
		}


		this_type& operator=(const this_type& x)
		{
			mPool = x.mPool;
			return *this;
		}


		void* allocate(size_t n, int /*flags*/ = 0)
		{
			(void)n;
			EASTL_ASSERT(n == kNodeSize);
			return mPool.allocate();
		}


		void* allocate(size_t n, size_t alignment, size_t offset, int /*flags*/ = 0)
		{
			(void)n;
			EASTL_ASSERT(n == kNodeSize);
			return mPool.allocate(alignment, offset);
		}


		void deallocate(void* p, size_t)
		{
			mPool.deallocate(p);
		}


		bool can_allocate() const
		{
			return mPool.can_allocate();
		}


		void reset(void* pNodeBuffer)
		{
			mPool.init(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset);
		}


		const char* get_name() const
		{
			return mPool.get_name();
		}


		void set_name(const char* pName)
		{
			mPool.set_name(pName);
		}
	}; // fixed_node_allocator




	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset>
	inline bool operator==(const fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& a, const fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& b)
	{
		return (&a == &b); // They are only equal if they are the same object.
	}


	template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset>
	inline bool operator!=(const fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& a, const fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& b)
	{
		return (&a != &b); // They are only equal if they are the same object.
	}


	///////////////////////////////////////////////////////////////////////////
	// fixed_hashtable_allocator
	///////////////////////////////////////////////////////////////////////////

	/// fixed_hashtable_allocator
	///
	/// Provides a base class for fixed hashtable allocations.
	/// To consider: Have this inherit from fixed_node_allocator.
	///
	/// Template parameters:
	///     bucketCount            The fixed number of hashtable buckets to provide.
	///     nodeCount              The number of objects the pool contains.
	///     nodeAlignment          The alignment of the objects to allocate.
	///     nodeAlignmentOffset    The alignment offset of the objects to allocate.
	///
	template <size_t bucketCount, size_t nodeSize, size_t nodeCount, 
		size_t nodeAlignment, size_t nodeAlignmentOffset>
	class fixed_hashtable_allocator
	{
	public:
		typedef fixed_pool pool_type;
		typedef fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>  this_type;

		enum
		{
			kBucketCount = bucketCount + 1, // '+1' because the hash table needs a null terminating bucket.
			kBucketsSize = bucketCount * sizeof(void*),
			kNodeSize = nodeSize,
			kNodeCount = nodeCount,
			kNodesSize = nodeCount * nodeSize, // Note that the kBufferSize calculation assumes that the compiler sets sizeof(T) to be a multiple alignof(T), and so sizeof(T) is always >= alignof(T).
			kBufferSize = kNodesSize + ((nodeAlignment > 1) ? nodeSize-1 : 0) + nodeAlignmentOffset, // Don't need to include kBucketsSize in this calculation, as fixed_hash_xxx containers have a separate buffer for buckets.
			kNodeAlignment = nodeAlignment,
			kNodeAlignmentOffset = nodeAlignmentOffset,
			kAllocFlagBuckets = 0x00400000 // Flag to allocator which indicates that we are allocating buckets and not nodes.
		};

	protected:
		pool_type mPool;
		void*     mpBucketBuffer;

	public:
		// Disabled because it causes compile conflicts.
		//fixed_hashtable_allocator(const char* pName)
		//{
		//    mPool.set_name(pName);
		//}

		fixed_hashtable_allocator(void* pNodeBuffer)
			: mPool(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset)
			, mpBucketBuffer(NULL)
		{
			// EASTL_ASSERT(false); // As it stands now, this is not supposed to be called.
		}

		fixed_hashtable_allocator(void* pNodeBuffer, void* pBucketBuffer)
			: mPool(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
			  mpBucketBuffer(pBucketBuffer)
		{
		}

		/// fixed_hashtable_allocator
		///
		/// Note that we are copying x.mpHead and mpBucketBuffer to our own fixed_pool. 
		/// See the discussion above in fixed_node_allocator for important information about this.
		///
		fixed_hashtable_allocator(const this_type& x)
			: mPool(x.mPool.mpHead, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset)
			, mpBucketBuffer(x.mpBucketBuffer)
		{
		}


		fixed_hashtable_allocator& operator=(const fixed_hashtable_allocator& x)
		{
			mPool = x.mPool;
			return *this;
		}


		void* allocate(size_t n, int flags = 0)
		{
			// We expect that the caller uses kAllocFlagBuckets when it wants us to allocate buckets instead of nodes.
			EASTL_CT_ASSERT(kAllocFlagBuckets == 0x00400000); // Currently we expect this to be so, because the hashtable has a copy of this enum.
			if((flags & kAllocFlagBuckets) == 0) // If we are allocating nodes and (probably) not buckets...
			{
				EASTL_ASSERT(n == kNodeSize);  (void)n; // Make unused var warning go away.
				return mPool.allocate();
			}

			// Don't allow hashtable buckets to overflow in this case.
			EASTL_ASSERT(n <= kBucketsSize);
			return mpBucketBuffer;
		}


		void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0)
		{
			// We expect that the caller uses kAllocFlagBuckets when it wants us to allocate buckets instead of nodes.
			if((flags & kAllocFlagBuckets) == 0) // If we are allocating nodes and (probably) not buckets...
			{
				EASTL_ASSERT(n == kNodeSize); (void)n; // Make unused var warning go away.
				return mPool.allocate(alignment, offset);
			}

			// Don't allow hashtable buckets to overflow in this case.
			EASTL_ASSERT(n <= kBucketsSize);
			return mpBucketBuffer;
		}


		void deallocate(void* p, size_t)
		{
			if(p != mpBucketBuffer) // If we are freeing a node and not buckets...
				mPool.deallocate(p);
		}


		bool can_allocate() const
		{
			return mPool.can_allocate();
		}


		void reset(void* pNodeBuffer)
		{
			// No need to modify mpBucketBuffer, as that is constant.
			mPool.init(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset);
		}

		const char* get_name() const
		{
			return mPool.get_name();
		}

		void set_name(const char* pName)
		{
			mPool.set_name(pName);
		}
	}; // fixed_hashtable_allocator


	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <size_t bucketCount, size_t nodeSize, size_t nodeCount, 
		size_t nodeAlignment, size_t nodeAlignmentOffset>
	inline bool operator==(
		const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& a,
		const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& b)
	{
		return (&a == &b); // They are only equal if they are the same object.
	}


	template <size_t bucketCount, size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset>
	inline bool operator!=(
		const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& a,
		const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& b)
	{
		return (&a != &b); // They are only equal if they are the same object.
	}



	///////////////////////////////////////////////////////////////////////////
	// fixed_vector_allocator
	///////////////////////////////////////////////////////////////////////////

	/// fixed_vector_allocator
	///
	/// Template parameters:
	///     nodeSize               The size of individual objects.
	///     nodeCount              The number of objects the pool contains.
	///     nodeAlignment          The alignment of the objects to allocate.
	///     nodeAlignmentOffset    The alignment offset of the objects to allocate.
	///     bEnableOverflow        Whether or not we should use the overflow heap if our object pool is exhausted.
	///     OverflowAllocator      Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
	///
	template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset>
	class fixed_vector_allocator
	{
	public:
		typedef fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset> this_type;

		enum
		{
			kNodeSize            = nodeSize,
			kNodeCount           = nodeCount,
			kNodesSize           = nodeCount * nodeSize, // Note that the kBufferSize calculation assumes that the compiler sets sizeof(T) to be a multiple alignof(T), and so sizeof(T) is always >= alignof(T).
			kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize-1 : 0) + nodeAlignmentOffset,
			kNodeAlignment       = nodeAlignment,
			kNodeAlignmentOffset = nodeAlignmentOffset
		};

        fixed_vector_allocator() = default;

		fixed_vector_allocator(void* /*pNodeBuffer*/)
		{
		}

		void* allocate(size_t /*n*/, int /*flags*/ = 0)
		{
			EASTL_ASSERT(false); // A fixed_vector should not reallocate, else the user has exhausted its space.
			return NULL;
		}

		void* allocate(size_t /*n*/, size_t /*alignment*/, size_t /*offset*/, int /*flags*/ = 0)
		{
			EASTL_ASSERT(false);
			return NULL;
		}

		void deallocate(void* /*p*/, size_t /*n*/)
		{
		}

		const char* get_name() const
		{
			return EASTL_FIXED_POOL_DEFAULT_NAME;
		}

		void set_name(const char* /*pName*/)
		{
		}
	}; // fixed_vector_allocator


	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset>
	inline bool operator==(
		const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& a,
		const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& b)
	{
		return (&a == &b); // They are only equal if they are the same object.
	}


	template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset>
	inline bool operator!=(
		const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& a,
		const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset>& b)
	{
		return (&a != &b); // They are only equal if they are the same object.
	}





	///////////////////////////////////////////////////////////////////////////
	// fixed_swap
	///////////////////////////////////////////////////////////////////////////

	template<typename Container>
	void fixed_swap(Container& a, Container& b)
	{
		Container temp(EASTL_MOVE(a)); // Can't use global swap because that could
		a = EASTL_MOVE(b); // itself call this swap function in return.
		b = EASTL_MOVE(temp);
	}

} // namespace eastl


#if defined(_MSC_VER)
	#pragma warning(pop)
#endif


#endif // Header include guard

